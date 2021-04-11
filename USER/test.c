/************************************************************
**	  PWM输出 实验
**  接线  ： 从PA1输出PWM信号。	
*************************************************************/

#include "CONFIG.H"	 	 

u16 time=0,speed=0;
u8 dis[7]={0},pwm_disp[4]={0};
int main(void)
{		
 	u16 pwmval=0;
	u8 dir=1,i;	
 	Stm32_Clock_Init(9);   //系统时钟设置
	delay_init(72);	       //延时初始化
	uart_init(72,9600);    //串口初始化 
	PWM_Init(900,0);      //不分频。PWM频率=72000/900=80Khz
	EXTIX_Init();         //外部中断初始化
	Timerx_Init(500,7199);//10Khz的计数频率，计数到500为50ms  
	Init_12864();
  while(1)
	{
 		delay_ms(10);	 

/*********PWM***************/		
		if(dir) pwmval++;
		  else    pwmval--;

 		if(pwmval>900)dir=0;
		if(pwmval==0)dir=1;									 
		LED0_PWM_VAL=pwmval;
		
		pwm_disp[0]=pwmval/100+0x30;           //PWM分解
		pwm_disp[1]=(pwmval/10)%10+0x30;
		pwm_disp[2]=pwmval%10+0x30;
		
		write_12864com(0x98+4);	            //显示在第4行
		delay_us(200);                    
		for(i=4;i>0;i--)
		{
			write_12864data(pwm_disp[4-i]);	    //显示PWM
			delay_us(200);                      //等待写入
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

























