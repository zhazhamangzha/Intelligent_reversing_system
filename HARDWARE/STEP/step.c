#include "config.h"  

//初始化PORTE为输出口.并使能时钟		    
//IO初始化
void STEP_Init(void)
{
	RCC->APB2ENR|=1<<6;      //使能PORTC时钟	
											  
	GPIOE->CRL&=0XFFFF0000;
	GPIOE->CRL|=0X00003333;  //PORTC低八位推挽输出
}
