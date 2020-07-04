#include "stm32f10x.h"
#include "IERG3810_TFTLCD.h"

int main(void){
    u16 x=10; // x starts at 10
    u16 y=10; // y starts at 10
    int i=0;
    IERG3810_TFTLCD_init();
    while(1){
        for(i=0;i<5;i++){ // looping from y position

            switch (i)
            {
            case 0:
                for(;x<30;x++){ // loop to draw 20 dots
                    IERG3810_TFTLCD_DrawDot(x,y,0); //black line
                }
                x=10;
                break;
            case 1:
                for(;x<30;x++){ // loop to draw 20 dots
                    IERG3810_TFTLCD_DrawDot(x,y+10,65535); //white line
                }
                x=10;
                break;
            case 2:
                for(;x<30;x++){ // loop to draw 20 dots
                    IERG3810_TFTLCD_DrawDot(x,y+20,2016); //green line
                }
                x=10;
                break;
            case 3:
                for(;x<30;x++){ // loop to draw 20 dots
                    IERG3810_TFTLCD_DrawDot(x,y+30,63488); //red line
                }
                x=10;
                break;
            case 4:
                for(;x<30;x++){ // loop to draw 20 dots
                    IERG3810_TFTLCD_DrawDot(x,y+40,31); //red line
                }
                x=10;
                break;
            default:
                break;
            }
        }
    }
}
