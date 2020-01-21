#include "stm32f10x.h"

void Delay(u32 count){
	u32 i;
	for(i=0;i<count;i++);
}
int main(void){
	u32 status=0;
	u32 buzz=0;
	//GPIO_InitTypeDef GPIO_InitStructure;//LED
	//GPIO_InitTypeDef KEY_InitStructure;//KEY2
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//for PB
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//for PE
	//GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
//	KEY_InitStructure.GPIO_Pin=GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	KEY_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//Pull up
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	GPIO_Init(GPIOE,&KEY_InitStructure);
	//GPIO_SetBits(GPIOB,GPIO_Pin_5);
	RCC->APB2ENR|=1<<3;//GPIOB
	RCC->APB2ENR|=1<<6;//GPIOE
	RCC->APB2ENR|=1<<2;//GPIOA
	GPIOE->CRL&=0xFF0F00FF;
	GPIOE->CRL|=0x00308800;
	GPIOE->ODR|=0x0000000C;
	GPIOB->CRL&=0xFF0FFFFF;//
	GPIOB->CRL|=0x00300000;//
	GPIOB->CRH&=0xFFFFFFF0;
	GPIOB->CRH|=0x00000003;
	GPIOA->CRL&=0xFFFFFFF0;
	GPIOA->CRL|=0x00000008;
	GPIOE->BSRR=1<<5;
	GPIOB->BRR=1<<8;
	while(1){
		//GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		//GPIOB->BRR=1<<5;
		//Delay(8000000);
		//GPIO_SetBits(GPIOB, GPIO_Pin_5);
		//GPIOB->BSRR=1<<5;
		//Delay(8000000);
//		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)==0){
//			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
//		}
//		else
//			GPIO_SetBits(GPIOB,GPIO_Pin_5);
		u32 temp=(GPIOE->IDR&0x0000000C);
		u32 temp_buzz=(GPIOA->IDR&0x00000001);
		if((temp&0x00000004)==0)
			GPIOB->BRR=1<<5;
		else
			GPIOB->BSRR=1<<5;
		if((temp&0x00000008)==0){
			if(status==0){
				GPIOE->BRR=1<<5;
				status=1;
                Delay(8000000);
			}
			else if(status==1){
				GPIOE->BSRR=1<<5;
				status=0;
                Delay(8000000);
			}
		}
		if(temp_buzz==1){
			if(buzz==0){
				GPIOB->BSRR=1<<8;
				buzz=1;
			}
			else{
				GPIOB->BRR=1<<8;
				buzz=0;
			}
		}
	}
}
