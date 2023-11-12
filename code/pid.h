#ifndef _pid_h_
#define _pid_h_
#include "all_define.h"
#include "flash.h"
#include "all_param.h"
#include "image_process.h"


extern uint16 delta_servo_pwm;
extern float speed;

void servo_output();
void moto_output();


#endif

