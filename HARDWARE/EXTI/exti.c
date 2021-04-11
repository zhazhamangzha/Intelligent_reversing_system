#include "CONFIG.h"

extern u16 time;

//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	time++;
	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
}

//外部中断初始化程序
//初始化PA0为中断输入.
void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟

	GPIOA->CRL&=0XFFFFFFF0;	  
	GPIOA->CRL|=0X00000008; //PA0为上拉输入
			   
	GPIOA->ODR|=1<<0;	   //PA0

	Ex_NVIC_Config(GPIO_A,0,RTIR);//shangsheng沿触发

	MY_NVIC_Init(2,3,EXTI0_IRQChannel,2);    //抢占2，子优先级3，组2  
}












