 /*
 ===============================================================================
 LCD HD44780 driver: header file
 ===============================================================================
 * @date    2-Feb-2016
 * @author  Domen Jurkovic
  
 */
#ifndef __LCD_H
#define __LCD_H

#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"

#include "stm32f0xx_gpio_init.h"
#include "delay_us.h"
#include "systick_millis.h"

#include "math.h"

/* Register select*/
#define LCD_RS_Port				GPIOB
#define LCD_RS_Pin				GPIO_Pin_13

/* Enable pin */
#define LCD_E_Port				GPIOB
#define LCD_E_Pin					GPIO_Pin_14

/* Data pins */
#define LCD_D4_Port				GPIOC
#define LCD_D4_Pin				GPIO_Pin_8

#define LCD_D5_Port				GPIOC
#define LCD_D5_Pin				GPIO_Pin_7

#define LCD_D6_Port				GPIOB
#define LCD_D6_Pin				GPIO_Pin_15

#define LCD_D7_Port				GPIOC
#define LCD_D7_Pin				GPIO_Pin_6

//#define GO_TO_NEW_LINE_IF_STRING_TOO_LONG

/* Commands*/
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

/* Flags for display entry mode */
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

/* Flags for display on/off control */
#define LCD_DISPLAYON           0x04
#define LCD_CURSORON            0x02
#define LCD_BLINKON             0x01

/* Flags for display/cursor shift */
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

/* Flags for function set */
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

typedef struct {
	uint8_t DisplayControl;
	uint8_t DisplayFunction;
	uint8_t DisplayMode;
	uint8_t Rows;
	uint8_t Cols;
	uint8_t currentX;
	uint8_t currentY;
} _lcd_options_t;		// private LCD structure

/*
	Initializes LCD (HD44780)
	rows: height of lcd
	cols: width of lcd
*/
void LCD_Init(uint8_t rows, uint8_t cols);
void LCD_PrintString(uint8_t y, uint8_t x, char* str);
void LCD_PrintNumber(uint8_t y, uint8_t x, int32_t number);
void LCD_PrintFloat(uint8_t y, uint8_t x, float number_f);

void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(void);
void LCD_BlinkOn(void);
void LCD_BlinkOff(void);
void LCD_CursorOn(void);
void LCD_CursorOff(void);
void LCD_ScrollLeft(void);
void LCD_ScrollRight(void);

/**********************************************************/
/*	CUSTOM CHARACTER FUNCTIONS */
/**********************************************************/
/*
	Creates custom character at specific location 
	LCD supports up to 8 custom characters, so locations are 0 - 7
	*data: Pointer to 8-bytes of data for one character
 */
void LCD_CreateChar(uint8_t location, uint8_t* data);

/**
 * @brief  Puts custom created character on LCD
 * @param  location: Location on LCD where character is stored, 0 - 7
 * @retval None
 */
void LCD_PutCustom(uint8_t y, uint8_t x, uint8_t location);


/**********************************************************/
/*	PRIVATE FUNCTIONS */
/**********************************************************/
void _lcd_init_pins(void);
void _lcd_send_command(uint8_t cmd);
void _lcd_send_command_4_bit(uint8_t cmd);
void _lcd_send_data(uint8_t data);
void _lcd_cursor_set(uint8_t row, uint8_t col);
void _lcd_enable_pulse(void);


#endif /* __LCD_H */
