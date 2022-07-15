#include "LPC17xx.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
void confPin(void);
void confUart(void);
int main (void){
	confPin();
	confUart();
	uint8_t info[] = "\rHola mundo\t-\tElectr�nica Digital 3\t-\tFCEFyN-UNC";
	//uint8_t info[]={(uint8_t)'\n',0x30,(uint8_t)'\n',0x31,(uint8_t)'\n',0x32,(uint8_t)'\n',0x33,(uint8_t)'\n',0x34,(uint8_t)'\n',0x35,(uint8_t)'\n',0x36,(uint8_t)'\n',0x37,(uint8_t)'\n',0x38,(uint8_t)'\n',0x39};

	while(1){
		UART_Send(LPC_UART0, info, sizeof(info), BLOCKING);
	}
	return 0;
}
void confPin(void){
	PINSEL_CFG_Type PinCfg;
	//configuraci�n pin de Tx y Rx
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 2;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);
	return;
}
void confUart(void){
	UART_CFG_Type UARTConfigStruct;
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	//configuraci�n por defecto:
	UART_ConfigStructInit(&UARTConfigStruct);
	//inicializa perif�rico
	UART_Init(LPC_UART0, &UARTConfigStruct);
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
	//Inicializa FIFO
	UART_FIFOConfig(LPC_UART0, &UARTFIFOConfigStruct);
	//Habilita transmisi�n
	UART_TxCmd(LPC_UART0, ENABLE);
	return;
}
