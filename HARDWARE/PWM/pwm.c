#include "CONFIG.H"

//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void PWM_Init(u16 arr,u16 psc)
{		 					
	RCC->APB1ENR|=1<<0; //TIM2时钟使能   
 
  GPIOA->CRL&=0XFFFFFF0F;
	GPIOA->CRL|=0X000000B0;//PA1设置为复用功能推挽输出模式（速率50Mhz）
	GPIOA->ODR|=1<<1;//PA1上拉初始化

	TIM2->ARR=arr;//设定计数器自动重装值 
	TIM2->PSC=psc;//预分频器不分频
	
	TIM2->CCMR1|=7<<12;  //CH2 PWM2模式		 
	TIM2->CCMR1|=1<<11; //CH2预装载使能	   

	TIM2->CCER|=1<<4;   //输入捕获2输出使能 

	TIM2->CR1=0x80;   //ARPE（自动装载预装载使能位）使能 
	TIM2->CR1|=0x01;    //使能定时器2				  
}  	 













