/*
 * VOFA.h
 *
 *  Created on: 2022Äê11ÔÂ20ÈÕ
 *      Author: The Beheaded
 */

#ifndef VOFA_H_
#define VOFA_H_

#define UART_VOFA  UART_2

typedef union
{
    float fdata;
    unsigned long ldata;
}FloatLongType;

void Float_to_Byte(float f,unsigned char byte[4]);
void Vofa_1data(float data);
void Vofa_2data(float data1, float data2);
void Vofa_4data(float data1,float data2,float data3,float data4);
void Vofa_5data(float data1,float data2,float data3,float data4,float data5);
void Vofa_6data(float data1,float data2,float data3,float data4,float data5,float data6);
void Vofa_8data(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8);




#endif /* VOFA_H_ */
