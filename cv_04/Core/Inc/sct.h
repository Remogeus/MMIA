/*
 * sct.h
 *
 *  Created on: Nov 8, 2020
 *      Author: Bc. Ondřej Povolný
 */

#ifndef SCT_H_
#define SCT_H_

void sct_led(uint32_t value);
void sct_init(void);
void sct_value(uint16_t value, uint8_t led);

#endif

