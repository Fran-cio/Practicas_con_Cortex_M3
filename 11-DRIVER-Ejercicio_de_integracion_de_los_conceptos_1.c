/*
 * Ejercicio_de_integracion_de_los_conceptos_1.c
 *
 *  Created on: 14 sep. 2021
 *      Author: francisco
 */

/*
	*Algoritmo de antirrebote de un pulsador: Escribir un programa en C
	*que ante la interrupción por flanco de subida del pin P0.1, configurado
	*como entrada digital con pull-down interno, se incremente un contador,
	*se deshabilite esta interrupción y se permita la interrupción por systick
	*cada 20 milisegundos. En cada interrupción del systick se testeará una vez
	*el pin P0.1. Solo para el caso de haber testeado 3 estados altos seguidos
	*se sacará por los pines del puerto P2.0 al P2.7 el equivalente en ascii del
	*valor del contador, se desactivará las interrupción por systick y se habilitará
	*nuevamente la interrupción por P0.1. Por especificación de diseño se pide que los
	*pines del puerto 2 que no sean utilizados deben estar enmascarados por hardware.
	*Considerar que el CPU se encuentra funcionando con el oscilador interno RC (4Mhz).
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


void configGPIO(void);
void configINT(void);
uint8_t toASCCI(uint8_t num);

//Aca solamente calculo el valor hipotetico si tuviera el clock en 4M, el clock sigue siendo el standar y va interrumpir cada menos.
uint32_t const CLOCKRELOJ= 4000000;
uint32_t const ticks =CLOCKRELOJ*20/1000;
//No lo usare para la app de driver

uint8_t cont=0;
uint8_t cont_p=0;

int main(void){
	configGPIO();
	configINT();
	while(1){

	}
	LPC_PINCON->PINMODE0;
	return 0;
}

void configGPIO(void)
{
	PINSEL_CFG_Type pinsel_0;
	pinsel_0.Portnum	=	PINSEL_PORT_0;
	pinsel_0.Pinnum		=	PINSEL_PIN_1;
	pinsel_0.Funcnum	=	PINSEL_FUNC_0;
	pinsel_0.Pinmode	=	PINSEL_PINMODE_PULLDOWN;

	PINSEL_ConfigPin(&pinsel_0);

	PINSEL_CFG_Type pinsel_2;
	pinsel_2.Portnum	=	PINSEL_PORT_2;
	pinsel_2.Funcnum	=	PINSEL_FUNC_0;

	for(uint8_t i=0; i<=7 ; i++)
	{
		pinsel_2.Pinnum	=	i;
		PINSEL_ConfigPin(&pinsel_2);

	}

	FIO_SetDir(PORT_ZERO, 2, INPUT);
	FIO_SetMask(PORT_ZERO, (~((1<<1))),1);
	FIO_SetMask(PORT_ZERO, (1<<1),0);


	FIO_SetDir(PORT_TWO	, 0xFF, OUTPUT);
	FIO_SetMask(PORT_TWO, (~(0xFF)),1);
	FIO_SetMask(PORT_TWO, (0xFF),0);

   return;
}

void configINT(void){
	FIO_IntCmd(PORT_ZERO, 2, 0);
	FIO_ClearInt(PORT_ZERO, 2);

    NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler(void){
    cont++;
    if(cont>10){
    	cont=0;
    }
	FIO_ClearInt(PORT_ZERO, 2);
    NVIC_DisableIRQ(EINT3_IRQn);
	SYSTICK_Cmd(1);
	SYSTICK_ClearCounterFlag();
	SYSTICK_IntCmd(1);
	SYSTICK_InternalInit(20);

	return;
}

void SysTick_Handler(void){
    if(FIO_ByteReadValue(PORT_ZERO,0)){
        cont_p++;
    }else{
    	cont_p=0;
    }
    if(cont_p>=3){
        LPC_GPIO2 -> FIOPIN0= toASCCI(cont);

        cont_p=0;
        SYSTICK_IntCmd(0);

        FIO_ClearInt(PORT_ZERO, 2);
        EXTI_ClearEXTIFlag(EXTI_EINT3_BIT_MARK);
        NVIC_EnableIRQ(EINT3_IRQn);
    }else{
    	SYSTICK_ClearCounterFlag();
    }
    return;
}

uint8_t toASCCI(uint8_t num){
	return num+48;
}
