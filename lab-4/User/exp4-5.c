#include "stm32f10x.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_Clock.h"
#include "IERG3810_USART.h"
#include "IERG3810_LED.h"
#define DS0_ON GPIOB->BRR = 1<<5;
#define DS1_ON GPIOE->BRR = 1<<5;
#define DS0_OFF GPIOB->BSRR = 1<<5;
#define DS1_OFF GPIOE->BSRR = 1<<5;
u32 sheep=0; // global variable
u16 ps2count=0;
u32 timeout=20000;
u8 ps2key;
u8 ps2key_prev=0;
void IERG3810_key2_ExtiInit(void){
    //KEY2 at PE2, EXTI-2, IRQ#8
    RCC->APB2ENR |= 1<<6;
    GPIOE->CRL &= 0xFFFFF0FF;
    GPIOE->CRL |= 0x00000800;
    GPIOE->ODR |= 1<<2;
    RCC->APB2ENR |= 0x01; //(RM0008 p.119)
    AFIO->EXTICR[0]&=0xFFFFF0FF; //p.185
    AFIO->EXTICR[0]|=0x00000400; //p.185
    EXTI->IMR |= 1<<2; //p.202
    EXTI->FTSR |= 1<<2; //p.203
    //EXTI->RTSR |= 1<<2; //p.203

    NVIC->IP[8]=0x65; //set priority of this interrupt. (D10337E p.8-4, 8-16)
    NVIC->ISER[0] |= (1<<8); // p.8-3
}
void IERG3810_keyUP_ExtiInit(void){
    //KEY UP at PA0, EXTI-0, IRQ#6
    RCC->APB2ENR |= 1<<2;
    GPIOA->CRL &= 0xFFFFFFF0;
    GPIOA->CRL |= 0x00000008;
    GPIOA->ODR |= 0<<0;
    RCC->APB2ENR |= 0x01; //(RM0008 p.119)
    AFIO->EXTICR[0]&=0xFFFFFFF0; //p.185
    AFIO->EXTICR[0]|=0x00000000; //p.185
    EXTI->IMR |= 1<<0; //p.202
    EXTI->FTSR |= 1<<0; //p.203
    //EXTI->RTSR |= 1<<0; //p.203

    NVIC->IP[6]=0x35; //set priority of this interrupt. (D10337E p.8-4, 8-16)
    NVIC->ISER[0] |= (1<<6); // p.8-3
}
void IERG3810_PS2key_ExtiInit(void){
    //PS2 Keyboard CLK at PC11, EXTI10:15, IRQ#40
    RCC->APB2ENR|=1<<4;
    GPIOC->CRH &= 0xFFFF0FFF;
    GPIOC->CRH |= 0x00008000;
    RCC->APB2ENR |= 0x01; 
    AFIO->EXTICR[2] &= 0xFFFF0FFF; 
    AFIO->EXTICR[2] |= 0x00002000;
    EXTI->IMR |= 1<<11;
    EXTI->FTSR |=1<<11;

    NVIC->IP[40] = 0x95; //set priority of this interrupt
    NVIC->ISER[1] |= (1<<8);

    //PS2 Keyboard DAT at PC10
    RCC->APB2ENR|=1<<4;
    GPIOC->CRH &= 0xFFFFF0FF;
    GPIOC->CRH |= 0x00000800;
}
void IERG3810_NVIC_SetPriorityGroup(u8 prigroup){
    // Set PRIGROUP AIRCR[10:8]
    u32 temp, temp1;
    temp1=prigroup&0x00000007; //only concern 3 bits
    temp1 <<=8; 
    temp=SCB->AIRCR; //ARM DDI0337E p.8-22
    temp &= 0x0000F8FF;
    temp |= 0x05FA0000;
    temp |= temp1;
    SCB->AIRCR=temp;
}
void EXTI2_IRQHandler(void){
    u8 i;
    for(i=0;i<10;i++){
        DS0_ON;
        Delay(1000000);
        DS0_OFF;
        Delay(1000000);
    }
    EXTI->PR=1<<2; //clear this exception pending bit
}
void EXTI0_IRQHandler(void){
    u8 i;
    for(i=0;i<10;i++){
        DS1_ON;
        Delay(1000000);
        DS1_OFF;
        Delay(1000000);
    }
    EXTI->PR |= 1<<0;
}
void EXTI15_10_IRQHandler(void){
    // student design here
    if(ps2count>0 && ps2count <9){ //check the data bits
        ps2key |= ((GPIOC->IDR & 0x00000400)>>10)<<(ps2count-1);
    }
    ps2count++;
    Delay(10); //We found that the processor is too fast and get error.
    // A short delay can eliminate the error.
    EXTI->PR = 1<<11; //Clear this exception pending bit
}
int main(void){
    IERG3810_clock_tree_init();
    //IERG3810_USART2_init(36,9600);
    IERG3810_LED_Init();
    IERG3810_NVIC_SetPriorityGroup(5); //set PRIGROUP
    IERG3810_key2_ExtiInit(); //Init KEY2 as an interrupt input
    IERG3810_keyUP_ExtiInit(); //Init KEYUP as an interrupt input
    IERG3810_PS2key_ExtiInit(); //Init PS2 keyboard as an interrupt input
    //USART_print(2,"1234567890");
    DS0_OFF;
    DS1_OFF;
    while(1){
        //USART_print(2, " --- ABCDEF ");
        
        sheep++; //COUNT SHEEP
        //if PS2 keyboard received data correctly
        if(ps2count>=11){
            //EXTI->IMR &= ~(1<<11); optional, suspend interrupt
            //-- student design program here
            if(ps2key_prev==0x70&&ps2key==0x70){
				DS0_ON;
			}
			if(ps2key_prev==0x69&&ps2key==0x69){
				DS1_ON;
			}
			if(ps2key_prev==0xF0&&ps2key==0x70){
				DS0_OFF;
			}
			if(ps2key_prev==0xF0&&ps2key==0x69){
				DS1_OFF;
			}
            ps2key_prev=ps2key;
			ps2count=0;
			ps2key=0;
            EXTI->PR = 1<<11; //Clear this exception pending bit
            //EXTI->IMR |= (1<<11); optional, resume interrupt
        } //end of "if PS2 keyboard received data correctly"
        timeout--;

        if (timeout ==0){ //Clear PS2 keyboard data when timeout
            timeout=20000;
            ps2key=0;
            ps2count=0;
        } // end of "clear PS2 keyboard data when timeout"
    }
}

