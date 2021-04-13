/************************************************************
 * 智能倒车系统IRS
 * 硬件连接：
 * PA：电机板J3
 * PB：开关板J3
 * PC：蜂鸣器（不需要连线）
 * PD：LCDJ2
 * PE：超声波J2
*************************************************************/

#include "CONFIG.H"	 	 

//变量声明
u16 force=0,sw=0;//力度force，总开关sw
float distance;//距离distance
u16 time=0,speed=0,alpha=901;//time测速计时，speed速度，alpha占空比
u8 dis[7]={0},i;

//函数声明
void PWM_Speed_Control();//电机速度控制

int main(void)
{
	//初始化
 	Stm32_Clock_Init(9);  //系统时钟设置
	delay_init(72);	      //延时初始化
	uart_init(72,9600);   //串口初始化 
	PWM_Init(900,0);      //不分频。PWM频率=72000/900=80Khz
	EXTIX_Init();         //外部中断初始化
	Timerx_Init(500,7199);//10Khz的计数频率，计数到500为50ms  
	Init_12864();
	STM_Init();           //蜂鸣器初始化

  	while(1)
	{
 		delay_ms(10);

		if(sw)
		{
			PWM_Speed_Control();
		}

/**********转速*******************/

		dis[0]=speed/100+0x30;           //转速分解
		dis[1]=(speed/10)%10+0x30;
		dis[2]=speed%10+0x30;
		dis[3]='r';
		dis[4]='/';
		dis[5]='s';
		
	  write_12864com(0x88+4);	         //显示在第3行
		delay_us(200);                    
		for(i=6;i>0;i--)
		{
			write_12864data(dis[6-i]);	    //显示转速
			delay_us(200);                  //等待写入
		}
	}	 
}

void PWM_Speed_Control()
{
	if(distance<10) alpha=901;
	else//调速方式未定
	{}
}
