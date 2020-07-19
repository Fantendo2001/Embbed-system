#include "stm32f10x.h"
#include "IERG3810_Clock.h"
#include "IERG3810_LED.h"
void IERG3810_NVIC_SetPriorityGroup(u8 prigroup){
	//set prigroup AIRCR[10:8]
	u32 temp, temp1;
	temp1 = prigroup & 0x00000007; // only concern 3 bits
	temp1 <<= 8; //why?
	temp = SCB->AIRCR;
	temp &= 0X0000F8FF;
  temp = 0x05FA0000;
	temp |= temp1;
	SCB->AIRCR = temp;
}
void IERG3810_TIM3_Init(u16 arr,u16 psc){
    //TIM3, IRQ#29
    RCC->APB1ENR|=1<<1;
	TIM3->ARR=arr;
	TIM3->PSC=psc;
	TIM3->DIER|=1<<0;
	TIM3->CR1|=0x01;
	NVIC->IP[29]=0x45;
	NVIC->ISER[0]|=(1<<29);
}
void TIM3_IRQHandler(void)
{
	if(TIM3->SR & 1<<0)
	{
		GPIOB->ODR ^= 1<<5;
	}
	TIM3->SR &= ~(1<<0);
}
void IERG3810_TIM4_Init(u16 arr, u16 psc)
{
    //TIM4, #IRQ30
	RCC->APB1ENR|=1<<2;
	TIM4->ARR=arr;
	TIM4->PSC=psc;
	TIM4->DIER|=1<<0;
	TIM4->CR1|=0x01;
	NVIC->IP[30]=0x45;
	NVIC->ISER[0]|=(1<<30);
}
void TIM4_IRQHandler(void)
{
	if(TIM4->SR & 1<<0)
	{
		 GPIOE->ODR ^= 1<<5;
	}
	TIM4->SR &= ~(1<<0);
}
int main(void){
    IERG3810_clock_tree_init();
    IERG3810_LED_Init();
    IERG3810_NVIC_SetPriorityGroup(5); //set PRIGROUP
    IERG3810_TIM3_Init(4999,7199); //set timer-3
    IERG3810_TIM4_Init(4999,1799); //set timer-4
    while(1){
        ;
    }
}

