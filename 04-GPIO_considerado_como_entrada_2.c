/*
 * Ejer2GpioEntrada.c
 *
 *  Created on: 1 sep. 2021
 *      Author: francisco
 */

/*
 *Considerando pulsadores normalmente abiertos conectados en un extremo a
 * masa y en el otro directamente a las entradas P0.0 y p0.1. Realizar un 
 * programa que identifique en una variable cual o cuales pulsadores han 
 * sido presionados. Las identificaciones posibles a implementar en esta 
 * variable van a ser "ninguno", "pulsador 1", "pulsador 2", "pulsador 1 y 
 * 2". 
*/
#include "LPC17xx.h"

int main(void){

	uint8_t pulsador1 = 0;
	uint8_t pulsador2 = 0;
	uint8_t pulsador1ypulsador2 = 0;
	uint8_t ninguno = 0;

	LPC_PINCON->PINSEL0 &= ~(0xF) ;

	LPC_GPIO0->FIODIR0  &= ~(0x3);

	LPC_PINCON->PINMODE0 &= ~(0xF);

	while(1){
		if(~(LPC_GPIO0->FIOPIN0)&(1)&&~(LPC_GPIO0->FIOPIN0)&(2)){
			pulsador1 = 0;
			pulsador2 = 0;
			ninguno = 0;

            pulsador1ypulsador2=1;
		}
		else if(~(LPC_GPIO0->FIOPIN0)&(1)){
			pulsador2 = 0;
			pulsador1ypulsador2 = 0;
			ninguno = 0;

			pulsador1=1;
		}
		else if(~(LPC_GPIO0->FIOPIN0)&(2)){
			pulsador1 = 0;
			pulsador1ypulsador2 = 0;
			ninguno = 0;

			pulsador2=1;
		}
		else{
			pulsador1 = 0;
			pulsador2 = 0;
			pulsador1ypulsador2 = 0;

			ninguno=1;
		}

	}

	return 0;
}

