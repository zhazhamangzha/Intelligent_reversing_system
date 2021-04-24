/************************************************************
 * ���ܵ���ϵͳIRS
 * Ӳ�����ӣ�
 * PA�������J3
 * PB��LED��J3
 * PC��������J2
 * PD��LCDJ2
*************************************************************/

#include "CONFIG.H"	 	 

#define CSB_TX PCout(8)         // PC8 ??????????????????? 
#define BEEN   PCout(10)				// ?????????????????

//��������
u16 force=0,start=0,mode=0;//mode=1��̤����ƣ�mode=0��������
u16 time=0,speed=0,alpha=901;//time???????????speed?????alpha?????
u8 spd[7]={0},i,showforce[6];      //use to show speed
u32   status=0,val,real_time;						      // ???????????????????????
char  s2[10], s1[10];						// LCD????????????????
float	dis;                      // ???????????
u8    ShowDistance[7] = { 0 };				// ?????????????????????


//????????????
void PWM_Speed_Control(void);//���ת�ٿ���
void tran(void);//����������
void Displayspd(void);//��ʾ�ٶ�
void DisplayDis(float value);//��ʾ����
void Display_force(void);//��ʾ̤������
void Display(void);//LCD��ʾ����

int main(void)
{
	//��ʼ��
 	Stm32_Clock_Init(9);  //????????????
	delay_init(72);	      //???????????
	uart_init(72,9600);   //??????????? 
	PWM_Init(900,0);      //??????????PWM????=72000/900=80Khz
	EXTIX_Init();         //????????????
	Adc_Init();           //ADC start
	Init_12864();         // ???????????
	Timer3_Init(500,7199);//10Khz?????????????????????500?50ms  
  Timer4_Init(10, 71);  // 1Mhz?????????????????????10?10us  
	Timer5_Init(10000,7199);
	LED_Init();

  	while(1)
	{
 		delay_ms(10);

		start=(GPIOB->IDR>>11)&0x0001;//��ȡ����
		//start=PBin(11);
		mode=(GPIOB->IDR>>15)&0x0001;//��ȡģʽ
		//mode=PBin(15);
			
		if(start)
		{
			tran();
			PWM_Speed_Control();

			//when distanse belows to 3,warning!
			if (dis<10&&dis!=0) BEEN=1;
			else BEEN=0;
			if (dis>80) dis=80;
		}
		
		Display();

	}
}

// ?????????????????????
void tran(void)
{
	u8 i;
	TIM4->CR1 |= 0x01;         // ?????????3
	TIM4->SR &= ~(1 << 0);       // ????????????3???????    
	status = 0;			       // ?????????????????????????????
	for (i = 0; i < 8; i++)         // ??????????????????????????????41.67kHz
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
	else if(dis>30) alpha=200;
	else
	{
		if(mode)//mode=1��Ϊ̤��ģʽ
		{
			if(force<=3000) alpha=700-1*force/5;
			else alpha=250-1*force/20;
		}
		else alpha=-35*dis+700;//mode=2Ϊ����ģʽ
	}
	LED0_PWM_VAL=alpha;
}

void Display(void)
{
	if(start)
	{
		Display_string(0,0,"  ���ܵ���ϵͳ  ");

		if(mode)
		{
			Display_string(0,1,"̤��ģʽ  ");
			Display_force();
		}
		else
		{
			Display_string(0,1,"����ģʽ  ");
			DisplayDis(dis*10);
		}

		if(dis<10) Display_string(5,1,"��ͣ��");
		else Display_string(5,1,"δͣ��");

		Displayspd();
	}
	else
	{
		Display_string(0,0,"                ");
		Display_string(0,1,"  ���ܵ���ϵͳ  ");
		Display_string(0,2,"                ");
		Display_string(0,3,"          δ����");
	}
}

void Displayspd(void)
{
	Display_string(0,3,"  �ٶ�    ");
	spd[0]=speed/100+'0';           //????
	spd[1]=(speed/10)%10+'0';
	spd[2]=speed%10+'0';
	spd[3]='r';
	spd[4]='/';
	spd[5]='s';
	Display_string(5,3,spd);
}

void DisplayDis(float value)
{
	Display_string(0,2,"  ����    ");
    val=(u32)value;
    ShowDistance[0] = (val/100)+48;
    ShowDistance[1] = (val%100)/10+48;
	ShowDistance[2] = '.';
    ShowDistance[3] = val%10+48;
    ShowDistance[4] = 'c';
    ShowDistance[5] = 'm';
	Display_string(5,2,ShowDistance);
}

void Display_force(void)
{
	Display_string(0,2,"̤������  "); 
	showforce[0] = force/1000+'0';
	showforce[1] = (force/100)%10+'0';
	showforce[2] = (force/10)%10+'0';
	showforce[3] = '.';
	showforce[4] = (force%10)+'0';
	showforce[5] = 'N';
	Display_string(5,2,showforce);
}
