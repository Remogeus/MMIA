/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "stm32f0xx.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void InitGPIO(void);
void BlinkLED(void);
void BlinkMorseArray(uint8_t *pole);
void BlinkMorseVariable(uint32_t sequence);

int main(void)
{
	InitGPIO();
    /* Loop forever */
	for(;;){
		uint8_t pole[32] = {1, 0, 1, 0 ,1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0 ,1, 0, 0};
		//BlinkLED();
		BlinkMorseArray(pole);

	}
}

/**
 * @brief: This function initializes the peripheral clock on GPIO port A and
 * 		   enables the pin PA5, where the LED is connected
 *
 * @param: none
 * @returns: none
 */

__INLINE void InitGPIO(void){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER5_0;
}

/**
 * @brief: This function blinks LED on pin PA5
 *
 * @param: none
 * @returns: none
 */

__INLINE void BlinkLED(void){
	GPIOA->ODR ^= (1<<5);
	for(volatile uint32_t i = 0; i < 100000; i++) {}

	//TODO: check if there's a way to delay without a loop (any delay function,
	//		macro, or something like that?)
}

/**
 * @brief: This function blinks LED on pin, using Morse code sequence in array
 *
 * @param: uint8_t pole - array containing Morse code sequence
 * @returns: none
 */

__INLINE void BlinkMorseArray(uint8_t *pole){
	for(uint8_t i = 0; i < sizeof(pole);i++){
		if(pole[i]){
			GPIOA->BSRR = (1<<5);
		}
		else{
			GPIOA->BRR = (1<<5);
		}
		for(volatile uint32_t i = 0; i < 100000; i++) {}
	}

}

/**
 * @brief: This function blinks LED on pin, using Morse code sequence in variable
 *
 * @param: uint8_t sequence - variable containing Morse code sequence
 * @returns: none
 */

__INLINE void BlinkMorseVariable(uint32_t sequence){
	for(volatile uint32_t i = 0; i < 100000; i++) {}
}
