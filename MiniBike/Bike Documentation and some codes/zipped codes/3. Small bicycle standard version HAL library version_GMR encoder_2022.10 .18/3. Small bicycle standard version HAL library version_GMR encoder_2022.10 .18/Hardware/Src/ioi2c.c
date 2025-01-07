#include "ioi2c.h"
#include "sys.h"
#include "delay.h"
#include "stm32f1xx_hal.h"

GPIO_InitTypeDef GPIO_InitStruct = {0};

/**************************************************************************
 * Function: void IIC_Init(void)
 * Description: Initialize the I2C interface.
 *************************************************************************/
void IIC_Init(void)
{
    // Enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Configure GPIOA Pin 8 as Output for SCL
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure GPIOB Pin 15 as Output for SDA
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Set default levels
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
}

/**************************************************************************
 * Function: int IIC_Start(void)
 * Description: Generate I2C start condition.
 *************************************************************************/
//int IIC_Start(void)
//{
//    SDA_OUT();
//    IIC_SDA = 1;
//    IIC_SCL = 1;
//    delay_us(1);
//    IIC_SDA = 0;
//    if (READ_SDA)
//        return 0;
//    delay_us(1);
//    IIC_SCL = 0;
//    return 1;
//}

/**************************************************************************
 * Function: void IIC_Stop(void)
 * Description: Generate I2C stop condition.
 *************************************************************************/
//void IIC_Stop(void)
//{
//    SDA_OUT();
//    IIC_SCL = 0;
//    IIC_SDA = 0;
//    delay_us(1);
//    IIC_SCL = 1;
//    IIC_SDA = 1;
//    delay_us(1);
//}

/**************************************************************************
 * Function: int IIC_Wait_Ack(void)
 * Description: Wait for acknowledgment from I2C slave.
 *************************************************************************/
//int IIC_Wait_Ack(void)
//{
//    u8 ucErrTime = 0;
//    SDA_IN();
//    IIC_SDA = 1;
//    delay_us(1);
//    IIC_SCL = 1;
//    delay_us(1);
//    while (READ_SDA)
//    {
//        ucErrTime++;
//        if (ucErrTime > 50)
//        {
//            IIC_Stop();
//            return 0;
//        }
//        delay_us(1);
//    }
//    IIC_SCL = 0;
//    return 1;
//}

/**************************************************************************
 * Function: void IIC_Ack(void)
 * Description: Send ACK signal.
 *************************************************************************/
//void IIC_Ack(void)
//{
//    IIC_SCL = 0;
//    SDA_OUT();
//    IIC_SDA = 0;
//    delay_us(1);
//    IIC_SCL = 1;
//    delay_us(1);
//    IIC_SCL = 0;
//}

/**************************************************************************
 * Function: void IIC_NAck(void)
 * Description: Send NACK signal.
 *************************************************************************/
//void IIC_NAck(void)
//{
//    IIC_SCL = 0;
//    SDA_OUT();
//    IIC_SDA = 1;
//    delay_us(1);
//    IIC_SCL = 1;
//    delay_us(1);
//    IIC_SCL = 0;
//}

/**************************************************************************
 * Function: void IIC_Send_Byte(u8 txd)
 * Description: Send one byte of data via I2C.
 *************************************************************************/
//void IIC_Send_Byte(u8 txd)
//{
//    u8 t;
//    SDA_OUT();
//    IIC_SCL = 0;
//    for (t = 0; t < 8; t++)
//    {
//        IIC_SDA = (txd & 0x80) >> 7;
//        txd <<= 1;
//        delay_us(1);
//        IIC_SCL = 1;
//        delay_us(1);
//        IIC_SCL = 0;
//        delay_us(1);
//    }
//}

/**************************************************************************
 * Function: int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
 * Description: Write multiple bytes to I2C device.
 *************************************************************************/
//int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
//{
//    int i;
//    if (!IIC_Start())
//        return 1;
//    IIC_Send_Byte(addr << 1);
//    if (!IIC_Wait_Ack())
//    {
//        IIC_Stop();
//        return 1;
//    }
//    IIC_Send_Byte(reg);
//    IIC_Wait_Ack();
//    for (i = 0; i < len; i++)
//    {
//        IIC_Send_Byte(data[i]);
//        if (!IIC_Wait_Ack())
//        {
//            IIC_Stop();
//            return 0;
//        }
//    }
//    IIC_Stop();
//    return 0;
//}

/**************************************************************************
 * Function: int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
 * Description: Read multiple bytes from I2C device.
 *************************************************************************/
//int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
//{
//    if (!IIC_Start())
//        return 1;
//    IIC_Send_Byte(addr << 1);
//    if (!IIC_Wait_Ack())
//    {
//        IIC_Stop();
//        return 1;
//    }
//    IIC_Send_Byte(reg);
//    IIC_Wait_Ack();
//    IIC_Start();
//    IIC_Send_Byte((addr << 1) + 1);
//    IIC_Wait_Ack();
//    while (len)
//    {
//        if (len == 1)
//            *buf = IIC_Read_Byte(0);
//        else
//            *buf = IIC_Read_Byte(1);
//        buf++;
//        len--;
//    }
//    IIC_Stop();
//    return 0;
//}

/**************************************************************************
 * Function: u8 IIC_Read_Byte(unsigned char ack)
 * Description: Read one byte via I2C.
 *************************************************************************/
//u8 IIC_Read_Byte(unsigned char ack)
//{
//    unsigned char i, receive = 0;
//    SDA_IN();
//    for (i = 0; i < 8; i++)
//    {
//        IIC_SCL = 0;
//        delay_us(2);
//        IIC_SCL = 1;
//        receive <<= 1;
//        if (READ_SDA)
//            receive++;
//        delay_us(2);
//    }
//    if (ack)
//        IIC_Ack();
//    else
//        IIC_NAck();
//    return receive;
//}
