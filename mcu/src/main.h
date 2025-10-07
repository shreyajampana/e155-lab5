// Shreya Jampana
// sjampana@hmc.edu
// 10/4/25
// This is the main.h file containing some defined variables and functions. 

#ifndef MAIN_H
#define MAIN_H

#include "/Users/shreyajampana/e155-lab5/mcu/lib/STM32L432KC.h"
#include <stm32l432xx.h>

///////////////////////////////////////////////////////////////////////////////
// Custom defines
///////////////////////////////////////////////////////////////////////////////

#define PIN_A PA1
#define PIN_B PA2
#define DELAY_TIM TIM2
#define PPR 408

void compute_velocity(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);


#endif // MAIN_H
