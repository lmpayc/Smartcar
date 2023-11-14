#include"pid.h"



//float speed;
//uint16 delta_servo_pwm;

int32 g_left_spd_error[4]={0};
int32 g_right_spd_error[4]={0};
float stop_position=0.0;
//float speed_difference[21]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.12,1.15,1.18,1.21,1.24,1.27,1.30,1.32,1.36,1.38,1.42,1.44,1.49,1.52};  //���ٱ�
int32 g_left_spd_output=0;
int32 g_right_spd_output=0;
char uart_flag;

int32 g_left_spd_output_p=0;
int32 g_left_spd_output_i=0;
int32 g_right_spd_output_p=0;
int32 g_right_spd_output_i=0;
int32 g_left_spd_output_d=0;
int32 g_right_spd_output_d=0;


void motor_speed_set(){   //�ٶȱջ�
    int g_spd_p1=0;
    int g_spd_i1=0;
    int g_spd_p2=0;
    int g_spd_i2=0;
    int g_spd_d1=0;
    int g_spd_d2=0;

    int32 left_set_speed=0;           //�ڴ��ٶ�ת���ɵ�������
    int32 right_set_speed=0;
    int spd_exp;


    g_spd_p1=(int32)data[5]/10;   //����������kp��ki��kd   //С�����һλ
    g_spd_i1=(int32)data[6]/10;
    g_spd_d1=(int32)data[7]/10;
      /*g_spd_p2=temp_para.speed_kp2;   //�ڶ���PID
    g_spd_i2=temp_para.speed_ki2;
    g_spd_d2=temp_para.speed_kd2;*/
    spd_exp=(float)data[8]/10;

    speed_get();

    left_set_speed=(int32)(spd_exp*LEFT_1M_ENCODER*TIME_PER);
    right_set_speed=(int32)(spd_exp*RIGHT_1M_ENCODER*TIME_PER);

    g_left_spd_error[0]=(int32) (left_set_speed)-LEFT_SPEED_NUMBER;    //��ʱ�ٶ�ƫ��
    g_right_spd_error[0]=(int32)(right_set_speed)-RIGHT_SPEED_NUMBER;

    g_left_spd_error[2]=g_left_spd_error[0]-g_left_spd_error[1];   //���ƫ����ϴ�ƫ��Ĳ�ֵ
    g_left_spd_error[3]=g_left_spd_error[1];
    g_left_spd_error[1]=g_left_spd_error[0];

    g_right_spd_error[2]=g_right_spd_error[0]-g_right_spd_error[1];   //���ƫ����ϴ�ƫ��Ĳ�ֵ
    g_right_spd_error[3]=g_right_spd_error[1];    //���ϴ�ƫ��ֵ
    g_right_spd_error[1]=g_right_spd_error[0];    //�ϴ�ƫ��ֵ

    //PID���
    g_left_spd_output_p+= (int32)(g_spd_p1* g_left_spd_error[2]);         //ʵ���ϳ˵���ε�ƫ��
    g_left_spd_output_i+= (int32)(g_spd_i1* g_left_spd_error[0]);         //���ƫ����ۻ�
    g_left_spd_output_d+= (int32)(g_spd_d1 *( g_left_spd_error[0]-g_left_spd_error[1]*2+g_left_spd_error[3]));   //��һ�μ���һ�� - ����һ��-���ϴΣ�

    g_right_spd_output_p+= (int32)(g_spd_p1* g_right_spd_error[2]);
    g_right_spd_output_i+= (int32)(g_spd_i1* g_right_spd_error[0]);
    g_right_spd_output_d+= (int32)(g_spd_d1*( g_right_spd_error[0]-g_right_spd_error[1]*2+g_right_spd_error[3]));

    if(g_left_spd_output_i>I_THRESHOLD){      //���������ֵ
            g_left_spd_output_i=I_THRESHOLD;
    }
    if(g_left_spd_output_i<-I_THRESHOLD){
            g_left_spd_output_i=-I_THRESHOLD;
    }
    if(g_right_spd_output_i>I_THRESHOLD){
            g_right_spd_output_i=I_THRESHOLD;
    }
    if(g_right_spd_output_i<-I_THRESHOLD){
            g_right_spd_output_i=-I_THRESHOLD;
    }

    g_left_spd_output = g_left_spd_output_p + g_left_spd_output_i+g_left_spd_output_d;  //PID�ۼ�
    g_right_spd_output = g_right_spd_output_p+g_right_spd_output_i+g_right_spd_output_d;

    //�����ֵ
     if(g_left_spd_output>S_THRESHOLD) {
            g_left_spd_output=S_THRESHOLD;
     }
     if(g_left_spd_output<-S_THRESHOLD){
            g_left_spd_output=-S_THRESHOLD;
     }
     if(g_right_spd_output>S_THRESHOLD){
            g_right_spd_output=S_THRESHOLD;
     }
     if(g_right_spd_output<-S_THRESHOLD){
            g_right_spd_output=-S_THRESHOLD;
     }

     motor_output(g_left_spd_output,g_right_spd_output);
  //  motor_output(data[0],data[0]);
}





void motor_output(int32 lMotorDuty, int32 rMotorDuty){
    int32 temp_left;
     int32 temp_right;
     uint32 left_out1;
     uint32 left_out2;
     uint32 right_out1;
     uint32 right_out2;

     temp_left=lMotorDuty/2;
     temp_right=rMotorDuty/2;

     left_out1=5000+temp_left;
     right_out1=5000+temp_right;
     left_out2=5000-temp_left;
     right_out2=5000-temp_right;


    pwm_set_duty( ATOM0_CH1_P21_3,left_out1);
    pwm_set_duty( ATOM0_CH3_P21_5,left_out2);//��������



    pwm_set_duty( ATOM0_CH6_P02_6,right_out1);
    pwm_set_duty( ATOM0_CH4_P02_4,right_out2);//�ҵ������


}







/*

void servo_output(){

    float kp;
    float ki;
    float kd;

    kp=(float)data[1]/10;
    ki=(float)data[2]/10;
    kd=(float)data[3]/10;


    delta_servo_pwm= (kp*g_image_err+kd*(g_image_err-g_iamge_last_err))+720;


    g_iamge_last_err=g_image_err;

    if(delta_servo_pwm>820) delta_servo_pwm=820;

    if(delta_servo_pwm<605) delta_servo_pwm=605;

    //tft180_show_int(0,110,delta_servo_pwm,3);

  // pwm_set_duty(ATOM0_CH2_P21_4,delta_servo_pwm);
    pwm_set_duty(ATOM0_CH1_P33_9,delta_servo_pwm);


}*/
