#ifndef __12864_H__
#define __12864_H__



#define uint8_t unsigned char
	
#define PIN_RES                  (   1 << 4)
#define PIN_PSB                  (   1 << 3)
#define PIN_EN                   (   1 << 2)
#define PIN_RW                   (   1 << 1)
#define PIN_RS                   (   1 << 0)

#define PIN_D0                   (   1 << 8 )
#define PIN_D1                   (   1 << 9 )
#define PIN_D2                   (   1 << 10)
#define PIN_D3                   (   1 << 11)
#define PIN_D4                   (   1 << 12)
#define PIN_D5                   (   1 << 13)
#define PIN_D6                   (   1 << 14)
#define PIN_D7                   (   1 << 15)


 //Ğ´1£¬0                                              
#define e(x)               GPIOD->ODR = (GPIOD->ODR & ~PIN_EN)  | (x ? PIN_EN :  0);                                               
#define rw(x)              GPIOD->ODR = (GPIOD->ODR & ~PIN_RW)  | (x ? PIN_RW :  0);                                             
#define rs(x)              GPIOD->ODR = (GPIOD->ODR & ~PIN_RS)  | (x ? PIN_RS :  0);                                          
#define psb(x)             GPIOD->ODR = (GPIOD->ODR & ~PIN_PSB) | (x ? PIN_PSB:  0);                                    
#define res(x)             GPIOD->ODR = (GPIOD->ODR & ~PIN_RES) | (x ? PIN_RES:  0);

#define D0(x)              GPIOD->ODR = (GPIOD->ODR & ~PIN_D0)  | (x ? PIN_D0 : 0);
#define D1(x)              GPIOD->ODR = (GPIOD->ODR & ~PIN_D1)  | (x ? PIN_D1 : 0);
#define D2(x)              GPIOD->ODR = (GPIOD->ODR & ~PIN_D2)  | (x ? PIN_D2 : 0);
#define D3(x)              GPIOD->ODR = (GPIOD->ODR & ~PIN_D3)  | (x ? PIN_D3 : 0);
#define D4(x)              GPIOD->ODR = (GPIOD->ODR & ~PIN_D4)  | (x ? PIN_D4 : 0);
#define D5(x)              GPIOD->ODR = (GPIOD->ODR & ~PIN_D5)  | (x ? PIN_D5 : 0);
#define D6(x)              GPIOD->ODR = (GPIOD->ODR & ~PIN_D6)  | (x ? PIN_D6 : 0);
#define D7(x)              GPIOD->ODR = (GPIOD->ODR & ~PIN_D7)  | (x ? PIN_D7 : 0);


//º¯ÊıÉùÃ÷
void write_12864com(uint8_t com);
void write_12864data(uint8_t dat);
void Init_12864(void);
void Display_string(uint8_t x,uint8_t y,uint8_t *s) ;

#endif
