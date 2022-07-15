/*
 * Utilizando_el_Timer_en_modo_Match_3.c
 *
 *  Created on: 23 sep. 2021
 *      Author: francisco
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

/*Escribir un programa para que por cada
 *presión de un pulsador, la frecuencia de
 *parpadeo disminuya a la mitad debido a
 *la modificación del registro del
 *Match 0. El pulsador debe producir
 *una interrupción por EINT2 con
 *flanco descendente. Adjuntar el
 *código en C.
 */


void delay(uint32_t count);
void configGPIO();
void configInt();
void confTimer();

uint32_t initial=1000;
uint8_t cont=1;

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
	pinsel0.Portnum	=	PINSEL_PORT_1;
	pinsel0.Pinnum	=	PINSEL_PIN_22;
	pinsel0.Funcnum	=	PINSEL_FUNC_3;

	PINSEL_ConfigPin(&pinsel0);

	FIO_SetDir(PORT_TWO, 1<<12, INPUT);
	FIO_SetDir(PORT_ONE, 1<<22, OUTPUT);

	return;
}

void confTimer(void){
	TIM_TIMERCFG_Type conft;
	TIM_MATCHCFG_Type confm;

	conft.PrescaleOption	=	TIM_PRESCALE_USVAL;
	conft.PrescaleValue		=	100;

	confm.MatchChannel		=0;
	confm.IntOnMatch		=DISABLE;
	confm.ResetOnMatch		=ENABLE;
	confm.StopOnMatch		=DISABLE;
	confm.ExtMatchOutputType=TIM_EXTMATCH_TOGGLE;
	confm.MatchValue		=initial;

	TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &conft);
	TIM_ConfigMatch(LPC_TIM1,&confm);

	TIM_Cmd(LPC_TIM1,ENABLE);

	 return;
}

void configInt(){
	PINSEL_CFG_Type pinsel2;
	pinsel2.Portnum	=	PINSEL_PORT_2;
	pinsel2.Pinnum	=	PINSEL_PIN_12;
	pinsel2.Pinmode	=	PINSEL_PINMODE_PULLUP;
	pinsel2.Funcnum	=	PINSEL_FUNC_1;

	PINSEL_ConfigPin(&pinsel2);

	EXTI_Init();
	EXTI_InitTypeDef exti;
	exti.EXTI_Line=EXTI_EINT2;
	exti.EXTI_Mode=EXTI_MODE_EDGE_SENSITIVE;
	exti.EXTI_polarity=EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;

	EXTI_Config(&exti);
	EXTI_ClearEXTIFlag(EXTI_EINT2_BIT_MARK);
	NVIC_EnableIRQ(EINT2_IRQn);    // Habilita de interrupciones externas.

	return;
}

void EINT2_IRQHandler(void){
	delay(10000000);
	initial=initial*2;

	TIM_UpdateMatchValue(LPC_TIM1, 0, initial);

	TIM_ResetCounter(LPC_TIM1);
	EXTI_ClearEXTIFlag(EXTI_EINT2_BIT_MARK);
	return;
}
void delay(uint32_t count){
	for(int cont=0; cont<count;cont++){};
}
