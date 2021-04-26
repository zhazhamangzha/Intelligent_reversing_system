#include "CONFIG.h"	 


//*************************************
//写数据地址
//*************************************
void write_12864com(uint8_t com)
{
	  uint8_t temp = 0x01;
	  uint8_t k[8] = {0};
    uint8_t i;
    rw(0);
	  rs(0);
	  delay_us(10);
	  for(i=0;i<8;i++)
		{
			if(com & temp)	
				k[i] = 1;
			else
				k[i] = 0;
			temp=temp << 1;
		}
		temp = 0x01;
		D0(k[0]);
		D1(k[1]);
		D2(k[2]);
		D3(k[3]);
		D4(k[4]);
		D5(k[5]);
		D6(k[6]);
		D7(k[7]);
	  e(1);
	  delay_us(100);
	  e(0);
	  delay_us(100);
}
//*************************************
//写数据
//*************************************
void write_12864data(uint8_t dat)
{
    uint8_t temp = 0x01;
	  uint8_t k[8] = {0};
		uint8_t i;
    rw(0);
	  rs(1);
	  delay_us(10);
		for(i=0;i<8;i++)
		{
			if(dat & temp)	
				k[i] = 1;
			else
				k[i] = 0;
			temp=temp << 1;
		}
		temp = 0x01;
		D0(k[0]);
		D1(k[1]);
		D2(k[2]);
		D3(k[3]);
		D4(k[4]);
		D5(k[5]);
		D6(k[6]);
		D7(k[7]);
	  e(1);
	  delay_us(100);
	  e(0);
	  delay_us(100);   
}
//********************************************************
//12864初始化
//********************************************************
void Init_12864()
{
	  //端口初始化
    RCC->APB2ENR|=1<<5;    //使能PORTD时钟
		 
		GPIOD->CRH&=0X00000000; 
		GPIOD->CRH|=0X33333333;//PD推挽输出   	 
	  GPIOD->ODR|=0XFFFFFFFF;//PD输出高
	
		GPIOD->CRL&=0X00000000; 
		GPIOD->CRL|=0X33333333;
	  GPIOD->ODR|=0XFFFFFFFF;     
	
		res(0);                //复位
		delay_ms(10);          //延时
		res(1);                //复位置高
		delay_ms(50);
		write_12864com(0x30);  //Extended Function Set :8BIT设置,RE=0: basic instruction set, G=0 :graphic display OFF
		delay_us(200);         //大于100uS的延时程序
		write_12864com(0x30);  //Function Set
		delay_us(50);          //大于37uS的延时
		write_12864com(0x0c);  //Display on Control
		delay_us(200);         //大于100uS的延时
		write_12864com(0x10);  //Cursor Display Control光标设置
		delay_us(200);         //大于100uS的延时程序
		write_12864com(0x01);  //清屏
		delay_ms(50);          //大于10mS的延时
		write_12864com(0x06);  //Enry Mode Set,光标从右向左加1位移动
		delay_us(200);         //大于100uS的延时
}
  
//**************************************************************************************************
//任意位置显示字符串
//**************************************************************************************************
void Display_string(uint8_t x,uint8_t y,uint8_t *s)   
{                                            //x为横坐标，y位纵坐标，*s表示指针，为数据的首地址
   switch(y)                                 //选择纵坐标            
   { 														  
	  case 0: write_12864com(0x80+x);break;    //第1行
	  case 1: write_12864com(0x90+x);break;    //第2行
	  case 2: write_12864com(0x88+x);break;    //第3行
	  case 3: write_12864com(0x98+x);break;    //第4行
    default:break;
	 }
	 
   while(*s!='\0')                          //写入数据，直到数据为空
   {  
      write_12864data(*s);                  //写数据
      delay_us(50);                         //等待写入
      s++;                                  //下一字符
   }
}
