/*
 * Systick_2.c
 *
 *  Created on: 8 sep. 2021
 *      Author: francisco
 */

#include "LPC17xx.h"

int8_t DUTY_CYCLE=1;
int8_t CONT=0;

void delay(int count);
void configGPIO();
void configInt();

int main(void){

	//configuro GPIO
	configGPIO();
	configInt();

	while(1){
		if(CONT<DUTY_CYCLE){
			LPC_GPIO0->FIOPIN2 = 0xFF;
		}
		else{
			LPC_GPIO0->FIOPIN2 = 0;
		}
	}

	return 0;
}

void configGPIO(){
	LPC_PINCON->PINSEL1 &=~ (3<<12) ;

	LPC_GPIO0->FIODIR2 |=  (1<<6); //Configuro salida

	LPC_PINCON->PINMODE4 |= (3<<21);

	LPC_GPIO0->FIOMASK2	= ~(1<<6);
	LPC_GPIO2->FIOMASK1	= ~(1<<2);

	return;
}

void configInt(){
	LPC_PINCON->PINSEL4 |= (1<<20) ;

	LPC_SC->EXTINT      |= 1;   //Limpia bandera de interrupci�n
	LPC_SC->EXTMODE     |= 1; //Selecciona interrupcion por flanco
	LPC_SC->EXTPOLAR    |= 1; //Interrumpe cuando el flanco es de subida
	NVIC_EnableIRQ(EINT0_IRQn);    // Habilita de interrupciones externas.
	NVIC_SetPriority (EINT0_IRQn, 31 );
	//NVIC_SetPriority();

	SysTick_Config(SystemCoreClock/1000);
	NVIC_SetPriority (SysTick_IRQn, (NVIC_GetPriority(EINT0_IRQn)-1) );

	return;
}

void SysTick_Handler(void) {
	CONT++;
	if(CONT==10){
		CONT=0;
	}
	SysTick->CTRL &= SysTick->CTRL;
	return;
}

void EINT0_IRQHandler(void){
	DUTY_CYCLE++;
	LPC_SC->EXTINT |= 1;   //Limpia bandera de interrupci�n
	if(DUTY_CYCLE>10){
		DUTY_CYCLE=1;
	}
	return;
}

void delay(int count){
	for(int cont=0; cont<count;cont++){};
}
