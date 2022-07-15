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

void configGPIO(void);
void configINT(void);
uint8_t toASCCI(uint8_t num);

//Aca solamente calculo el valor hipotetico si tuviera el clock en 4M, el clock sigue siendo el standar y va interrumpir cada menos.
uint32_t const CLOCKRELOJ= 4000000;
uint32_t const ticks =CLOCKRELOJ*20/1000;

uint8_t cont=0;
uint8_t cont_p=0;

int main(void){
	configGPIO();
	configINT();
	while(1){

	}
	return 0;
}

void configGPIO(void){
    LPC_PINCON ->PINSEL0 &= ~(0b11<<2);
    LPC_PINCON ->PINSEL4 &= ~(0xFFFF);

    LPC_GPIO0 ->FIODIR0 &= ~(1<<1);
    LPC_GPIO2 -> FIODIR0 = 0XFF;

    LPC_GPIO2 -> FIOMASK = ~(0XFF);
    LPC_GPIO0 -> FIOMASK0 = ~(1<<1);

    LPC_PINCON -> PINMODE0 |= (0b11<<2);
}

void configINT(void){
	LPC_GPIOINT -> IO0IntClr |= (1<<1);
    LPC_GPIOINT -> IO0IntEnR |= (1<<1);

    NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler(void){
    cont++;
    if(cont>10){
    	cont=0;
    }
    LPC_GPIOINT -> IO0IntClr |= (1<<1);
    NVIC_DisableIRQ(EINT3_IRQn);
	if(SysTick_Config(ticks)){
    	while(1){

    	}
    }
}

void SysTick_Handler(void){
    if(LPC_GPIO0 ->FIOPIN0){
        cont_p++;
    }else{
    	cont_p=0;
    }
    if(cont_p>=3){
        LPC_GPIO2 -> FIOPIN0= toASCCI(cont);
        LPC_GPIOINT ->IO0IntClr |= (1<<1);
        NVIC_EnableIRQ(EINT3_IRQn);
        cont_p=0;
        SysTick -> CTRL = 0;
    }else{
        SysTick -> CTRL &= SysTick -> CTRL;
    }
}

uint8_t toASCCI(uint8_t num){
	return num+48;
}
