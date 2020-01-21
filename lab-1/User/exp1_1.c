#include "stm32f10x.h"

void Delay(u32 count){
	u32 i;
	for(i=0;i<count;i++);
}
int main(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	while(1){
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		Delay(8000000);
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		Delay(8000000);
	}
}
