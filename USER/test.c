/************************************************************
**	  PWM��� ʵ��
**  ����  �� ��PA1���PWM�źš�	
*************************************************************/

#include "CONFIG.H"	 	 

u16 time=0,speed=0;
u8 dis[7]={0},pwm_disp[4]={0};
int main(void)
{		
 	u16 pwmval=0;
	u8 dir=1,i;	
 	Stm32_Clock_Init(9);   //ϵͳʱ������
	delay_init(72);	       //��ʱ��ʼ��
	uart_init(72,9600);    //���ڳ�ʼ�� 
	PWM_Init(900,0);      //����Ƶ��PWMƵ��=72000/900=80Khz
	EXTIX_Init();         //�ⲿ�жϳ�ʼ��
	Timerx_Init(500,7199);//10Khz�ļ���Ƶ�ʣ�������500Ϊ50ms  
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
		
		pwm_disp[0]=pwmval/100+0x30;           //PWM�ֽ�
		pwm_disp[1]=(pwmval/10)%10+0x30;
		pwm_disp[2]=pwmval%10+0x30;
		
		write_12864com(0x98+4);	            //��ʾ�ڵ�4��
		delay_us(200);                    
		for(i=4;i>0;i--)
		{
			write_12864data(pwm_disp[4-i]);	    //��ʾPWM
			delay_us(200);                      //�ȴ�д��
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

























