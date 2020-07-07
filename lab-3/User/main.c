#include "stm32f10x.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_Clock.h"

void showstring(u16 x,u16 y,u8* str,u16 str_size, u16 color, u16 bgcolor){
    u16 i;
    for(i=0; i < str_size; i++){
        IERG3810_TFTLCD_ShowChar_overlay(x+ i*10,y,str[i],color);
    }
}
int main(void){
    IERG3810_TFTLCD_init();
    while(1){
        u8 str[10]={0x31,0x31,0x0035,0x0035,0x0031,0x0030,0x0037,0x0037,0x0036,0x0033};
        IERG3810_TFTLCD_FillRectangle(0x780F,0,120,0,320);
        IERG3810_TFTLCD_FillRectangle(0xFFE0,120,240,0,320);
        showstring(80,160,str,10,0, 2020);
        //IERG3810_TFTLCD_ShowChinChar(80,140,0,0, 2020);
		//IERG3810_TFTLCD_ShowChinChar(100,140,1,0, 2020);
        Delay(8000000);
    }
}
