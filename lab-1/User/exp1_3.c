#include "stm32f10x.h"

void Delay(u32 count){
	u32 i;
	for(i=0;i<count;i++);
}
int main(void){
	//	GPIO_InitTypeDef GPIO_InitStructure;//LED
	//	GPIO_InitTypeDef KEY_InitStructure;//KEY2
	//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//for PB
	//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//for PE
	//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
//	KEY_InitStructure.GPIO_Pin=GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	KEY_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//Pull up
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	GPIO_Init(GPIOE,&KEY_InitStructure);
	//	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	RCC->APB2ENR|=1<<3;
	GPIOB->CRL&=0xFF0FFFFF;//Set Mode Out PP
	GPIOB->CRL|=0x00300000;//Speed 50MHz
	while(1){
		//	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		GPIOB->BRR=1<<5;
		Delay(8000000);
		//	GPIO_SetBits(GPIOB, GPIO_Pin_5);
		GPIOB->BSRR=1<<5;
		Delay(8000000);
//		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)==0){
//			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
//		}
//		else
//			GPIO_SetBits(GPIOB,GPIO_Pin_5);
	}
}
