#include "CONFIG.h"

extern u16 time;

//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	time++;
	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
}

//�ⲿ�жϳ�ʼ������
//��ʼ��PA0Ϊ�ж�����.
void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��

	GPIOA->CRL&=0XFFFFFFF0;	  
	GPIOA->CRL|=0X00000008; //PA0Ϊ��������
			   
	GPIOA->ODR|=1<<0;	   //PA0

	Ex_NVIC_Config(GPIO_A,0,RTIR);//shangsheng�ش���

	MY_NVIC_Init(2,3,EXTI0_IRQChannel,2);    //��ռ2�������ȼ�3����2  
}












