#ifndef _pid_h_
#define _pid_h_
#include "all_define.h"
#include "flash.h"
#include "all_param.h"
#include "image_process.h"
#include "user_headfile.h"




#define I_THRESHOLD (9500)//�����޷�   4500
#define S_THRESHOLD (10000)//����޷�   5000

extern int32 g_left_spd_error[4];    //�����ٶ�ƫ�4������
extern int32 g_right_spd_error[4];   //�����ٶ�ƫ��
extern int32 g_left_spd_output;     //�������
extern int32 g_right_spd_output;   //�������
extern char uart_flag;
extern float stop_position;

extern int32 g_left_spd_output_p;     //�ٶȱջ�PID
extern int32 g_left_spd_output_i;
extern int32 g_right_spd_output_p;
extern int32 g_right_spd_output_i;
extern int32 g_left_spd_output_d;
extern int32 g_right_spd_output_d;





void motor_output(int32 lMotorDuty, int32 rMotorDuty);
void motor_speed_set();

#endif

