#ifndef _all_param_h_
#define _all_param_h_

#include "zf_common_headfile.h"
#include "all_define.h"

/******************************flash���*************************************************/
extern int data[data_number];
extern uint32 Data[data_number];
extern int x[data_number][5];
extern int p[5];//pwm���
extern int state;
extern uint16 duty1 ;




/******************************�������*******************************/
//����״̬����
extern uint8 key1_status;
extern uint8 key2_status;
extern uint8 key3_status;
extern uint8 key4_status;

//��һ�ο���״̬����
extern uint8 key1_last_status;
extern uint8 key2_last_status;
extern uint8 key3_last_status;
extern uint8 key4_last_status;

//���ر�־λ
extern uint8 key1_flag;
extern uint8 key2_flag;
extern uint8 key3_flag;
extern uint8 key4_flag;




#endif
