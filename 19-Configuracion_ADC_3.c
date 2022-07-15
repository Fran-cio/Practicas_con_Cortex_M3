/*
 * Configuracion_ADC_3.c
 *
 *  Created on: 10 oct. 2021
 *      Author: francio
 */

/*
 * a.- Modificar el valor del Match del ejemplo visto en clases
 * para que la frecuencia de muestreo del ADC sea de 20 Kmuestras/seg.
 * El resultado de la conversión deben mostrarse por 12 pines de salida
 * del GPIO0.  Adjuntar el código en C.
 *
 * b.- Modificar el código realizado en el punto "a" utilizando ahora
 * el Driver provisto para este microcontrolador.
 */

#include "LPC17xx.h"

#include "lpc17xx_adc.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_clkpwr.h"


void confGPIO(void); // Prototipo de la funci�n de conf. de puertos
void confADC(void); //Prototipo de la funci�n de conf. de interrupciones externas
void confTimer(void);
void confADC_Driver(void);
void confTimer_Driver(void);
void retardo(void);

int main(void) {
	confGPIO();
	confTimer_Driver();
	confADC_Driver();
	while(1){
		LPC_GPIO0->FIOSET = (1<<22);
		retardo();
		LPC_GPIO0->FIOCLR = (1<<22);
		retardo();
	}
	return 0;
}

void retardo (void){
	uint32_t conta;
	for(conta = 0; conta<1000000;conta++){}
	return;
}

void confGPIO(void){
	PINSEL_CFG_Type pinsel;
	pinsel.Portnum=PINSEL_PORT_0;
	pinsel.Funcnum=PINSEL_FUNC_0;

	pinsel.Portnum=0;
	pinsel.Pinnum=25;
	pinsel.Pinmode=PINSEL_PINMODE_TRISTATE;
	pinsel.Funcnum=PINSEL_FUNC_1;

	PINSEL_ConfigPin(&pinsel);

	for (uint8_t i = 0; i < 12; ++i)
	{
		pinsel.Pinnum=i;
		PINSEL_ConfigPin(&pinsel);
	}
	GPIO_SetDir(0, 0xFFF, 1);
	FIO_HalfWordSetMask(0, 0, (~(0xFFF)), 1);

	LPC_GPIO0->FIODIR |= (1<<22)|(1<<9);
	return;
}

void confADC(void)
{
	LPC_SC->PCONP |= (1 << 12);
	LPC_ADC->ADCR |= (1 << 21);    //enable ADC
	LPC_SC->PCLKSEL0 |= (3<<24);  //CCLK/8
	LPC_ADC->ADCR &=~(255 << 8);  //[15:8] CLKDIV
	//LPC_ADC->ADCR |= (1 << 0);   // channel
	LPC_ADC->ADCR &= ~(1 << 16);   // no burst
	LPC_ADC->ADCR |= (1 << 26);    // match
	LPC_ADC->ADCR &= ~(3 << 24);   // match
	LPC_ADC->ADCR |= (1 << 27);    //edge
	//LPC_PINCON->PINSEL4 |= (1<<20);
	LPC_PINCON->PINMODE1 |= (1<<15); //neither pull-up nor pull-down.
	LPC_PINCON->PINSEL1 |= (1<<14);
	LPC_ADC->ADINTEN |= (1<<0);
	NVIC_EnableIRQ(ADC_IRQn);
	return;
}

void confADC_Driver(void)
{
	CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_ADC, CLKPWR_PCLKSEL_CCLK_DIV_4);

	ADC_Init(LPC_ADC, 192307);
	ADC_IntConfig(LPC_ADC, ADC_ADGINTEN, SET);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);

	ADC_BurstCmd(LPC_ADC, 0);
	ADC_EdgeStartConfig(LPC_ADC, ADC_START_ON_FALLING);
	ADC_StartCmd(LPC_ADC, ADC_START_ON_MAT01);

	PINSEL_CFG_Type pinsel;
	pinsel.Portnum=PINSEL_PORT_0;
	pinsel.Pinmode=PINSEL_PINMODE_TRISTATE;
	pinsel.Pinnum=PINSEL_PIN_23;
	pinsel.Funcnum=PINSEL_FUNC_1;
	PINSEL_ConfigPin(&pinsel);

	NVIC_EnableIRQ(ADC_IRQn);
	return;
}

void confTimer_Driver(void)
{
	TIM_MATCHCFG_Type confM;
	TIM_TIMERCFG_Type	confT;

	confT.PrescaleOption=TIM_PRESCALE_TICKVAL;
	confT.PrescaleValue=1;

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &confT);

	confM.ExtMatchOutputType=TIM_EXTMATCH_TOGGLE;
	confM.IntOnMatch=DISABLE;
	confM.MatchChannel=1;
	confM.MatchValue=100000000/40000;
	confM.ResetOnMatch=ENABLE;
	confM.StopOnMatch=DISABLE;

	TIM_ConfigMatch(LPC_TIM0, &confM);
	TIM_ResetCounter(LPC_TIM0);
	TIM_Cmd(LPC_TIM0, ENABLE);

	return;
}


void confTimer(void){
	LPC_SC->PCONP |= (1 << 1); //Por defecto timer 0 y 1 estan siempre prendidos
	LPC_SC->PCLKSEL0 |= (1<<2); // configuraci�n del clock del perif�rico
	//LPC_PINCON->PINSEL3 |= (3<<24);
	LPC_TIM0->EMR|=(3<<6); //toggle
	LPC_TIM0->MR1 = 100000000/40000;
	LPC_TIM0->MCR = (1<<4); //reset on match
	LPC_TIM0->TCR = 3;      // hab y reset
	LPC_TIM0->TCR &= ~(1<<1);
	return;
}


void ADC_IRQHandler(void)
{
	static uint16_t ADC0Value = 0;
	float volt = 0;
	ADC0Value = ((LPC_ADC->ADDR0)>>4) & 0xFFF; //Variable auxiliar para observar el valor del registro de captura
	FIO_HalfWordSetValue(0, 0, ADC0Value);
	FIO_HalfWordClearValue(0, 0, (~(ADC0Value)));

	if(ADC0Value>2055)
		LPC_GPIO0->FIOSET |= (1<<9);
	else
		LPC_GPIO0->FIOCLR |= (1<<9);
	volt=(ADC0Value/4096)*3.3;
	return;
}

