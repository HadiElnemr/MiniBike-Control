/*
 * iic.c
 *
 *  Created on: Sep 29, 2022
 *      Author: Administrator
 */
#include "iic.h"
/**************************************************************************
Function: Simulate IIC start signal
Input   : none
Output  : 1
�������ܣ�ģ��IIC��ʼ�ź�
��ڲ�������
����  ֵ��1
**************************************************************************/
int IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA_SET;
	if(!READ_SDA)return 0;
	IIC_SCL_SET;
	HAL_Delay_us(1);
	IIC_SDA_RESET;//START:when CLK is high,DATA change form high to low
	if(READ_SDA)return 0;
	HAL_Delay_us(1);
	IIC_SCL_RESET;//ǯסI2C���ߣ�׼�����ͻ��������
	return 1;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Stop(void)
*��������:	    //����IICֹͣ�ź�
****************************************************************************/
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL_RESET;
	IIC_SDA_RESET;//STOP:when CLK is high DATA change form low to high
	HAL_Delay_us(1);
 	IIC_SCL_SET;
 	IIC_SDA_SET;//����I2C���߽����ź�
 	HAL_Delay_us(1);
}

/**************************Function Implementation**************************
* Function Prototype: uint8_t IIC_Wait_Ack(void)
* Function Description: Waits for an ACK (Acknowledge) signal.
* Return Value: 1 - If ACK failed
*               0 - If ACK succeeded
***************************************************************************/
int IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����
	IIC_SDA_SET;
	HAL_Delay_us(1);
	IIC_SCL_SET;
	HAL_Delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			IIC_Stop();
			return 0;
		}
		HAL_Delay_us(1);
	}
	IIC_SCL_RESET;//ʱ�����0
	return 1;
}

/**************************Function Implementation***************************
* Function Prototype: void IIC_Ack(void)
* Function Description: Sends an ACK (Acknowledge) signal.
*****************************************************************************/
void IIC_Ack(void)
{
	IIC_SCL_RESET;
	SDA_OUT();
	IIC_SDA_RESET;
	HAL_Delay_us(1);
	IIC_SCL_SET;
	HAL_Delay_us(1);
	IIC_SCL_RESET;
}

/**************************Function Implementation***************************
* Function Prototype: void IIC_NAck(void)
* Function Description: Sends a NACK (Not Acknowledge) signal.
*****************************************************************************/
void IIC_NAck(void)
{
	IIC_SCL_RESET;
	SDA_OUT();
	IIC_SDA_SET;
	HAL_Delay_us(1);
	IIC_SCL_SET;
	HAL_Delay_us(1);
	IIC_SCL_RESET;
}

/**************************Function Implementation********************************************
* Function Prototype: void IIC_Send_Byte(uint8_t txd)
* Function Description: Sends one byte of data via IIC.
***********************************************************************************************/
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
	SDA_OUT();
	IIC_SCL_RESET;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
    	if((txd&0x80)>>7) IIC_SDA_SET;
    	else			  IIC_SDA_RESET;
    	txd<<=1;
    	HAL_Delay_us(1);
    	IIC_SCL_SET;
    	HAL_Delay_us(1);
    	IIC_SCL_RESET;
    	HAL_Delay_us(1);
    }
}

/**************************Function Implementation********************************************
* Function Prototype: bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
* Function Description: Writes a single byte of data to a specified register of the target device.
***********************************************************************************************/
int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
		int i;
    if (!IIC_Start())
        return 1;
    IIC_Send_Byte(addr << 1 );
    if (!IIC_Wait_Ack()) {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
		for (i = 0; i < len; i++) {
        IIC_Send_Byte(data[i]);
        if (!IIC_Wait_Ack()) {
            IIC_Stop();
            return 0;
        }
    }
    IIC_Stop();
    return 0;
}

/**************************Function Implementation********************************************
* Function Prototype: bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
* Function Description: Sends a single byte of data to a specified register of the target device.
***********************************************************************************************/
int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (!IIC_Start())
        return 1;
    IIC_Send_Byte(addr << 1);
    if (!IIC_Wait_Ack()) {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte((addr << 1)+1);
    IIC_Wait_Ack();
    while (len) {
        if (len == 1)
            *buf = IIC_Read_Byte(0);
        else
            *buf = IIC_Read_Byte(1);
        buf++;
        len--;
    }
    IIC_Stop();
    return 0;
}


/**************************Function Implementation********************************************
* Function Prototype: uint8_t IIC_Read_Byte(unsigned char ack)
* Function Description: Reads 1 byte. If ack=1, an ACK signal is sent; if ack=0, a NACK signal is sent.
***********************************************************************************************/
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
		IIC_SCL_RESET;
		HAL_Delay_us(2);
		IIC_SCL_SET;
		receive<<=1;
		if(READ_SDA)receive++;
		HAL_Delay_us(2);
    }
    if (ack)
        IIC_Ack(); //����ACK
    else
        IIC_NAck();//����nACK
    return receive;
}

