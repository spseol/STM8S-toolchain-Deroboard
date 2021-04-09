/*
 * stm8_hd_44780.h
 *
 *  Created on: 28. 6. 2020
 *      Author: misa
 */

#ifndef STM8_HD_44780_H_
#define STM8_HD_44780_H_

#include "stm8s.h"

/*
  driver for HD77480 / ST7066 or equivalents
  V2.0
 
 
  Library needs external _delay_us() function (like in "delay.h" library). Delays can be unprecise.
  Macros LCD_LINES a LCD_COLUMNS selects display variant.
  Library supports push-pull or open drain communication (LCD contains at most of pins internal pullups), selecting thru PUSHPULL_VARIANT/OPENDRAIN_VARIANT
  if using LCD without backlight, consider power lcd from GPIO pin (and when display unpowered use lcd_deinit_hw to setup all control pins to inputs)
 
  for porting to another platform modify: 
  - lcd_init_hw() 
	- lcd_bus_outputs()
	- lcd_bus_inputs()
	- lcd_deinit_hw()
	- lcd_bus_set()
	- lcd_bus_read()
	- LCD_XX_L and LCD_XX_H macros
	- do not forgot provide _delay_us() function (not need to be precise)

   Typical (!) LCD pinout
    1 - GND
    2 - VCC
    3 - Contrast
    4 - RS
    5 - RW
    6 - E
    7 - D0 (not used)
    8 - D1 (not used)
    9 - D2 (not used)
   10 - D3 (not used)
   11 - D4
   12 - D5
   13 - D6
   14 - D7
   15 - Background (A) or neg.volt.generator output
   16 - Background (K)
 */

// select one variant of communication push-pull for 3.3V LCD, Open drain for 5V LCD with external pullup at enable pin)
#define PUSHPULL_VARIANT
//#define OPENDRAIN_VARIANT

// uncomment if powering lcd from GPIO pin (feature for low power applications)
//#define USE_POWER_PIN


// lcd_gotoxy() should work for 20x4, 16x2, 20x2 ... 16x1 is atypic and must be added, 8x2 not tested
// good informations here: http://web.alfredstate.edu/faculty/weimandn/lcd/lcd_addressing/lcd_addressing_index.html
#define LCD_LINES 2 // 1 , 2 or 4
#define LCD_COLUMNS 16 // 8 , 12 , 16 , 20 , 40 (not all are properly implemented)
// select used font (most common is 5x8px font)
//#define USED_FONT	LCD_FONT5X10
#define USED_FONT	LCD_FONT5X8

// define your own pinout. Do not forgot that driver needs 5V tolerant pins
#define LCD_RS_PORT     GPIOF
#define LCD_RW_PORT     GPIOF
#define LCD_E_PORT      GPIOF
#define LCD_D4_PORT     GPIOG
#define LCD_D5_PORT     GPIOG
#define LCD_D6_PORT     GPIOG
#define LCD_D7_PORT     GPIOG

#define LCD_RS_PIN  GPIO_PIN_7
#define LCD_RW_PIN  GPIO_PIN_6
#define LCD_E_PIN   GPIO_PIN_5
#define LCD_D4_PIN  GPIO_PIN_0
#define LCD_D5_PIN  GPIO_PIN_1
#define LCD_D6_PIN  GPIO_PIN_2
#define LCD_D7_PIN  GPIO_PIN_3

// define only if powering LCD from GPIO (feature for low power applications)
#ifdef USE_POWER_PIN
	#define LCD_PWR_PORT    GPIOE
	#define LCD_PWR_PIN  		GPIO_PIN_0
#endif

// If need, you can change delays and timeouts. All in us.
#define LCD_STARTUP_DELAY1 15000
#define LCD_STARTUP_DELAY2 4100
#define LCD_STARTUP_DELAY3 100
#define LCD_E_DELAY 1
#define LCD_COMMAND_DELAY 40
#define LCD_BUSY_TIMEOUT 500 // maximum attempts to read busy flags


#define LCD_RS_L    GPIO_WriteLow(LCD_RS_PORT,LCD_RS_PIN)
#define LCD_RS_H    GPIO_WriteHigh(LCD_RS_PORT,LCD_RS_PIN)
#define LCD_RW_L    GPIO_WriteLow(LCD_RW_PORT,LCD_RW_PIN)
#define LCD_RW_H    GPIO_WriteHigh(LCD_RW_PORT,LCD_RW_PIN)
#define LCD_E_L     GPIO_WriteLow(LCD_E_PORT,LCD_E_PIN)
#define LCD_E_H     GPIO_WriteHigh(LCD_E_PORT,LCD_E_PIN)
#define LCD_D4_L    GPIO_WriteLow(LCD_D4_PORT,LCD_D4_PIN)
#define LCD_D4_H    GPIO_WriteHigh(LCD_D4_PORT,LCD_D4_PIN)
#define LCD_D5_L    GPIO_WriteLow(LCD_D5_PORT,LCD_D5_PIN)
#define LCD_D5_H    GPIO_WriteHigh(LCD_D5_PORT,LCD_D5_PIN)
#define LCD_D6_L    GPIO_WriteLow(LCD_D6_PORT,LCD_D6_PIN)
#define LCD_D6_H    GPIO_WriteHigh(LCD_D6_PORT,LCD_D6_PIN)
#define LCD_D7_L    GPIO_WriteLow(LCD_D7_PORT,LCD_D7_PIN)
#define LCD_D7_H    GPIO_WriteHigh(LCD_D7_PORT,LCD_D7_PIN)

