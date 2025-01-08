/*
 * icc.h
 *
 *  Created on: Sep 29, 2022
 *      Author: Administrator
 */

#ifndef INC_IIC_H_
#define INC_IIC_H_
#include "main.h"

//IO��������1 IO port initialization
#define SDA_IN()  {GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRH|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRH|=(u32)3<<28;}

//IO�������� IO port initialization
#define IIC_SCL_SET  	HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_SET)
#define IIC_SCL_RESET	HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET)
#define IIC_SDA_SET  	HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_SET)
#define IIC_SDA_RESET	HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_RESET)
#define READ_SDA  		HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port, IIC_SDA_Pin)

//IIC���в������� Initialize IIC IO port
void IIC_Init(void);                //��ʼ��IIC��IO�� Initialize IIC IO port
int IIC_Start(void);				//����IIC��ʼ�ź� Start IIC
void IIC_Stop(void);	  			//����IICֹͣ�ź� Stop IIC
void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ� Send one byte of data via I2C
uint8_t IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ� Read one byte of data via I2C
int IIC_Wait_Ack(void); 			//IIC�ȴ�ACK�ź� Wait for ACK signal
void IIC_Ack(void);					//IIC����ACK�ź� Send ACK signal
void IIC_NAck(void);				//IIC������ACK�ź�  Send NACK signal

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);
unsigned char I2C_Readkey(unsigned char I2C_Addr);

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
uint8_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);
uint8_t IICwriteBits(uint8_t dev,uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data);
uint8_t IICwriteBit(uint8_t dev,uint8_t reg,uint8_t bitNum,uint8_t data);
uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);

int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

#endif /* INC_IIC_H_ */
