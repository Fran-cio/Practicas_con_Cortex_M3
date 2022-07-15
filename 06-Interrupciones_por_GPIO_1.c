/*
 * EjerInterrup1.c
 *
 *  Created on: 1 sep. 2021
 *      Author: francisco
 */

/*
 * Realizar un programa que configure el puerto P2.0 y P2.1 para que 
 * provoquen una interrupción por flanco de subida para el primer pin y 
 * por flanco de bajada para el segundo. Cuando la interrupción sea por 
 * P2.0 se enviará por el pin P0.0 la secuencia de bits 010011010. Si la 
 * interrupción es por P2.1 se enviará por el pin P0.1 la secuencia 
 * 011100110. Las secuencias se envían únicamente cuando se produce una 
 * interrupción, en caso contrario la salida de los pines tienen valores 
 * 1 lógicos. ¿que prioridad tienen configuradas por defecto estas 
 * interrupciones?
 */

#include "LPC17xx.h"

int8_t estado=0;
int8_t secuencia[9];

void delay(int count);
void configGPIO();
void configInt();

int main(void){

	//configuro GPIO
	configGPIO();
	configInt();

	while(1){
		if(estado==0){
			LPC_GPIO0->FIOPIN0=3;
		}
		else if(estado==1){
			LPC_GPIO0->FIOPIN0=0;
			for(int8_t i=0; i<9 ; i++){
				LPC_GPIO0->FIOPIN0=secuencia[i];
				delay(3000000);
			}
			estado=0;
		}

	}

	return 0;
}

void configGPIO(){
	LPC_PINCON->PINSEL0 &=~ (0xF) ;
	LPC_PINCON->PINSEL4 &=~ (0xF) ;
	LPC_PINCON->PINSEL3 = 0 ; //Los menos sig. estan reservados asi que uso los mas sig.

	LPC_GPIO0->FIODIR0 |=  (0x3); //Configuro salida
	LPC_GPIO2->FIODIR0 &=~ (0x3); //Configuro entrada

	LPC_PINCON->PINMODE4 |= 0xF; //Activo pulldown del puerto 1 alto

	LPC_GPIO0->FIOMASK0	=~	(0x3);

	return;
}

void configInt(){
	LPC_GPIOINT->IO2IntEnR |= (0x3);
	LPC_GPIOINT->IO2IntClr |= (0x3);

	NVIC_EnableIRQ(EINT3_IRQn);

	return;
}

void delay(int count){
	for(int cont=0; cont<count;cont++){};
}

void EINT3_IRQHandler(void){

	if(LPC_GPIOINT->IO2IntStatR&(1)){
		estado=1;
		int8_t aux[9]={0b00,0b01,0b00,0b00,0b01,0b01,0b00,0b01,0b00};
		for(int8_t i=0; i<9 ; i++){
			secuencia[i]=aux[i];
		}
	}
	else if(LPC_GPIOINT->IO2IntStatR&(2)){
		estado=1;

		int8_t aux[9]={0b00,0b10,0b10,0b10,0b00,0b00,0b10,0b10,0b00};
		for(int8_t i=0; i<9 ; i++){
			secuencia[i]=aux[i];
		}
	}
	LPC_GPIOINT->IO2IntClr |= (0x3);

	return;
}
/*
 *Por defecto todas las interrupciones de ese registro
 *vienen con prioridad maxima, es decir 0, pero se le puede asignar un rango de 0-32
*/
