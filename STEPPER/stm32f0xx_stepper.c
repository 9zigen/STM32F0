/**
  ******************************************************************************
  * @file    stm32f0xx_stepper.c
  * @author  Domen Jurkovic
  * @version V1.3
  * @date    5-Jan-2014
  * @brief   This file provides functions to manage the
  *          2 and/or 4 pin stepper motor
  *           
  *  @verbatim
 ===============================================================================
                       ##### How to use this driver #####
 ===============================================================================
    (#) Fill stepper struct: stepper_struct stepper1; 
					Note: for accesibility of variables and other stepper data from all functions and interrupt rutines, 
					it is best to declare this variable as global.
				It is esential to fill at these structure variables: (example)
					motor_pin_1_bank = GPIOA;
					motor_pin_1 = GPIO_Pin_4;
					motor_pin_2_bank = GPIOA;
					motor_pin_2 = GPIO_Pin_5;
					// omit these if 2 pin moto is used.
					motor_pin_3_bank = GPIOA;	
					motor_pin_3 = GPIO_Pin_6;
					motor_pin_4_bank = GPIOA;
					motor_pin_4 = GPIO_Pin_7;
					
					//define number of steps per output shaft revolution(including gear ratio)
					steps_per_revolution = 4067;	
						Note:  4076 is default for 28YBJ-48
					
					// if 4 pin motor is used, you can use half-stepping:
					use_half_step = USE_HALF_STEP;
					
					// define if last motor step state is preserved for one extra step delay time or reseted imediately. 
					maintain_position = DONT_MAINTAIN_POS;	
						Note: useful feature when heavy load(flywheel or some heavy gears) are used.
		
		(#) Init stepper: 
			- stepperInit_2pin(stepper_struct* current_stepper);
			- stepperInit_4pin(stepper_struct* current_stepper);
		
		(#) Set stepper speed: [pulses per second] (up to 1000 for 28YBJ-48)
			- setSpeed(&stepper, 500);	
				
		(#)	If needed set (after init()!): 
			- correction_pulses = ___;
					This is number of steps before output shaft actually moves. The same for any direction.
		
		(#) Move methods: 
			- step(&stepper, 500);	
				Move stepper for 500 steps CW. -500 = CCW; 
			
			- moveToTargetPos(&stepper); 
				Move to target_step_number. target_step_number can be changed in some interrupt routine. 
			
			- moveToTargetPosOptimally(&stepper); 
				Move to target_step_number optimally - by calculating the shortest way to reach target_step_number. 
				target_step_number therefore must be in range from -((steps_per_revolution/2)-1) to +(steps_per_revolution/2);
					Note: use setHomePos() or target_step_number = 0; to avoid out-of-range parameters.
				target_step_number can be changed in some interrupt routine. 
				
				
		(#) Other set&move methods:
			- setHomePos(stepper_struct* current_stepper);			
				Set home position. Current step number is new "home" or zero position. All target step numbers 
				are calculated from that point. 
			
			- moveToHomePos(stepper_struct* current_stepper);		
				Move back to home position. 
					Note: To move back home optimally, set 
						target_step_number = 0;
					and use 
						moveToTargetPosOptimally();
						
			- int32_t angleToPulses(int32_t angle);
				Convert angle to pulses - relative to stepper motor defines.
				
			Note: this driver uses TIM16 for delay generating
			Note: this driver was tested with 28YBJ-48 small stepper motor:
				http://arduino-info.wikispaces.com/SmallSteppers
 
*/

/* Includes ------------------------------------------------------------------*/
#include <stm32f0xx_stepper.h>
#include <stdlib.h>

uint8_t TIM16_update_flag = 0;

void stepperInit_2pin(stepper_struct* current_stepper)
{
  // set default values in current_stepper struct
	current_stepper->step_number = 0;							// which step the motor is on
  current_stepper->direction = DIRECTION_CW;		// motor direction
  current_stepper->use_half_step = 0; 					// 1 when the stepper motor is to be driven with half steps (only 4-wire)	
	setSpeed(current_stepper, DEFAULT_PPS);				// set default pulsesPerSecond
	current_stepper->current_step_number = 0;			// set default current position to home position = 0;
	current_stepper->target_step_number = 0;			// set default target position to be the same as home position = 0;
	current_stepper->correction_pulses = 0;				// number of correction pulses - number of steps before output shaft actually moves
	
	// setup the pins on the microcontroller:
  gpio_pinSetup(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_50MHz);
	gpio_pinSetup(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_50MHz);
  // set default GPIO values
	GPIO_ResetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
	GPIO_ResetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
	
	// pin_count is used by the stepMotor() method:
  current_stepper->pin_count = 2;
	current_stepper->number_of_steps = 4;
	
	timer16_init();
}