// commands
#define LCD_DISPLAY_CLEAR           (0b1<<0)

#define LCD_RETURN_HOME             (0b1<<1)

#define LCD_ENTRY_MODE_SET          (0b1<<2)
	#define LCD_DECREMENT           (0b0<<1)
	#define LCD_INCREMENT           (0b1<<1)
	#define LCD_ENTRY_W_SHIFT      (0b1<<0)
	#define LCD_DISPLAY_NOSHIFT    (0b0<<0)

#define LCD_DISPLAY_ONOFF   (0b1<<3)
	#define LCD_ON             (0b1<<2)
	#define LCD_OFF            (0b0<<2)
	#define LCD_CURSOR_ON      (0b1<<1)
	#define LCD_CURSOR_OFF     (0b0<<1)
	#define LCD_BLINK_ON       (0b1<<0)
	#define LCD_BLINK_OFF      (0b0<<0)

#define LCD_CURSOR_OR_DISPLAY_SHIFT		(0b1<<4)
	#define LCD_DISPLAY_SHIFT          (0b1<<3)
	#define LCD_CURSOR_SHIFT           (0b0<<3)
	#define LCD_SHIFT_RIGHT            (0b1<<2)
	#define LCD_SHIFT_LEFT             (0b0<<2)

#define LCD_FUNCTION_SET    (0b1<<5)
 #define LCD_8BITS          (0b1<<4)
 #define LCD_4BITS          (0b0<<4)
 #define LCD_2LINES         (0b1<<3)
 #define LCD_1LINES         (0b0<<3)
 #define LCD_FONT5X10       (0b1<<2)
 #define LCD_FONT5X8        (0b0<<2)

#define LCD_SET_CGRAM       (0b1<<6)

#define LCD_SET_DDRAM       (0b1<<7)


void lcd_bus_set(uint8_t data); 			// write output values on D7..D4 bus
void lcd_init_hw(void);								// init GPIOs etc.
void lcd_deinit_hw(void);							// set all LCD control GPIOs as floating input
void lcd_bus_outputs(void);						// set D7..D4 as outputs (for writing to LCD)
void lcd_bus_inputs(void);						// set D7..D4 as inputs (for reading from LCD)
uint8_t lcd_bus_read(void);						// read 4bit value from D7..D4
void lcd_e_tick(void);								// generate positive pulse/clock at Enable pin
void lcd_command(uint8_t command);		// sends command to LCD and waits until busy
void lcd_data(uint8_t data);					// sends data to LCD and waits until busy
uint8_t lcd_read(void);								// read 8bit value from LCD (MSB is busy flag, LSBs address counter)
uint8_t lcd_busy_wait(void);					// wait until LCD is busy or timeout
void lcd_bus_sleep(void);							// set LCD pins to High level to cancel internal pullup currents (for low power applications)
void lcd_bus_wakeup(void);						// set LCD pins back to default state (call prior to lcd operations if previously called lcd_bus_sleep)

void lcd_init(void);																		// initialisate HW and display
void lcd_gotoxy(uint8_t column, uint8_t line);					// set cursor at desired position
void lcd_puts(char* text);															// prints string to LCD
void lcd_store_symbol(uint8_t pos, uint8_t* charmap); 	// write users own charaters into LCD CGRAM
//void _delay_us(uint16_t Delay);

#define lcd_putchar(X)							lcd_data(X)
#define lcd_busy() 									(lcd_read() & 0b1<<7)  					// check busy status flag
#define lcd_clear() 								lcd_command(LCD_DISPLAY_CLEAR) 	// clears entire display
#define lcd_home() 									lcd_command(LCD_RETURN_HOME)		// set cursor and display shift to home position
#define lcd_cursor_on() 				lcd_command(LCD_DISPLAY_ONOFF | LCD_ON | LCD_CURSOR_ON | LCD_BLINK_OFF) // activate underscore cursor
#define lcd_cursor_blink() 	lcd_command(LCD_DISPLAY_ONOFF | LCD_ON | LCD_CURSOR_OFF | LCD_BLINK_ON) 		// activate blinking block 5x8 cursor
#define lcd_cursor_off() 			lcd_command(LCD_DISPLAY_ONOFF | LCD_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF) 	// deactivate both cursor (underscore and blink)


#ifdef USE_POWER_PIN
	#define lcd_power_off(void) GPIO_WriteLow(LCD_PWR_PORT,LCD_PWR_PIN)
#endif




#endif /* STM8_HD_44780_H_ */
