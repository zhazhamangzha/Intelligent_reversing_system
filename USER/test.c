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

#define CSB_TX PCout(8)         //PC8
#define BEEN   PCout(10)				//PC10超声波蜂鸣器

//变量定义
u16 adc=0,start=0,mode=0,sw=0;//adc为模拟量，start表示系统是否开关，mode表示转动方向，sw为二维码图片显示开关
u16 time=0,speed=0,alpha=901;//time为直流电机运行计数，speed为直流电机速度，aphla设置TIM2->CCR2的值控制占空比
u8 spd[7]={0},showadc[10],ShowDistance[7];//spd为显示速度的字符数组，showadc为显示adc的数组，showDistance为显示距离的字符数组
u32 status=0,val,real_time;//status记录超声波时间，val是显示函数值，real_time为超声波相关变量
float dis;//dis表示获取到的距离


//函数声明
void PWM_Speed_Control(void);//电机转速控制函数
void tran(void);//超声波函数
void Displayspd(void);//显示速度
void DisplayDis(float value);//显示距离
void Display_adc(void);//显示货物重量
void Display(void);//LCD显示函数

int main(void)
{
	//初始化
 	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);//延时初始化
	uart_init(72,9600);//串口初始化 
	PWM_Init(900,0);//PWM初始化
	EXTIX_Init();//外部中断初始化
	Adc_Init();//ADC初始化
	Init_12864();//LCD12864初始化
	STEP_Init();//步进电机初始化
	LED_Init();//LED板初始化
	Timer3_Init(500,7199);//10khz分频，计数到500为50ms
	Timer4_Init(10, 71); //1Mhz分频，计数到10为10us
	Timer5_Init(10000,7199);//10khz分频，计数到10000为1s

	//系统开机显示1s图片
	clnGDR_12864();//清屏
	Display_QRcode();//显示图片
	delay_ms(1000);
	clnGDR_12864();//清屏

	//主程序
  	while(1)
	{
 		delay_ms(10);

		start=(GPIOB->IDR>>11)&0x0001;//获取系统开关
		mode=(GPIOB->IDR>>15)&0x0001;//获取运转方向
		sw=(GPIOB->IDR>>13)&0x0001;//获取图片显示开关

		if(start)//系统开启
		{
			tran();
			PWM_Speed_Control();

			//距离小于10则使能蜂鸣器
			if (dis<10&&dis!=0) BEEN=1;
			else BEEN=0;
			if (dis>80) dis=80;
		}
		else//系统关闭
		{
			BEEN=0;//停止蜂鸣器
			LED0_PWM_VAL=901;//停止直流电机转动
		}

		Display();//显示
	}
}

//超声波函数
void tran(void)
{
	u8 i;
	TIM4->CR1 |= 0x01;//使能定时器4
	TIM4->SR &= ~(1 << 0);//消除中断标志位
	status = 0;//定时器状态清0
	for (i = 0; i < 8; i++)//发送信号 41.67kHz
	{
		CSB_TX = 1;
		delay_us(12);
		CSB_TX = 0;
		delay_us(12);
	}
	delay_ms(10);
}

//电机转速控制函数
void PWM_Speed_Control(void)
{
	if(dis<10) alpha=901;//若距离小于0，电机停转
	else if(dis>30) alpha=200;//距离大于30则匀速转动
	else alpha=-35*dis+700;//根据距离控制转速
	LED0_PWM_VAL=alpha;//改变TIM2->CCR2
}

//LCD显示函数
void Display(void)
{
	if(sw)//图片显示开关开启
	{
		//清理文字
		Display_string(0,0,"                ");
		Display_string(0,1,"                ");
		Display_string(0,2,"                ");
		Display_string(0,3,"                ");

		Display_QRcode();//显示图片
		while(1)//循环读取开关直到图片显示开关关闭
		{
			sw=(GPIOB->IDR>>13)&0x0001;
			if(!sw) break;//若开关关闭则跳出循环
			delay_ms(10);
		}
	}

	clnGDR_12864();//清理图片
	if(start)//系统开关开启，显示系统名称、运转方向，速度、距离、货物重量(adc)
	{
		Display_string(0,0,"叉车模拟系统    ");
		if(mode) Display_string(7,0,"↓");
		else Display_string(7,0,"↑");
		Displayspd();
		DisplayDis(dis*10);
		Display_adc();
	}
	else//系统显示开关关闭，显示系统名称及“未开机”
	{
		Display_string(0,0,"                ");
		Display_string(0,1,"  叉车模拟系统  ");
		Display_string(0,2,"                ");
		Display_string(0,3,"          未开机");
	}
}

//显示速度
void Displayspd(void)
{
	Display_string(0,1,"  速度    ");

	//将速度值转换为字符数组并显示
	spd[0]=speed/100+'0';
	spd[1]=(speed/10)%10+'0';
	spd[2]=speed%10+'0';
	spd[3]='r';
	spd[4]='/';
	spd[5]='s';
	Display_string(5,1,spd);
}

//显示距离
void DisplayDis(float value)
{
	Display_string(0,2,"离地距离  ");

	//将距离值转换为字符数组并显示
    val=(u32)value;//将value值转为整型方便操作
    ShowDistance[0] = (val/100)+48;
    ShowDistance[1] = (val%100)/10+48;
	ShowDistance[2] = '.';
    ShowDistance[3] = val%10+48;
    ShowDistance[4] = 'c';
    ShowDistance[5] = 'm';
	Display_string(5,2,ShowDistance);
}

//显示货物重量
void Display_adc(void)
{
	Display_string(0,3,"货物重量  "); 

	//将adc值转换为字符数组并显示	
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
