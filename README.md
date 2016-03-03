# STM32F01

## Quick description
STM32F0xx: custom functions and libraries. Updated (simmilar) libraries can be found in STM32L1 directory. Can be easily ported to other STM32 devices.

### 1. GPIO
Initialize GPIO with single function (e.g. without creating and filling gpio structure).

Example: 
```
	gpio_pinSetup(GPIOC, GPIO_Pin_9, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz);
	gpio_pinSetup_AF(GPIOA, GPIO_Pin_2, GPIO_AF_0, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_10MHz);
```
Works also for setting interrupt pins.
```
	gpio_pinSetup(GPIOC, GPIO_Pin_4, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_50MHz);
	gpio_pinSetup_interrupt(GPIOC, GPIO_Pin_4, EXTI_Trigger_Falling, 0,);		
```
	
### 2. MILLIS
Function for initializing systick to create Arduino-like millis function.

Example:
```
	systick_millis_init();
	millis();
	delay(50);
```
	
### 3. STEPPER
Stepper motor driver for 2 or 4 pin motors. 
Include functions for optimal move to selected position, home position, ...
Examples included. 

### 4. USART
Arduino-like serial print of data (integers, floats, strings, raw data, ...)

Example:
```
	UART_Init();	//Supports character RX interrupt mode.

	printStringLn("String with new line");
	printLn();	// only new line
	printNumberLn(1234567890, DEC); // BIN, HEX
	printFloatLn(123.456);
	...
```

