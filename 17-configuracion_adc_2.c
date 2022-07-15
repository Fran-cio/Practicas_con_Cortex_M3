/*
 * configuracion_adc_2.c
 *
 *  Created on: 7 oct. 2021
 *      Author: francio
 */


void confADC(void){
LPC_SC->PCONP |= (1 << 12);
LPC_ADC->ADCR |= (1 << 21);
LPC_SC->PCLKSEL0 |= (0b01<<24);  //CCLK
LPC_SC->PCLKSEL0 &= ~(0b10<<24);  //CCLK
LPC_ADC->ADCR |=(64<<8); //13M/65=20k

LPC_ADC->ADCR |= 0xF;

//Y aca vemos al adc en modo burst
LPC_ADC->ADCR |= (1 << 16);
LPC_PINCON->PINMODE1 |= (1<<15);
LPC_PINCON->PINSEL1 |= (1<<14);
LPC_ADC->ADINTEN = 1;
NVIC_EnableIRQ(ADC_IRQn);

return;

