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
u16 time=0,speed=0,alpha=901,switchv;//time测速计时，speed速度，alpha占空比
u8 spd[7]=0,dis[6]={0},force[6]=0,i;

//函数声明
void PWM_Speed_Control();           //电机速度控制
void Displayspd(u16 spdvalue);      //速度显示函数
void DisplayDis(float value);       //距离显示函数
void Display_force(u8 forcevalue);  //踩踏板力度显示函数


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
			if(distance<5) GPIOE->ODR|=0x00000010;//距离过近，蜂鸣器报警
			else GPIOE->ODR&=0xFFFFFFEF;//消除报警
		}

/******************系统开关显示****************/
Display_string(0,0,"系统是否开启");

		if(0x0001&(switchv>>8)) Display_string(0,3,"■");
		else Display_string(0,3,"□");
			
		
/*******************速度显示*******************/
Display_string(0,1,"速度"); 		
void Displayspd(u16 spdvalue)
{

		spd[0]=speed/100+0x30;           //转速分解
		spd[1]=(speed/10)%10+0x30;
		spd[2]=speed%10+0x30;
		spd[3]='r';
		spd[4]='/';
		spd[5]='s';
		Display_string(5,1,spd);

	  write_12864com(0x88+4);	         //显示在第3行
		delay_us(200);                    
		for(i=6;i>0;i--)
		{
			write_12864data(dis[6-i]);	    //显示转速
			delay_us(200);                  //等待写入
		} 
}

/*********************距离显示*****************/
Display_string(0,2,"距离");
void DisplayDis(float value)
{
    u32 val = (u32)value;

    dis[0] = (val/100)+48;
    dis[1] = (val%100)/10+48;
	  dis[2] = '.';
    dis[3] = val%10+48;
    dis[4] = 'c';
    dis[5] = 'm';
	  Display_string(5,2,dis);


}

/********************踩踏板力度显示函数*****************/
Display_string(0,3,"踩踏板力度"); 
void Display_force(u8 forcevalue)
{
	//u16 force=  (u16)forcevalue;
	force[0] = forcevalue/1000+'0';
	force[1] = (forcevalue/100)%10+'0';
	force[2] = (forcevalue/10)%10+'0';
	force[3] = (forcevalue%10)+'0';
	force[4] = 'N';
	Display_string(5,3,force);
}


void PWM_Speed_Control()
{
	if(distance<10) alpha=901;
	else//调速方式未定
	{}
}
