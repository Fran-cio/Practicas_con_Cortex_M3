/*
===============================================================================
 Name        : ejer2.c
 Author      : $Francisco Ciordia Cantarella
 Version     : 1.0V
 Description : main definition
===============================================================================
*/

/*
 * En los pines P2.0 a P2.7 se encuentra conectado un display de 7
 * segmentos. Utilizando la variable numDisplay
 * [10] ={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
 * 0x7F, 0x67} que codifica los números del 0 a 9 para ser
 * mostrados en el display, realizar un programa que muestre
 *  indefinidamente la cuenta de 0 a 9 en dicho display.
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

void delay(int count);

int main(void) {
	uint32_t count=5000000;
	uint8_t numDisplay[10]={0x6D,0x7D,0x3F,0x3F,0x07,0x7F,0x67,0x06,0x5B,0x66};
	PINSEL_CFG_Type pin_sel;

	pin_sel.Portnum	=	PINSEL_PORT_2;
	pin_sel.Funcnum	= 	PINSEL_FUNC_0;

	for(uint8_t i=0;i<=6;i++){
		pin_sel.Pinnum	=	i;
		PINSEL_ConfigPin(&pin_sel);
	}

	FIO_ByteSetDir(PORT_TWO,0,0x7F,OUTPUT);
	FIO_ByteSetMask(PORT_TWO,0,(~(0x7F)),1);

	while(1){
		for(int i=0;i<10;i++){
			LPC_GPIO2->FIOPIN0=numDisplay[i];
			delay(count);
		}
	}
    return 0 ;
}

void delay(int count){
	for(int cont=0; cont<count;cont++){};
}
