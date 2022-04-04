#include "stm8_hd44780.h"
#include "delay.h"

/*void _delay_us(uint16_t Delay){
while(Delay--);
}*/

// writes users own symbol into Character Graphic RAM of LCD dríver
// there are usualy 8 positions (characters 0 to 7), specified by "pos"
// each character is defined by 8 bytes array (charmap)
// bytes in array represents lines of symbol. First element in array is most top line
// in 5x8 dots character, lower 5 bits of each byte contains information, higher 3 bits should be cleared
void lcd_store_symbol(uint8_t pos, uint8_t * charmap)
{
    uint8_t i;
    if (pos > 7)
        return;
    lcd_command(LCD_SET_CGRAM | pos * 8);
    for (i = 0; i < 8; i++) {
        lcd_data(charmap[i]);
    }
    lcd_command(LCD_SET_DDRAM | 0);     // other funtions (like lcd_puts) relies that address counter points into DDRAM
}


// put string on display (from actual cursor position)
void lcd_puts(char *text)
{
    while (*text) {
        lcd_data(*text);
        text++;
    }
}

void lcd_init(void)
{
    lcd_init_hw();
    // after MCU startup is Enable pin in High (due compatibility between push-pull and open-drain variants)
    lcd_bus_outputs();
    _delay_us(LCD_STARTUP_DELAY1);

    lcd_bus_set(0b0011);
    lcd_e_tick();
    _delay_us(LCD_STARTUP_DELAY2);

    lcd_bus_set(0b0011);
    lcd_e_tick();
    _delay_us(LCD_STARTUP_DELAY3);

    lcd_bus_set(0b0011);
    lcd_e_tick();
    lcd_bus_set(0b0010);
    lcd_e_tick();
    _delay_us(LCD_COMMAND_DELAY);

#if LCD_LINES==1
    lcd_command(LCD_FUNCTION_SET | LCD_4BITS | LCD_1LINES | USED_FONT);
#else
    lcd_command(LCD_FUNCTION_SET | LCD_4BITS | LCD_2LINES | USED_FONT);
#endif
    lcd_command(LCD_ENTRY_MODE_SET | LCD_INCREMENT | LCD_DISPLAY_NOSHIFT);
    lcd_command(LCD_DISPLAY_ONOFF | LCD_ON | LCD_CURSOR_OFF |
                LCD_BLINK_OFF);
    lcd_command(LCD_CURSOR_OR_DISPLAY_SHIFT | LCD_CURSOR_SHIFT |
                LCD_SHIFT_RIGHT);
    lcd_command(LCD_DISPLAY_CLEAR);
    lcd_command(LCD_RETURN_HOME);
}

void lcd_gotoxy(uint8_t column, uint8_t line)
{
    uint8_t tmp = 0;

#if LCD_LINES==4
    if (column > (LCD_COLUMNS - 1)) {
        column = LCD_COLUMNS - 1;
    }
    if (line == 0) {
        tmp = 0x00;
    } else if (line == 1) {
        tmp = 0x40;
    } else if (line == 2) {
        tmp = 0x14;
    } else {
        tmp = 0x54;
    }
    tmp = tmp + column;
#endif

#if LCD_LINES==2
    if (column > (LCD_COLUMNS - 1)) {
        column = LCD_COLUMNS - 1;
    }
    if (line == 0) {
        tmp = 0x00;
    } else if (line == 1) {
        tmp = 0x40;
    }
    tmp = tmp + column;
#endif

#if LCD_LINES==1
    tmp = column;
#endif

    lcd_command(LCD_SET_DDRAM | tmp);
}

