/************************************************************
 * ÖÇÄÜµ¹³µÏµÍ³IRS
 * Ó²¼þÁ¬½Ó£º
 * PA£ºµç»ú°åJ3
 * PB£º¿ª¹Ø°åJ3
 * PD£ºLCDJ2
 * PE£º³¬Éù²¨J2
*************************************************************/

#include "CONFIG.H"	 	 

#define CSB_TX PCout(8)         // PC8 ï¿½ï¿½ï¿½Í³ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ 
#define BEEN   PCout(10)				// ï¿½ï¿½à±?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
u16 force=0,start=0;//ï¿½ï¿½ï¿½ï¿½forceï¿½ï¿½ï¿½Ü¿ï¿½ï¿½ï¿½sw
u16 time=0,speed=0,alpha=901;//timeï¿½ï¿½ï¿½Ù¼ï¿½Ê±ï¿½ï¿½speedï¿½Ù¶È£ï¿½alphaÕ¼ï¿½Õ±ï¿½
u8 spd[7]={0},i,showforce[6];      //use to show speed
u32   status=0,val,real_time;						      // ï¿½ï¿½ï¿½ë¶¨Ê±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Öµ
char  s2[10], s1[10];						// LCDï¿½ï¿½Ê¾ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
float	dis;                      // Ê©ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Öµ
u8    ShowDistance[7] = { 0 };				// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê¾ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
u8    power[10];                //ï¿½ï¿½ï¿½ï¿½Ê¾ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?


//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
void PWM_Speed_Control(void);//ï¿½ï¿½ï¿½ï¿½Ù¶È¿ï¿½ï¿½ï¿?
void tran(void); 					      // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
void Displayspd(void);      //ËÙ¶ÈÏÔÊ¾º¯Êý
void DisplayDis(float value);       //¾àÀëÏÔÊ¾º¯Êý
void Display_force(void);  //²ÈÌ¤°åÁ¦¶ÈÏÔÊ¾º¯Êý

int main(void)
{
	//ï¿½ï¿½Ê¼ï¿½ï¿½
 	Stm32_Clock_Init(9);  //ÏµÍ³Ê±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	delay_init(72);	      //ï¿½ï¿½Ê±ï¿½ï¿½Ê¼ï¿½ï¿½
	uart_init(72,9600);   //ï¿½ï¿½ï¿½Ú³ï¿½Ê¼ï¿½ï¿½ 
	PWM_Init(900,0);      //ï¿½ï¿½ï¿½ï¿½Æµï¿½ï¿½PWMÆµï¿½ï¿½=72000/900=80Khz
	EXTIX_Init();         //ï¿½â²¿ï¿½Ð¶Ï³ï¿½Ê¼ï¿½ï¿½
	Timer3_Init(500,7199);//10Khzï¿½Ä¼ï¿½ï¿½ï¿½Æµï¿½Ê£ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½500Îª50ms  
	Timer4_Init(10, 71);  // 1Mhzï¿½Ä¼ï¿½ï¿½ï¿½Æµï¿½Ê£ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½10Îª10us  
	Init_12864();         // Òºï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½
	EXTIX_Init();         // ï¿½â²¿ï¿½Ð¶Ï³ï¿½Ê¼ï¿½ï¿½
	Adc_Init();           //ADC start
	LED_Init();


  	while(1)
	{
 		delay_ms(10);

		start=(GPIOB->IDR&&0x0100)>>8;//¶Á¿ª¹ØÁ¿
		if(start)
		{
			tran();		//ï¿½ï¿½ï¿½ä³¬ï¿½ï¿½ï¿½ï¿½
			PWM_Speed_Control();

			//when distanse belows to 3,warning!
			if (dis<10&&dis!=0) BEEN=1;
			else BEEN=0;		//ï¿½ï¿½à±?ï¿½ï¿½
			if (dis>80) dis=80;		//ï¿½ï¿½Ö¹ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä²ï¿½ï¿½Ê§ï¿½ï¿½

			/******************ÏµÍ³¿ª¹ØÏÔÊ¾****************/
			Display_string(0,0,"ÏµÍ³ÊÇ·ñ¿ªÆô");

			if(start) Display_string(0,3,"¡ö");
			else Display_string(0,3,"¡õ");
			
			Displayspd();
			DisplayDis(dis*10);
			Display_force();
		}

	}
}

// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
void tran(void)
{
	u8 i;
	TIM4->CR1 |= 0x01;         // Ê¹ï¿½Ü¶ï¿½Ê±ï¿½ï¿½3
	TIM4->SR &= ~(1 << 0);       // ï¿½ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ï¿?3ï¿½Ð¶Ï±ï¿½Ö¾Î»    
	status = 0;			       // ï¿½ï¿½ï¿½ë¶¨Ê±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Öµï¿½ï¿½ï¿½ï¿½
	for (i = 0; i < 8; i++)         // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÅºÅ·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æµï¿½ï¿½41.67kHz
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
	else if(dis>30)//ï¿½ï¿½ï¿½Ù·ï¿½Ê½Î´ï¿½ï¿½
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
	Display_string(0,1,"ËÙ¶È");
	spd[0]=speed/100+0x30;           //×ªËÙ·Ö½â
	spd[1]=(speed/10)%10+0x30;
	spd[2]=speed%10+0x30;
	spd[3]='r';
	spd[4]='/';
	spd[5]='s';
	Display_string(5,1,spd);
}

void DisplayDis(float value)
{
	Display_string(0,2,"¾àÀë");
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
	Display_string(0,3,"²ÈÌ¤°åÁ¦¶È"); 
	showforce[0] = force/1000+'0';
	showforce[1] = (force/100)%10+'0';
	showforce[2] = (force/10)%10+'0';
	showforce[3] = (force%10)+'0';
	showforce[4] = 'N';
	Display_string(5,3,showforce);
}
