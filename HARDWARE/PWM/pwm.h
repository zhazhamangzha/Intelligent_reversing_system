#ifndef __PWM_H
#define __PWM_H


//ͨ���ı�TIM3->CCR2��ֵ���ı�ռ�ձ�
#define LED0_PWM_VAL TIM2->CCR2 

void PWM_Init(u16 arr,u16 psc);

#endif























