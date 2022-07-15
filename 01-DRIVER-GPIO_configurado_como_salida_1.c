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

/*	Una famosa empresa de calzados a incorporado a sus zapatillas 10
 * 	luces leds comandadas por un microcontrolador LPC1769 y ha pedido
 * 	a su grupo de ingenieros que diseñen 2 secuencias de luces que
 * 	cada cierto tiempo se vayan intercalando (secuencia A - secuencia
 * 	B- secuencia A- ... ). Como todavía no se ha definido la
 * 	frecuencia a la cual va a funcionar el CPU del microcontrolador,
 * 	las funciones de retardos que se incorporen deben tener como
 * 	parametros de entrada variables que permitan modificar el
 * 	tiempo de retardo que se vaya a utilizar finalmente. Se
 * 	pide escribir el código que resuelva este pedido, considerando
 * 	que los leds se encuentran conectados en los puertos P0,0 al
 * 	P0.9. Adjuntar el archivo .c bajo el nombre
 * 	"eje1ApellidoDelEstudiante.c"*/

void delay(uint32_t count);

int main(void) {
	uint16_t numDisplay1[2]={0x0033,0x00CC};
	uint16_t numDisplay2[5]={0x01C7,0x038E,0x071C,0x0639,0x0473};

	uint8_t len1=sizeof(numDisplay1)/sizeof(numDisplay1[0]);
	uint8_t len2=sizeof(numDisplay2)/sizeof(numDisplay2[0]);
	PINSEL_CFG_Type conf_sel_1;
	conf_sel_1.Portnum 	=	PINSEL_PORT_0;
	conf_sel_1.Funcnum	=	PINSEL_FUNC_0;

	for(uint8_t i=0;i<=9;i++){
			conf_sel_1.Pinnum	=	i;
			PINSEL_ConfigPin(&conf_sel_1);
	}
	FIO_HalfWordSetDir(0, 0, 0b1111111111, OUTPUT);
	FIO_HalfWordSetMask(0, 0, (~(0b1111111111)),0);

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
