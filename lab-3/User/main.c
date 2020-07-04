#include "stm32f10x.h"
#include "IERG3810_TFTLCD.h"


int main(void){
    IERG3810_TFTLCD_init();
    while(1){
       IERG3810_TFTLCD_FillRectangle(2020,0,100,220,100);
       // yellow rectangle, x=100, y=100, length & width = 100
    }
}
