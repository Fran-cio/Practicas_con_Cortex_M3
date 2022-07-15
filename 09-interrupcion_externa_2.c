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
	LPC_PINCON ->PINSEL3 &= ~(0XFFFF);//config gpio std
	LPC_GPIO1 -> FIODIR1 = 0xF;//pines de 1.16 a 1.23 como salida

	return;
}

void ConfigINT(){
	//seleciono las interrupciónes
	LPC_PINCON ->PINSEL4 |= (0b0101<<20);
	LPC_PINCON ->PINSEL4 &= (0b0101<<20);

	LPC_SC->EXTINT 	|= 0b11;
	LPC_SC->EXTMODE	|= (1<<1);
	LPC_SC->EXTMODE	&= ~(1);
	LPC_SC ->EXTPOLAR |= 0b11;

	NVIC_EnableIRQ(EINT0_IRQn);
	NVIC_EnableIRQ(EINT1_IRQn);

	NVIC_SetPriority(EINT0_IRQn, NVIC_GetPriority(EINT1_IRQn)+1);

	return;
}
void EINT0_IRQHandler(){}

void EINT1_IRQHandler(){
	if(NVIC_GetPendingIRQ(EINT0_IRQn)){
		LPC_GPIO2 -> FIOPIN1 = tecla_A;
	}else{
		LPC_GPIO2 -> FIOPIN1 = tecla_a;
	}
	LPC_SC->EXTINT 	|= 0b11;

	return;
}
