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

	//configuro GPIO
	LPC_PINCON->PINSEL0 &=~ (0xC0FFFFFF) ;
	LPC_PINCON->PINSEL1 &=~ (0x3F) ;//configuro GPIO
	LPC_PINCON->PINSEL3 = 0 ; //Los menos sig. estan reservados asi que uso los mas sig.

	LPC_GPIO0->FIODIR |=(0x78FFF); //Configuro salida
	LPC_GPIO1->FIODIRH = 0; //Configuro entrada

	LPC_PINCON->PINMODE3 = 0xFFFFFFFF; //Activo pulldown del puerto 1 alto

	LPC_GPIO0->FIOMASK=~(0x78FFF); //Para usar directamente FIOPIN enmascaro el resto del puerto

	while(1){
		for(uint8_t i=0; i<LONG_ARREGLO ; i++){
			delay(10);
			/*
			 * Coloco en valor en el arreglo amedida que el for avanza
			 */
			data[i]	= LPC_GPIO1->FIOPINH;
			/*
			 * Obtengo el promedio
			 */
			aux=0;
			for(uint8_t j=0; j<LONG_ARREGLO ; j++){
				aux+=data[j];
			}
			promedio=aux/LONG_ARREGLO;

			LPC_GPIO0->FIOPINL =((promedio&(1<<12))<<3);//coloco el bit 15
			LPC_GPIO0->FIOMASKL|=(1<<15);//Lo enmascaro

			LPC_GPIO0->FIOPINL=(promedio&(0xFFF));//Coloco los primeros 12 sin modificar 15

			LPC_GPIO0->FIOMASKL&=~(1<<15);//Desenmascaro el 15

			LPC_GPIO0->FIOPINH=((promedio&~(0x1FFF))>>13);//Muevo los bits que me faltaron agregar y los igualo a la parte alta del pin
		}
	}

	return 0;
}

void delay(int count){
	for(int cont=0; cont<count;cont++){};
}

