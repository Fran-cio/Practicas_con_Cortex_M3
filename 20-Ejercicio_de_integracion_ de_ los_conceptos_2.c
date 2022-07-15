/*
 * Ejercicio_de_integracion_ de_ los_conceptos_2.c
 *
 *  Created on: 13 oct. 2021
 *      Author: francisco
 */

#include "LPC17xx.h"

#include "lpc17xx_adc.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_exti.h"



/*
 * Realizar un código en C que permita filtrar las muestras provenientes del ADC con un filtro
 * promediador móvil de N muestras, donde  N inicialmente es igual a 2 y puede ser incrementado
 * en dos unidades cada vez que se presiona un pulsador conectado en P0.6 hasta llegar a un valor
 * de 600.  A su vez, utilizando el bit de overrun y un timer, complete el código realizado para
 * que en caso de producirse el evento de pérdida de datos por parte del ADC, se deje de muestrear
 * y se saque por el pin MATCH2.1 una señal cuadrada de 440 Hz. Considerar una frecuencia de cclk
 * de 60 Mhz y configurar el ADC para obtener una frecuencia de muestreo de 5 Kmuestras/seg.
 */

void confGPIO(void); // Prototipo de la funci�n de conf. de puertos
void confInt(void);
void confADC_Driver(void);
void confTimer_Driver(void);
void confTimer2_Driver(void);

uint16_t 	temp[600],
			N=2,
			i=0,
			promedio=0;

int main(void) {
	confGPIO();
	confInt();
	confTimer_Driver();
	confADC_Driver();
	while(1)
	{
		if(ADC_GlobalGetStatus(LPC_ADC, 0) && N==1)
		{
			TIM_Cmd(LPC_TIM0, DISABLE);
			CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCTIM0, DISABLE);

			ADC_PowerdownCmd(LPC_ADC, 0);
			CLKPWR_ConfigPPWR(CLKPWR_PCLKSEL_ADC, DISABLE);

			NVIC_DisableIRQ(ADC_IRQn);

			confTimer2_Driver();
			N=1;
		}

	}
	return 0;
}

void confInt(void)
{
	FIO_IntCmd(PINSEL_PORT_0, (1<<6), 0);
	FIO_ClearInt(PINSEL_PORT_0, (1<<6));

	NVIC_EnableIRQ(EINT3_IRQn);

	return;
}

void confGPIO(void){
	PINSEL_CFG_Type pinsel;
	pinsel.Portnum=PINSEL_PORT_0;
	pinsel.Funcnum=PINSEL_FUNC_0;
	pinsel.Pinnum=PINSEL_PIN_6;

	PINSEL_ConfigPin(&pinsel);

	pinsel.Portnum=PINSEL_PORT_0;
	pinsel.Pinnum=PINSEL_PIN_7;
	pinsel.Funcnum=PINSEL_FUNC_3;

	PINSEL_ConfigPin(&pinsel);

	GPIO_SetDir(0, (1<<6), 1);
	FIO_SetMask(0, (1<<6), 0);

	return;
}

void confADC_Driver(void)
{
	CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_ADC, 3);

	ADC_Init(LPC_ADC, 200000);
	ADC_IntConfig(LPC_ADC, ADC_ADGINTEN, SET);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);
	ADC_BurstCmd(LPC_ADC, DISABLE);
	ADC_StartCmd(LPC_ADC, ADC_START_ON_MAT01);
	ADC_EdgeStartConfig(LPC_ADC, ADC_START_ON_FALLING);

	PINSEL_CFG_Type pinsel;
	pinsel.Portnum=PINSEL_PORT_0;
	pinsel.Pinnum=PINSEL_PIN_23;
	pinsel.Funcnum=PINSEL_FUNC_1;
	pinsel.Pinmode=PINSEL_PINMODE_TRISTATE;
	PINSEL_ConfigPin(&pinsel);

	NVIC_EnableIRQ(ADC_IRQn);
	return;
}

void confTimer_Driver(void)
{
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCTIM0, ENABLE);
	CLKPWR_SetPCLKDiv(CLKPWR_PCONP_PCTIM0, CLKPWR_PCLKSEL_CCLK_DIV_1);
	TIM_MATCHCFG_Type confM;
	TIM_TIMERCFG_Type	confT;

	confT.PrescaleOption=TIM_PRESCALE_TICKVAL;
	confT.PrescaleValue=1;

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &confT);

	confM.ExtMatchOutputType=TIM_EXTMATCH_TOGGLE;
	confM.IntOnMatch=DISABLE;
	confM.MatchChannel=1;
	confM.MatchValue=60000000/10000;
	confM.ResetOnMatch=ENABLE;
	confM.StopOnMatch=DISABLE;

	TIM_ConfigMatch(LPC_TIM0, &confM);
	TIM_ResetCounter(LPC_TIM0);
	TIM_Cmd(LPC_TIM0, ENABLE);

	return;
}

void confTimer2_Driver(void)
{
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCTIM2, ENABLE);
	CLKPWR_SetPCLKDiv(CLKPWR_PCONP_PCTIM2, CLKPWR_PCLKSEL_CCLK_DIV_1);
	TIM_MATCHCFG_Type confM;
	TIM_TIMERCFG_Type	confT;

	confT.PrescaleOption=TIM_PRESCALE_TICKVAL;
	confT.PrescaleValue=1;

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &confT);

	confM.ExtMatchOutputType=TIM_EXTMATCH_TOGGLE;
	confM.IntOnMatch=DISABLE;
	confM.MatchChannel=1;
	confM.MatchValue=60000000/880;
	confM.ResetOnMatch=ENABLE;
	confM.StopOnMatch=DISABLE;

	TIM_ConfigMatch(LPC_TIM0, &confM);
	TIM_ResetCounter(LPC_TIM0);
	TIM_Cmd(LPC_TIM0, ENABLE);

	TIM_Cmd(LPC_TIM0, DISABLE);
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCTIM0, DISABLE);
	return;
}

void EINT3_IRQHandler()
{
	if(N<=600)
		N+=2;

	FIO_ClearInt(PINSEL_PORT_0, (1<<6));

	return;
}

void ADC_IRQHandler(void)
{
	promedio=0;
	static uint16_t ADC0Value = 0;
	ADC0Value = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);
	temp[i]=ADC0Value;

	for(uint16_t j=0; j<N;j++)
	{
		promedio+=temp[j];
	}
	promedio/=N;

	i++;
	if(i>=N){
		i=0;
	}

	return;
}

