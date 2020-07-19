#include "stm32f10x.h"
#include "IERG3810_clock.h"

void IERG3810_clock_tree_init(void){
	u8 PLL=7;
	unsigned char temp=0;
	RCC->CFGR &= 0xF8FF0000;
	RCC->CR &= 0xFEF6FFFF;
	RCC->CR|=0x00010000; //HSEON=1, (RM0008 page-119)
	while(!(RCC->CR>>17)); //Check HSERDY, (RM0008 page-119)
	RCC->CFGR=0x00000400; //PPRE1=100, PPRE2=000, (RM0008 page-119)
	RCC->CFGR|=PLL<<18; //PLLMUL=111, (RM0008 page-119)
	RCC->CFGR|=1<<16; //PLLSRC=1, (RM0008 page-119)
	FLASH->ACR|=0x32; //set FLASH with 2 wait states
	RCC->CR|=0x01000000; //PLLON=1, (RM0008, page-119)
	while(!(RCC->CR>>25)); //check PLLRDY, (RM0008, page-119)
	RCC->CFGR|=0x00000002; //SW=10, (RM0008, page-119)
	while(temp!=0x02){ //check SWS, (RM0008, page-119)
		temp=RCC->CFGR>>2;
		temp&=0x03;
	}
}
void Delay(u32 count){
	u32 i;
	for(i=0;i<count;i++);
}
