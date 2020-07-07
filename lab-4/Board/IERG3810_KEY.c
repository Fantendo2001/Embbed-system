#include "stm32f10x.h"
#include "IERG3810_KEY.h"

// put your procedure and code here
void IERG3810_KEY_Init(void){
	RCC->APB2ENR|=1<<6;//GPIOE
	RCC->APB2ENR|=1<<2;//GPIOA
	GPIOA->CRL&=0xFFFFFFF0;
	GPIOA->CRL|=0x00000008;
	GPIOE->CRL&=0xFFF000FF;
	GPIOE->CRL|=0x00088800;
	GPIOE->ODR|=0x0000001C;
}
