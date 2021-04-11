#ifndef __USART_H
#define __USART_H
#include <stm32f10x_map.h>
#include <stm32f10x_nvic.h> 
#include <string.h>
#include "stdio.h"	 

#define USART_FLAG_TXE                       ((u16)0x0080)
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 pclk2,u32 bound);
int SendChar(int ch);
void UART_Send_Str(char *s);

#endif	   
















