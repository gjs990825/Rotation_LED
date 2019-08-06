#if !defined(_SPI_H_)
#define _SPI_H_

#include "sys.h"

void HardwareSPI_Init(void);
uint8_t SPI2_RW(uint8_t data);

#endif // _SPI_H_
