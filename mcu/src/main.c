// main.c
// Shreya Jampana
// sjampana@hmc.edu
// 10/4/25

#include "/Users/shreyajampana/e155-lab5/mcu/src/main.h"

int32_t counter;

int main(void) {
    // Enable encoder as input
    gpioEnable(GPIO_PORT_A); 
    pinMode(PIN_A, GPIO_INPUT);
    pinMode(PIN_B, GPIO_INPUT);

    // Initialize timers
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    initTIM(DELAY_TIM);

    // Enable SYSCFG clock domain in RCC
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Configure EXTICR for the encoder interrupts
    SYSCFG->EXTICR[1] |= _VAL2FLD(SYSCFG_EXTICR1_EXTI1, 0b000);
    SYSCFG->EXTICR[1] |= _VAL2FLD(SYSCFG_EXTICR1_EXTI2, 0b000);

    // Enable interrupts globally
    __enable_irq();

    // Configure interrupts for encoder A signal
    EXTI->IMR1 |= (1 << gpioPinOffset(PIN_A)); // Configure the mask bit
    EXTI->RTSR1 |= (1 << gpioPinOffset(PIN_A)); // Enable rising edge trigger
    EXTI->FTSR1 |= (1 << gpioPinOffset(PIN_A)); // Enable falling edge trigger

    // Configure interrupts for encoder B signal
    EXTI->IMR1 |= (1 << gpioPinOffset(PIN_B)); // Configure the mask bit
    EXTI->RTSR1 |= (1 << gpioPinOffset(PIN_B)); // Enable rising edge trigger
    EXTI->FTSR1 |= (1 << gpioPinOffset(PIN_B)); // Enable falling edge trigger

    //Turn on EXTI interrupt in NVIC_ISER
    NVIC->ISER[0] |= (1 << EXTI1_IRQn);
    NVIC->ISER[0] |= (1 << EXTI2_IRQn);

    while(1){   
        compute_velocity();
        delay_millis(TIM2, 1000);
    }
}


    // Encoder A Interrupt
    void EXTI1_IRQHandler(void) {
    // Check that the encoder A was what triggered our interrupt
    if (EXTI->PR1 & (1 << gpioPinOffset(PIN_A))){
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << gpioPinOffset(PIN_A));
        
        // CW rising edge of A
        if (digitalRead(PIN_A) && !digitalRead(PIN_B)) {
            counter++;
        }

        // CCW rising edge of A
        if (digitalRead(PIN_A) && digitalRead(PIN_B)) {
            counter--;
        }

        // CW falling edge of A
        if (!digitalRead(PIN_A) && digitalRead(PIN_B)) {
            counter++;
        }

        // CCW falling edge of A
        if (!digitalRead(PIN_A) && !digitalRead(PIN_B)) {
            counter--;
        }
    }
    }

  
    // Encoder B Interrupt
    void EXTI2_IRQHandler(void) {
    // Check that the button was what triggered our interrupt
    if (EXTI->PR1 & (1 << gpioPinOffset(PIN_B))){
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << gpioPinOffset(PIN_B));

        // CW rising edge of B
        if (digitalRead(PIN_A) && digitalRead(PIN_B)) {
            counter++;
        }

        // CCW rising edge of B
        if (!digitalRead(PIN_A) && digitalRead(PIN_B)) {
            counter--;
        }

        // CW falling edge of B
        if (!digitalRead(PIN_A) && !digitalRead(PIN_B)) {
            counter++;
        }

        // CCW falling edge of B
        if (digitalRead(PIN_A) && !digitalRead(PIN_B)) {
            counter--;
        }
    }
    }

      void compute_velocity(void) {
        //Calculating the velocity
        float velocity = ((float)counter)/(PPR*4);
        printf("Counter: %i \n", counter);

        //Resetting the counter
        counter = 0;
        
        //Printing the velocity
        printf("The velocity of the motor is %f rev/s \n", velocity);
    }