void stepperInit_4pin(stepper_struct* current_stepper)
{
	// set default values in current_stepper struct
	current_stepper->step_number = 0;							// which step the motor is on
  current_stepper->direction = DIRECTION_CW;		// motor direction
	setSpeed(current_stepper, DEFAULT_PPS);				// set default pulsesPerSecond
	current_stepper->current_step_number = 0;			// set default home position to current position = 0;
	current_stepper->target_step_number = 0;			// set default target position to be the same as home position = 0;
	current_stepper->correction_pulses = 0;				// number of correction pulses - number of steps before output shaft actually moves
	
  // setup the pins on the microcontroller:
  gpio_pinSetup(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_50MHz);
	gpio_pinSetup(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_50MHz);
	gpio_pinSetup(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_50MHz);
	gpio_pinSetup(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_50MHz);
  // set default GPIO values
	GPIO_ResetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
	GPIO_ResetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
	GPIO_ResetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
	GPIO_ResetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
	
  // pin_count is used by the stepMotor():
  current_stepper->pin_count = 4;
	if(current_stepper->use_half_step == USE_HALF_STEP)
	{
		current_stepper->number_of_steps = 8;
	}
	else{
		current_stepper->number_of_steps = 4;
	}
	timer16_init();
}

void timer16_init()
{
	TIM_TimeBaseInitTypeDef timer16;
	RCC_ClocksTypeDef system_freq;
	NVIC_InitTypeDef timer16_int;
	uint32_t timer_freq;
	uint32_t timer_prescaler;
	uint32_t timer_periode;
	
	RCC_GetClocksFreq(&system_freq);	//get system clocks
	timer_freq = system_freq.HCLK_Frequency;	//TIM16 clock 
	timer_prescaler = TIM16_INCREMENT_RESOLUTION * (timer_freq / 1000000);	
	timer_periode = (1000000 / DEFAULT_PPS) / TIM16_INCREMENT_RESOLUTION;	// default pulses per second
	// 1000000/pulses_per_second
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);	//check smt32f0xx_rcc.c
	timer16.TIM_Prescaler = timer_prescaler;
	timer16.TIM_CounterMode = TIM_CounterMode_Up;
	timer16.TIM_Period = timer_periode -1;						
	timer16.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM16, &timer16);
	
	timer16_int.NVIC_IRQChannel = TIM16_IRQn;
	timer16_int.NVIC_IRQChannelPriority = 3;	// low priority
	timer16_int.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&timer16_int);

	TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
	TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);	//check smt32f0xx_tim.c
	TIM_Cmd(TIM16, DISABLE);
	TIM16_update_flag = 0;
}	

void TIM16_IRQHandler()
{
	if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET)
  {
		TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
		TIM16_update_flag = 1;
		
	}
}

/*
Sets the speed in pulsesPerSecond: 1<1000
*/
void setSpeed(stepper_struct* current_stepper, uint32_t pulsesPerSecond)
{
  uint32_t period = (1000000 / pulsesPerSecond) / TIM16_INCREMENT_RESOLUTION;
	current_stepper->stepper_speed = period - 1;
	//TIM_SetAutoreload(TIM3, period-1);
}

/*
  * Moves the motor steps_to_move steps.  If the number is negative, the motor moves in the reverse direction.
	* blocking function - doesn't allow target change.
 */
