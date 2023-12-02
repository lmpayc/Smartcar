#ifndef _all_define_h_
#define _all_define_h_

#include"zf_common_headfile.h"

/******************************通用宏定义******************************************************/
/*#define TRUE  1
#define FALSE 0*/
#define ERROR 2
#define FLAG_ON 1
#define FLAG_OFF 0
#define FLAG_LEFT 2
#define FLAG_RIGHT 3


/******************************flash存储相关******************************************************/
#define data_number 16
#define page_number 7
#define  READ 1
#define  WRITE 2
#define EDIT 3
#define PWM_CH ATOM2_CH0_P33_4
#define PWM_DEBUG 4


/******************************图像定义****************************/
#define bin_image_H 64
#define bin_image_W 128   //80

#define IMAGE_HIGHT MT9V03X_H
#define IMAGE_WIDTH MT9V03X_W/2
/*#define SEARCH_LEFT 1
#define SEARCH_BOT MT9V03X_DVP_H //最高的3/4为下边界
#define SEARCH_TOP (MT9V03X_DVP_H/24) //高度的1/24作为上边界
#define SEARCH_RIGHT MT9V03X_DVP_W-1*/

/******************************按键相关*******************************/
#define KEY1    (P22_3)
#define KEY2    (P22_2)
#define KEY3    (P23_1)
#define KEY4    (P22_1)
#define KEY5    (P22_0)

/*******************************电机引脚******************************/
#define     MOTOR_LEFT_PWM1        ATOM0_CH2_P21_4
#define     MOTOR_LEFT_PWM2        ATOM0_CH3_P21_5
#define     MOTOR_RIGHT_PWM1       ATOM0_CH6_P02_6
#define     MOTOR_RIGHT_PWM2       ATOM0_CH4_P02_4
#define     MOTOR_PWM_FREQ         (17000)

#endif
