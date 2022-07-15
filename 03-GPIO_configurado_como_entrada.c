/*
 * Ejer3Gpioentrada.c
 *
 *  Created on: 26 ago. 2021
 *      Author: francisco
 */

/*
 * Configurar el pin P0.4 como entrada digital con resistencia de pull up
 * y utilizarlo para decidir si el valor representado por los pines P0.0
 * al P0.3 van a ser sumados o restados al valor guardado en la variable
 * "acumulador".El valor inicial de "acumulador" es 0.
*/

#include "LPC17xx.h"

int main(void){

	uint8_t acumulador;

	LPC_PINCON->PINSEL0 &=~(0x3FF) ;

	LPC_GPIO0->FIODIR0  &=~(0x1F);

	LPC_PINCON->PINMODE0 &= ~(0x300);

	while(1){
		LPC_GPIO0->FIOMASK0=0;
		if(LPC_GPIO0->FIOPIN0&(1<<5)){
            LPC_GPIO0->FIOMASK0=~(0x000F);
			acumulador+=(LPC_GPIO0->FIOPIN0);
		}
		else{
			LPC_GPIO0->FIOMASK0=~(0x000F);
			acumulador-=(LPC_GPIO0->FIOPIN0&(0xF));
		}

	}

	return 0;
}