void step(stepper_struct* current_stepper, int steps_to_move)
{  
  int steps_left = abs(steps_to_move);  // how many steps to take
	 
  // determine direction based on whether steps_to_move is + or -:
  if (steps_to_move > 0) {current_stepper->direction = DIRECTION_CW;}
  if (steps_to_move < 0) {current_stepper->direction = DIRECTION_CCW;}

  // decrement the number of steps, moving one step each time:
  while(steps_left > 0) {			

		// increment or decrement the step number, depending on direction:
		if( current_stepper->direction == DIRECTION_CW) {
			current_stepper->current_step_number++;	// increment overall step number - from home position
			current_stepper->step_number++;
			if( (current_stepper->step_number) == current_stepper->number_of_steps) {
				current_stepper->step_number = 0;
			}
		} 
		else { 
			current_stepper->current_step_number--;	// decrement overall step number - from home position
			if (current_stepper->step_number == 0) {
				current_stepper->step_number = current_stepper->number_of_steps;
			}
			current_stepper->step_number--;
		}
		// decrement the steps left:
		steps_left--;
		
		if (current_stepper->use_half_step)
		{
			// we have 8 steps, next step number will be 0, 1, 2, 3, 4, 5, 6 or 7:// JZ 20091104
			stepMotor(current_stepper, current_stepper->step_number % 8);	// JZ 20091104
		} else {
			// step the motor to step number 0, 1, 2, or 3:
			stepMotor(current_stepper, current_stepper->step_number % 4);
		}
		
		// create step delay
		TIM16_update_flag = 0;
		TIM_SetAutoreload(TIM16, current_stepper->stepper_speed);	//set period
		TIM_SetCounter(TIM16, 0);
		TIM_Cmd(TIM16, ENABLE);
		while(TIM16_update_flag != 1);
		TIM_Cmd(TIM16, DISABLE);
	}	
	if (current_stepper->maintain_position == MAINTAIN_POS){	//add aditional step delay, before pins are reseted
		TIM16_update_flag = 0;
		TIM_SetCounter(TIM16, 0);
		TIM_Cmd(TIM16, ENABLE);
		while(TIM16_update_flag != 1);
		TIM_Cmd(TIM16, DISABLE);
	}
	//reset motor pins
	GPIO_ResetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
	GPIO_ResetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
	if(current_stepper->pin_count == 4){
		GPIO_ResetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
		GPIO_ResetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
	}
	
}


/*
  * Moves the motor target position.  If the number is negative, the motor moves in the reverse direction.
	* blocking function - allow target change.
 */
void moveToTargetPos(stepper_struct* current_stepper)
{
	int32_t steps_to_move = current_stepper->target_step_number - current_stepper->current_step_number;
	
	while (steps_to_move != 0){
		if(steps_to_move >= 0){
			steps_to_move += current_stepper->correction_pulses;
		}
		else{
			steps_to_move -= current_stepper->correction_pulses;
		}
		
		if (steps_to_move > 0) // determine direction based on whether steps_to_move is + or -:
		{
			current_stepper->direction = DIRECTION_CW;
			current_stepper->current_step_number++;	// increment overall step number - from home position
			current_stepper->step_number++;					// increment motor current step number (1-number_of_steps)

			if( (current_stepper->step_number) == current_stepper->number_of_steps)	{
				current_stepper->step_number = 0;
			}
			if (current_stepper->use_half_step){			
				stepMotor(current_stepper, current_stepper->step_number % 8);	// step the motor to step number 0, 1, 2, 3, 4, 5, 6 or 7
			} 
			else{
				stepMotor(current_stepper, current_stepper->step_number % 4); // step the motor to step number 0, 1, 2, or 3
			}
		}
		else{	//direction = DIRECTION_CCW			
			current_stepper->direction = DIRECTION_CCW;
			current_stepper->current_step_number--;	// decrement overall step number - from home position
				
			if (current_stepper->step_number == 0) {
				current_stepper->step_number = current_stepper->number_of_steps -1;
			}
			else{
				current_stepper->step_number--;	// decrement motor current step number (1-number_of_steps)
			}
			
			if (current_stepper->use_half_step){
				stepMotor(current_stepper, current_stepper->step_number % 8);	// step the motor to step number 0, 1, 2, 3, 4, 5, 6 or 7
			} 
			else{
				stepMotor(current_stepper, current_stepper->step_number % 4); // step the motor to step number 0, 1, 2, or 3
			}
		}
	
		// create step delay
		TIM16_update_flag = 0;
		TIM_SetAutoreload(TIM16, current_stepper->stepper_speed);	//set period
		TIM_SetCounter(TIM16, 0);
		TIM_Cmd(TIM16, ENABLE);
		while(TIM16_update_flag != 1);
		TIM_Cmd(TIM16, DISABLE);
		
		// update steps to move(in the middle of this function some interrupt my change target position)
		steps_to_move = current_stepper->target_step_number - current_stepper->current_step_number;
	}	// end of while loop - step to target.
	
	if (current_stepper->maintain_position == MAINTAIN_POS){	//add aditional step delay, before pins are reseted
		TIM16_update_flag = 0;
		TIM_SetCounter(TIM16, 0);
		TIM_Cmd(TIM16, ENABLE);
		while(TIM16_update_flag != 1);
		TIM_Cmd(TIM16, DISABLE);
	}
	//reset motor pins
	GPIO_ResetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
	GPIO_ResetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
	if(current_stepper->pin_count == 4){
		GPIO_ResetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
		GPIO_ResetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
	}
}

