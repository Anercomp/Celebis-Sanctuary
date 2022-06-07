/*
 * hardware.h
 *
 *  Created on: 03.06.2022
 *      Author: Anercomp
 */
#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

#define DataOutPin GPIO_PIN_7 	//PA7
#define DataInPin GPIO_PIN_5	//PB5
#define SO_Pin GPIO_PIN_7		//PB7
#define SC_Pin GPIO_PIN_6		//PB6

#define LED_ON GPIOC->BRR = GPIO_PIN_13;
#define LED_OFF GPIOC->BSRR = GPIO_PIN_13;

void setPB5Output();
void setPB5Input();

#endif /* HARDWARE_H_ */
