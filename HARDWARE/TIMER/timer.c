#include "CONFIG.h"

extern u16 time,speed,adc,start,mode;
static u8 timer_50ms=0;
extern u32 status;

u8 step_s=0,i;
u8 Step[8][4]={{1,0,0,0},  //A
			   {1,0,1,0},  //AB
			   {0,0,1,0},  //B
			   {0,1,1,0},  //BC
			   {0,1,0,0},  //C
			   {0,1,0,1},  //CD
			   {0,0,0,1},  //D
			   {1,0,0,1}}; //DA

//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//����ж�
	{
		//ֱ�����
	  	timer_50ms++;    
	  	if(timer_50ms>=20)  
	  	{
		  	speed = time;
		  	time = 0;
		  	timer_50ms = 0;
	  	}

		//�������
		if(start)
		{
			for(i=0;i<4;i++)
				PEout(i)=Step[step_s][i];
			if(mode) step_s=(step_s+1)%8;
			else step_s=7-((8-step_s)%8);
		}
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}

//��ʱ��4�жϷ������	 
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//����ж�
	{
		status++; 	  	
	}				   
	TIM4->SR&=~(1<<0);//����жϱ�־λ 	    
}

//��ʱ��5�жϷ������
void TIM5_IRQHandler(void)
{
	if(TIM5->SR&0x0001)
	{
		adc=Get_Adc(ADC_CH2);
	}
	TIM5->SR&=~(1<<0);
}

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void Timer3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;//TIM3ʱ��ʹ��    
 	TIM3->ARR=arr;  //�趨�������Զ���װֵ//�պ�1ms    
	TIM3->PSC=psc;  //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	//����������Ҫͬʱ���òſ���ʹ���ж�
	TIM3->DIER|=1<<0;   //��������ж�				
	TIM3->DIER|=1<<6;   //�������ж�	   
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  MY_NVIC_Init(1,3,TIM3_IRQChannel,2);//��ռ1�������ȼ�3����2									 
}

void Timer4_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;//TIM3ʱ��ʹ��  
 	TIM4->ARR=arr;  //�趨�������Զ���װֵ//�պ�1ms    
	TIM4->PSC=psc;  //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	TIM4->DIER|=1<<0;   //��������ж�				
	TIM4->DIER|=1<<6;   //�������ж�	   
	//TIM3->CR1|=0x01;    //�˴��Ȳ�Ҫʹ�ܶ�ʱ��3��Ҫ�ȴ�����������ʱͬ���������������ǰ��ʼ���
  MY_NVIC_Init(1,3,TIM4_IRQChannel,2);//��ռ1�������ȼ�3����2
}

void Timer5_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<3;
	TIM5->ARR=arr;
	TIM5->PSC=psc;
	TIM5->DIER|=1<<0;
	TIM5->DIER|=1<<6;
	TIM5->CR1|=0x01;
	MY_NVIC_Init(2,3,TIM5_IRQChannel,2);
}
