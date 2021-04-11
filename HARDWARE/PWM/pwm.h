#ifndef __PWM_H
#define __PWM_H


//通过改变TIM3->CCR2的值来改变占空比
#define LED0_PWM_VAL TIM2->CCR2 

void PWM_Init(u16 arr,u16 psc);

#endif























