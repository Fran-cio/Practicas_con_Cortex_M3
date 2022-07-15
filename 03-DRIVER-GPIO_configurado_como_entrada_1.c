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

int main(void){

	uint8_t acumulador;

	PINSEL_CFG_Type pinsel;
	pinsel.Portnum	=	PINSEL_PORT_0;
	pinsel.Pinmode	=	PINSEL_PINMODE_PULLUP;
	pinsel.Funcnum	= 	PINSEL_FUNC_0;

	for(uint8_t i=0;i<=4;i++){
		pinsel.Pinnum	=	i;
		PINSEL_ConfigPin(&pinsel);
	}

	FIO_ByteSetDir(PORT_ZERO,0,0b11111,INPUT	);


	while(1){
		if(FIO_ByteReadValue(PORT_ZERO, 0)&(1<<4)){
			acumulador+=FIO_ByteReadValue(PORT_ZERO, 0)&(0xF);
		}
		else{
			acumulador-=FIO_ByteReadValue(PORT_ZERO, 0)&(0xF);
		}

	}

	return 0;
}
