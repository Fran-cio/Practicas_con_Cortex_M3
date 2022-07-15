/*
 * Utilizando_el_Timer_en_modo_Match_2.c
 *
 *  Created on: 23 sep. 2021
 *      Author: francisco
 */
#include "LPC17xx.h"
/*Escribir un programa para que por cada presión de un pulsador, la frecuencia de parpadeo de un led disminuya a la mitad debido a la
 * modificación del pre-escaler del Timer 2. El pulsador debe producir una interrupción por EINT1 con flanco descendente. Adjuntar el
 * código en c.
*/

void delay(uint32_t count);
void configGPIO();
void configInt();
void confTimer();

uint32_t const initial=7000000;
uint8_t cont=0;

int main(void){
	configGPIO();
	configInt();
	confTimer();

	while(1){

	}

	return 0;
}
void configGPIO(void){
	LPC_PINCON->PINSEL0|=(3<<12);
	LPC_PINCON->PINMODE4|=(3<<20);
}

void confTimer(void){
	LPC_SC->PCONP |=(1<<22);
	LPC_SC ->PCLKSEL1 |= (1<<12);

	LPC_TIM2->EMR       |=(3<<4);   // pag. 509
	LPC_TIM2->MR0        = initial;//
	LPC_TIM2->MCR       |= (1<<1);       // pag. 507
	LPC_TIM2->MCR       &= ~(1<<0);       // pag. 507
	LPC_TIM2->TCR        = 3;       // pag. 505
	LPC_TIM2->TCR       &= ~(1<<1);
	LPC_TIM2->PR        =  0;
  return;
}

void configInt(){
	LPC_PINCON->PINSEL4 |= (0b01<<22) ;

	LPC_SC->EXTINT      |= 2;   //Limpia bandera de interrupci�n
	LPC_SC->EXTMODE     |= 2; //Selecciona interrupcion por flanco
	LPC_SC->EXTPOLAR    |= 2; //Interrumpe cuando el flanco es de Subida
	NVIC_EnableIRQ(EINT1_IRQn);    // Habilita de interrupciones externas.

	return;
}

void EINT1_IRQHandler(void){
	delay(10000000);

	LPC_TIM2->PR        =  (1<<cont);
	cont++;
	LPC_TIM2->TCR       |= (1<<1);
	LPC_TIM2->TCR       &= ~(1<<1);
	LPC_SC->EXTINT |= 2;   //Limpia bandera de interrupci�n

	return;
}
void delay(uint32_t count){
	for(int cont=0; cont<count;cont++){};
}


