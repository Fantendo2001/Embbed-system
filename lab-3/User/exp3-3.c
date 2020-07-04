#include "stm32f10x.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_Clock.h"

int main(void){
    int display=0;
    IERG3810_TFTLCD_init();
    IERG3810_TFTLCD_FillRectangle(0,0,240,0,320);
    while(1){
        IERG3810_TFTLCD_FillRectangle(0,0,240,0,320);
        Delay(8000000);
        IERG3810_TFTLCD_SevenSegment(2020,80,80,9-display);
        Delay(8000000);
        IERG3810_TFTLCD_FillRectangle(0,80,80,80,160);
        display++;
        display=display%10;
    }
}
