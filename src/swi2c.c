// (c) Michal Dudka
#include "swi2c.h"

// read num bytes to array data from slave with desired I2C slv_addres, from desired pointer (address)
// Generates I2C sequence SLA+W + 1Byte (Address) + RST + SLA+R + num*Byte (data)
// returns 0xaa if bus is busy too long (function cant start I2C transfer)
// returns 0xff if bus error (blocked bus)
// returns 0x01 if slave not present (NACK)
// returns 0x00 if success
// slave address in 8bit representation (right aligned 7bit value)
uint8_t swi2c_read_buf(uint8_t slv_addr, uint8_t address, uint8_t * data,
                       uint16_t num)
{
    uint8_t i = 0, bit;
    uint8_t ack;
    uint8_t mask;

    slv_addr <<=  1;

// --- Generate START ---
    if (swi2c_START()) {
        return 0xaa;
    }
// --- SLA+W ---
    mask = 1 << 7;
    while (mask) {
        if (swi2c_writebit(slv_addr & mask)) {
            return 0xff;
        }
        mask = mask >> 1;
    }
    ack = swi2c_readbit();
    if (ack) {
        if (swi2c_STOP()) {
            return 0xff;
        }
        return ack;
    }
// --- Data address (pointer in slave) ---
    mask = 1 << 7;
    while (mask) {
        if (swi2c_writebit(address & mask)) {
            return 0xff;
        }
        mask = mask >> 1;
    }
    ack = swi2c_readbit();
    if (ack) {
        if (swi2c_STOP()) {
            return 0xff;
        }
        return ack;
    }
// --- Generate RESTART ---
    if (swi2c_RESTART()) {
        return 0xff;
    }
// --- SLA+R ---
    mask = 1 << 7;
    while (mask) {
        if (swi2c_writebit((slv_addr | 1) & mask)) {
            return 0xff;
        }
        mask = mask >> 1;
    }
    ack = swi2c_readbit();
    if (ack) {
        if (swi2c_STOP()) {
            return 0xff;
        }
        return ack;
    }

// --- Data ---
    for (i = 0; i < num; i++) {
        mask = 1 << 7;
        while (mask) {
            bit = swi2c_readbit();
            if (bit == 0) {
                data[i] &= ~mask;
            } else if (bit == 1) {
                data[i] |= mask;
            } else {
                swi2c_STOP();
                return 0xff;
            }
            mask = mask >> 1;
        }
        if ((i + 1) == num) {
            if (swi2c_writebit(1)) {
                return 0xff;
            }                   // NACK
        } else {
            if (swi2c_writebit(0)) {
                return 0xff;
            }                   // ACK
        }

    }
// --- STOP ---
    if (swi2c_STOP()) {
        return 0xff;
    }
    return 0;
}


// send num bytes from array data to slave with desired I2C slv_addres, to desired pointer (address)
// Generates I2C sequence SLA+W + 1Byte (Address) + num*Byte (data)
// returns 0xaa if bus is busy too long (function cant start I2C transfer)
// returns 0xff if bus error (blocked bus)
// returns 0x01 if slave not present (NACK)
// returns 0x00 if success
// slave address in 8bit representation (right aligned 7bit value)
uint8_t swi2c_write_buf(uint8_t slv_addr, uint8_t address, uint8_t * data,
                        uint16_t num)
{
    uint8_t i;
    uint8_t ack;
    uint8_t mask;

    slv_addr <<=  1;

// --- Generate START ---
    if (swi2c_START()) {
        return 0xaa;
    }
// --- SLA+W ---
    mask = 1 << 7;
    while (mask) {
        if (swi2c_writebit(slv_addr & mask)) {
            return 0xff;
        }
        mask = mask >> 1;
    }
    ack = swi2c_readbit();
    if (ack) {
        if (swi2c_STOP()) {
            return 0xff;
        }
        return ack;
    }
// --- Data address (pointer in slave) ---
    mask = 1 << 7;
    while (mask) {
        if (swi2c_writebit(address & mask)) {
            return 0xff;
        }
        mask = mask >> 1;
    }
    ack = swi2c_readbit();
    if (ack) {
        if (swi2c_STOP()) {
            return 0xff;
        }
        return ack;
    }
// --- Data ---
    for (i = 0; i < num; i++) {
        mask = 1 << 7;
        while (mask) {
            if (swi2c_writebit(data[i] & mask)) {
                return 0xff;
            }
            mask = mask >> 1;
        }
        ack = swi2c_readbit();
        if (ack) {
            if (swi2c_STOP()) {
                return 0xff;
            }
            return ack;
        }
    }

// --- STOP ---
    if (swi2c_STOP()) {
        return 0xff;
    }
    return 0;
}



// test (ACK/NACK) of selected I2C slave address (like a ping - testing slave present on bus)
// slave address in 8bit representation (left aligned 7bit value)
// returns 0 if is slave present on bus
// returns 1 if slave is not present on bus
// returns 0xff if timeout error
// returns 0xaa if busy bus
uint8_t swi2c_test_slave(uint8_t slvaddr)
{
    uint8_t ack;
    uint8_t mask = 1 << 7;
    if (swi2c_START()) {
        return 0xaa;
    }
    while (mask) {
        if (swi2c_writebit(slvaddr & mask)) {
            return 0xff;
        }
        mask = mask >> 1;
    }
    ack = swi2c_readbit();
    if (swi2c_STOP()) {
        return 0xff;
    }
    return ack;
}

