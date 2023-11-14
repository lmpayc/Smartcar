#ifndef _all_define_h_
#define _all_define_h_

#include"zf_common_headfile.h"

#define bin_image_H 64
#define bin_image_W 128   //80

/******************************flash存储相关******************************************************/
#define data_number 16
#define page_number 7
#define  READ 1
#define  WRITE 2
#define EDIT 3
#define PWM_CH ATOM2_CH0_P33_4
#define PWM_DEBUG 4


/******************************图像基础定义****************************/
#define IMAGE_HIGHT MT9V03X_H
#define IMAGE_WIDTH MT9V03X_W/2
/*#define SEARCH_LEFT 1
#define SEARCH_BOT MT9V03X_DVP_H //最高的3/4为下边界
#define SEARCH_TOP (MT9V03X_DVP_H/24) //高度的1/24作为上边界
#define SEARCH_RIGHT MT9V03X_DVP_W-1*/

/******************************按键相关*******************************/
#define KEY1    (P22_0)
#define KEY2    (P22_1)
#define KEY3    (P22_2)
#define KEY4    (P22_3)





#endif
