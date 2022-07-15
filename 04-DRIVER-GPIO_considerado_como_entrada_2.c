
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

int main(void)
{

	uint8_t pulsador1 = 0;
	uint8_t pulsador2 = 0;
	uint8_t pulsador1ypulsador2 = 0;
	uint8_t ninguno = 0;

	PINSEL_CFG_Type pinsel;
	pinsel.Portnum	=	PINSEL_PORT_0;
	pinsel.Pinmode	=	PINSEL_PINMODE_PULLUP;
	pinsel.Funcnum	=	PINSEL_FUNC_0;

	for(uint8_t i=0;i<=1;i++){
		pinsel.Pinnum	=	i;
		PINSEL_ConfigPin(&pinsel);
	}

	FIO_ByteSetDir(PORT_ZERO,0, 0b11, INPUT);

	while(1){
		if(~(FIO_ByteReadValue(PORT_ZERO,0))&(1)&&
		   ~(FIO_ByteReadValue(PORT_ZERO,0))&(2))
		{
			pulsador1 = 0;
			pulsador2 = 0;
			ninguno = 0;

            pulsador1ypulsador2=1;
		}
		else if(~(FIO_ByteReadValue(PORT_ZERO,0))&(1))
		{
			pulsador2 = 0;
			pulsador1ypulsador2 = 0;
			ninguno = 0;

			pulsador1=1;
		}
		else if(~(FIO_ByteReadValue(PORT_ZERO,0))&(2))
		{
			pulsador1 = 0;
			pulsador1ypulsador2 = 0;
			ninguno = 0;

			pulsador2=1;
		}
		else
		{
			pulsador1 = 0;
			pulsador2 = 0;
			pulsador1ypulsador2 = 0;

			ninguno=1;
		}

	}

	return 0;
}

