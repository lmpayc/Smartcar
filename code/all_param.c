#include"all_param.h"



/******************************flash*************************************************/
int data[data_number];
uint32 Data[data_number];
int x[data_number][5];
int p[5];    //pwm输出
int state=WRITE;
uint16 duty1 = 757;




/******************************按键*******************************/
//开关状态变量
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;

//上一次开关状态变量
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;

//开关标志位
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;
