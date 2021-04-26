#include "CONFIG.h"	 


//*************************************
//д���ݵ�ַ
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
//д����
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
//12864��ʼ��
//********************************************************
void Init_12864()
{
	  //�˿ڳ�ʼ��
    RCC->APB2ENR|=1<<5;    //ʹ��PORTDʱ��
		 
		GPIOD->CRH&=0X00000000; 
		GPIOD->CRH|=0X33333333;//PD�������   	 
	  GPIOD->ODR|=0XFFFFFFFF;//PD�����
	
		GPIOD->CRL&=0X00000000; 
		GPIOD->CRL|=0X33333333;
	  GPIOD->ODR|=0XFFFFFFFF;     
	
		res(0);                //��λ
		delay_ms(10);          //��ʱ
		res(1);                //��λ�ø�
		delay_ms(50);
		write_12864com(0x30);  //Extended Function Set :8BIT����,RE=0: basic instruction set, G=0 :graphic display OFF
		delay_us(200);         //����100uS����ʱ����
		write_12864com(0x30);  //Function Set
		delay_us(50);          //����37uS����ʱ
		write_12864com(0x0c);  //Display on Control
		delay_us(200);         //����100uS����ʱ
		write_12864com(0x10);  //Cursor Display Control�������
		delay_us(200);         //����100uS����ʱ����
		write_12864com(0x01);  //����
		delay_ms(50);          //����10mS����ʱ
		write_12864com(0x06);  //Enry Mode Set,�����������1λ�ƶ�
		delay_us(200);         //����100uS����ʱ
}
  
//**************************************************************************************************
//����λ����ʾ�ַ���
//**************************************************************************************************
void Display_string(uint8_t x,uint8_t y,uint8_t *s)   
{                                            //xΪ�����꣬yλ�����꣬*s��ʾָ�룬Ϊ���ݵ��׵�ַ
   switch(y)                                 //ѡ��������            
   { 														  
	  case 0: write_12864com(0x80+x);break;    //��1��
	  case 1: write_12864com(0x90+x);break;    //��2��
	  case 2: write_12864com(0x88+x);break;    //��3��
	  case 3: write_12864com(0x98+x);break;    //��4��
    default:break;
	 }
	 
   while(*s!='\0')                          //д�����ݣ�ֱ������Ϊ��
   {  
      write_12864data(*s);                  //д����
      delay_us(50);                         //�ȴ�д��
      s++;                                  //��һ�ַ�
   }
}
