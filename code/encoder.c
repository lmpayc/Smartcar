#include "encoder.h"


EncoderInfo  gl_EncoderInfo;
EncoderInfo  l_EncoderInfo;

float RIGHT_SPEED=0.0;   //�����ٶ�
float LEFT_SPEED=0.0;
int32 RIGHT_SPEED_NUMBER=0;     //�����ֵ��ٶ�����
int32 LEFT_SPEED_NUMBER=0;
float speed_r[MOVINGAVERAGE_NUM]={0.0};     //����5���ٶ�ֵ
float speed_l[MOVINGAVERAGE_NUM]={0.0};     //����5���ٶ�ֵ
int32 speed_r_num[MOVINGAVERAGE_NUM]={0};    //����5���ٶȶ�Ӧ��������
int32 speed_l_num[MOVINGAVERAGE_NUM]={0};    //����5���ٶȶ�Ӧ��������
float DISTANCE=0.0;               //С���߹��ľ���



void EncoderInit(void){
    encoder_dir_init(TIM6_ENCODER,TIM6_ENCODER_CH1_P20_3,TIM6_ENCODER_CH2_P20_0);  //�������
    encoder_dir_init(TIM2_ENCODER,TIM2_ENCODER_CH1_P33_7,TIM2_ENCODER_CH2_P33_6);   //�б�����

    encoder_clear_count(QTIMER_LEFT_PORT);   //����������
    encoder_clear_count(QTIMER_RIGHT_PORT);

}


void ReadEncoder(void){
    int16 l_temp,r_temp;

    l_temp=0;
    r_temp=0;

    l_temp=encoder_get_count(TIM6_ENCODER);    //���������
    r_temp=-encoder_get_count(TIM2_ENCODER);    //���ұ�����

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


    for(i=MOVINGAVERAGE_NUM-1;i>=1;i--)   //����5���ٶ����飬����һλ
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

   for(i=0;i<MOVINGAVERAGE_NUM;i++)   //5���ٶ����
      {
        sum_r+=speed_r[i];
        sum_l+=speed_l[i];
        sum_r_num+=speed_r_num[i];
        sum_l_num+=speed_l_num[i];
      }

      RIGHT_SPEED_NUMBER=sum_r_num/MOVINGAVERAGE_NUM;  //����ٶ�Ϊ5���ٶ�ƽ��ֵ
      LEFT_SPEED_NUMBER=sum_l_num/MOVINGAVERAGE_NUM;
      RIGHT_SPEED=sum_r/MOVINGAVERAGE_NUM;
      LEFT_SPEED=sum_l/MOVINGAVERAGE_NUM;
      DISTANCE+=(RIGHT_SPEED+LEFT_SPEED)*TIME_PER/2.0;


}

