#ifndef _pid_h_
#define _pid_h_
#include "all_define.h"
#include "flash.h"
#include "all_param.h"
#include "image_process.h"
#include "user_headfile.h"




#define I_THRESHOLD (9500)//积分限幅   4500
#define S_THRESHOLD (10000)//输出限幅   5000

extern int32 g_left_spd_error[4];    //左轮速度偏差，4组数据
extern int32 g_right_spd_error[4];   //右轮速度偏差
extern int32 g_left_spd_output;     //左轮输出
extern int32 g_right_spd_output;   //右轮输出
extern char uart_flag;
extern float stop_position;

extern int32 g_left_spd_output_p;     //速度闭环PID
extern int32 g_left_spd_output_i;
extern int32 g_right_spd_output_p;
extern int32 g_right_spd_output_i;
extern int32 g_left_spd_output_d;
extern int32 g_right_spd_output_d;





void motor_output(int32 lMotorDuty, int32 rMotorDuty);
void motor_speed_set();

#endif

