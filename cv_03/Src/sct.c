/*
 * sct.c
 *
 *  Created on: Nov 8, 2020
 *      Author: Bc. Ondřej Povolný
 */

/*
 * Definition of helper functions for setting up the signals
 * of a shift register
 *
 * According to datasheet:
 * ~LA - D4 - PB5
 * SDI - D5 - PB4
 * CLK - D3 - PB3
 * ~OE - D6 - PB10
 *
 */

#include "stm32f0xx.h"
#include "sct.h"

#define sct_nla(x) do { if (x) GPIOB->BSRR = (1 << 5); else GPIOB->BRR = (1 << 5); } while (0)
#define sct_sdi(x) do { if (x) GPIOB->BSRR = (1 << 4); else GPIOB->BRR = (1 << 4); } while (0)
#define sct_clk(x) do { if (x) GPIOB->BSRR = (1 << 3); else GPIOB->BRR = (1 << 3); } while (0)
#define sct_noe(x) do { if (x) GPIOB->BSRR = (1 << 10); else GPIOB->BRR = (1 << 10); } while (0)

void sct_led(uint32_t value){
	for (uint8_t j = 0; j < 32; j++){
		sct_sdi(value & 1);
		value >>= 1;
		sct_clk(1);
		sct_clk(0);
	}
	sct_nla(1);
	sct_nla(0);
}

void sct_init(void){
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; /* enable clock */
	GPIOB->MODER |= GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER10_0;
	sct_led(0);
	sct_noe(0);
}

