/**
  ******************************************************************************
  * @file    stm32l1xx_stepper.h
  * @author  Domen Jurkovic
  * @version V1.3
  * @date    9-December-2014
  * @brief   This file provides functions to manage the
  *          2 and/or 4 pin stepper motor
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F0XX_STEPPER_H_
#define __STM32F0XX_STEPPER_H_

/* Includes ------------------------------------------------------------------*/
#include <stm32f0xx.h>

#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_tim.h>
#include <stm32f0xx_misc.h>
#include <stm32f0xx_gpio_init.h>

#ifdef __cplusplus
  extern "C" {
#endif

//other driver defines
#define USE_HALF_STEP	1
#define MAINTAIN_POS	1
#define DONT_MAINTAIN_POS	0
		
//timer config		
#define TIM16_INCREMENT_RESOLUTION	125	// in microseconds [us]. Timer increments every INCREMENT_RESOLUTION us
#define DEFAULT_PPS 500								// default pulses per second - speed

		
typedef enum
{
	DIRECTION_CCW  = 0,   // < Counter-Clockwise
	DIRECTION_CW = 1  		// < Clockwise
} direction_t;

typedef struct
{ 
	// user must define these variables
	GPIO_TypeDef* motor_pin_1_bank; // pin1
	uint32_t motor_pin_1;
	GPIO_TypeDef* motor_pin_2_bank;	// pin2
	uint32_t motor_pin_2;
	GPIO_TypeDef* motor_pin_3_bank;	// pin3
	uint32_t motor_pin_3;
	GPIO_TypeDef* motor_pin_4_bank;	// pin4
	uint32_t motor_pin_4;				
	uint8_t use_half_step;			// 1 when the StepperEx motor is to be driven with half steps (only 4-wire)
	uint8_t maintain_position;	// MAINTAIN_POS to hold position after last step

	// user can also set/read these values
	int32_t steps_per_revolution;	// steps in one output shaft rotation
	uint32_t correction_pulses;		// number of correction pulses - number of steps before output shaft actually moves
	int32_t target_step_number;		// target step number
	int32_t current_step_number;	// current number of steps from home position. +/-
	uint32_t stepper_speed;   		// speed in pulses per second
	
	// stepper motor "private variables" - asigned in stepperInit_ function. Can be readed if needed. 
	uint8_t number_of_steps;  // total number of steps this motor can take
	uint8_t pin_count;        // whether you're driving the motor with 2 or 4 pins
	uint8_t step_number;      // which step the motor is on
	direction_t direction;		// Direction of rotation
	
}stepper_struct;

// set stepper: GPIO pins, USE_HALF_STEP, DONT_MAINTAIN_POS
void stepperInit_2pin(stepper_struct* current_stepper);
void stepperInit_4pin(stepper_struct* current_stepper);

void setSpeed(stepper_struct* current_stepper, uint32_t pulsesPerSecond);	// set speed- pulsesPerSecond: 1<
void setHomePos(stepper_struct* current_stepper);			// set current position as home - reference position
void moveToHomePos(stepper_struct* current_stepper);		// move to home position

// blocking move function.
void step(stepper_struct* current_stepper, int steps_to_move);

// blocking move function. Allow change of target position during rotation
void moveToTargetPos(stepper_struct* current_stepper);

// blocking move function. Allow change of target position during rotation, move optimally
void moveToTargetPosOptimally(stepper_struct* current_stepper);

// convert angle to pulses - relative to stepper motor steps_per_revolution
int32_t angleToPulses(stepper_struct* current_stepper, int32_t angle);

/* "private function" */
// step motor
void stepMotor(stepper_struct* current_stepper, int this_step);

// set timer 16 for delay and interrupt setup.
void timer16_init( void );		
		
#ifdef __cplusplus
}
#endif

#endif /* __STM32F0XX_STEPPER_H_ */