// initialise GPIOs
void lcd_init_hw(void)
{

    //LCD_RS_L;
    //LCD_RW_L;
    //LCD_E_H; // Enable is high after MCU reset, now we do not want generate falling edge (LCD reads command/data at falling edge !)
#ifdef USE_POWER_PIN
    GPIO_Init(LCD_PWR_PORT, LCD_PWR_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
#endif

#ifdef OPENDRAIN_VARIANT
    GPIO_Init(LCD_RS_PORT, LCD_RS_PIN, GPIO_MODE_OUT_OD_LOW_SLOW);
    GPIO_Init(LCD_RW_PORT, LCD_RW_PIN, GPIO_MODE_OUT_OD_LOW_SLOW);
    GPIO_Init(LCD_E_PORT, LCD_E_PIN, GPIO_MODE_OUT_OD_HIGH_SLOW);

    GPIO_Init(LCD_D4_PORT, LCD_D4_PIN, GPIO_MODE_OUT_OD_HIGH_SLOW);
    GPIO_Init(LCD_D5_PORT, LCD_D5_PIN, GPIO_MODE_OUT_OD_HIGH_SLOW);
    GPIO_Init(LCD_D6_PORT, LCD_D6_PIN, GPIO_MODE_OUT_OD_HIGH_SLOW);
    GPIO_Init(LCD_D7_PORT, LCD_D7_PIN, GPIO_MODE_OUT_OD_HIGH_SLOW);
#else
    GPIO_Init(LCD_RS_PORT, LCD_RS_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LCD_RW_PORT, LCD_RW_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LCD_E_PORT, LCD_E_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
#endif

    lcd_bus_outputs();
}

// set data bus as outputs for writing to LCD
void lcd_bus_outputs(void)
{
#ifdef OPENDRAIN_VARIANT
// open drain variant doesnt need change port direction
#else
    GPIO_Init(LCD_D4_PORT, LCD_D4_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LCD_D5_PORT, LCD_D5_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LCD_D6_PORT, LCD_D6_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LCD_D7_PORT, LCD_D7_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
#endif
}

// set data bus as inputs for reading from LCD
void lcd_bus_inputs(void)
{
#ifdef OPENDRAIN_VARIANT
// open drain in high level is input ;)
    LCD_D4_H;
    LCD_D5_H;
    LCD_D6_H;
    LCD_D7_H;
#else
    GPIO_Init(LCD_D4_PORT, LCD_D4_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(LCD_D5_PORT, LCD_D5_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(LCD_D6_PORT, LCD_D6_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(LCD_D7_PORT, LCD_D7_PIN, GPIO_MODE_IN_FL_NO_IT);
#endif
}

// set all control GPIOs to inputs (useful if powering LCD from GPIO pin when LCD is OFF)
void lcd_deinit_hw(void)
{
    GPIO_Init(LCD_RS_PORT, LCD_RS_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(LCD_RW_PORT, LCD_RW_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(LCD_E_PORT, LCD_E_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(LCD_D4_PORT, LCD_D4_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(LCD_D5_PORT, LCD_D5_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(LCD_D6_PORT, LCD_D6_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(LCD_D7_PORT, LCD_D7_PIN, GPIO_MODE_IN_FL_NO_IT);
}

// write 4bit value do data pins (D4..D7)
void lcd_bus_set(uint8_t data)
{
    if (data & (1 << 0)) {
        LCD_D4_H;
    } else {
        LCD_D4_L;
    }
    if (data & (1 << 1)) {
        LCD_D5_H;
    } else {
        LCD_D5_L;
    }
    if (data & (1 << 2)) {
        LCD_D6_H;
    } else {
        LCD_D6_L;
    }
    if (data & (1 << 3)) {
        LCD_D7_H;
    } else {
        LCD_D7_L;
    }
}

// read 4bit value from data pins (D4..D7)
uint8_t lcd_bus_read(void)
{
    uint8_t tmp = 0;
    if (GPIO_ReadInputPin(LCD_D4_PORT, LCD_D4_PIN)) {
        tmp |= 1 << 0;
    }
    if (GPIO_ReadInputPin(LCD_D5_PORT, LCD_D5_PIN)) {
        tmp |= 1 << 1;
    }
    if (GPIO_ReadInputPin(LCD_D6_PORT, LCD_D6_PIN)) {
        tmp |= 1 << 2;
    }
    if (GPIO_ReadInputPin(LCD_D7_PORT, LCD_D7_PIN)) {
        tmp |= 1 << 3;
    }
    return tmp;
}

// generate "tick" on Enable signal to write data/command to LCD
void lcd_e_tick(void)
{
    LCD_E_H;
    _delay_us(LCD_E_DELAY);
    LCD_E_L;
    _delay_us(LCD_E_DELAY);
}

// writes command to LCD and waits until complete
void lcd_command(uint8_t command)
{
    LCD_RS_L;
    _delay_us(LCD_E_DELAY);
    lcd_bus_set((command >> 4) & 0b1111);
    lcd_e_tick();
    lcd_bus_set(command & 0b1111);
    lcd_e_tick();
    lcd_busy_wait();
}

// writes command to LCD and waits until busy
void lcd_data(uint8_t data)
{
    LCD_RS_H;
    _delay_us(LCD_E_DELAY);
    lcd_bus_set((data >> 4) & 0b1111);
    lcd_e_tick();
    lcd_bus_set(data & 0b1111);
    lcd_e_tick();
    lcd_busy_wait();
}

// read byte from LCD (addres counter and busy flag)
uint8_t lcd_read(void)
{
    uint8_t tmp = 0;
    LCD_RS_L;
    lcd_bus_inputs();
    LCD_RW_H;
    _delay_us(LCD_E_DELAY);
    LCD_E_H;
    _delay_us(LCD_E_DELAY);
    tmp = lcd_bus_read() << 4;
    LCD_E_L;
    _delay_us(LCD_E_DELAY);
    LCD_E_H;
    _delay_us(LCD_E_DELAY);
    tmp |= lcd_bus_read();
    LCD_E_L;
    _delay_us(LCD_E_DELAY);
    LCD_RW_L;
    lcd_bus_outputs();
    return tmp;
}

// use in low power applications to minialisation LCD current during "inactivity" periods
// set all control pins (except E) to high level to remove internal pullup currents
void lcd_bus_sleep(void)
{
    LCD_RS_H;
    LCD_RW_H;
    lcd_bus_inputs();
}

// restores default state (LCD_RW_L, bus output) of control pins (recovering from "bus sleep")
void lcd_bus_wakeup(void)
{
    LCD_RW_L;
    LCD_D4_H;
    LCD_D5_H;
    LCD_D6_H;
    LCD_D7_H;
    lcd_bus_outputs();
}

// waints while busy flag is set or timeout
uint8_t lcd_busy_wait(void)
{
    uint16_t timeout = LCD_BUSY_TIMEOUT;
    while (timeout && lcd_busy()) {
        timeout--;
    }
    if (timeout == 0) {
        return 1;
    }                           // error, busy still ==1
    else {
        return 0;
    }
}

/*#ifdef USE_POWER_PIN
	void lcd_power_off(void){
	GPIO_WriteLow(LCD_PWR_PORT,LCD_PWR_PIN);
	}
#endif*/
