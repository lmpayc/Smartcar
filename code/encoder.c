#include "encoder.h"


EncoderInfo  gl_EncoderInfo;
EncoderInfo  l_EncoderInfo;

float RIGHT_SPEED=0.0;   //左右速度
float LEFT_SPEED=0.0;
int32 RIGHT_SPEED_NUMBER=0;     //左右轮的速度脉冲
int32 LEFT_SPEED_NUMBER=0;
float speed_r[MOVINGAVERAGE_NUM]={0.0};     //右轮5次速度值
float speed_l[MOVINGAVERAGE_NUM]={0.0};     //左轮5次速度值
int32 speed_r_num[MOVINGAVERAGE_NUM]={0};    //左轮5次速度对应的脉冲数
int32 speed_l_num[MOVINGAVERAGE_NUM]={0};    //右轮5次速度对应的脉冲数
float DISTANCE=0.0;               //小车走过的距离



void EncoderInit(void){
    encoder_dir_init(TIM6_ENCODER,TIM6_ENCODER_CH1_P20_3,TIM6_ENCODER_CH2_P20_0);  //左编码器
    encoder_dir_init(TIM2_ENCODER,TIM2_ENCODER_CH1_P33_7,TIM2_ENCODER_CH2_P33_6);   //有编码器

    encoder_clear_count(QTIMER_LEFT_PORT);   //计数器清零
    encoder_clear_count(QTIMER_RIGHT_PORT);

}


void ReadEncoder(void){
    int16 l_temp,r_temp;

    l_temp=0;
    r_temp=0;

    l_temp=encoder_get_count(TIM6_ENCODER);    //读左编码器
    r_temp=-encoder_get_count(TIM2_ENCODER);    //读右编码器

    gl_EncoderInfo.lEncoderCount=l_temp-l_EncoderInfo.lEncoderCount;
    gl_EncoderInfo.rEncoderCount=r_temp-l_EncoderInfo.rEncoderCount;

    l_EncoderInfo.lEncoderCount=l_temp;
    l_EncoderInfo.rEncoderCount=r_temp;

}

void speed_get(void){
    int i=0;
    float sum_r=0.0;
    float sum_l=0.0;
    int32 sum_r_num=0;
    int32 sum_l_num=0;

    ReadEncoder();


    for(i=MOVINGAVERAGE_NUM-1;i>=1;i--)   //更新5次速度数组，后移一位
      {
        speed_r[i]=speed_r[i-1];
        speed_l[i]=speed_l[i-1];
        speed_r_num[i]=speed_r_num[i-1];
        speed_l_num[i]=speed_l_num[i-1];
      }


    speed_r[0]=(float)(gl_EncoderInfo.rEncoderCount)/(float)(RIGHT_1M_ENCODER*TIME_PER);    //
    speed_l[0]=(float)(gl_EncoderInfo.lEncoderCount)/(float)(LEFT_1M_ENCODER*TIME_PER);
    speed_r_num[0]=gl_EncoderInfo.rEncoderCount;
    speed_l_num[0]=gl_EncoderInfo.lEncoderCount;

   for(i=0;i<MOVINGAVERAGE_NUM;i++)   //5次速度求和
      {
        sum_r+=speed_r[i];
        sum_l+=speed_l[i];
        sum_r_num+=speed_r_num[i];
        sum_l_num+=speed_l_num[i];
      }

      RIGHT_SPEED_NUMBER=sum_r_num/MOVINGAVERAGE_NUM;  //输出速度为5次速度平均值
      LEFT_SPEED_NUMBER=sum_l_num/MOVINGAVERAGE_NUM;
      RIGHT_SPEED=sum_r/MOVINGAVERAGE_NUM;
      LEFT_SPEED=sum_l/MOVINGAVERAGE_NUM;
      DISTANCE+=(RIGHT_SPEED+LEFT_SPEED)*TIME_PER/2.0;


}

