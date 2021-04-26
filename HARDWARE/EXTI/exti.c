#include "CONFIG.h"

extern u16 time;
extern u32 real_time;
extern u32 status;
extern float dis;

//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	time++;
	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
}

//�ⲿ�ж�0�������
void EXTI9_5_IRQHandler(void)
{
    TIM4->CR1&=~(0x01);                  // ��ʹ�ܶ�ʱ��3

    real_time = status ;
    dis=(float)real_time*1.7/10-2;       // �������	��ʱ10us��S=Vt/2����2��������
   
	EXTI->PR=1<<9;  //���LINE9_5�ϵ��жϱ�־λ  
}

//�ⲿ�жϳ�ʼ������
//��ʼ��PA0Ϊ�ж�����.
void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��

	GPIOA->CRL&=0XFFFFFFF0;	  
	GPIOA->CRL|=0X00000008; //PA0Ϊ��������
			   
	GPIOA->ODR|=1<<0;	   //PA0

	Ex_NVIC_Config(GPIO_A,0,RTIR);//�����ش���

	MY_NVIC_Init(2,3,EXTI0_IRQChannel,2);    //��ռ2�������ȼ�3����2 

	RCC->APB2ENR|=1<<4;      // ʹ��PORTCʱ��    

	GPIOC->CRH&=0XFFFFF000;  // PC9���óɳ��ⲿ�ж�����Դ 
	GPIOC->CRH|=0X00000383;   
	
	GPIOC->ODR|=0<<8;
	GPIOC->ODR|=1<<9;	
	GPIOC->ODR|=0<<10;       // �����ʼ��

	Ex_NVIC_Config(GPIO_C,9,FTIR); // �½��ش���

	MY_NVIC_Init(2,2,EXTI9_5_IRQChannel,2);    //��ռ2�������ȼ�2����2	    
}












