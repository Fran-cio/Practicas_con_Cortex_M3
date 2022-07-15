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
	LPC_PINCON->PINSEL0 &=~ (0x3) ;
	LPC_PINCON->PINSEL4 &=~ (0x3) ;

	LPC_GPIO0->FIODIR0 &=~ (0x1); //Configuro salida
	LPC_GPIO2->FIODIR0 &=~ (0x1); //Configuro entrada

	LPC_PINCON->PINMODE0 |= 0x3;
	LPC_PINCON->PINMODE4 |= 0x3; //Activo pulldown del puerto 1 alto

	return;
}

void configInt(){
	LPC_GPIOINT->IO0IntEnR |= (0x1);
	LPC_GPIOINT->IO0IntClr |= (0x1);

	LPC_GPIOINT->IO2IntEnR |= (0x1);
	LPC_GPIOINT->IO2IntClr |= (0x1);

	NVIC_EnableIRQ(EINT3_IRQn);

	return;
}

void EINT3_IRQHandler(void){

	if(LPC_GPIOINT->IO2IntStatR&(1)){
		aux=111001011010110;
	}
	else if(LPC_GPIOINT->IO0IntStatR&(1)){
		aux=100111;
	}
	LPC_GPIOINT->IO0IntClr |= (0x1);
	LPC_GPIOINT->IO2IntClr |= (0x1);

	return;
}
