/*
 * Systick_2.c
 *
 *  Created on: 8 sep. 2021
 *      Author: francisco
 */

/*
 * 1.- Configure el Systick Timer de modo que genere
 * una forma de onda llamada PWM tal como la que se
 * muestra en la figura adjunta. Esta señal debe ser
 * sacada por el pin P0.22 para que controle la
 * intensidad de brillo del led. El periodo de la
 * señal debe ser de 10 mseg con un duty cycle de
 * 10%. Configure la interrupción externa EINT0
 * de modo que cada vez que se entre en una rutina
 * de interrupción externa el duty cycle incremente
 * en un 10% (1 mseg). Esto se repite hasta llegar
 * al 100%, luego, si se entra nuevamente a la
 * interrupción externa, el duty cycle volverá al 10%.
 *
 * 2.- Modificar los niveles de prioridad para
 * que la interrupción por systick tenga mayor
 * prioridad que la interrupción externa.
 *
 * 3. Adjuntar el .C  del código generado.
*/

#include "LPC17xx.h"

#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_exti.h"

// TODO: insert other definitions and declarations here

#define	OUTPUT	    (uint8_t) 1
#define INPUT	    (uint8_t) 0

#define PIN_22		((uint32_t)(1<<22))
#define PORT_ZERO	(uint8_t)	0
#define PORT_ONE	(uint8_t)	1
#define PORT_TWO	(uint8_t)	2
#define PORT_THREE	(uint8_t)	3


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
	PINSEL_CFG_Type pinsel_0;
	pinsel_0.Portnum	=	PINSEL_PORT_0;
	pinsel_0.Pinnum		=	PINSEL_PIN_22;
	pinsel_0.Funcnum	=	PINSEL_FUNC_0;

	PINSEL_ConfigPin(&pinsel_0);

	FIO_SetDir(PINSEL_PORT_0, (1<<22), OUTPUT);
	FIO_SetMask(PINSEL_PIN_0,(~(1<<22)) ,1);

	LPC_PINCON->PINMODE4 |= (3<<21);

	LPC_GPIO0->FIOMASK2	= ~(1<<6);
	LPC_GPIO2->FIOMASK1	= ~(1<<2);

	return;
}

void configInt(){
	PINSEL_CFG_Type pinsel_1;
	pinsel_1.Portnum	=	PINSEL_PORT_1;
	pinsel_1.Pinnum		=	PINSEL_PIN_10;
	pinsel_1.Funcnum	=	PINSEL_FUNC_1;
	pinsel_1.Pinmode	= 	PINSEL_PINMODE_PULLDOWN;

	PINSEL_ConfigPin(&pinsel_1);
	EXTI_Init();
	EXTI_InitTypeDef exti;
	exti.EXTI_Line		=	EXTI_EINT0;
	exti.EXTI_Mode		=	EXTI_MODE_EDGE_SENSITIVE;
	exti.EXTI_polarity	=	EXTI_POLARITY_HIGH_ACTIVE_OR_RISING_EDGE;

	EXTI_Config(&exti);
	//Limpia bandera de interrupci�n
	//Selecciona interrupcion por flanco
	//Interrumpe cuando el flanco es de subida

	EXTI_ClearEXTIFlag(EXTI_EINT0_BIT_MARK);
	NVIC_EnableIRQ(EINT0_IRQn);    // Habilita de interrupciones externas.
	NVIC_SetPriority (EINT0_IRQn, 31 );
	//NVIC_SetPriority();

	SYSTICK_Cmd(1);
	SYSTICK_IntCmd(1);
	SYSTICK_InternalInit(1);

	NVIC_SetPriority (SysTick_IRQn, (NVIC_GetPriority(EINT0_IRQn)-1) );

	return;
}

void SysTick_Handler(void) {
	CONT++;
	if(CONT==10){
		CONT=0;
	}
	SYSTICK_ClearCounterFlag();
	return;
}

void EINT0_IRQHandler(void){
	delay(100000);
	DUTY_CYCLE++;
	EXTI_ClearEXTIFlag(EXTI_EINT0_BIT_MARK);  //Limpia bandera de interrupci�n
	if(DUTY_CYCLE>10){
		DUTY_CYCLE=1;
	}
	return;
}

void delay(int count){
	for(int cont=0; cont<count;cont++){};
}
