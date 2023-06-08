#include "pca95555.h"
#include "hardware/i2c.h"
#include <stdbool.h>
#include <stdint.h>

void Pca95555_Init(Pca95555 *self, i2c_inst_t *i2c, bool A0, bool A1, bool A2)
{
    self->address = 0b0100000;
    if (A0) self->address |= 0b0000001;
    if (A1) self->address |= 0b0000010;
    if (A2) self->address |= 0b0000100;
    self->i2c = i2c;
}


void Pca95555_SetOutputRegister(Pca95555 *self, uint8_t O0, uint8_t O1)
{
    uint8_t buff[] = {2, O0, O1};
    i2c_write_blocking(self->i2c, self->address, buff, 3, false);
}

void Pca95555_ReadOutputRegister(Pca95555 *self, uint8_t *out)
{
    uint8_t reg_addr = 2;
    i2c_write_blocking(self->i2c, self->address, &reg_addr, 1, true);
    i2c_read_blocking(self->i2c, self->address, out, 2, false);
}

void Pca95555_SetAllInputsOneOutput(Pca95555 *self, uint8_t outputPinNo)
{
    uint8_t buff[] = {6,0xFF,0xFF};
    if (outputPinNo < 8)
    {
        buff[1] ^= (1 << outputPinNo);
    }
    else 
    {
        buff[2] ^= (1 << (outputPinNo - 8));
    }
    i2c_write_blocking(self->i2c, self->address, buff, 3, false);
}

uint8_t Pca95555_ReadI0(Pca95555 *self)
{
    uint8_t result;
    uint8_t reg_addr = 0;
    i2c_write_blocking(self->i2c, self->address, &reg_addr, 1, true);
    i2c_read_blocking(self->i2c, self->address, &result, 1, false);
    return result;
}

uint8_t Pca95555_ReadI1(Pca95555 *self)
{
    uint8_t result;
    uint8_t reg_addr = 1;
    i2c_write_blocking(self->i2c, self->address, &reg_addr, 1, true);
    i2c_read_blocking(self->i2c, self->address, &result, 1, false);
    return result;
}
