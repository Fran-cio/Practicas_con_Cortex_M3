/*
 * interrupción_externa.c
 *
 *  Created on: 9 sep. 2021
 *      Author: francio
 */

/*
 * Configurar la interrupción externa EINT1 para que interrumpa por flanco 
 * de bajada y la interrupción EINT2 para que interrumpa por flanco de 
 * subida. En la interrupción por flanco de bajada configurar el systick 
 * para desbordar cada 25 mseg, mientras que en la interrupción por flanco 
 * de subida configurarlo para que desborde cada 60 mseg. Considerar que 
 * EINT1 tiene mayor prioridad que EINT2.
 */

#include "LPC17xx.h"

void delay(int count);
void configGPIO();
void configInt();
uint8_t CONT=0;

int main(void){
	configInt();

	while(1){

	}

	return 0;
}

void configInt(){
	LPC_PINCON->PINSEL4 |= (0b0101<<22) ;

	LPC_SC->EXTINT      |= 6;   //Limpia bandera de interrupci�n
	LPC_SC->EXTMODE     |= 6; //Selecciona interrupcion por flanco
	LPC_SC->EXTPOLAR    |= 4; //Interrumpe cuando el flanco es de subida
	NVIC_EnableIRQ(EINT1_IRQn);    // Habilita de interrupciones externas.
	NVIC_EnableIRQ(EINT2_IRQn);    // Habilita de interrupciones externas.

	NVIC_SetPriority (EINT1_IRQn, 30 );
	NVIC_SetPriority (EINT2_IRQn, 31 );

	return;
}

void SysTick_Handler(void) {
	CONT++;
	SysTick->CTRL &= SysTick->CTRL;
	return;
}

void EINT1_IRQHandler(void){
	//Colocar un debounce aca
	LPC_SC->EXTINT |= 2;   //Limpia bandera de interrupci�n
	SysTick_Config(SystemCoreClock*0.025);
	return;
}

void EINT2_IRQHandler(void){
	LPC_SC->EXTINT |= 4;   //Limpia bandera de interrupci�n
	SysTick_Config(SystemCoreClock*0.060);
	return;
}


