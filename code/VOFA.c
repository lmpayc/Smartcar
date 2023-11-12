/*
 * VOFA.c
 *
 *  Created on: 2022Äê11ÔÂ20ÈÕ
 *      Author: The Beheaded
 */



#include "VOFA.h"
#include "zf_driver_uart.h"
#include "zf_common_typedef.h"

void Float_to_Byte(float f,unsigned char byte[])
{
    FloatLongType fl;
    fl.fdata=f;
    byte[0]=(unsigned char)fl.ldata;
    byte[1]=(unsigned char)(fl.ldata>>8);
    byte[2]=(unsigned char)(fl.ldata>>16);
    byte[3]=(unsigned char)(fl.ldata>>24);
}

void Vofa_1data(float data)
{
    uint8 i,buff[4];

    Float_to_Byte(data,buff);
    for(i=0;i<4;i++)
    uart_write_byte(UART_VOFA, buff[i]);

    uart_write_byte(UART_VOFA, 0x00);
    uart_write_byte(UART_VOFA, 0x00);
    uart_write_byte(UART_VOFA, 0x80);
    uart_write_byte(UART_VOFA, 0x7f);
}

void Vofa_2data(float data1, float data2)
{
    uint8 i,buff[4];

    Float_to_Byte(data1,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);
    Float_to_Byte(data2,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    uart_write_byte(UART_VOFA, 0x00);
    uart_write_byte(UART_VOFA, 0x00);
    uart_write_byte(UART_VOFA, 0x80);
    uart_write_byte(UART_VOFA, 0x7f);
}

void Vofa_4data(float data1,float data2,float data3,float data4)
{
    uint8 i,buff[4];

    Float_to_Byte(data1,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data2,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data3,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data4,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    uart_write_byte(UART_VOFA, 0x00);
    uart_write_byte(UART_VOFA, 0x00);
    uart_write_byte(UART_VOFA, 0x80);
    uart_write_byte(UART_VOFA, 0x7f);
}
void Vofa_5data(float data1,float data2,float data3,float data4,float data5)
{
    uint8 i,buff[4];

    Float_to_Byte(data1,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data2,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data3,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data4,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data5,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    uart_write_byte(UART_VOFA, 0x00);
    uart_write_byte(UART_VOFA, 0x00);
    uart_write_byte(UART_VOFA, 0x80);
    uart_write_byte(UART_VOFA, 0x7f);
}

void Vofa_6data(float data1,float data2,float data3,float data4,float data5,float data6)
{
    uint8 i,buff[4];

    Float_to_Byte(data1,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data2,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data3,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data4,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data5,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data6,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    uart_write_byte(UART_VOFA, 0x00);
    uart_write_byte(UART_VOFA, 0x00);
    uart_write_byte(UART_VOFA, 0x80);
    uart_write_byte(UART_VOFA, 0x7f);
}

void Vofa_8data(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8)
{
    uint8 i,buff[4];

    Float_to_Byte(data1,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data2,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data3,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data4,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data5,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data6,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data7,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    Float_to_Byte(data8,buff);
    for(i=0;i<4;i++)
        uart_write_byte(UART_VOFA, buff[i]);

    uart_write_byte(UART_VOFA, 0x00);
    uart_write_byte(UART_VOFA, 0x00);
    uart_write_byte(UART_VOFA, 0x80);
    uart_write_byte(UART_VOFA, 0x7f);
}
