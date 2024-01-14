#include "servo.h"


uint16 delta_servo_pwm;



void servo_output(){

    float dir_kp1;
    float dir_kd1;
    float dir_kp2;
    float dir_kd2;

    int duty;


    dir_kp1=(float)(data[1])/100;
    dir_kd1=(float)(data[2])/100;
    dir_kp2=(float)(data[3])/100;
    dir_kd2=(float)(data[4])/100;


    /*直线和弯道两套PD*/
  /*  if(Search_Stop_Line==5&&right_islandpass_flag==FLAG_OFF){
        delta_servo_pwm= (dir_kp2*g_image_err+dir_kd2*(g_image_err-g_iamge_last_err))+810;   //直线pid
        duty=(int)(dir_kp2*g_image_err+dir_kd2*(g_image_err-g_iamge_last_err));
    }

    else{

     */

    //delta_servo_pwm= (dir_kp1*g_image_err+dir_kd1*(g_image_err-g_iamge_last_err))+810;   //弯道PID
    duty=(int)(dir_kp1*g_image_err+dir_kd1*(g_image_err-g_iamge_last_err));


    //}



    g_iamge_last_err=g_image_err;

    /*输出限幅*/
    if(duty>65){
        duty=65;
    }

    if(duty<-60){
        duty=-60;
    }




    /*入库出库判断*/
    if(in_right_stock==FLAG_ON){
        delta_servo_pwm=875;
    }


    delta_servo_pwm=duty+810;

    //if(delta_servo_pwm>875) delta_servo_pwm=875;
    //if(delta_servo_pwm<750) delta_servo_pwm=750;



    pwm_set_duty(ATOM2_CH0_P00_9,delta_servo_pwm);

 //  pwm_set_duty(ATOM2_CH0_P00_9,data[0]);


}
