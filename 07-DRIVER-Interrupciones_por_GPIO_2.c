/*
 * EjerInterrup1.c
 *
 *  Created on: 1 sep. 2021
 *      Author: francisco
 */

/*
 * Realizar un programa que configure el puerto P0.0 y P2.0 para que 
 * provoquen una interrupción por flanco de subida. Si la interrupción es 
 * por P0.0 guardar el valor binario 100111 en la variable "auxiliar", si 
 * es por P2.0 guardar el valor binario 111001011010110.
 */

#include "LPC17xx.h"

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

int16_t aux;

void delay(int count);
void configGPIO();
void configInt();

int main(void){

	//configuro GPIO
	configGPIO();
	configInt();

	while(1){

	}

	return 0;
}

void configGPIO(){
	PINSEL_CFG_Type pinsel_0;

	pinsel_0.Portnum	=	PINSEL_PORT_0;
	pinsel_0.Pinnum		=	PINSEL_PIN_0;
	pinsel_0.Pinmode	=	PINSEL_PINMODE_PULLDOWN;
	pinsel_0.Funcnum	=	PINSEL_FUNC_0;

	PINSEL_ConfigPin(&pinsel_0);

	pinsel_0.Portnum	=	PINSEL_PORT_2;
	PINSEL_ConfigPin(&pinsel_0);

	FIO_SetDir(PORT_ZERO,1,INPUT);
	FIO_SetDir(PORT_TWO,1,INPUT);

	return;
}

void configInt(){
	FIO_IntCmd(PORT_ZERO,1,0);
	FIO_IntCmd(PORT_TWO,1,0);

	FIO_IntCmd(PORT_ZERO,0,1);
	FIO_IntCmd(PORT_TWO,0,1);

	FIO_ClearInt(PORT_ZERO,1);
	FIO_ClearInt(PORT_TWO,1);

	NVIC_EnableIRQ(EINT3_IRQn);

	return;
}

void EINT3_IRQHandler(void){

	if(FIO_GetIntStatus(PORT_TWO,1,0)){
		aux=111001011010110;
	}
	else if(FIO_GetIntStatus(PORT_ZERO,1,0)){
		aux=100111;
	}
	FIO_ClearInt(PORT_ZERO,1);
	FIO_ClearInt(PORT_TWO,1);

	return;
}
