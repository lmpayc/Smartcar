#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "all_define.h"
#include "zf_common_headfile.h"
#include "user_headfile.h"

#define         QTIMER_LEFT_PHASEA   TIM6_ENCODER_CH1_P20_3     //CH1通道
#define         QTIMER_LEFT_PHASEB   TIM6_ENCODER_CH2_P20_0        //CH2通道
#define         QTIMER_LEFT_PORT     TIM6_ENCODER        //左编码器定时器
#define         QTIMER_RIGHT_PHASEA  TIM2_ENCODER_CH1_P33_7
#define         QTIMER_RIGHT_PHASEB  TIM2_ENCODER_CH2_P33_6
#define         QTIMER_RIGHT_PORT     TIM2_ENCODER       //右编码器定时器

#define         TIME_PER  (0.005)   //5ms
#define         RIGHT_1M_ENCODER  (11609)
#define         LEFT_1M_ENCODER   (11609)
#define         MOVINGAVERAGE_NUM (5)      //记录速度的5次值

typedef struct EncoderInfo{      //编码器信息
       int16 lEncoderCount;
       int16 rEncoderCount;
} EncoderInfo;

extern EncoderInfo  gl_EncoderInfo;   //这一次编码器的计数
extern EncoderInfo  l_EncoderInfo;   //上次编码器信息

extern float RIGHT_SPEED;//右轮速度
extern float LEFT_SPEED;//左轮速度
extern float DISTANCE;
extern int32 RIGHT_SPEED_NUMBER;     //相应速度下对应的脉冲数
extern int32 LEFT_SPEED_NUMBER;

void EncoderInit(void);
void ReadEncoder(void);
void speed_get(void);


#endif
