#include "servo.h"


uint16 delta_servo_pwm;



void servo_output(){

    float kp1;
    float kd1;

    kp1=(float)data[1]/10;
    kd1=(float)data[2]/10;


    delta_servo_pwm= (kp1*g_image_err+kd1*(g_image_err-g_iamge_last_err))+720;


    g_iamge_last_err=g_image_err;

    if(delta_servo_pwm>820) delta_servo_pwm=820;

    if(delta_servo_pwm<605) delta_servo_pwm=605;

    //tft180_show_int(0,110,delta_servo_pwm,3);

  // pwm_set_duty(ATOM0_CH2_P21_4,delta_servo_pwm);
    pwm_set_duty(ATOM0_CH1_P33_9,delta_servo_pwm);


}
