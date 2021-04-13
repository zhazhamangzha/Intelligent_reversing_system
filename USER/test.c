/************************************************************
 * ���ܵ���ϵͳIRS
 * Ӳ�����ӣ�
 * PA�������J3
 * PB�����ذ�J3
 * PC��������������Ҫ���ߣ�
 * PD��LCDJ2
 * PE��������J2
*************************************************************/

#include "CONFIG.H"	 	 

//��������
u16 force=0,sw=0;//����force���ܿ���sw
float distance;//����distance
u16 time=0,speed=0,alpha=901;//time���ټ�ʱ��speed�ٶȣ�alphaռ�ձ�
u8 dis[7]={0},i;

//��������
void PWM_Speed_Control();//����ٶȿ���

int main(void)
{
	//��ʼ��
 	Stm32_Clock_Init(9);  //ϵͳʱ������
	delay_init(72);	      //��ʱ��ʼ��
	uart_init(72,9600);   //���ڳ�ʼ�� 
	PWM_Init(900,0);      //����Ƶ��PWMƵ��=72000/900=80Khz
	EXTIX_Init();         //�ⲿ�жϳ�ʼ��
	Timerx_Init(500,7199);//10Khz�ļ���Ƶ�ʣ�������500Ϊ50ms  
	Init_12864();
	STM_Init();           //��������ʼ��

  	while(1)
	{
 		delay_ms(10);

		if(sw)
		{
			PWM_Speed_Control();
		}

/**********ת��*******************/

		dis[0]=speed/100+0x30;           //ת�ٷֽ�
		dis[1]=(speed/10)%10+0x30;
		dis[2]=speed%10+0x30;
		dis[3]='r';
		dis[4]='/';
		dis[5]='s';
		
	  write_12864com(0x88+4);	         //��ʾ�ڵ�3��
		delay_us(200);                    
		for(i=6;i>0;i--)
		{
			write_12864data(dis[6-i]);	    //��ʾת��
			delay_us(200);                  //�ȴ�д��
		}
	}	 
}

void PWM_Speed_Control()
{
	if(distance<10) alpha=901;
	else//���ٷ�ʽδ��
	{}
}
