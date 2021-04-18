/************************************************************
 * ���ܵ���ϵͳIRS
 * Ӳ�����ӣ�
 * PA�������J3
 * PB�����ذ�J3
 * PD��LCDJ2
 * PE��������J2
*************************************************************/

#include "CONFIG.H"	 	 

#define CSB_TX PCout(8)         // PC8 ���ͳ��������� 
#define BEEN   PCout(10)				// ���?��������

//��������
u16 force=0,start=0;//����force���ܿ���sw
u16 time=0,speed=0,alpha=901;//time���ټ�ʱ��speed�ٶȣ�alphaռ�ձ�
u8 spd[7]={0},i,showforce[6];      //use to show speed
u32   status=0,val,real_time;						      // ���붨ʱ����������ֵ
char  s2[10], s1[10];						// LCD��ʾ��������
float	dis;                      // ʩ������ֵ
u8    ShowDistance[7] = { 0 };				// ������ʾ�������?
u8    power[10];                //����ʾ�������?


//��������
void PWM_Speed_Control(void);//����ٶȿ���?
void tran(void); 					      // ��������������
void Displayspd(void);      //�ٶ���ʾ����
void DisplayDis(float value);       //������ʾ����
void Display_force(void);  //��̤��������ʾ����

int main(void)
{
	//��ʼ��
 	Stm32_Clock_Init(9);  //ϵͳʱ������
	delay_init(72);	      //��ʱ��ʼ��
	uart_init(72,9600);   //���ڳ�ʼ�� 
	PWM_Init(900,0);      //����Ƶ��PWMƵ��=72000/900=80Khz
	EXTIX_Init();         //�ⲿ�жϳ�ʼ��
	Timer3_Init(500,7199);//10Khz�ļ���Ƶ�ʣ�������500Ϊ50ms  
	Timer4_Init(10, 71);  // 1Mhz�ļ���Ƶ�ʣ�������10Ϊ10us  
	Init_12864();         // Һ����ʼ��
	EXTIX_Init();         // �ⲿ�жϳ�ʼ��
	Adc_Init();           //ADC start
	LED_Init();


  	while(1)
	{
 		delay_ms(10);

		start=(GPIOB->IDR&&0x0100)>>8;//��������
		if(start)
		{
			tran();		//���䳬����
			PWM_Speed_Control();

			//when distanse belows to 3,warning!
			if (dis<10&&dis!=0) BEEN=1;
			else BEEN=0;		//���?��
			if (dis>80) dis=80;		//��ֹ������������Ĳ��ʧ��

			/******************ϵͳ������ʾ****************/
			Display_string(0,0,"ϵͳ�Ƿ���");

			if(start) Display_string(0,3,"��");
			else Display_string(0,3,"��");
			
			Displayspd();
			DisplayDis(dis*10);
			Display_force();
		}

	}
}

// ��������������
void tran(void)
{
	u8 i;
	TIM4->CR1 |= 0x01;         // ʹ�ܶ�ʱ��3
	TIM4->SR &= ~(1 << 0);       // �����ʱ��?3�жϱ�־λ    
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

void PWM_Speed_Control(void)
{
	if(dis<10) alpha=901;
	else if(dis>30)//���ٷ�ʽδ��
	{
		alpha=200;
	}
	else
	{
		alpha=-35*dis+1250;
	}
	LED0_PWM_VAL=alpha;
}

void Displayspd(void)
{
	Display_string(0,1,"�ٶ�");
	spd[0]=speed/100+0x30;           //ת�ٷֽ�
	spd[1]=(speed/10)%10+0x30;
	spd[2]=speed%10+0x30;
	spd[3]='r';
	spd[4]='/';
	spd[5]='s';
	Display_string(5,1,spd);
}

void DisplayDis(float value)
{
	Display_string(0,2,"����");
    val = (u32)value;
    ShowDistance[0] = (val/100)+48;
    ShowDistance[1] = (val%100)/10+48;
		ShowDistance[2] = '.';
    ShowDistance[3] = val%10+48;
    ShowDistance[4] = 'c';
    ShowDistance[5] = 'm';
		Display_string(5,2,ShowDistance);
}

void Display_force()
{
	Display_string(0,3,"��̤������"); 
	showforce[0] = force/1000+'0';
	showforce[1] = (force/100)%10+'0';
	showforce[2] = (force/10)%10+'0';
	showforce[3] = (force%10)+'0';
	showforce[4] = 'N';
	Display_string(5,3,showforce);
}
