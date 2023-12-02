#ifndef _FSM_H_
#define _FSM_H_

#include "all_define.h"
#include "zf_common_headfile.h"
#include "user_headfile.h"


/*����״̬*/
extern uint8 CROSSROAD_FLAG;//ʮ�ּ����
//extern uint8 FORK_FLAG;//��ڼ����
extern uint8 ALL_WHITE_FLAG;//ȫ���б��
extern uint8 LOST_LEFT_FLAG;//����
extern uint8 LOST_RIGHT_FLAG;//�Ҷ���
extern uint8 ISLAND_CHEAK_FLAG;   //�������
extern uint8 ZEBRA_FLAG;    //�����߼���־
extern uint8 STOP_FLAG;  //ͣ��


/*ʮ��*/
extern uint8 CROSS_ALL_FLAG;   //���ʮ�ֱ�־
extern uint8 crossin_flag;//ʮ�ֲ��߱�־
extern uint8 crosspass_flag;//ͨ��ʮ�ֱ�־
extern uint8 crossend_flag;//ʮ�ֽ���

/*����*/
extern uint8 r_island_cheak_flag;         //�����ڶ��׶�ʶ��
extern uint8 l_island_cheak_flag;
extern uint8 left_islandout_flag;         //��������־
extern uint8 right_islandout_flag;
extern uint8 left_islandin_flag;           //�����б�־
extern uint8 right_islandin_flag;
extern uint8 left_islandend_flag;
extern uint8 right_islandend_flag;
extern uint8 ISLAND_FLAG;                //������һ�׶�ʶ��
extern uint8 limit_left_island;
extern uint8 limit_right_island;
extern uint8 left_islandpass_flag;       //���뻷��ʶ�𣨴�ʱ��ʼ���ߣ�
extern uint8 right_islandpass_flag;

/*�������*/
extern uint8 OUT_STOCK_FLAG;
extern uint8 out_left_stock;
extern uint8 out_right_stock;
extern uint8 in_left_stock;
extern uint8 in_right_stock;

/*�µ�*/
extern uint8 RAMP_FLAG;
extern uint8 ramp_pass_flag;


/*������*/
extern uint8 zebra_left_flag;
extern uint8 zebra_right_flag;
extern uint8 ZEBRA_FLAG;
extern uint8 FIRST_ZEBRA_FLAG;


void all_judge();      //Ԫ���ж�
void all_white_row_judge(void);
void crossroad_pass_fsm();
void lost_line_check();
void island_judge();    //�������
void island_pass_fsm();


#endif



