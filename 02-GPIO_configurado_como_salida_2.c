/*
===============================================================================
 Name        : ejer2.c
 Author      : $Francisco Ciordia Cantarella
 Version     : 1.0V
 Description : main definition
===============================================================================
*/

/*
 * En los pines P2.0 a P2.7 se encuentra conectado un display de 7 segmentos. Utilizando la variable numDisplay [10] ={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 
 * 0x7F, 0x67} que codifica los números del 0 a 9 para ser mostrados en el display, realizar un programa que muestre indefinidamente la cuenta de 0 a 9 en dicho display.
*/


#include "LPC17xx.h"

void delay(int count);

int main(void) {
	uint32_t count=5000000;
	uint8_t numDisplay[10]={0x6D,0x7D,0x3F,0x3F,0x07,0x7F,0x67,0x06,0x5B,0x66};

	LPC_PINCON->PINSEL4 &= ~(0xFFFF);
	LPC_GPIO2->FIODIR0	|= (0x7F);

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
