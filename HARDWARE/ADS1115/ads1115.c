#include "ads1115.h"
#include "iic.h"
#include "delay.h"

adsGain_t m_gain = GAIN_TWOTHIRDS;

// 写16位数据
int ADS1115_Write(uint8_t SlaveAddress, uint8_t address, uint16_t data)
{
    if (!I2C_Start())
        return 0;
    I2C_SendByte(SlaveAddress);
    if (!I2C_WaitAck())
    {
        I2C_Stop();
        return 0;
    }
    I2C_SendByte(address); // address pointer regster data
    I2C_WaitAck();
    I2C_SendByte((uint8_t)(data >> 8));
    I2C_WaitAck();
    I2C_SendByte((uint8_t)(data & 0xFF));
    I2C_WaitAck();
    I2C_Stop();
    return 1;
}

// 读取16位数据
uint16_t ADS1115_Read(uint8_t SlaveAddress, uint8_t address)
{
    uint16_t REG_data = 0;
    if (!I2C_Start())
        return 0;
    I2C_SendByte(SlaveAddress);
    if (!I2C_WaitAck())
    {
        I2C_Stop();
        return 0;
    }
    I2C_SendByte(address);
    I2C_WaitAck();
    I2C_Stop();
    I2C_Start();
    I2C_SendByte(SlaveAddress + 1);
    I2C_WaitAck();

    REG_data = I2C_ReadByte() << 8;
    I2C_Ack();
    REG_data |= I2C_ReadByte();
    I2C_Ack();

    I2C_Stop();
    return REG_data;
}

void ADS1115_Init(void)
{
    uint16_t regVal = ADS1115_Read(ADS1015_ADDRESS, ADS1015_REG_POINTER_CONFIG);
    regVal &= 0xF1FF;

    regVal = 0X8183;

    ADS1115_Write(ADS1015_ADDRESS, ADS1015_REG_POINTER_CONFIG, regVal);
}

/**************************************************************************/
/*!
    @brief  Gets a single-ended ADC reading from the specified channel
*/
/**************************************************************************/
uint16_t ADS1115_ReadADCSingleEnded(uint8_t channel)
{
    if (channel > 3)
    {
        return 0;
    }

    // Start with default values
    uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
                      ADS1015_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
                      ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                      ADS1015_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
                      ADS1015_REG_CONFIG_DR_1600SPS |   // 1600 samples per second (default)
                      ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

    // Set PGA/voltage range
    config |= m_gain;

    // Set single-ended input channel
    switch (channel)
    {
    case (0):
        config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
        break;
    case (1):
        config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
        break;
    case (2):
        config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
        break;
    case (3):
        config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
        break;
    }

    // Set 'start single-conversion' bit
    config |= ADS1015_REG_CONFIG_OS_SINGLE;

    // Write config register to the ADC

    ADS1115_Write(ADS1015_ADDRESS, ADS1015_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    delay_ms(ADS1115_CONVERSIONDELAY);

    // Read the conversion results
    // Shift 12-bit results right 4 bits for the ADS1015
    return ADS1115_Read(ADS1015_ADDRESS, ADS1015_REG_POINTER_CONVERT);
}
