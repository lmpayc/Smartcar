#include"pid.h"

#define cnt_1m 11609


float speed;
uint16 delta_servo_pwm;
static int16 l_cnt;

void get_speed(){
    int16 get_cnt;

    get_cnt = encoder_get_count(TIM5_ENCODER);
    speed= (get_cnt-l_cnt)/(cnt_1m*0.01);
    l_cnt = get_cnt;
}



void moto_output(){

    pwm_set_duty( ATOM0_CH0_P21_2,data[0]);//µç»ú¿ØÖÆ
    pwm_set_duty( ATOM0_CH1_P21_3,data[0]);
}


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

   pwm_set_duty(ATOM0_CH2_P21_4,delta_servo_pwm);
    //pwm_set_duty(ATOM0_CH1_P33_9,delta_servo_pwm);


}
