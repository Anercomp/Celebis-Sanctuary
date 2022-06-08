/*
 * hardware.c
 *
 *  Created on: 03.06.2022
 *      Author: Anercomp
 */

#include "hardware.h"
#include "stm32f1xx.h"
//#include "RccConfig.h"

void setPB5Input()
{
	//GPIOB->MODER &= ~(GPIO_MODER_MODER5);
}

void setPB5Output()
{
	//GPIOB->MODER |= GPIO_MODER_MODER5_0;
}
