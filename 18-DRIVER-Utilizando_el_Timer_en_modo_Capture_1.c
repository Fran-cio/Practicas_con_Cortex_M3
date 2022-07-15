/*
 * 18-DRIVER-Utilizando_el_Timer_en_modo_Capture_1.c
 *
 *  Created on: 9 oct. 2021
 *      Author: francio
 */
#include "math.h"

#include "LPC17xx.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"

void configgpio(void);
void configtimer(void);
uint32_t tiempo;
uint32_t cr0;
uint32_t cr1;

int main(void)
{
	SystemInit();
	cr0=0,cr1=0;
	tiempo=0;
	configgpio();
	configtimer();

	while(1)
	{
		if(cr0<cr1)
		{
			tiempo = (cr1-cr0);
		}
		else
		{
			tiempo = (cr0-cr1);
		}
	}
	return 0;
}
void configgpio()
{
	PINSEL_CFG_Type pinsel;
	pinsel.Portnum=PINSEL_PORT_1;
	pinsel.Funcnum=PINSEL_FUNC_3;

	for(uint8_t i=26;i<=27;i++)
	{
		pinsel.Pinnum=i;
		PINSEL_ConfigPin(&pinsel);
	}
}
void configtimer()
{
	LPC_TIM0->MCR=0;
	LPC_SC->PCLKSEL0    |= (1<<2);
	TIM_CAPTURECFG_Type configCap;
	TIM_TIMERCFG_Type configT;

	configT.PrescaleOption= TIM_PRESCALE_USVAL;
	configT.PrescaleValue=1000;
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &configT);

	configCap.FallingEdge=ENABLE,
	configCap.RisingEdge=DISABLE;
	configCap.IntOnCaption=ENABLE;
	for (uint8_t i= 0; i <= 1; ++i)
	{
		configCap.CaptureChannel=i;
		TIM_ConfigCapture(LPC_TIM0, &configCap);
	}
	TIM_ResetCounter(LPC_TIM0);
	TIM_Cmd(LPC_TIM0,ENABLE);

	NVIC_EnableIRQ(TIMER0_IRQn);

	return;
}
void TIMER0_IRQHandler(void){
	if(TIM_GetIntStatus(LPC_TIM0, TIM_CR0_INT))
	{
		cr0=TIM_GetCaptureValue(LPC_TIM0,0);
		TIM_ClearIntPending(LPC_TIM0, TIM_CR0_INT);
	}
	if(TIM_GetIntStatus(LPC_TIM0, TIM_CR1_INT))
	{
		cr1=TIM_GetCaptureValue(LPC_TIM0,1);
		TIM_ClearIntPending(LPC_TIM0, TIM_CR1_INT);
	}
}