// initialise SDA and SCL pins to Outputs with Open Drain
void swi2c_init(void)
{
    GPIO_Init(SCL_GPIO, SCL_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
    GPIO_Init(SDA_GPIO, SDA_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
}

// --- Private functions ---

// read bit value from bus
// returns 0 if read 0
// returns 1 if read 1
// returns 0xff if timeout condition
uint8_t swi2c_readbit(void)
{
    uint16_t timeout = SWI2C_TIMEOUT;
    uint8_t retval;
    SDA_HIGH;                   // release SDA
    SWI2C_SETUP_TIME;
    SCL_HIGH;
    while (SCL_stat() == RESET && timeout) {
        timeout--;
    }
    if (timeout == 0) {
        return 0xff;
    }
    SWI2C_SCL_HIGH_TIME;
    if (SDA_stat() == RESET) {
        retval = 0;
    } else {
        retval = 1;
    }
    SCL_LOW;
    SWI2C_HOLD_TIME;            // hold time
    return retval;
}

// write one bit on bus
// returns 0xff if SCL line blocked (timeout)
// returns 0 if success
uint8_t swi2c_writebit(uint8_t bit)
{
    uint16_t timeout = SWI2C_TIMEOUT;
    if (bit) {
        SDA_HIGH;
    } else {
        SDA_LOW;
    }                           // set desired SDA value
    SWI2C_SETUP_TIME;           // setup time
    SCL_HIGH;
    while (SCL_stat() == RESET && timeout) {
        timeout--;
    }                           // wait until SCL is not high
    if (timeout == 0) {
        SDA_HIGH;
        return 0xff;
    }                           // generate timeout error if SCL is held Low too long
    SWI2C_SCL_HIGH_TIME;
    SCL_LOW;
    SWI2C_HOLD_TIME;            // hold time
    return 0;
}

// generate RESTART bit
// returns 1 if bus is busy (SDA or SCL is Low)
// return 0 if success
uint8_t swi2c_RESTART(void)
{
    uint16_t timeout = SWI2C_TIMEOUT;
    SCL_LOW;
    SDA_HIGH;
    while (SDA_stat() == RESET && timeout) {
        timeout--;
    }
    if (timeout == 0) {
        SCL_HIGH;
        return 0xff;
    }
    SWI2C_SS_TIME;
    SCL_HIGH;
    while (SCL_stat() == RESET && timeout) {
        timeout--;
    }
    if (timeout == 0) {
        return 0xff;
    }
    SWI2C_SS_TIME;
    SDA_LOW;
    SWI2C_SS_TIME;
    SCL_LOW;
    SWI2C_SS_TIME;
    return 0;
}

// generate START bit
// returns 0xff if bus is busy (SDA or SCL is Low)
// return 0 if success
uint8_t swi2c_START(void)
{
    if (SCL_stat() == RESET || SDA_stat() == RESET) {
        SDA_HIGH;
        SCL_HIGH;
        return 0xff;
    }
    SDA_LOW;
    SWI2C_SS_TIME;
    SCL_LOW;
    SWI2C_SS_TIME;
    return 0;
}

// generate STOP bit
// return 0xff if timeout error (SCL is held low too long)
// return 0 if success
uint8_t swi2c_STOP(void)
{
    uint16_t timeout = SWI2C_TIMEOUT;
    uint8_t retval = 0;
    SDA_LOW;
    SWI2C_SS_TIME;
    SCL_HIGH;
    while (SCL_stat() == RESET && timeout) {
        timeout--;
    }
    if (timeout == 0) {
        retval = 0xff;
    }
    SWI2C_SS_TIME;
    SDA_HIGH;
    return retval;
}

// Try to recover bus from failure
// returns 0 if bus is free (success)
// returns 0xff if SCL is hold low too long
// returns 0xee if SDA is still held low 
uint8_t swi2c_recover(void)
{
    uint16_t timeout = SWI2C_TIMEOUT;
    uint8_t i;
    SCL_HIGH;                   // release both lines
    SDA_HIGH;
    SWI2C_SETUP_TIME;
// if both lines are High => everything OK, Bus is free
    if (SCL_stat() != RESET && SDA_stat() != RESET) {
        return 0;
    }
// if some slave holds SDA in LOW
    if (SDA_stat() == RESET) {
        for (i = 0; i < 9; i++) {       // try nine times try to read one bit and pray for SDA release
            SCL_LOW;
            SWI2C_HOLD_TIME;
            SCL_HIGH;
            while (SCL_stat() == RESET && timeout) {
                timeout--;
            }
            if (timeout == 0) {
                return 0xff;
            }
            SWI2C_SCL_HIGH_TIME;
            if (SDA_stat() != RESET) {  // if slave released SDA line, generate STOP
                return (swi2c_STOP());
            }
        }
        return 0xee;
    }
}
