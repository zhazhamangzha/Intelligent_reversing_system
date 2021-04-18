/************************************************************
 * ���ܵ���ϵͳIRS
 * Ӳ�����ӣ�
 * PA�������J3
 * PB�����ذ�J3
 * PC��������������Ҫ���ߣ�
 * PD��LCDJ2
 * PE��������J2
*************************************************************/

#include "CONFIG.H"	 	 

//��������
u16 force=0,sw=0;//����force���ܿ���sw
float distance;//����distance
u16 time=0,speed=0,alpha=901,switchv;//time���ټ�ʱ��speed�ٶȣ�alphaռ�ձ�
u8 spd[7]=0,dis[6]={0},force[6]=0,i;

//��������
void PWM_Speed_Control();           //����ٶȿ���
void Displayspd(u16 spdvalue);      //�ٶ���ʾ����
void DisplayDis(float value);       //������ʾ����
void Display_force(u8 forcevalue);  //��̤��������ʾ����


int main(void)
{
	//��ʼ��
 	Stm32_Clock_Init(9);  //ϵͳʱ������
	delay_init(72);	      //��ʱ��ʼ��
	uart_init(72,9600);   //���ڳ�ʼ�� 
	PWM_Init(900,0);      //����Ƶ��PWMƵ��=72000/900=80Khz
	EXTIX_Init();         //�ⲿ�жϳ�ʼ��
	Timerx_Init(500,7199);//10Khz�ļ���Ƶ�ʣ�������500Ϊ50ms  
	Init_12864();
	STM_Init();           //��������ʼ��

  	while(1)
	{
 		delay_ms(10);

		if(sw)
		{
			PWM_Speed_Control();
			if(distance<5) GPIOE->ODR|=0x00000010;//�������������������
			else GPIOE->ODR&=0xFFFFFFEF;//��������
		}

/******************ϵͳ������ʾ****************/
Display_string(0,0,"ϵͳ�Ƿ���");

		if(0x0001&(switchv>>8)) Display_string(0,3,"��");
		else Display_string(0,3,"��");
			
		
/*******************�ٶ���ʾ*******************/
Display_string(0,1,"�ٶ�"); 		
void Displayspd(u16 spdvalue)
{

		spd[0]=speed/100+0x30;           //ת�ٷֽ�
		spd[1]=(speed/10)%10+0x30;
		spd[2]=speed%10+0x30;
		spd[3]='r';
		spd[4]='/';
		spd[5]='s';
		Display_string(5,1,spd);

	  write_12864com(0x88+4);	         //��ʾ�ڵ�3��
		delay_us(200);                    
		for(i=6;i>0;i--)
		{
			write_12864data(dis[6-i]);	    //��ʾת��
			delay_us(200);                  //�ȴ�д��
		} 
}

/*********************������ʾ*****************/
Display_string(0,2,"����");
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

/********************��̤��������ʾ����*****************/
Display_string(0,3,"��̤������"); 
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
	else//���ٷ�ʽδ��
	{}
}
