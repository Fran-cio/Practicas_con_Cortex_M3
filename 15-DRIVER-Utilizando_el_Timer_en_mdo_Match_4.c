/*
 * Utilizando_el_Timer_en_mdo_Match_4.c
 *
 *  Created on: 24 sep. 2021
 *      Author: francio
 */

#include "LPC17xx.h"

#include "lpc17xx_timer.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"


// TODO: insert other definitions and declarations here

#define	OUTPUT	    (uint8_t) 1
#define INPUT	    (uint8_t) 0

#define PIN_22		((uint32_t)(1<<22))
#define PORT_ZERO	(uint8_t)	0
#define PORT_ONE	(uint8_t)	1
#define PORT_TWO	(uint8_t)	2
#define PORT_THREE	(uint8_t)	3

/*Utilizando los 4 registros match del Timer 0 y las salidas P0.0,
 * P0.1, P0.2 y P0.3, realizar un programa en C que permita obtener
 * las formas de ondas adjuntas, donde los pulsos en alto tienen una
 * duración de 5 mseg. Un pulsador conectado a la entrada EINT3,
 * permitirá elegir entre las dos secuencias mediante una rutina
 * de servicio a la interrupción. La prioridad de la interrupción
 * del Timer tiene que ser mayor que la del pulsador. Estas formas
 * de ondas son muy útiles para controlar un motor paso a paso.
 * Adjuntar el código en C .*/

void configGPIO();
void configInt();
void confTimer();
void delay(uint32_t count);

uint32_t const _5mS = 50000000;

uint8_t secuencias=0;

int main(void){

	configGPIO();
	configInt();
	confTimer();

	while(1){
	}

	return 0;

}
void configGPIO(void){
	PINSEL_CFG_Type pinsel0;
	pinsel0.Portnum	=	PINSEL_PORT_0;
	pinsel0.Funcnum	=	PINSEL_FUNC_0;

	for(uint8_t i=0; i<=4;i++){
		pinsel0.Pinnum=	i;
		PINSEL_ConfigPin(&pinsel0);
	}
	FIO_SetDir(PORT_ZERO, 0b11111 , OUTPUT);
	FIO_ByteSetMask(PORT_ZERO, 0, 0xFF, 0);

	LPC_PINCON->PINSEL0 &= ~(0xFF); //los puertos de salida como GPIO
	LPC_GPIO0->FIODIR0  |= (0xF);   //seteo los puertos como salida

	return;
}

void configInt(){
	PINSEL_CFG_Type pinsel2;
	pinsel2.Portnum	=	PINSEL_PORT_2;
	pinsel2.Pinnum	=	PINSEL_PIN_13;
	pinsel2.Funcnum	=	PINSEL_FUNC_1;
	pinsel2.Pinmode	=	PINSEL_PINMODE_PULLUP;

	PINSEL_ConfigPin(&pinsel2);
	FIO_SetMask(PORT_TWO, (1<<13), 0);

	EXTI_InitTypeDef 	exti;
	EXTI_Init();
	exti.EXTI_Line	= 	EXTI_EINT3;
	exti.EXTI_Mode	=	EXTI_MODE_EDGE_SENSITIVE;
	exti.EXTI_polarity	=	EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;

	EXTI_Config(&exti);

	EXTI_ClearEXTIFlag(EXTI_EINT3);

	NVIC_EnableIRQ(EINT3_IRQn);    // Habilita de interrupciones externas.

	return;
}

