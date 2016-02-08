 /*
 ===============================================================================
            ##### STM32F0-discovery board: extra gpio functions #####
																		header file
 ===============================================================================
 * @date    31-Dec-2014
 * @author  Domen Jurkovic
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L1xx_GPIO_INIT_H
#define __STM32L1xx_GPIO_INIT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

#include <stm32f0xx_gpio.h>
#include <stm32f0xx_exti.h>
#include <stm32f0xx_syscfg.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_misc.h>


#ifdef __cplusplus
  extern "C" {
#endif

/*
	Toggle bit
*/
void gpio_toggleBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void gpio_pinSetup(GPIO_TypeDef* GPIOx,	uint32_t pinNumber, GPIOMode_TypeDef pinMode, GPIOOType_TypeDef pinType, GPIOPuPd_TypeDef pinPull, GPIOSpeed_TypeDef pinSpeed);
void gpio_pinSetup_AF(GPIO_TypeDef* GPIOx, uint32_t pinNumber, uint8_t GPIO_AF, GPIOOType_TypeDef pinType, GPIOPuPd_TypeDef pinPull, GPIOSpeed_TypeDef pinSpeed);
void gpio_pinSetup_interrupt(GPIO_TypeDef* GPIOx, uint32_t pinNumber, EXTITrigger_TypeDef EXTI_Trigger, uint8_t NVIC_IRQChannelPriority);

		#ifdef __cplusplus
}
#endif

#endif /*__STM32F0xx_GPIO_INIT_H */

