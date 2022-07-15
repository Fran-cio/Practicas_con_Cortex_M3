/*
 * Utilizando_el_Timer_en_modo_Match_1.c
 *
 *  Created on: 23 sep. 2021
 *      Author: francisco
 */
#include "LPC17xx.h"

/*
 * Utilizando el Timer 1, escribir un código en C para que por cada presión de un pulsador, la frecuencia de parpadeo de un led disminuya a la mitad
 * debido a la modificación del reloj que llega al periférico. El pulsador debe producir una interrupción por GPIO0 con flanco descendente. Adjuntar
 * el código en C.
*/

void delay(uint32_t count);
void configGPIO();
void configInt();
void confTimer();

uint32_t const initial=10000000;
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
	LPC_PINCON->PINMODE4&=~(3<<20);
}

void confTimer(void){
	LPC_SC->PCONP |=(1<<2);
	LPC_SC ->PCLKSEL0 |= (0b01<<4);
	LPC_SC ->PCLKSEL0 &= (0b01<<4);

	LPC_TIM1->EMR       |=(3<<4);   // pag. 509
	LPC_TIM1->MR0        = initial;//
	LPC_TIM1->MCR       |= (1<<1);       // pag. 507
	LPC_TIM1->MCR       &= ~(1<<0);       // pag. 507
	LPC_TIM1->TCR        = 3;       // pag. 505
	LPC_TIM1->TCR       &= ~(1<<1);
  return;
}

void configInt(){
	LPC_PINCON->PINSEL4 |= (0b01<<20) ;

	LPC_SC->EXTINT      |= 1;   //Limpia bandera de interrupci�n
	LPC_SC->EXTMODE     |= 1; //Selecciona interrupcion por flanco
	LPC_SC->EXTPOLAR    &= ~(1); //Interrumpe cuando el flanco es de BAJADA
	NVIC_EnableIRQ(EINT0_IRQn);    // Habilita de interrupciones externas.

	return;
}

void EINT0_IRQHandler(void){
	delay(10000000);
	uint8_t aux;
	switch (cont%4) {
		case 0:
			aux=0b01;
			break;
		case 1:
			aux=0b10;
			break;
		case 2:
			aux=0b00;
			break;
		case 3:
			aux=0b11;
			break;
	}
	cont++;

	LPC_SC ->PCLKSEL0 |= (aux<<4);
	LPC_SC ->PCLKSEL0 &= (aux<<4);
	LPC_TIM1->TCR        = 3;       // pag. 505
	LPC_TIM1->TCR       &= ~(1<<1);
	LPC_SC->EXTINT |= 1;   //Limpia bandera de interrupci�n

	return;
}
void delay(uint32_t count){
	for(int cont=0; cont<count;cont++){};
}