void confTimer(void){
	TIM_TIMERCFG_Type 	conf_t;
	TIM_MATCHCFG_Type	conf_m;

	conf_t.PrescaleOption	=	TIM_PRESCALE_USVAL;
	conf_t.PrescaleValue	=	100;

	conf_m.ExtMatchOutputType= TIM_EXTMATCH_NOTHING;
	conf_m.IntOnMatch	=	ENABLE;
	conf_m.ResetOnMatch	=	DISABLE;
	conf_m.StopOnMatch	=	DISABLE;

	for(uint8_t	i=0;i<=2;i++){
		conf_m.MatchValue=5000*(i+1);
		conf_m.MatchChannel=i;
		TIM_ConfigMatch(LPC_TIM0, &conf_m);
	}
	conf_m.MatchValue=5000*4;
	conf_m.MatchChannel=3;
	conf_m.ResetOnMatch=ENABLE;
	TIM_Init(LPC_TIM0,  TIM_TIMER_MODE, &conf_t);
	TIM_ConfigMatch(LPC_TIM0, &conf_m);

	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
	TIM_ClearIntPending(LPC_TIM0, TIM_MR1_INT);
	TIM_ClearIntPending(LPC_TIM0, TIM_MR2_INT);
	TIM_ClearIntPending(LPC_TIM0, TIM_MR3_INT);

	TIM_Cmd(LPC_TIM0,ENABLE);
	NVIC_EnableIRQ(TIMER0_IRQn);
	return;
}

void EINT3_IRQHandler(void){
	delay(10000000);
	if (secuencias) {
		secuencias=0;
	}
	else {
		secuencias=1;
	}
	EXTI_ClearEXTIFlag(EXTI_EINT3);
	return;

}
void TIMER0_IRQHandler(void){
	if(secuencias)
	{
		if(TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT))
		{
			FIO_ByteClearValue(PORT_ZERO, 0, (1<<2));
			FIO_ByteSetValue(PORT_ZERO, 0, (1<<0));
			TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
		}
		else if(TIM_GetIntStatus(LPC_TIM0, TIM_MR1_INT))
		{
			FIO_ByteClearValue(PORT_ZERO, 0, (1<<3));
			FIO_ByteSetValue(PORT_ZERO, 0, (1<<1));
			TIM_ClearIntPending(LPC_TIM0, TIM_MR1_INT);
		}
		else if(TIM_GetIntStatus(LPC_TIM0, TIM_MR2_INT))
		{
			FIO_ByteClearValue(PORT_ZERO, 0, (1<<0));
			FIO_ByteSetValue(PORT_ZERO, 0, (1<<2));
			TIM_ClearIntPending(LPC_TIM0, TIM_MR2_INT);
		}
		else if(TIM_GetIntStatus(LPC_TIM0, TIM_MR3_INT))
		{
			FIO_ByteClearValue(PORT_ZERO, 0, (1<<1));
			FIO_ByteSetValue(PORT_ZERO, 0, (1<<3));
			TIM_ClearIntPending(LPC_TIM0, TIM_MR3_INT);
		}
	}
	else
	{
		if(TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT))
		{
			FIO_ByteClearValue(PORT_ZERO, 0, (1<<3));
			FIO_ByteSetValue(PORT_ZERO, 0, (1<<0));
			TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
		}
		else if(TIM_GetIntStatus(LPC_TIM0, TIM_MR1_INT)){
			FIO_ByteClearValue(PORT_ZERO, 0, (1<<0));
			FIO_ByteSetValue(PORT_ZERO, 0, (1<<1));
			TIM_ClearIntPending(LPC_TIM0, TIM_MR1_INT);
		}
		else if(TIM_GetIntStatus(LPC_TIM0, TIM_MR2_INT))
		{
			FIO_ByteClearValue(PORT_ZERO, 0, (1<<1));
			FIO_ByteSetValue(PORT_ZERO, 0, (1<<2));
			TIM_ClearIntPending(LPC_TIM0, TIM_MR2_INT);
		}
		else if(TIM_GetIntStatus(LPC_TIM0, TIM_MR3_INT))
		{
			FIO_ByteClearValue(PORT_ZERO, 0, (1<<2));
			FIO_ByteSetValue(PORT_ZERO, 0, (1<<3));
			TIM_ClearIntPending(LPC_TIM0, TIM_MR3_INT);
		}
	}
}

void delay(uint32_t count){
	for(int cont=0; cont<count;cont++){};
}
