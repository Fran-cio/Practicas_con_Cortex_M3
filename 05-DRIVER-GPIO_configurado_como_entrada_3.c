/*
 * Ejer3GpioEntrada.c
 *
 *  Created on: 1 sep. 2021
 *      Author: francisco
 */

/*
 * Escribir un programa en C que permita realizar un promedio movil con 
 * los últimos 8 datos ingresados por el puerto 1. Considerar que cada 
 * dato es un entero signado y está formado por los 16 bits menos 
 * significativos de dicho puerto. El resultado, también de 16 bits, debe 
 * ser sacado por los pines P0.0 al P0.11 y P0.15 al P0.18.  Recordar que 
 * en un promedio movil primero se descarta el dato mas viejo de los 8 
 * datos guardados, se ingresa un nuevo dato proveniente del puerto y se 
 * realiza la nueva operación de promedio con esos 8 datos disponibles, 
 * así sucesivamente. Considerar el uso de un retardo antes de tomar una 
 * nueva muestra por el puerto. 
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


const int8_t LONG_ARREGLO=8;

void delay(int count);

int main(void){
	int16_t data[8]={0,0,0,0,0,0,0,0};//Inicio en 0
	/*
	 * Por cuestiones de diseño creo 2 variables para no estar pisando constantemente "promedio"
	 * Ademas la suma de los 8 elementos generaba overflow asi que aux tiene que ser de 32 bits
	 */
	int16_t promedio;
	int32_t aux;

	//configuro GPIO para los que van a ser puertos de salida
	PINSEL_CFG_Type pinsel_out;
	pinsel_out.Portnum 	= PINSEL_PORT_0;
	pinsel_out.Funcnum	=	PINSEL_FUNC_0;

	for(uint8_t i=0;i<=11;i++){
		pinsel_out.Pinnum	=	i;
		PINSEL_ConfigPin(&pinsel_out);
	}
	for(uint8_t i=15;i<=18;i++){
		pinsel_out.Pinnum	=	i;
		PINSEL_ConfigPin(&pinsel_out);
	}

	//configuro GPIO para los que van a ser puertos de salida
	PINSEL_CFG_Type pinsel_in;
	pinsel_in.Portnum 	= 	PINSEL_PORT_1;
	pinsel_in.Pinnum	= 	PINSEL_PIN_16 	|	PINSEL_PIN_17	|
							PINSEL_PIN_18 	|	PINSEL_PIN_19	|
							PINSEL_PIN_20 	|	PINSEL_PIN_21	|
							PINSEL_PIN_22 	|	PINSEL_PIN_23	|
							PINSEL_PIN_24 	|	PINSEL_PIN_25	|
							PINSEL_PIN_26 	|	PINSEL_PIN_27	|
							PINSEL_PIN_28 	|	PINSEL_PIN_29	|
							PINSEL_PIN_30 	|	PINSEL_PIN_31	;
	pinsel_in.Funcnum	=	PINSEL_FUNC_0;
	pinsel_in.Pinmode	=	PINSEL_PINMODE_PULLDOWN;

	for(uint8_t i=16;i<=31;i++){
		pinsel_in.Pinnum	=	i;
		PINSEL_ConfigPin(&pinsel_in);
	}
	//Configuro los puertos como salida y coloco mascara
	FIO_SetDir(PORT_ZERO,0x78FFF,OUTPUT);
	FIO_SetMask(PORT_ZERO,(~(0x78FFF)),1);
	//Confguro los puertos como entrada
	FIO_ByteSetDir(PORT_ONE,3,0xFF,INPUT);

	while(1){
		for(uint8_t i=0; i<LONG_ARREGLO ; i++){
			delay(10);
			/*
			 * Coloco en valor en el arreglo amedida que el for avanza
			 */
			data[i]	= FIO_HalfWordReadValue(PORT_ONE,1);
			/*
			 * Obtengo el promedio
			 */
			aux=0;
			for(uint8_t j=0; j<LONG_ARREGLO ; j++){
				aux+=data[j];
			}
			promedio=aux/LONG_ARREGLO;

			LPC_GPIO0->FIOPINL =((promedio&(1<<12))<<3);//coloco el bit 15
			FIO_HalfWordSetMask(PORT_ZERO, 0,(1<<15) , 1);;//Lo enmascaro

			LPC_GPIO0->FIOPINL=(promedio&(0xFFF));//Coloco los primeros 12 sin modificar 15

			FIO_HalfWordSetMask(PORT_ZERO, 0,(1<<15) , 0);;//Desenmascaro el 15

			LPC_GPIO0->FIOPINH=((promedio&~(0x1FFF))>>13);//Muevo los bits que me faltaron agregar y los igualo a la parte alta del pin
		}
	}

	return 0;
}

void delay(int count){
	for(int cont=0; cont<count;cont++){};
}

