#include "CONFIG.h"

extern u16 time;
extern u32 real_time;
extern u32 status;
extern float dis;

//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	time++;
	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
}

//外部中断0服务程序
void EXTI9_5_IRQHandler(void)
{
    TIM4->CR1&=~(0x01);                  // 关使能定时器3

    real_time = status ;
    dis=(float)real_time*1.7/10-2;       // 计算距离	定时10us，S=Vt/2（减2是误差补尝）
   
	EXTI->PR=1<<9;  //清除LINE9_5上的中断标志位  
}

//外部中断初始化程序
//初始化PA0为中断输入.
void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟

	GPIOA->CRL&=0XFFFFFFF0;	  
	GPIOA->CRL|=0X00000008; //PA0为上拉输入
			   
	GPIOA->ODR|=1<<0;	   //PA0

	Ex_NVIC_Config(GPIO_A,0,RTIR);//上升沿触发

	MY_NVIC_Init(2,3,EXTI0_IRQChannel,2);    //抢占2，子优先级3，组2 

	RCC->APB2ENR|=1<<4;      // 使能PORTC时钟    

	GPIOC->CRH&=0XFFFFF000;  // PC9设置成成外部中断输入源 
	GPIOC->CRH|=0X00000383;   
	
	GPIOC->ODR|=0<<8;
	GPIOC->ODR|=1<<9;	
	GPIOC->ODR|=0<<10;       // 输出初始化

	Ex_NVIC_Config(GPIO_C,9,FTIR); // 下降沿触发

	MY_NVIC_Init(2,2,EXTI9_5_IRQChannel,2);    //抢占2，子优先级2，组2	    
}












