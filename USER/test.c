/************************************************************
 * �泵ģ��ϵͳ
 * Ӳ�����ӣ�
 * PA�������J3
 * PB��LED��J3
 * PC��������J2
 * PD��LCDJ2
 * PE�������J2
*************************************************************/

#include "CONFIG.H"

#define CSB_TX PCout(8)         //PC8
#define BEEN   PCout(10)				//PC10������������

//��������
u16 adc=0,start=0,mode=0,sw=0;//adcΪģ������start��ʾϵͳ�Ƿ񿪹أ�mode��ʾת������swΪ��ά��ͼƬ��ʾ����
u16 time=0,speed=0,alpha=901;//timeΪֱ��������м�����speedΪֱ������ٶȣ�aphla����TIM2->CCR2��ֵ����ռ�ձ�
u8 spd[7]={0},showadc[10],ShowDistance[7];//spdΪ��ʾ�ٶȵ��ַ����飬showadcΪ��ʾadc�����飬showDistanceΪ��ʾ������ַ�����
u32 status=0,val,real_time;//status��¼������ʱ�䣬val����ʾ����ֵ��real_timeΪ��������ر���
float dis;//dis��ʾ��ȡ���ľ���


//��������
void PWM_Speed_Control(void);//���ת�ٿ��ƺ���
void tran(void);//����������
void Displayspd(void);//��ʾ�ٶ�
void DisplayDis(float value);//��ʾ����
void Display_adc(void);//��ʾ��������
void Display(void);//LCD��ʾ����

int main(void)
{
	//��ʼ��
 	Stm32_Clock_Init(9);//ϵͳʱ������
	delay_init(72);//��ʱ��ʼ��
	uart_init(72,9600);//���ڳ�ʼ�� 
	PWM_Init(900,0);//PWM��ʼ��
	EXTIX_Init();//�ⲿ�жϳ�ʼ��
	Adc_Init();//ADC��ʼ��
	Init_12864();//LCD12864��ʼ��
	STEP_Init();//���������ʼ��
	LED_Init();//LED���ʼ��
	Timer3_Init(500,7199);//10khz��Ƶ��������500Ϊ50ms
	Timer4_Init(10, 71); //1Mhz��Ƶ��������10Ϊ10us
	Timer5_Init(10000,7199);//10khz��Ƶ��������10000Ϊ1s

	//ϵͳ������ʾ1sͼƬ
	clnGDR_12864();//����
	Display_QRcode();//��ʾͼƬ
	delay_ms(1000);
	clnGDR_12864();//����

	//������
  	while(1)
	{
 		delay_ms(10);

		start=(GPIOB->IDR>>11)&0x0001;//��ȡϵͳ����
		mode=(GPIOB->IDR>>15)&0x0001;//��ȡ��ת����
		sw=(GPIOB->IDR>>13)&0x0001;//��ȡͼƬ��ʾ����

		if(start)//ϵͳ����
		{
			tran();
			PWM_Speed_Control();

			//����С��10��ʹ�ܷ�����
			if (dis<10&&dis!=0) BEEN=1;
			else BEEN=0;
			if (dis>80) dis=80;
		}
		else//ϵͳ�ر�
		{
			BEEN=0;//ֹͣ������
			LED0_PWM_VAL=901;//ֱֹͣ�����ת��
		}

		Display();//��ʾ
	}
}

//����������
void tran(void)
{
	u8 i;
	TIM4->CR1 |= 0x01;//ʹ�ܶ�ʱ��4
	TIM4->SR &= ~(1 << 0);//�����жϱ�־λ
	status = 0;//��ʱ��״̬��0
	for (i = 0; i < 8; i++)//�����ź� 41.67kHz
	{
		CSB_TX = 1;
		delay_us(12);
		CSB_TX = 0;
		delay_us(12);
	}
	delay_ms(10);
}

//���ת�ٿ��ƺ���
void PWM_Speed_Control(void)
{
	if(dis<10) alpha=901;//������С��0�����ͣת
	else if(dis>30) alpha=200;//�������30������ת��
	else alpha=-35*dis+700;//���ݾ������ת��
	LED0_PWM_VAL=alpha;//�ı�TIM2->CCR2
}

//LCD��ʾ����
void Display(void)
{
	if(sw)//ͼƬ��ʾ���ؿ���
	{
		//��������
		Display_string(0,0,"                ");
		Display_string(0,1,"                ");
		Display_string(0,2,"                ");
		Display_string(0,3,"                ");

		Display_QRcode();//��ʾͼƬ
		while(1)//ѭ����ȡ����ֱ��ͼƬ��ʾ���عر�
		{
			sw=(GPIOB->IDR>>13)&0x0001;
			if(!sw) break;//�����عر�������ѭ��
			delay_ms(10);
		}
	}

	clnGDR_12864();//����ͼƬ
	if(start)//ϵͳ���ؿ�������ʾϵͳ���ơ���ת�����ٶȡ����롢��������(adc)
	{
		Display_string(0,0,"�泵ģ��ϵͳ    ");
		if(mode) Display_string(7,0,"��");
		else Display_string(7,0,"��");
		Displayspd();
		DisplayDis(dis*10);
		Display_adc();
	}
	else//ϵͳ��ʾ���عرգ���ʾϵͳ���Ƽ���δ������
	{
		Display_string(0,0,"                ");
		Display_string(0,1,"  �泵ģ��ϵͳ  ");
		Display_string(0,2,"                ");
		Display_string(0,3,"          δ����");
	}
}

//��ʾ�ٶ�
void Displayspd(void)
{
	Display_string(0,1,"  �ٶ�    ");

	//���ٶ�ֵת��Ϊ�ַ����鲢��ʾ
	spd[0]=speed/100+'0';
	spd[1]=(speed/10)%10+'0';
	spd[2]=speed%10+'0';
	spd[3]='r';
	spd[4]='/';
	spd[5]='s';
	Display_string(5,1,spd);
}

//��ʾ����
void DisplayDis(float value)
{
	Display_string(0,2,"��ؾ���  ");

	//������ֵת��Ϊ�ַ����鲢��ʾ
    val=(u32)value;//��valueֵתΪ���ͷ������
    ShowDistance[0] = (val/100)+48;
    ShowDistance[1] = (val%100)/10+48;
	ShowDistance[2] = '.';
    ShowDistance[3] = val%10+48;
    ShowDistance[4] = 'c';
    ShowDistance[5] = 'm';
	Display_string(5,2,ShowDistance);
}

//��ʾ��������
void Display_adc(void)
{
	Display_string(0,3,"��������  "); 

	//��adcֵת��Ϊ�ַ����鲢��ʾ	
	showadc[0] = ' ';
	showadc[1] = adc/1000+'0';
	showadc[2] = (adc/100)%10+'0';
	showadc[3] = (adc/10)%10+'0';
	showadc[4] = '.';
	showadc[5] = (adc%10)+'0';
	showadc[6] = 'k';
	showadc[7] = 'g';
	Display_string(4,3,showadc);
}
