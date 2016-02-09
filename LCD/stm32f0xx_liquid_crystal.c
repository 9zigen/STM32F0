 /*
 ===============================================================================
 LCD HD44780 driver: c file
 ===============================================================================
 * @date    2-Feb-2016
 * @author  Domen Jurkovic
  
 */

#include "stm32f0xx_liquid_crystal.h"

/* Private variable */
static _lcd_options_t _lcd_options;

void LCD_Init(uint8_t rows, uint8_t cols) {
	systick_millis_init();	// Initialize milisecond delay 
	delay_us_init();				// Initialize microsecond delay 
	
	_lcd_init_pins();	// Init pinout 
	delay(50);	//At least 40ms
	
	// Set LCD width and height 
	_lcd_options.Rows = rows;
	_lcd_options.Cols = cols;
	// Set cursor pointer to beginning for LCD 
	_lcd_options.currentX = 0;
	_lcd_options.currentY = 0;
	
	_lcd_options.DisplayFunction = LCD_4BITMODE | LCD_5x8DOTS | LCD_1LINE;
	if (rows > 1) {
		_lcd_options.DisplayFunction |= LCD_2LINE;
	}
	
	/* Try to set 4bit mode */
	_lcd_send_command_4_bit(0x03);
	delay(5);
	
	/* Second try */
	_lcd_send_command_4_bit(0x03);
	delay(5);
	
	/* Third goo! */
	_lcd_send_command_4_bit(0x03);
	delay(5);	
	
	/* Set 4-bit interface */
	_lcd_send_command_4_bit(0x02);
	delay_us(100);
	
	// Set # lines, font size, etc.
	_lcd_send_command(LCD_FUNCTIONSET | _lcd_options.DisplayFunction);

	// Turn the display on, no cursor, no blinking
	_lcd_options.DisplayControl = LCD_DISPLAYON;
	LCD_DisplayOn();

	LCD_Clear();

	// Default font & direction
	_lcd_options.DisplayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	_lcd_send_command(LCD_ENTRYMODESET | _lcd_options.DisplayMode);
	delay(5);
}

/*
	Print string on lcd
	y location (row)	
	x location (col)
	*str: pointer to string to display
 */
void LCD_PrintString(uint8_t y, uint8_t x, char* str) {
	_lcd_cursor_set(y, x);
	while (*str) {
		#ifdef GO_TO_NEW_LINE_IF_STRING_TOO_LONG
			if (_lcd_options.currentX >= _lcd_options.Cols) {
				_lcd_options.currentX = 0;
				_lcd_options.currentY++;
				_lcd_cursor_set(_lcd_options.currentY, _lcd_options.currentX);
			}
			if (*str == '\n') {
				_lcd_options.currentY++;
				_lcd_cursor_set(_lcd_options.currentY, _lcd_options.currentX);
			} else if (*str == '\r') {
				_lcd_cursor_set(_lcd_options.currentY, 0);
			} else {
				_lcd_send_data(*str);
				_lcd_options.currentX++;
			}
			str++;
		#else
			if (*str == '\n') {
				_lcd_options.currentY++;
				_lcd_cursor_set(_lcd_options.currentY, _lcd_options.currentX);
			} else if (*str == '\r') {
				_lcd_cursor_set(_lcd_options.currentY, 0);
			} else {
				_lcd_send_data(*str);
				_lcd_options.currentX++;
			}
			str++;
		#endif
		
	}
}

/*
	Print number on lcd
	y location (row)	
	x location (col)
	number = in32_t (range: -2147483647 to 2147483647)
 */
void LCD_PrintNumber(uint8_t y, uint8_t x, int32_t number){
	char buf[3*sizeof(number)+1];		// emty 8-bit buffer with size of 3*int32_t number; max 12 characters, including minus; + ending character
	char *str = &buf[sizeof(buf)-1];	// pointer on buffer address of the last byte
	uint32_t _number;
	char c;
	*str = '\0';
	
	if(number < 0){
		LCD_PrintString(y, x, "-");
		x++;
		number = -number;
	}
	
	do{
		_number = number;
		number /= 10;
		c = _number - 10 * number;	// residue
		str--;
		if(c < 10){
			*str = c + '0';
		}
		else{
			*str = c + 'A' - 10;
		}
	} while(number);
	LCD_PrintString(y, x, str);
}

/*
	Print float on lcd
	y location (row)	
	x location (col)
	number = float (range: -2147483647 to 2147483647)
 */
void LCD_PrintFloat(uint8_t y, uint8_t x, float number_f){
	int32_t integer_part = (int32_t) number_f;
	float decimal_part = (number_f - (float)integer_part) * 10000;
		
	if((integer_part == 0) && (decimal_part < 0)){
		LCD_PrintString(y, x, "-");
		x++;
	}
	LCD_PrintNumber(y, x, integer_part);
	x = _lcd_options.currentX++;
	LCD_PrintString(y, x, ".");
	x = _lcd_options.currentX++;
	
	
	if(decimal_part < 0){
		decimal_part = -decimal_part;
	}
	if(decimal_part < 1000){
		
		if(decimal_part < 100){
			LCD_PrintNumber(y, x, 0); 
			x = _lcd_options.currentX++;
		}
		if(decimal_part < 10){
			LCD_PrintNumber(y, x, 0); 
			x = _lcd_options.currentX++;
		}
		LCD_PrintNumber(y, x, 0); 
		x = _lcd_options.currentX++;
	}
	LCD_PrintNumber(y, x, (uint32_t)decimal_part); 
}

