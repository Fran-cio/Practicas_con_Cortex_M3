/*
 * Configuración_ADC.c
 *
 *  Created on: 30 sep. 2021
 *      Author: francio
 */
#include "LPC17xx.h"


/*Configuración ADC
A partir del código ejemplo visto en clase:

1.- Determine la frecuencia de trabajo y de muestreo del ADC suponiendo una frecuencia de cclk de 100 Mhz. ¿En que modo se encuentra trabajando el ADC?

2.- Modifique el ejemplo para considerar la utilización de dos canales de conversión. Adjuntar el código en C. ¿A que frecuencia se encuentra muestreando cada canal?
*/


void confGPIO(void);
void confADC(void);

uint16_t ADC0Value = 0;

int main(void)
{
confGPIO();
confADC();
while(1){}
return 0;
}

void confGPIO(void)
{
LPC_GPIO0->FIODIR |= (1<<22);
return;
}

void confADC(void){
LPC_SC->PCONP |= (1 << 12);
LPC_ADC->ADCR |= (1 << 21); //habilita el ADC
LPC_SC->PCLKSEL0 |= (3<<24);  //CCLK/8
LPC_ADC->ADCR &=~(255 << 8);  //[15:8] CLKDIV

/*1- Teniendo en cuenta que el cclk es de 100Mhz y este pasa por el divisor de perifericos de 8 y
 * luego por un divisor del adc que en este caso esta en 0. Tenemos la cuenta de 100Mhz/(8*(0+1))=12,5MHz
 * Teniendo en cuenta que el canal del conversor es unico, la frecuencia de muestreo es esa misma.
 * Este resutaltado es viable ya que la frec de muestreo max es de 13Mhz
 */

//LPC_ADC->ADCR |= (1 << 0);   // channel

//Y aca vemos al adc en modo burst
LPC_ADC->ADCR |= (1 << 16);   // burst
LPC_PINCON->PINMODE1 |= (1<<15); //neither pull-up nor pull-down.
LPC_PINCON->PINSEL1 |= (1<<14);
LPC_ADC->ADINTEN = 1;
NVIC_EnableIRQ(ADC_IRQn);

return;
}

void confADC2(void)
{
LPC_SC->PCONP |= (1 << 12);
LPC_ADC->ADCR |= (1 << 21); //habilita el ADC
LPC_SC->PCLKSEL0 |= (3<<24);  //CCLK/8
LPC_ADC->ADCR &=~(255 << 8);  //[15:8] CLKDIV

LPC_ADC->ADCR |= 3;   // channel 0 y 1

//Y aca vemos al adc en modo burst
LPC_ADC->ADCR |= (1 << 16);   // burst
//Seleciono como entradas analogicas y desactivo las pull
LPC_PINCON->PINMODE1 |= (0b0101<<15);
LPC_PINCON->PINMODE1 &= (0b0101<<15);
LPC_PINCON->PINSEL1 |= (0b0101<<15);
LPC_PINCON->PINMODE1 &= (0b0101<<15);

//activo las interrupciones de ambos puertos
LPC_ADC->ADINTEN = 3;
NVIC_EnableIRQ(ADC_IRQn);
/*
 * Manteniendo las configuraciones previas, el modo burst permite tener las
 * 2 entradas convirtiendo constantemente. Teniendo en cuenta que ahora tengo
 * 2 entradas, la frec de trabajo se divide por 2, siendo 6Mhz de frec de muestreo
 * por entrada analogica
 */
return;
}

void ADC_IRQHandler(void)
{
ADC0Value = ((LPC_ADC->ADDR0)>>4) & 0xFFF; //Variable auxiliar para observar el valor del registro de captura

if (ADC0Value<2054){
LPC_GPIO0->FIOSET = (1<<22);
}
else{
LPC_GPIO0->FIOCLR = (1<<22);
}

return;
}