/* 
* Blocking move function. Allow change of target position, move optimally
* range: from -((steps_per_revolution/2)-1) to +(steps_per_revolution/2);
*/
void moveToTargetPosOptimally(stepper_struct* current_stepper)
{
	int32_t steps_to_move = 0;
	int32_t abs_steps_to_move = 0;
	
	while (	current_stepper->target_step_number != current_stepper->current_step_number){
		
		steps_to_move = current_stepper->target_step_number - current_stepper->current_step_number;
		abs_steps_to_move = abs(steps_to_move);
		if (abs_steps_to_move > (current_stepper->steps_per_revolution/2)){
			//steps_to_move = -(STEPS_PER_REVOLUTION - abs_steps_to_move);
			if (steps_to_move >= 0) steps_to_move = -(current_stepper->steps_per_revolution - abs_steps_to_move);
			else steps_to_move = current_stepper->steps_per_revolution - abs_steps_to_move;
		}
		
		if (steps_to_move == 0) break;
				
		if (steps_to_move > 0) // direction = DIRECTION_CW:
		{
			current_stepper->direction = DIRECTION_CW;
			current_stepper->current_step_number++;	// increment overall step number - from home position
			current_stepper->step_number++;					// increment motor current step number (1-number_of_steps)
			
			if( (current_stepper->step_number) == current_stepper->number_of_steps){
				current_stepper->step_number = 0;
			}

			if (current_stepper->use_half_step){
				stepMotor(current_stepper, current_stepper->step_number % 8);	// step the motor to step number 0, 1, 2, 3, 4, 5, 6 or 7
			} 
			else{
				stepMotor(current_stepper, current_stepper->step_number % 4); // step the motor to step number 0, 1, 2, or 3
			}
		}
		else{	//direction = DIRECTION_CCW
			current_stepper->direction = DIRECTION_CCW;
			current_stepper->current_step_number--;	// increment overall step number - from home position
						
			if (current_stepper->step_number == 0){
				current_stepper->step_number = current_stepper->number_of_steps -1;
			}
			else{
				current_stepper->step_number--;	// increment motor current step number (1-number_of_steps)
			}
			
			if (current_stepper->use_half_step){
				stepMotor(current_stepper, current_stepper->step_number % 8);	// step the motor to step number 0, 1, 2, 3, 4, 5, 6 or 7
			} 
			else{
				stepMotor(current_stepper, current_stepper->step_number % 4); // step the motor to step number 0, 1, 2, or 3
			}
		}
	
		// create step delay
		TIM16_update_flag = 0;
		TIM_SetAutoreload(TIM16, current_stepper->stepper_speed);	//set period
		TIM_SetCounter(TIM16, 0);
		TIM_Cmd(TIM16, ENABLE);
		while(TIM16_update_flag != 1);
		TIM_Cmd(TIM16, DISABLE);
		
		if (current_stepper->current_step_number > (current_stepper->steps_per_revolution/2)){
			current_stepper->current_step_number = -((current_stepper->steps_per_revolution/2)-1);
		}
		if (current_stepper->current_step_number < -((current_stepper->steps_per_revolution/2)-1)){
			current_stepper->current_step_number = (current_stepper->steps_per_revolution/2);
		}
		
	}	// end of while loop - step to target.
	
	if (current_stepper->maintain_position == MAINTAIN_POS){	//add aditional step delay, before pins are reseted
		TIM16_update_flag = 0;
		TIM_SetCounter(TIM16, 0);
		TIM_Cmd(TIM16, ENABLE);
		while(TIM16_update_flag != 1);
		TIM_Cmd(TIM16, DISABLE);
	}
	//reset motor pins
	GPIO_ResetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
	GPIO_ResetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
	if(current_stepper->pin_count == 4){
		GPIO_ResetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
		GPIO_ResetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
	}
}