void LCD_Clear(void) {
	_lcd_send_command(LCD_CLEARDISPLAY);
	delay(3);
}

void LCD_DisplayOn(void) {
	_lcd_options.DisplayControl |= LCD_DISPLAYON;
	_lcd_send_command(LCD_DISPLAYCONTROL | _lcd_options.DisplayControl);
}

void LCD_DisplayOff(void) {
	_lcd_options.DisplayControl &= ~LCD_DISPLAYON;
	_lcd_send_command(LCD_DISPLAYCONTROL | _lcd_options.DisplayControl);
}

void LCD_BlinkOn(void) {
	_lcd_options.DisplayControl |= LCD_BLINKON;
	_lcd_send_command(LCD_DISPLAYCONTROL | _lcd_options.DisplayControl);
}

void LCD_BlinkOff(void) {
	_lcd_options.DisplayControl &= ~LCD_BLINKON;
	_lcd_send_command(LCD_DISPLAYCONTROL | _lcd_options.DisplayControl);
}

void LCD_CursorOn(void) {
	_lcd_options.DisplayControl |= LCD_CURSORON;
	_lcd_send_command(LCD_DISPLAYCONTROL | _lcd_options.DisplayControl);
}

void LCD_CursorOff(void) {
	_lcd_options.DisplayControl &= ~LCD_CURSORON;
	_lcd_send_command(LCD_DISPLAYCONTROL | _lcd_options.DisplayControl);
}

void LCD_ScrollLeft(void) {
	_lcd_send_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void LCD_ScrollRight(void) {
	_lcd_send_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LCD_CreateChar(uint8_t location, uint8_t *data) {
	uint8_t i;
	/* We have 8 locations available for custom characters */
	location &= 0x07;
	_lcd_send_command(LCD_SETCGRAMADDR | (location << 3));
	
	for (i = 0; i < 8; i++) {
		_lcd_send_data(data[i]);
	}
}

void LCD_PutCustom(uint8_t y, uint8_t x, uint8_t location) {
	_lcd_cursor_set(y, x);
	_lcd_send_data(location);
}

/* Private functions */
void _lcd_send_command(uint8_t cmd) {
	/* Command mode */
	GPIO_ResetBits(LCD_RS_Port, LCD_RS_Pin);
		
	/* High nibble */
	_lcd_send_command_4_bit(cmd >> 4);
	/* Low nibble */
	_lcd_send_command_4_bit(cmd & 0x0F);
}

void _lcd_send_data(uint8_t data) {
	/* Data mode */
	GPIO_SetBits(LCD_RS_Port, LCD_RS_Pin);
	
	/* High nibble */
	_lcd_send_command_4_bit(data >> 4);
	/* Low nibble */
	_lcd_send_command_4_bit(data & 0x0F);
}

void _lcd_send_command_4_bit(uint8_t cmd) {
	/* Set output port */
	GPIO_WriteBit(LCD_D7_Port, LCD_D7_Pin, (BitAction) (cmd & 0x08));
	GPIO_WriteBit(LCD_D6_Port, LCD_D6_Pin, (BitAction) (cmd & 0x04));
	GPIO_WriteBit(LCD_D5_Port, LCD_D5_Pin, (BitAction) (cmd & 0x02));
	GPIO_WriteBit(LCD_D4_Port, LCD_D4_Pin, (BitAction) (cmd & 0x01));
	_lcd_enable_pulse();
}

void _lcd_cursor_set(uint8_t row, uint8_t col){
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	
	/* Go to beginning */
	if (row >= _lcd_options.Rows) {
		row = 0;
	}
	
	/* Set current column and row */
	_lcd_options.currentX = col;
	_lcd_options.currentY = row;
	
	/* Set location address */
	_lcd_send_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void _lcd_init_pins(void) {
	/*Configure GPIO pin EN */
	gpio_pinSetup(LCD_E_Port, LCD_E_Pin, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_10MHz);
  /*Configure GPIO pin RS */
  gpio_pinSetup(LCD_RS_Port, LCD_RS_Pin, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_10MHz);
	/*Configure GPIO pin DB4 */
  gpio_pinSetup(LCD_D4_Port, LCD_D4_Pin, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_10MHz);
	/*Configure GPIO pin DB5 */
  gpio_pinSetup(LCD_D5_Port, LCD_D5_Pin, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_10MHz);
	/*Configure GPIO pin DB6 */
  gpio_pinSetup(LCD_D6_Port, LCD_D6_Pin, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_10MHz);
	/*Configure GPIO pin DB7 */
  gpio_pinSetup(LCD_D7_Port, LCD_D7_Pin, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_10MHz);
	  	
	// GPIO initial state
	GPIO_ResetBits(LCD_E_Port, LCD_E_Pin);
	GPIO_ResetBits(LCD_RS_Port, LCD_RS_Pin);
	GPIO_ResetBits(LCD_D4_Port, LCD_D4_Pin);
	GPIO_ResetBits(LCD_D5_Port, LCD_D5_Pin);
	GPIO_ResetBits(LCD_D6_Port, LCD_D6_Pin);
	GPIO_ResetBits(LCD_D7_Port, LCD_D7_Pin);
}

void _lcd_enable_pulse(void){
	GPIO_SetBits(LCD_E_Port, LCD_E_Pin);
	delay_us(2);
	GPIO_ResetBits(LCD_E_Port, LCD_E_Pin);
	delay_us(100);
}
