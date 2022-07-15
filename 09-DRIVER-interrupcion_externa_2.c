/*
 * interrupcion_externa_2.c
 *
 *  Created on: 15 sep. 2021
 *      Author: francio
 */


/*Escribir un programa en C que permita sacar por los pines P1.16
 * al P1.23 el equivalente en ascii de "a" si la interrupción se
 * ha realizado por interrupción de EINT1 y no hay interrupción
 * pendiente de EINT0. Para el caso en el cual se produzca una
 * interrupción por EINT1 y exista una interrupción pendiente
 * de EINT0 sacar por el  puerto el equivalente en ascii de
 * "A". La interrupción de EINT1 es por el flanco de subida
 * producido por un pulsador identificado como "tecla a/A",
 * mientras que la interrupción EINT0 es por el nivel alto de
 * un pulsador etiquetado como "Activación de Mayusculas".
 * */

/*Nota: Valerse de la configuración de los niveles de prioridad
 * para que la pulsación conjunta de "Activación de Mayúsculas"
 * con "tecla a/A" de como resultado a la salida el equivalente
 * en ascii "A".
 * */


#include "LPC17xx.h"

#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_exti.h"


// TODO: insert other definitions and declarations here

#define	OUTPUT	    (uint8_t) 1
#define INPUT	    (uint8_t) 0

#define PIN_22		((uint32_t)(1<<22))
#define PORT_ZERO	(uint8_t)	0
#define PORT_ONE	(uint8_t)	1
#define PORT_TWO	(uint8_t)	2
#define PORT_THREE	(uint8_t)	3

void config_GPIO(void);
void config_timer(void);

uint8_t const tecla_a=(int)'a';
uint8_t const tecla_A=(int)'A';

void ConfigINT();
void ConfigGPIO();

int main (void){
	ConfigGPIO();
	ConfigINT();

	while(1){}

	return 0;
}

void ConfigGPIO(){
	PINSEL_CFG_Type pinsel_0;
	pinsel_0.Portnum	=	PINSEL_PORT_1;
	pinsel_0.Funcnum	=	PINSEL_FUNC_0;

	for(uint8_t i=16;i<=23;i++){
		pinsel_0.Pinmode	=	i;
		PINSEL_ConfigPin(&pinsel_0);
	}
	FIO_SetDir(PORT_ONE,(0b11111111<<16),OUTPUT);

	return;
}

void ConfigINT(){
	//seleciono las interrupciónes
	PINSEL_CFG_Type pinsel_1;
	pinsel_1.Portnum	=	PINSEL_PORT_2;
	pinsel_1.Funcnum	=	PINSEL_FUNC_1;
	pinsel_1.Pinmode	=	PINSEL_PINMODE_PULLDOWN;

	for(uint8_t i=10;i<=11;i++){
			pinsel_1.Pinnum	=	i;
			PINSEL_ConfigPin(&pinsel_1);
	}
	EXTI_Init();
	EXTI_InitTypeDef exti;
	exti.EXTI_polarity	=	EXTI_POLARITY_HIGH_ACTIVE_OR_RISING_EDGE;

	exti.EXTI_Line	=	EXTI_EINT0;
	exti.EXTI_Mode	=	EXTI_MODE_LEVEL_SENSITIVE;

	EXTI_Config(&exti);

	exti.EXTI_Line	=	EXTI_EINT1;
	exti.EXTI_Mode	=	EXTI_MODE_EDGE_SENSITIVE;

	EXTI_Config(&exti);

	NVIC_SetPriority(EINT0_IRQn, NVIC_GetPriority(EINT1_IRQn)+1);

	NVIC_EnableIRQ(EINT1_IRQn);
	NVIC_EnableIRQ(EINT0_IRQn);


	return;
}
void EINT0_IRQHandler(){}

void EINT1_IRQHandler(){
	if(NVIC_GetPendingIRQ(EINT0_IRQn)){
		LPC_GPIO1 -> FIOPIN1 = tecla_A;
	}else{
		LPC_GPIO1 -> FIOPIN1 = tecla_a;
	}
	EXTI_ClearEXTIFlag(EXTI_EINT0_BIT_MARK);
	EXTI_ClearEXTIFlag(EXTI_EINT1_BIT_MARK);

	return;
}
