/*
 * tp_siscum.c
 *
 *  Created on: 8 jun. 2022
 *      Author: gatu-
 */
#include "LPC17xx.h"

#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_gpio.h"

#define De_mS_uS(num) (num*1000)

#define _ADC_INT    ADC_ADINTEN2
#define _ADC_CHANNEL    ADC_CHANNEL_2
#define De_mS_A_Cuentas_For(num) ((num*SystemCoreClock)/(1000*11))


__IO uint32_t adc_value;
uint32_t initial=1;
uint8_t aux;

void ADC_IRQHandler(void);
void TIMER1_IRQHandler(void);
void EINT0_IRQHandler(void);

void configPin(void);
void configADC(void);
void confTimer(void);
void configInt();
void delay_t(uint32_t tiempo_mS);

int main (void)
{
  configPin();
  configADC();
  confTimer();
  configInt();

  while(1)
  {
    // Start conversion

  }

  return 0;
}

void configInt(){
	PINSEL_CFG_Type pinsel2;
	pinsel2.Portnum	=	PINSEL_PORT_2;
	pinsel2.Pinnum	=	PINSEL_PIN_10;
	pinsel2.Pinmode	=	PINSEL_PINMODE_PULLDOWN;
	pinsel2.Funcnum	=	PINSEL_FUNC_1;

	PINSEL_ConfigPin(&pinsel2);

	EXTI_Init();
	EXTI_InitTypeDef exti;
	exti.EXTI_Line=EXTI_EINT0;
	exti.EXTI_Mode=EXTI_MODE_EDGE_SENSITIVE;
	exti.EXTI_polarity=EXTI_POLARITY_HIGH_ACTIVE_OR_RISING_EDGE;

	EXTI_Config(&exti);
	EXTI_ClearEXTIFlag(EXTI_EINT0_BIT_MARK);
	NVIC_EnableIRQ(EINT0_IRQn);    // Habilita de interrupciones externas.

	return;
}

void configPin()
{
	PINSEL_CFG_Type pinsel;
	pinsel.Portnum=PINSEL_PORT_0;
	pinsel.Pinmode=PINSEL_PINMODE_TRISTATE;

	for (uint8_t i = 0; i < 12; ++i)
	{
		pinsel.Pinnum=i;
		PINSEL_ConfigPin(&pinsel);
	}
	GPIO_SetDir(0, 0xFFF, 1);
	FIO_HalfWordSetMask(0, 0, (~(0xFFF)), 1);

	pinsel.Portnum=PINSEL_PORT_2;
	pinsel.Pinmode=PINSEL_PINMODE_PULLDOWN;
	pinsel.Funcnum=PINSEL_FUNC_0;
	pinsel.Pinnum=0;
	PINSEL_ConfigPin(&pinsel);

	GPIO_SetDir(2, 1, 1);

	return;
}

void confTimer(void){
	TIM_TIMERCFG_Type conft;
	TIM_MATCHCFG_Type confm;

	conft.PrescaleOption	=	TIM_PRESCALE_USVAL;
	conft.PrescaleValue		=	 De_mS_uS(1000);

	confm.MatchChannel		=	0;
	confm.IntOnMatch		=	ENABLE;
	confm.ResetOnMatch		=	ENABLE;
	confm.StopOnMatch		=	DISABLE;
	confm.ExtMatchOutputType=	TIM_EXTMATCH_NOTHING;
	confm.MatchValue		=	initial;

	TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &conft);
	TIM_ConfigMatch(LPC_TIM1,&confm);
	TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

	TIM_Cmd(LPC_TIM1,ENABLE);
	NVIC_EnableIRQ(TIMER1_IRQn);

	 return;
}

void TIMER1_IRQHandler(void){
	LPC_GPIO2->FIOSET |= (1);
	delay_t(1);
	LPC_GPIO2->FIOCLR |= (1);

	TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);
	ADC_StartCmd(LPC_ADC, ADC_START_NOW);
}

void configADC(){
	ADC_Init(LPC_ADC, 192307);
	ADC_IntConfig(LPC_ADC, ADC_ADGINTEN, SET);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);

	//ADC_BurstCmd(LPC_ADC, 1);
	ADC_BurstCmd(LPC_ADC, 0);
	ADC_EdgeStartConfig(LPC_ADC, ADC_START_ON_FALLING);

	PINSEL_CFG_Type pinsel;
	pinsel.Portnum=PINSEL_PORT_0;
	pinsel.Pinmode=PINSEL_PINMODE_TRISTATE;
	pinsel.Pinnum=PINSEL_PIN_23;
	pinsel.Funcnum=PINSEL_FUNC_1;
	PINSEL_ConfigPin(&pinsel);

	NVIC_EnableIRQ(ADC_IRQn);
	return;
}

void ADC_IRQHandler(void)
{
	static uint16_t ADC0Value = 0;

	ADC0Value = ((LPC_ADC->ADDR0)>>4) & 0xFFF; //Variable auxiliar para observar el valor del registro de captura
	FIO_HalfWordSetValue(0, 0, ADC0Value);
	FIO_HalfWordClearValue(0, 0, (~(ADC0Value)));

	return;
}
void delay_t(uint32_t tiempo_mS)
{
	/*
	 * Cada ciclo for tarda 1.1*10 a la -7 segundos. Entonces hago la regla de 3
	 *
	 * 1.1/100M -> 	1
	 * tiempo 	->	x
	 *
	 * entonces:
	 *
	 * x=tiempo*100M / 1.1 que para evitar el punto flotante hago = tiempo*100M*10 / 11.
	 *
	 * Para simplificar calculos, el valor ingresado es directamente el valor en mS
	 */
	tiempo_mS = De_mS_A_Cuentas_For(tiempo_mS);
	for(uint32_t cont=0; cont<tiempo_mS;cont++);
}

void delay(uint32_t count){
	for(int cont=0; cont<count;cont++){};
}

void EINT0_IRQHandler(void){
	delay(10000000);

	if(aux == initial)
	{
		aux = initial+1;
	}
	else if(aux == initial+1)
	{
		aux = initial+2;
	}
	else if(aux == initial+2)
	{
		aux = initial+4;
	}
	else
	{
		aux = initial;
	}

	TIM_UpdateMatchValue(LPC_TIM1, 0, aux);

	TIM_ResetCounter(LPC_TIM1);
	EXTI_ClearEXTIFlag(0);
	return;
}
