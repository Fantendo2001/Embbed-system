#include "stm32f10x.h"
typedef struct{
    u16 LCD_REG;
    u16 LCD_RAM;
}LCD_TypeDef;

#define LCD_BASE ((u32)(0x6C000000|0x000007FE))
#define LCD ((LCD_TypeDef*) LCD_BASE)
#define LCD_LIGHT_ON GPIOB->BSRR |= 0x000000001;

void IERG3810_TFTLCD_init(void);
void IERG3810_TFTLCD_SetParameter(void);
void IERG3810_TFTLCD_WrReg(u16);
void IERG3810_TFTLCD_WrData(u16);
void IERG3810_TFTLCD_DrawDot(u16, u16, u16);
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
void IERG3810_TFTLCD_SetParameter(void){
    IERG3810_TFTLCD_WrReg(0x01); //Software reset
    IERG3810_TFTLCD_WrReg(0x11); //Exit_sleep_mode

    IERG3810_TFTLCD_WrReg(0x3A); //Set_pixel_format
    IERG3810_TFTLCD_WrData(0x55); //65536 colors

    IERG3810_TFTLCD_WrReg(0x29); //Display ON

    IERG3810_TFTLCD_WrReg(0x36); //Memory Access Control (section 8.2.29, p. 127)
    IERG3810_TFTLCD_WrData(0xCA);
}
void IERG3810_TFTLCD_init(void){//set FSMC
    RCC->AHBENR|=1<<8; //FSMC
    RCC->APB2ENR|=1<<3; //PORTB
    RCC->APB2ENR|=1<<5; //PORTD
    RCC->APB2ENR|=1<<6; //PORTE
    RCC->APB2ENR|=1<<8; //PORTG
    GPIOB->CRL&=0xFFFFFFF0; //PB0
    GPIOB->CRL|=0x00000003;

    //PORTD
    GPIOD->CRH&=0x00FFF000;
    GPIOD->CRH|=0xBB000BBB;
    GPIOD->CRL&=0xFF00FF00;
    GPIOD->CRL|=0x00BB00BB;
    //PORTE
    GPIOE->CRH&=0x00000000;
    GPIOE->CRH|=0xBBBBBBBB;
    GPIOE->CRL&=0x0FFFFFFF;
    GPIOE->CRL|=0xB0000000;
    //PORTG12
    GPIOG->CRH&=0xFFF0FFFF;
    GPIOG->CRH|=0x000B0000;
    GPIOG->CRL&=0xFFFFFFF0; //PG0->RS
    GPIOG->CRL|=0x0000000B;

    //LCD uses FSMC Bank 4 memory bank
    //Use mode A
    FSMC_Bank1->BTCR[6]=0x00000000; //FSMC_BCR4 (reset)
    FSMC_Bank1->BTCR[7]=0x00000000; //FSMC_BTR4 (reset)
    FSMC_Bank1E->BWTR[6]=0x00000000; //FSMC_BWTR4 (reset)
    FSMC_Bank1->BTCR[6]|=1<<12; //FSMC_BCR4 -> WREN
    FSMC_Bank1->BTCR[6]|=1<<14; //FSMC_BCR4 -> EXTMOD
    FSMC_Bank1->BTCR[6]|=1<<4; //FSMC_BCR4 -> MWID
    FSMC_Bank1->BTCR[7]|=0<<28; //FSMC_BTR4 -> ACCMOD
    FSMC_Bank1->BTCR[7]|=1<<0; //FSMC_BTR4 -> ADDSET
    FSMC_Bank1->BTCR[7]|=0xF<<8; //FSMC_BTR4 -> DATAST
    FSMC_Bank1E->BWTR[6]|=0<<28; //FSMC_BWTR4 -> ACCMOD
    FSMC_Bank1E->BWTR[6]|=0<<0; //FSMC_BWTR4 -> ADDSET
    FSMC_Bank1E->BWTR[6]|=3<<8; //FSMC_BWTR4 -> DATAST
    FSMC_Bank1->BTCR[6]|=1<<0; //FSMC_BCR4 -> FACCEN
    IERG3810_TFTLCD_SetParameter(); //special setting for LCD module
    LCD_LIGHT_ON;
}
void IERG3810_TFTLCD_WrReg(u16 regval){
    LCD->LCD_REG=regval;
}
void IERG3810_TFTLCD_WrData(u16 data){
    LCD->LCD_RAM=data;
}
void IERG3810_TFTLCD_DrawDot(u16 x, u16 y, u16 color){
    IERG3810_TFTLCD_WrReg(0x2A); //set x position
    IERG3810_TFTLCD_WrData(x>>8);
    IERG3810_TFTLCD_WrData(x & 0xFF);
    IERG3810_TFTLCD_WrData(0x01);
    IERG3810_TFTLCD_WrData(0x3F);
    IERG3810_TFTLCD_WrReg(0x2B); //set y position
    IERG3810_TFTLCD_WrData(y>>8);
    IERG3810_TFTLCD_WrData(y & 0xFF);
    IERG3810_TFTLCD_WrData(0x01);
    IERG3810_TFTLCD_WrData(0xDF);
    IERG3810_TFTLCD_WrReg(0x2C); //set point with color
    IERG3810_TFTLCD_WrData(color);
}