#include "CONFIG.H"

//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void PWM_Init(u16 arr,u16 psc)
{		 					
	RCC->APB1ENR|=1<<0; //TIM2ʱ��ʹ��   
 
  GPIOA->CRL&=0XFFFFFF0F;
	GPIOA->CRL|=0X000000B0;//PA1����Ϊ���ù����������ģʽ������50Mhz��
	GPIOA->ODR|=1<<1;//PA1������ʼ��

	TIM2->ARR=arr;//�趨�������Զ���װֵ 
	TIM2->PSC=psc;//Ԥ��Ƶ������Ƶ
	
	TIM2->CCMR1|=7<<12;  //CH2 PWM2ģʽ		 
	TIM2->CCMR1|=1<<11; //CH2Ԥװ��ʹ��	   

	TIM2->CCER|=1<<4;   //���벶��2���ʹ�� 

	TIM2->CR1=0x80;   //ARPE���Զ�װ��Ԥװ��ʹ��λ��ʹ�� 
	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��2				  
}  	 













