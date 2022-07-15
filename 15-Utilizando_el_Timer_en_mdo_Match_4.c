/*
 * Utilizando_el_Timer_en_mdo_Match_4.c
 *
 *  Created on: 24 sep. 2021
 *      Author: francio
 */

#include "LPC17xx.h"

/*Utilizando los 4 registros match del Timer 0 y las salidas P0.0,
 * P0.1, P0.2 y P0.3, realizar un programa en C que permita obtener
 * las formas de ondas adjuntas, donde los pulsos en alto tienen una
 *  duración de 5 mseg. Un pulsador conectado a la entrada EINT3,
 *  permitirá elegir entre las dos secuencias mediante una rutina
 *  de servicio a la interrupción. La prioridad de la interrupción
 *  del Timer tiene que ser mayor que la del pulsador. Estas formas
 *   de ondas son muy útiles para controlar un motor paso a paso.
 *   Adjuntar el código en C .*/

void configGPIO();
void configInt();
void confTimer();
void delay(uint32_t count);

uint32_t const _5mS = 50000000;

uint8_t secuencias=0;

int main(void){

	configGPIO();
	configInt();
	confTimer();

	while(1){
	}

	return 0;

}
void configGPIO(void){
	LPC_PINCON->PINSEL0 &= ~(0xFF); //los puertos de salida como GPIO
	LPC_GPIO0->FIODIR0  |= (0xF);   //seteo los puertos como salida
}

void configInt(){
	LPC_PINCON->PINSEL4 |= (0b01<<26) ;
	LPC_PINCON->PINSEL4 &= (0b01<<26) ;

	LPC_SC->EXTINT      |= (1<<4);   //Limpia bandera de interrupci�n
	LPC_SC->EXTMODE     |= (1<<4); //Selecciona interrupcion por flanco
	LPC_SC->EXTPOLAR    &= ~(1<<4); //Interrumpe cuando el flanco es de BAJADA
	NVIC_EnableIRQ(EINT3_IRQn);    // Habilita de interrupciones externas.

	return;
}

void confTimer(void){
	LPC_SC->PCONP |=(1<<1);//Prendo el timer 0
	LPC_SC ->PCLKSEL0 |= (0b01<<2);
	LPC_SC ->PCLKSEL0 &= (0b01<<2);//Cclk=Pclk

	LPC_TIM0->MCR       |= (1<<0)| (1<<3) | (1<<6) | (3<<9);//activo la interrupciones
	LPC_TIM0->MR0	= _5mS*1;
	LPC_TIM0->MR1	= _5mS*2;
	LPC_TIM0->MR2	= _5mS*3;
	LPC_TIM0->MR3	= _5mS*4;

	LPC_TIM0->TCR    =	3;
	LPC_TIM0->TCR	&=	~(1<<1);//arranco y reinicio el clock

	LPC_TIM0->IR	|=	0xF;
	NVIC_EnableIRQ(TIMER0_IRQn);
	return;
}

void EINT3_IRQHandler(void){
	delay(10000000);
	if (secuencias) {
		secuencias=0;
	}
	else {
		secuencias=1;
	}
	LPC_SC->EXTINT |= 4;   //Limpia bandera de interrupci�n

	return;

}
void TIMER0_IRQHandler(void){
	if(secuencias){
		if(LPC_TIM0->IR & (1)){
			LPC_GPIO0->FIOCLR0|=(1<<2);
			LPC_GPIO0->FIOSET0|=(1<<0);
			LPC_TIM0->IR |= (1);
		}else if(LPC_TIM0->IR & (1<<1)){
			LPC_GPIO0->FIOCLR0|=(1<<3);
			LPC_GPIO0->FIOSET0|=(1<<1);
			LPC_TIM0->IR |= (1<<1);
		}else if(LPC_TIM0->IR & (1<<2)){
			LPC_GPIO0->FIOCLR0|=(1<<0);
			LPC_GPIO0->FIOSET0|=(1<<2);
			LPC_TIM0->IR |= (1<<2);
		}else if(LPC_TIM0->IR & (1<<3)){
			LPC_GPIO0->FIOCLR0|=(1<<1);
			LPC_GPIO0->FIOSET0|=(1<<3);
			LPC_TIM0->IR |= (1<<3);
		}
	}else{
		if(LPC_TIM0->IR & (1)){
			LPC_GPIO0->FIOCLR0|=(1<<3);
			LPC_GPIO0->FIOSET0|=(1<<0);
			LPC_TIM0->IR |= (1);
		} if(LPC_TIM0->IR & (1<<1)){
			LPC_GPIO0->FIOCLR0|=(1<<0);
			LPC_GPIO0->FIOSET0|=(1<<1);
			LPC_TIM0->IR |= (1<<1);
		}else if(LPC_TIM0->IR & (1<<2)){
			LPC_GPIO0->FIOCLR0|=(1<<1);
			LPC_GPIO0->FIOSET0|=(1<<2);
			LPC_TIM0->IR |= (1<<2);
		}else if(LPC_TIM0->IR & (1<<3)){
			LPC_GPIO0->FIOCLR0|=(1<<2);
			LPC_GPIO0->FIOSET0|=(1<<3);
			LPC_TIM0->IR |= (1<<3);
		}
	}
}

void delay(uint32_t count){
	for(int cont=0; cont<count;cont++){};
}
