#include "stm32f10x.h"
#include "IERG3810_USART.h"
#include "IERG3810_Clock.h"

int main(void){
	IERG3810_clock_tree_init();
	IERG3810_USART1_init(72,9600);
	
	IERG3810_USART2_init(36,9600);
	USART_print(1,"Han HU");
	USART_print(2,"Han HU");
	while(1){
			USART_print(1,"Han HU");
	USART_print(2,"Han HU");
//		USART2->DR=0x41;
//	 	Delay(50000);
//	 	USART2->DR=0x42;
//	 	Delay(50000);
//	 	Delay(1000000);
	}
}
