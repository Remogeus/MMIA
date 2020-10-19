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
#define SYS_CLOCK ((uint32_t) 8000000) /* 8MHz, used for  re-usability*/
#define LED_TIME_BLINK ((uint32_t) 300) /* 300ms between blinks */
#define LED_TIME_SHORT ((uint32_t) 100) /* 100ms LED on */
#define LED_TIME_LONG ((uint32_t) 1000) /* 100ms LED on */
#define DEBOUNCE_TIME ((uint32_t) 40)	/* 40ms between debounces */
#define DEBOUNCE_TIME_SHORT ((uint32_t) 5)	/* 5ms between debounces */

volatile uint32_t Tick;

void PeripheralsInit(void);
void InterruptInit(void);
__STATIC_INLINE void blikac(void);
__STATIC_INLINE void tlacitka(void);

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
	SysTick_Config(SYS_CLOCK/1000); /* 1000 us = 1ms */
	PeripheralsInit();
	InterruptInit();
    /* Loop forever */
	while(1){
		blikac();
		tlacitka();
	}
}

/**
 * @brief 	This function initializes the peripheral clock on GPIO ports
 * 			A, B and C and enables the pins:
 * 				PA4, PB0 as output
 * 				PC0, PC1 as pullup input
 *
 * @param 	none
 * @returns none
 */
__INLINE void PeripheralsInit(void){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN; /* enable clock */
	
	GPIOA->MODER |= GPIO_MODER_MODER4_0;
	GPIOB->MODER |= GPIO_MODER_MODER0_0;
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR0_0;
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR1_0;
}

/**
 * @brief 	This function enables the external interrupt on EXTI0 (PC0)
 *
 * @param 	none
 * @returns none
 */

__INLINE void InterruptInit(void){
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC;
	EXTI->IMR |= EXTI_IMR_MR0;
	EXTI->FTSR |= EXTI_FTSR_TR0;
	NVIC_EnableIRQ(EXTI0_1_IRQn);
}

/**
 * @brief 	Blink LED1 after every 300ms - a heartbeat, so to speak
 *
 * @param 	none
 * @returns none
 */
__STATIC_INLINE void blikac(void){
	static uint32_t delay;
	if(Tick > delay + LED_TIME_BLINK){
		GPIOA->ODR ^= (1<<4);
		delay = Tick;
	}
}


/**
 * @brief 	Handles button presses - if S1 is pressed, light up
 * 			LED2 for LED_TIME_LONG, if S2 is pressed, light up
 * 			LED2 for LED_TIME_SHORT
 *
 * @param 	none
 * @returns none
 */
__STATIC_INLINE void tlacitka(void){
	static uint32_t off_time;
	static uint32_t delay_s1;
	static uint32_t delay_s2;

	if(Tick > delay_s1 + DEBOUNCE_TIME_SHORT){
		static uint16_t debounce = 0xFFFF;

		debounce <<= 1;

		if(GPIOC->IDR & (1<<1)){
			debounce |= 0x0001;
		}
		if(debounce == 0x7FFF){
			off_time = Tick + LED_TIME_LONG;
			GPIOB->BSRR = (1<<0);
		}
	}

	if(Tick > delay_s2 + DEBOUNCE_TIME){
		static uint32_t old_s2;
		uint32_t new_s2 = GPIOC->IDR & (1<<0);

		if(old_s2 && !new_s2){
			off_time = Tick + LED_TIME_SHORT;
			GPIOB->BSRR = (1<<0);
		}
		old_s2 = new_s2;

	}

	if(Tick > off_time){
		GPIOB->BRR = (1<<0);
	}
}

/**
 * @brief 	SysTick handling function
 *
 * @param 	none
 * @returns none
 */
void SysTick_Handler(void){
	Tick++;
}

/**
 * @brief 	Interrupt handler that negates PB0
 *
 * @param 	none
 * @returns none
 */

void EXTI0_1_IRQHandler(void){
	if(EXTI->PR & EXTI_PR_PR0){
		EXTI->PR |= EXTI_PR_PR0;
		GPIOB->BRR = (1<<0);
		/* equivalent - GPIOB->BRR = 0b1, shift used for consistency and for re-usability */
	}
}


