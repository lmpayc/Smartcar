#include "servo.h"


uint16 delta_servo_pwm;



void servo_output(){

    float dir_kp1;
    float dir_kd1;

    dir_kp1=(float)data[1]/10;
    dir_kd1=(float)data[2]/10;



    /*直线和弯道两套PD*/
    delta_servo_pwm= (dir_kp1*g_image_err+dir_kd1*(g_image_err-g_iamge_last_err))+825;   //ouput_pwm
    g_iamge_last_err=g_image_err;

    /*输出限幅*/
    if(delta_servo_pwm>910) delta_servo_pwm=910;
    if(delta_servo_pwm<730) delta_servo_pwm=730;

    /*入库出库判断*/

    pwm_set_duty(ATOM2_CH0_P00_9,delta_servo_pwm);
}
