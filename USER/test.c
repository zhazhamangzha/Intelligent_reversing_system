/************************************************************
 * 叉车模拟系统
 * 硬件连接：
 * PA：电机板J3
 * PB：LED板J3
 * PC：超声波J2
 * PD：LCDJ2
 * PE：电机板J2
*************************************************************/

#include "CONFIG.H"	 	 

#define CSB_TX PCout(8)         // PC8 ??????????????????? 
#define BEEN   PCout(10)				// ?????????????????

//变量定义
u16 adc=0,start=0,mode=0,sw=0;//mode=1则向下，mode=0则向上
u16 time=0,speed=0,alpha=901;//time???????????speed?????alpha?????
u8 spd[7]={0},showadc[10];      //use to show speed
u32   status=0,val,real_time;						      // ???????????????????????
char  s2[10], s1[10];						// LCD????????????????
float	dis;                      // ???????????
u8    ShowDistance[7] = { 0 };				// ?????????????????????


//????????????
void PWM_Speed_Control(void);//电机转速控制
void tran(void);//超声波函数
void Displayspd(void);//显示速度
void DisplayDis(float value);//显示距离
void Display_adc(void);//显示货物重量
void Display(void);//LCD显示函数

int main(void)
{
	//初始化
 	Stm32_Clock_Init(9);  //????????????
	delay_init(72);	      //???????????
	uart_init(72,9600);   //??????????? 
	PWM_Init(900,0);      //??????????PWM????=72000/900=80Khz
	EXTIX_Init();         //????????????
	Adc_Init();           //ADC start
	Init_12864();         // ???????????
	STEP_Init();
	LED_Init();
	Timer3_Init(500,7199);//10Khz?????????????????????500?50ms  
	Timer4_Init(10, 71);  // 1Mhz?????????????????????10?10us  
	Timer5_Init(10000,7199);

	
	clnGDR_12864();
	Display_QRcode();
	delay_ms(1000);
	clnGDR_12864();

  	while(1)
	{
 		delay_ms(10);

		start=(GPIOB->IDR>>11)&0x0001;//获取开关
		//start=PBin(11);
		mode=(GPIOB->IDR>>15)&0x0001;//获取模式
		//mode=PBin(15);
		sw=(GPIOB->IDR>>13)&0x0001;
			
		if(start)
		{
			tran();
			PWM_Speed_Control();

			//when distanse belows to 3,warning!
			if (dis<10&&dis!=0) BEEN=1;
			else BEEN=0;
			if (dis>80) dis=80;
		}
		else
		{
			BEEN=0;
			LED0_PWM_VAL=901;
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
	else alpha=-35*dis+700;
	LED0_PWM_VAL=alpha;
}

void Display(void)
{
	if(sw)
	{
		Display_string(0,0,"                ");
		Display_string(0,1,"                ");
		Display_string(0,2,"                ");
		Display_string(0,3,"                ");
		Display_QRcode();
		while(1)
		{
			sw=(GPIOB->IDR>>13)&0x0001;
			if(!sw) break;
			delay_ms(10);
		}
	}
	clnGDR_12864();
	if(start)
	{
		Display_string(0,0,"叉车模拟系统    ");
		if(mode) Display_string(7,0,"↓");
		else Display_string(7,0,"↑");
		Displayspd();
		DisplayDis(dis*10);
		Display_adc();
	}
	else
	{
		Display_string(0,0,"                ");
		Display_string(0,1,"  叉车模拟系统  ");
		Display_string(0,2,"                ");
		Display_string(0,3,"          未开机");
	}
}

void Displayspd(void)
{
	Display_string(0,1,"  速度    ");
	spd[0]=speed/100+'0';           //????
	spd[1]=(speed/10)%10+'0';
	spd[2]=speed%10+'0';
	spd[3]='r';
	spd[4]='/';
	spd[5]='s';
	Display_string(5,1,spd);
}

void DisplayDis(float value)
{
	Display_string(0,2,"离地距离  ");
    val=(u32)value;
    ShowDistance[0] = (val/100)+48;
    ShowDistance[1] = (val%100)/10+48;
	ShowDistance[2] = '.';
    ShowDistance[3] = val%10+48;
    ShowDistance[4] = 'c';
    ShowDistance[5] = 'm';
	Display_string(5,2,ShowDistance);
}

void Display_adc(void)//显示货物重量
{
	Display_string(0,3,"货物重量  "); 
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