/**************************Function Implementation********************************************
* Function Prototype: unsigned char I2C_ReadOneByte(unsigned char I2C_Addr, unsigned char addr)
* Function Description: Read a single byte from a specified register of the target device.
* Parameters:
*       I2C_Addr - Target device address
*       addr     - Register address
* Return Value:
*       The value read from the specified register.
***********************************************************************************************/
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
	unsigned char res=0;

	IIC_Start();
	IIC_Send_Byte(I2C_Addr);	   //����д����
	res++;
	IIC_Wait_Ack();
	IIC_Send_Byte(addr); res++;  //���͵�ַ
	IIC_Wait_Ack();
	//IIC_Stop();//����һ��ֹͣ����
	IIC_Start();
	IIC_Send_Byte(I2C_Addr+1); res++;          //�������ģʽ
	IIC_Wait_Ack();
	res=IIC_Read_Byte(0);
    IIC_Stop();//����һ��ֹͣ����

	return res;
}


/**************************Function Implementation********************************************
* Function Prototype: uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
* Function Description: Read a specified number of bytes (`length`) from a specified register of the target device.
* Parameters:
*       dev    - Target device address
*       reg    - Register address
*       length - Number of bytes to read
*       *data  - Pointer where the read data will be stored
* Return Value:
*       The number of bytes successfully read.
***********************************************************************************************/
uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data){
    uint8_t count = 0;

	IIC_Start();
	IIC_Send_Byte(dev);	   //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //���͵�ַ
    IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(dev+1);  //�������ģʽ
	IIC_Wait_Ack();

    for(count=0;count<length;count++){

		 if(count!=length-1)data[count]=IIC_Read_Byte(1);  //��ACK�Ķ�����
		 	else  data[count]=IIC_Read_Byte(0);	 //���һ���ֽ�NACK
	}
    IIC_Stop();//����һ��ֹͣ����
    return count;
}

/**************************Function Implementation********************************************
* Function Prototype: uint8_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
* Function Description: Write multiple bytes to a specified register of the target device.
* Parameters:
*       dev    - Target device address
*       reg    - Register address
*       length - Number of bytes to write
*       *data  - Pointer to the data to be written
* Return Value:
*       Indicates whether the operation was successful.
***********************************************************************************************/
uint8_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data){

 	uint8_t count = 0;
	IIC_Start();
	IIC_Send_Byte(dev);	   //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //���͵�ַ
    IIC_Wait_Ack();
	for(count=0;count<length;count++){
		IIC_Send_Byte(data[count]);
		IIC_Wait_Ack();
	 }
	IIC_Stop();//����һ��ֹͣ����

    return 1; //status == 0;
}

/**************************Function Implementation********************************************
* Function Prototype: uint8_t IICreadByte(uint8_t dev, uint8_t reg, uint8_t *data)
* Function Description: Read a single byte value from a specified register of the target device.
* Parameters:
*       dev   - Target device address
*       reg   - Register address
*       *data - Pointer to store the read data
* Return Value:
*       1 (indicating success)
***********************************************************************************************/
uint8_t IICreadByte(uint8_t dev, uint8_t reg, uint8_t *data){
	*data=I2C_ReadOneByte(dev, reg);
    return 1;
}

/**************************Function Implementation********************************************
* Function Prototype: unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
* Function Description: Write one byte to a specified register of the target device.
* Parameters:
*       dev   - Target device address
*       reg   - Register address
*       data  - The byte to be written
* Return Value:
*       1 (indicating success)
***********************************************************************************************/
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data){
    return IICwriteBytes(dev, reg, 1, &data);
}

/**************************Function Implementation********************************************
* Function Prototype: uint8_t IICwriteBits(uint8_t dev, uint8_t reg, uint8_t bitStart, uint8_t length, uint8_t data)
* Function Description: Modify multiple bits in one byte of a specified register in the target device.
* Parameters:
*       dev       - Target device address
*       reg       - Register address
*       bitStart  - Starting position of the bits to be modified in the target byte
*       length    - Number of bits to be modified
*       data      - The new value for the specified bits
* Return Value:
*       Success   - 1
*       Failure   - 0
***********************************************************************************************/

uint8_t IICwriteBits(uint8_t dev,uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data)
{

    uint8_t b;
    if (IICreadByte(dev, reg, &b) != 0) {
        uint8_t mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return IICwriteByte(dev, reg, b);
    } else {
        return 0;
    }
}

/**************************Function Implementation********************************************
* Function Prototype: uint8_t IICwriteBit(uint8_t dev, uint8_t reg, uint8_t bitNum, uint8_t data)
* Function Description: Modify a specific bit in one byte of a specified register in the target device.
* Parameters:
*       dev     - Target device address
*       reg     - Register address
*       bitNum  - The bit position to be modified in the target byte
*       data    - When 0, the target bit will be cleared (set to 0); otherwise, the target bit will be set
* Return Value:
*       Success - 1
*       Failure - 0
***********************************************************************************************/

uint8_t IICwriteBit(uint8_t dev, uint8_t reg, uint8_t bitNum, uint8_t data){
    uint8_t b;
    IICreadByte(dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return IICwriteByte(dev, reg, b);
}


