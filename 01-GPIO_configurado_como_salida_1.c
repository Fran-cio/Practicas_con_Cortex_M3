/*
===============================================================================
 Name        : Ejer1.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "LPC17xx.h"

/*Una famosa empresa de calzados a incorporado a sus zapatillas 10 luces leds comandadas por un microcontrolador LPC1769 y ha pedido a su grupo de ingenieros que diseñen 2 secuencias de luces que cada cierto tiempo se vayan intercalando (secuencia A - secuencia B- secuencia A- ... ). Como todavía no se ha definido la frecuencia a la cual va a funcionar el CPU del microcontrolador, las funciones de retardos que se incorporen deben tener como parametros de entrada variables que permitan modificar el tiempo de retardo que se vaya a utilizar finalmente. Se pide escribir el código que resuelva este pedido, considerando que los leds se encuentran conectados en los puertos P0,0 al P0.9. Adjuntar el archivo .c bajo el nombre "eje1ApellidoDelEstudiante.c"*/

void delay(uint32_t count);

int main(void) {
	uint16_t numDisplay1[2]={0x0033,0x00CC};
	uint16_t numDisplay2[5]={0x01C7,0x038E,0x071C,0x0639,0x0473};

	uint8_t len1=sizeof(numDisplay1)/sizeof(numDisplay1[0]);
	uint8_t len2=sizeof(numDisplay2)/sizeof(numDisplay2[0]);


	LPC_PINCON->PINSEL0 &= ~(0xFFFFF);
	LPC_GPIO0->FIODIRL	|= 	(0x03FF);
	LPC_GPIO0->FIOMASKL |= ~(0x03FF);

	while(1){
		for(uint8_t k=0;k<10;k++){
			for(uint8_t i=0;i<len1;i++){
				LPC_GPIO0->FIOPINL = (numDisplay1[i]);
				delay(7000000);
			}
		}
		for(uint8_t k=0;k<10;k++){
			for(uint8_t i=0;i<len2;i++){
				LPC_GPIO0->FIOPINL =(numDisplay2[i]);
				delay(7000000);
			}
		}
	}
    return 0 ;
}

void delay(uint32_t count){
	for(int cont=0; cont<count;cont++){};
}
