#include "CONFIG.h"

void LED_Init(void)
{
    RCC->APB2ENR|=1<<3;//使能PORTB

    GPIOE->CRH&=0x00000000;
    GPIOE->CRH|=0x44444444;
}
