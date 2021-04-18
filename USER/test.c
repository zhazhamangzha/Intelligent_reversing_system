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

#define CSB_TX PCout(8)         // PC8 ���ͳ��������� 
#define BEEN   PCout(10)				// ��౨��������

//��������
u16 force=0,start=0;//����force���ܿ���sw
float distance;//����distance
u16 time=0,speed=0,alpha=901;//time���ټ�ʱ��speed�ٶȣ�alphaռ�ձ�
u8 ShowSpeed[7]={0},i;      //use to show speed
u32   status = 0;						      // ���붨ʱ����������ֵ
char  s2[10], s1[10];						// LCD��ʾ��������
u32   real_time;	 				      // ����ֵ
float	dis;						          // �������ֵ
float pow;                      // ʩ������ֵ
u8    ShowDistance[7] = { 0 };				// ������ʾ�������
u8    power[10];                //����ʾ�������


//��������
void PWM_Speed_Control();//����ٶȿ���
void tran(void); 					      // ��������������
void DisplayDis(float value);		// �����ʾ����
void DisplayPow(u16 power);     // ʩ����ʾ����
void Display(u8 Pos);						// λ����ʾ����
void GameStart(void);           // ���ؼ�����

int main(void)
{
	//��ʼ��
 	Stm32_Clock_Init(9);  //ϵͳʱ������
	delay_init(72);	      //��ʱ��ʼ��
	uart_init(72,9600);   //���ڳ�ʼ�� 
	PWM_Init(900,0);      //����Ƶ��PWMƵ��=72000/900=80Khz
	EXTIX_Init();         //�ⲿ�жϳ�ʼ��
	Timerx_Init(500,7199);//10Khz�ļ���Ƶ�ʣ�������500Ϊ50ms  
	Timerx_Init(10, 71);  // 1Mhz�ļ���Ƶ�ʣ�������10Ϊ10us  
	Init_12864();         // Һ����ʼ��
	STM_Init();           //��������ʼ��
	EXTIX_Init();         // �ⲿ�жϳ�ʼ��
	Adc_Init();           //ADC start


  	while(1)
	{
 		delay_ms(10);
		DisplayPow(Get_Adc(ADC_CH2));

		if(start)
		{
			tran();		//���䳬����
			PWM_Speed_Control();

			//when distanse belows to 3,warning!
			if (dis < 3 && dis != 0)
				BEEN = 1;
			else
				BEEN = 0;		//��౨��
			if (dis > 80)
				dis = 80;		//��ֹ������������Ĳ��ʧ��
			DisplayDis(dis * 10);
			DisplayPow(pow);
			//if(distance<3) GPIOE->ODR|=0x00000010;//�������������������
			//else GPIOE->ODR&=0xFFFFFFEF;//��������
		}

/**********ת��*******************/

		ShowSpeed[0]=speed/100+0x30;           //ת�ٷֽ�
		ShowSpeed[1]=(speed/10)%10+0x30;
		ShowSpeed[2]=speed%10+0x30;
		ShowSpeed[3]='r';
		ShowSpeed[4]='/';
		ShowSpeed[5]='s';
		
	  write_12864com(0x88+4);	         //��ʾ�ڵ�3��
		delay_us(200);                    
		for(i=6;i>0;i--)
		{
			write_12864data(ShowSpeed[6-i]);	    //��ʾת��
			delay_us(200);                  //�ȴ�д��
		}
	}	 
}

// ��������������
void tran(void)
{
	u8 i;
	TIM4->CR1 |= 0x01;         // ʹ�ܶ�ʱ��3
	TIM4->SR &= ~(1 << 0);       // �����ʱ��3�жϱ�־λ    
	status = 0;			       // ���붨ʱ����������ֵ����
	for (i = 0; i < 8; i++)         // �������źŷ���������Ƶ��41.67kHz
	{
		CSB_TX = 1;
		delay_us(12);
		CSB_TX = 0;
		delay_us(12);
	}
	delay_ms(10);
}

//��ʾ����ο�����
void DisplayDis(float value)
{
	u32 val = (u32)value;
	ShowDistance[0] = (val / 100) + 48;
	ShowDistance[1] = (val % 100) / 10 + 48;
	ShowDistance[2] = '.';
	ShowDistance[3] = val % 10 + 48;
	ShowDistance[4] = ' ';
	ShowDistance[5] = 'c';
	ShowDistance[6] = 'm';
	Display(0x9C);
}

void PWM_Speed_Control()
{
	if(distance<10) alpha=901;
	else//���ٷ�ʽδ��
	{}
}
