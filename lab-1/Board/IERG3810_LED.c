#include "stm32f10x.h"
#include "IERG3810_LED.h"

// put your procedure and code here
void IERG3810_LED_Init(void){
	RCC->APB2ENR|=1<<3;//GPIOB
	RCC->APB2ENR|=1<<6;//GPIOE
	GPIOB->CRL&=0xFF0FFFFF;//
	GPIOB->CRL|=0x00300000;//
	GPIOE->CRL&=0xFF0FFFFF;//
	GPIOE->CRL|=0x00300000;//
	GPIOE->BSRR=1<<5;
	GPIOB->BSRR=1<<5;
}