// set home position
void setHomePos(stepper_struct* current_stepper)
{
	current_stepper->current_step_number = 0;
}

// move to home position
void moveToHomePos(stepper_struct* current_stepper)
{
	if (current_stepper->current_step_number > 0){
		step(current_stepper, - current_stepper->current_step_number);
	}
	else{
		step(current_stepper, current_stepper->current_step_number);
	}
}

/* convert angle to pulses - relative to stepper motor defines
 * in: degres; max=429000 (=about 1193 rotations if no external gear is used and motor is: 28YBJ-48)
 * relative to stepper steps_per_revolution. 
*/ 
int32_t angleToPulses(stepper_struct* current_stepper, int32_t angle)
{
	int32_t angle_per_pulse = 360*10000 / current_stepper->steps_per_revolution;	//single pulse is  __ degrees
	int32_t pulses = angle*10000 / angle_per_pulse;		// */ 10000 - to avoid calculating with floats 
	return pulses;
}

/*
 * Moves the motor forward or backwards.
 */
void stepMotor(stepper_struct* current_stepper, int this_step)
{
	if (current_stepper->pin_count == 2) {
    switch (this_step) {
      case 0: /* 01 */
				GPIO_ResetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_SetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				break;
      case 1: /* 11 */
				GPIO_SetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_SetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				break;
      case 2: /* 10 */
				GPIO_SetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_ResetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				break;
      case 3: /* 00 */
				GPIO_ResetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_ResetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				break;
    } 
  }
  if ((current_stepper->pin_count == 4) && (current_stepper->use_half_step == 0)) {
    switch (this_step) {
      case 0:    // 1100
				GPIO_SetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_SetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				GPIO_ResetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
				GPIO_ResetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
				break;
      case 1:    // 0110
				GPIO_ResetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_SetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				GPIO_SetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
				GPIO_ResetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
				break;
			case 2:    //0011
				GPIO_ResetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_ResetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				GPIO_SetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
				GPIO_SetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
				break;
      case 3:    //1001
      	GPIO_SetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_ResetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				GPIO_ResetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
				GPIO_SetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
				break;
    } 
  }
  if ((current_stepper->pin_count == 4) && (current_stepper->use_half_step == 1)) {
    switch (this_step) {
      case 0:    // 1000
				GPIO_SetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_ResetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				GPIO_ResetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
				GPIO_ResetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
				break;
      case 1:    // 1100
				GPIO_SetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_SetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				GPIO_ResetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
				GPIO_ResetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
				break;
      case 2:    // 0100
				GPIO_ResetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_SetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				GPIO_ResetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
				GPIO_ResetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
				break;
      case 3:    // 0110
				GPIO_ResetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_SetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				GPIO_SetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
				GPIO_ResetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
				break;
      case 4:    //0010
				GPIO_ResetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_ResetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				GPIO_SetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
				GPIO_ResetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
				break;
      case 5:    //0011
				GPIO_ResetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_ResetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				GPIO_SetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
				GPIO_SetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
				break;
      case 6:    //0001
				GPIO_ResetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_ResetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				GPIO_ResetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
				GPIO_SetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
				break;
      case 7:    //1001
				GPIO_SetBits(current_stepper->motor_pin_1_bank, current_stepper->motor_pin_1);
				GPIO_ResetBits(current_stepper->motor_pin_2_bank, current_stepper->motor_pin_2);
				GPIO_ResetBits(current_stepper->motor_pin_3_bank, current_stepper->motor_pin_3);
				GPIO_SetBits(current_stepper->motor_pin_4_bank, current_stepper->motor_pin_4);
				break;
    } 
  }
}

