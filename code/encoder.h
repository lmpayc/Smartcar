#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "all_define.h"
#include "zf_common_headfile.h"
#include "user_headfile.h"

#define         QTIMER_LEFT_PHASEA   TIM6_ENCODER_CH1_P20_3     //CH1ͨ��
#define         QTIMER_LEFT_PHASEB   TIM6_ENCODER_CH2_P20_0        //CH2ͨ��
#define         QTIMER_LEFT_PORT     TIM6_ENCODER        //���������ʱ��
#define         QTIMER_RIGHT_PHASEA  TIM2_ENCODER_CH1_P33_7
#define         QTIMER_RIGHT_PHASEB  TIM2_ENCODER_CH2_P33_6
#define         QTIMER_RIGHT_PORT     TIM2_ENCODER       //�ұ�������ʱ��

#define         TIME_PER  (0.005)   //5ms
#define         RIGHT_1M_ENCODER  (11609)
#define         LEFT_1M_ENCODER   (11609)
#define         MOVINGAVERAGE_NUM (5)      //��¼�ٶȵ�5��ֵ

typedef struct EncoderInfo{      //��������Ϣ
       int16 lEncoderCount;
       int16 rEncoderCount;
} EncoderInfo;

extern EncoderInfo  gl_EncoderInfo;   //��һ�α������ļ���
extern EncoderInfo  l_EncoderInfo;   //�ϴα�������Ϣ

extern float RIGHT_SPEED;//�����ٶ�
extern float LEFT_SPEED;//�����ٶ�
extern float DISTANCE;
extern int32 RIGHT_SPEED_NUMBER;     //��Ӧ�ٶ��¶�Ӧ��������
extern int32 LEFT_SPEED_NUMBER;

void EncoderInit(void);
void ReadEncoder(void);
void speed_get(void);


#endif
