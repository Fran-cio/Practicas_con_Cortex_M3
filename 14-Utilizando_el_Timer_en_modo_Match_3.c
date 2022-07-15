/*
 * Utilizando_el_Timer_en_modo_Match_3.c
 *
 *  Created on: 23 sep. 2021
 *      Author: francisco
 */

#include "LPC17xx.h"

//Escribir un programa para que por cada presión de un pulsador, la frecuencia de parpadeo disminuya a la mitad debido a
//la modificación del registro del Match 0. El pulsador debe producir una interrupción por EINT2 con flanco descendente. Adjuntar el código en C.


void delay(uint32_t count);
void configGPIO();
void configInt();
void confTimer();

uint32_t const initial=80000000;
uint8_t cont=1;

int main(void){
	configGPIO();
	configInt();
	confTimer();

	while(1){

	}

	return 0;
}
void configGPIO(void){
	LPC_PINCON->PINSEL3|=(3<<12);
	LPC_PINCON->PINMODE4&=~(3<<24);
}

void confTimer(void){
	LPC_SC->PCONP |=(1<<2);
	LPC_SC ->PCLKSEL0 |= (1<<4);

	LPC_TIM1->EMR       |=(3<<4);   // pag. 509
	LPC_TIM1->MR0        = initial;//
	LPC_TIM1->MCR       |= (1<<1);       // pag. 507
	LPC_TIM1->MCR       &= ~(1<<0);       // pag. 507
	LPC_TIM1->TCR        = 3;       // pag. 505
	LPC_TIM1->TCR       &= ~(1<<1);
  return;
}

void configInt(){
	LPC_PINCON->PINSEL4 |= (0b01<<24) ;

	LPC_SC->EXTINT      |= 4;   //Limpia bandera de interrupci�n
	LPC_SC->EXTMODE     |= 4; //Selecciona interrupcion por flanco
	LPC_SC->EXTPOLAR    &= ~(4); //Interrumpe cuando el flanco es de BAJADA
	NVIC_EnableIRQ(EINT2_IRQn);    // Habilita de interrupciones externas.

	return;
}

void EINT2_IRQHandler(void){
	delay(10000000);
	switch (cont%4) {
		case 0:
			LPC_TIM1->MR0=initial;
			break;
		case 1:
			LPC_TIM1->MR0=initial/2;
			break;
		case 2:
			LPC_TIM1->MR0=initial/4;
			break;
		case 3:
			LPC_TIM1->MR0=initial/8;
			break;
	}
	cont++;
	LPC_TIM1->TC=0;
	LPC_SC->EXTINT |= 4;   //Limpia bandera de interrupci�n

	return;
}
void delay(uint32_t count){
	for(int cont=0; cont<count;cont++){};
}
