/*
 * interrupción_externa.c
 *
 *  Created on: 9 sep. 2021
 *      Author: francio
 */

/*
 * Configurar la interrupción externa EINT1 para que interrumpa por flanco 
 * de bajada y la interrupción EINT2 para que interrumpa por flanco de 
 * subida. En la interrupción por flanco de bajada configurar el systick 
 * para desbordar cada 25 mseg, mientras que en la interrupción por flanco 
 * de subida configurarlo para que desborde cada 60 mseg. Considerar que 
 * EINT1 tiene mayor prioridad que EINT2.
 */

#include "LPC17xx.h"

#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_systick.h"


// TODO: insert other definitions and declarations here

#define	OUTPUT	    (uint8_t) 1
#define INPUT	    (uint8_t) 0

#define PIN_22		((uint32_t)(1<<22))
#define PORT_ZERO	(uint8_t)	0
#define PORT_ONE	(uint8_t)	1
#define PORT_TWO	(uint8_t)	2
#define PORT_THREE	(uint8_t)	3

void delay(int count);
void configGPIO();
void configInt();

uint16_t CONT=0;

int main(void){
	configInt();

	while(1){

	}

	return 0;
}

void configInt(){
	PINSEL_CFG_Type pinsel;
	pinsel.Portnum=PORT_TWO;
	pinsel.Funcnum=PINSEL_FUNC_1;
	pinsel.Pinmode=PINSEL_PINMODE_PULLUP;

	for(uint8_t i=11;i<=12;i++){
		pinsel.Pinnum	=	i;
		PINSEL_ConfigPin(&pinsel);
	}

	EXTI_Init();
	EXTI_InitTypeDef exti;
	exti.EXTI_Mode=EXTI_MODE_EDGE_SENSITIVE;
	exti.EXTI_polarity=EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;
	for(uint8_t i=1;i<=2;i++){
		exti.EXTI_Line=i;
		EXTI_Config(&exti);
	}

	NVIC_EnableIRQ(EINT1_IRQn);    // Habilita de interrupciones externas.
	NVIC_EnableIRQ(EINT2_IRQn);    // Habilita de interrupciones externas.

	NVIC_SetPriority (EINT1_IRQn, 30 );
	NVIC_SetPriority (EINT2_IRQn, 31 );

	SYSTICK_Cmd(1);
	SYSTICK_IntCmd(1);

	return;
}

void SysTick_Handler(void) {
	CONT++;
	SYSTICK_ClearCounterFlag();
	return;
}

void EINT1_IRQHandler(void){
	//Colocar un debounce aca
	EXTI_ClearEXTIFlag(EXTI_EINT1_BIT_MARK);  //Limpia bandera de interrupci�n
	SYSTICK_InternalInit(25);
	return;
}

void EINT2_IRQHandler(void){
	EXTI_ClearEXTIFlag(EXTI_EINT2_BIT_MARK);  //Limpia bandera de interrupci�n
	SYSTICK_InternalInit(60);
	return;
}


