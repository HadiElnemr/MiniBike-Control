/*
 * delay.h
 *
 *  Created on: Sep 29, 2022
 *      Author: Administrator
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_
#include "main.h"

#define DWT_CTRL        *(uint32_t*)0xE0001000
#define DWT_CYCCNT      *(uint32_t*)0xE0001004
#define DEM_CR          *(uint32_t*)0xE000EDFC

#define __HAL_MAX_DELAY  4294967295/(HAL_RCC_GetSysClockFreq()/1000)

void delay_init(void);
void delay_us(u32 us);
void delay_ms(u16 ms);
void HAL_Delay_us(uint32_t us);
uint32_t DWT_CNT_GET(void);


#endif /* INC_DELAY_H_ */
