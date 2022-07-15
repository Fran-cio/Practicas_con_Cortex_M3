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

int8_t estado=0;
int8_t secuencia[9];

void delay(int count);
void configGPIO();
void configInt();

int main(void){

	//configuro GPIOs
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
	//configuramos el puerto 0 para salida
	PINSEL_CFG_Type pinsel_out;
	pinsel_out.Portnum	=	PINSEL_PORT_0;
	pinsel_out.Funcnum	=	PINSEL_PINMODE_NORMAL;

	for(uint8_t i=0;i<=1;i++){
		pinsel_out.Pinnum	=	i;
		PINSEL_ConfigPin(&pinsel_out);
	}
	//configuramos el puerto 2.0 para entrada con pulldown
	PINSEL_CFG_Type pinsel_in_0;
	pinsel_in_0.Portnum	=	PINSEL_PORT_2;
	pinsel_in_0.Pinnum	=	PINSEL_PIN_0;
	pinsel_in_0.Funcnum	=	PINSEL_PINMODE_NORMAL;
	pinsel_in_0.Pinmode	=	PINSEL_PINMODE_PULLDOWN;


	PINSEL_ConfigPin(&pinsel_in_0);

	//configuramos el puerto 2.1 para entrada con pullup
	PINSEL_CFG_Type pinsel_in_1;
	pinsel_in_1.Portnum	=	PINSEL_PORT_2;
	pinsel_in_1.Pinnum	=	PINSEL_PIN_1;
	pinsel_in_1.Funcnum	=	PINSEL_PINMODE_NORMAL;
	pinsel_in_1.Pinmode	=	PINSEL_PINMODE_PULLUP;

	PINSEL_ConfigPin(&pinsel_in_1);

	FIO_ByteSetDir(PORT_ZERO,0,3,OUTPUT);
	FIO_ByteSetMask(PORT_ZERO,0,(~(3)),1);

	FIO_ByteSetDir(PORT_TWO,0,3,INPUT);

	return;
}

void configInt(){
	FIO_IntCmd(PORT_TWO,1,0);
	FIO_IntCmd(PORT_TWO,2,1);

	FIO_ClearInt(PORT_TWO,3);

	NVIC_EnableIRQ(EINT3_IRQn);

	return;
}

void delay(int count){
	for(int cont=0; cont<count;cont++){};
}

void EINT3_IRQHandler(void){

	if(GPIO_GetIntStatus(PORT_TWO,0,0)){
		estado=1;
		int8_t aux[9]={0b00,0b01,0b00,0b00,0b01,0b01,0b00,0b01,0b00};
		for(int8_t i=0; i<9 ; i++){
			secuencia[i]=aux[i];
		}
	}
	else if(GPIO_GetIntStatus(PORT_TWO,1,1)){
		estado=1;

		int8_t aux[9]={0b00,0b10,0b10,0b10,0b00,0b00,0b10,0b10,0b00};
		for(int8_t i=0; i<9 ; i++){
			secuencia[i]=aux[i];
		}
	}
	FIO_ClearInt(PORT_TWO,3);

	return;
}
/*
 *Por defecto todas las interrupciones de ese registro
 *vienen con prioridad maxima, es decir 0, pero se le puede asignar un rango de 0-32
*/
