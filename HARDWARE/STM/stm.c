#include "CONFIG.h"

void  STM_Init(void)
{    
	//先初始化IO口
 	RCC->APB2ENR|=1<<4;    //使能PORTC口时钟 
	GPIOC->CRL&=0XFFF0FFFF;//PC4
	GPIOC->CRL|=0X00030000;
}
