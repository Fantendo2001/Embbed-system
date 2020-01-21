#include "stm32f10x.h"
#include "IERG3810_Buzzer.h"
#include "IERG3810_KEY.h"
#include "IERG3810_LED.h"

void Delay(u32 count){
	u32 i;
	for(i=0;i<count;i++);
}
int main(void){
	u32 status=0;
	u32 buzz=0;
	IERG3810_LED_Init();
	IERG3810_KEY_Init();
	IERG3810_Buzzer_Init();
	while(1){
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
			}
			else if(status==1){
				GPIOE->BSRR=1<<5;
				status=0;
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
