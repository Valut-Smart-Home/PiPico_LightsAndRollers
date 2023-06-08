#ifndef __PCA95555_H
#define __PCA95555_H

#include <stdbool.h>
#include <stdint.h>
#include "hardware/i2c.h"

#define Pca95555_SetUpTimeNs 350

typedef struct  {
    i2c_inst_t *i2c;
    uint8_t address;
} Pca95555;

void Pca95555_Init(Pca95555 *self, i2c_inst_t *i2c, bool A0, bool A1, bool A2);
void Pca95555_SetOutputRegister(Pca95555 *self, uint8_t O0, uint8_t O1);
void Pca95555_ReadOutputRegister(Pca95555 *self, uint8_t *out);
void Pca95555_SetAllInputsOneOutput(Pca95555 *self, uint8_t outputPinNo);
uint8_t Pca95555_ReadI0(Pca95555 *self);
uint8_t Pca95555_ReadI1(Pca95555 *self);

#endif //__PCA95555_H