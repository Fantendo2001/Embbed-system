#include "stm32f10x.h"
#include "IERG3810_Clock.h"
#include "IERG3810_LED.h"
#define LED0_PWM_VAL TIM3->CCR2;
u8 task1HeartBeat = 0;
u8 task2HeartBeat = 0;
#define DS0_ON GPIOB->BRR |= 1<<5;
#define DS0_OFF GPIOB->BSRR |= 1<<5;
#define DS1_ON GPIOE->BRR |= 1<<5;
#define DS1_OFF GPIOE->BSRR |= 1<<5;
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
	//5.2
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
void DS0_turnOff(){
    //level one subroutine
    GPIOB->BSRR = 1<<5; //turn off DS0 directly
}
void DS0_turnOff2(){
    //level two subroutine
    DS0_turnOff(); //call level one subroutine
}
void IERG3810_SYSTICK_Init10ms(void)
{
	SysTick->CTRL=0;
	SysTick->LOAD=89999;
	SysTick->CTRL|=0x00000003;
}
void IERG3810_TIM3_PwmInit(u16 arr, u16 psc)
{
	RCC->APB2ENR|=1<<3;
	GPIOB->CRL&=0xFF0FFFFF;
	GPIOB->CRL|=0x00B00000;
	RCC->APB2ENR|=1<<0;
	AFIO->MAPR&=0xFFFFF3FF;
	AFIO->MAPR|=1<<11;
	RCC->APB1ENR|=1<<1;
	TIM3->ARR=arr;
	TIM3->PSC=psc;
	TIM3->CCMR1|=7<<12;
	TIM3->CCMR1|=1<<11;
	TIM3->CCER|=1<<4;
	TIM3->CR1=0x0080;
	TIM3->CR1|=0x01;
}
int main(void){
    u16 led0pwmval=0;
	u8 dir=1;
	IERG3810_clock_tree_init();
	IERG3810_LED_Init();
	IERG3810_TIM3_PwmInit(9999, 47);
	IERG3810_TIM4_Init(4999,1799);
	IERG3810_NVIC_SetPriorityGroup(5);
	while(1)
	{
		Delay(1500);
		if(dir)
		{led0pwmval++;}
		else
		{led0pwmval--;}
		if(led0pwmval>5000)
		{dir=0;}
		if(led0pwmval==0)
		{dir=1;}
		
		TIM3->CCR2 =led0pwmval;
	}
}

