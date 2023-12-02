#include"pid.h"



//float speed;
//uint16 delta_servo_pwm;

int32 g_left_spd_error[4]={0};
int32 g_right_spd_error[4]={0};
float stop_position=0.0;
//float speed_difference[21]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.12,1.15,1.18,1.21,1.24,1.27,1.30,1.32,1.36,1.38,1.42,1.44,1.49,1.52};  //差速表
int32 g_left_spd_output=0;
int32 g_right_spd_output=0;
char uart_flag;

int32 g_left_spd_output_p=0;
int32 g_left_spd_output_i=0;
int32 g_right_spd_output_p=0;
int32 g_right_spd_output_i=0;
int32 g_left_spd_output_d=0;
int32 g_right_spd_output_d=0;


void motor_speed_set(){   //速度闭环
    float g_spd_p1=0;
    float g_spd_i1=0;
    float g_spd_p2=0;
    float g_spd_i2=0;
    float g_spd_d1=0;
    float g_spd_d2=0;

    int32 left_set_speed=0;           //期待速度转换成的脉冲数
    int32 right_set_speed=0;
    float spd_exp1;


    g_spd_p1=(float)data[5]/100;   //从扇区读入kp，ki，kd   //小数点后一位
    g_spd_i1=(float)data[6]/100;
    g_spd_d1=(float)data[7]/100;
      /*g_spd_p2=0;   //第二套PID
    g_spd_i2=0;
    g_spd_d2=0;*/
    spd_exp1=(float)data[8]/10;    //不同赛道对应不同速度

    speed_get();

    /*直线加速,直线弯道交界处,特殊元素处理,弯道(差速表计算)对应不同的Kp,Ki,kd*/

    left_set_speed=(int32)(spd_exp1*LEFT_1M_ENCODER*TIME_PER);
    right_set_speed=(int32)(spd_exp1*RIGHT_1M_ENCODER*TIME_PER);







    g_left_spd_error[0]=(int32) (left_set_speed)-LEFT_SPEED_NUMBER;    //此时速度偏差
    g_right_spd_error[0]=(int32)(right_set_speed)-RIGHT_SPEED_NUMBER;

    g_left_spd_error[2]=g_left_spd_error[0]-g_left_spd_error[1];   //这次偏差和上次偏差的差值
    g_left_spd_error[3]=g_left_spd_error[1];
    g_left_spd_error[1]=g_left_spd_error[0];

    g_right_spd_error[2]=g_right_spd_error[0]-g_right_spd_error[1];   //这次偏差和上次偏差的差值
    g_right_spd_error[3]=g_right_spd_error[1];    //上上次偏差值
    g_right_spd_error[1]=g_right_spd_error[0];    //上次偏差值

    //PID输出
    g_left_spd_output_p+= (int32)(g_spd_p1* g_left_spd_error[2]);         //实际上乘的这次的偏差
    g_left_spd_output_i+= (int32)(g_spd_i1* g_left_spd_error[0]);         //这次偏差的累积
    g_left_spd_output_d+= (int32)(g_spd_d1 *( g_left_spd_error[0]-g_left_spd_error[1]*2+g_left_spd_error[3]));   //这一次减上一次 - （上一次-上上次）

    g_right_spd_output_p+= (int32)(g_spd_p1* g_right_spd_error[2]);
    g_right_spd_output_i+= (int32)(g_spd_i1* g_right_spd_error[0]);
    g_right_spd_output_d+= (int32)(g_spd_d1*( g_right_spd_error[0]-g_right_spd_error[1]*2+g_right_spd_error[3]));

    if(g_left_spd_output_i>I_THRESHOLD){      //积分输出阈值
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

    g_left_spd_output = g_left_spd_output_p + g_left_spd_output_i+g_left_spd_output_d;  //PID累加
    g_right_spd_output = g_right_spd_output_p+g_right_spd_output_i+g_right_spd_output_d;

    //输出阈值
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


    pwm_set_duty( ATOM0_CH3_P21_5,left_out1);
    pwm_set_duty(ATOM0_CH2_P21_4 ,left_out2);//右电机控制

    pwm_set_duty( ATOM3_CH5_P33_13,right_out1);
    pwm_set_duty( ATOM3_CH4_P33_12,right_out2);//左电机控制



}








