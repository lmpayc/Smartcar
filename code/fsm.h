#ifndef _FSM_H_
#define _FSM_H_

#include "all_define.h"
#include "zf_common_headfile.h"
#include "user_headfile.h"


/*所有状态*/
extern uint8 CROSSROAD_FLAG;//十字检测标记
//extern uint8 FORK_FLAG;//岔口检测标记
extern uint8 ALL_WHITE_FLAG;//全白行标记
extern uint8 LOST_LEFT_FLAG;//左丢线
extern uint8 LOST_RIGHT_FLAG;//右丢线
extern uint8 ISLAND_CHEAK_FLAG;   //环岛检测
extern uint8 ZEBRA_FLAG;    //斑马线检测标志
extern uint8 STOP_FLAG;  //停车


/*十字*/
extern uint8 CROSS_ALL_FLAG;   //检测十字标志
extern uint8 crossin_flag;//十字补线标志
extern uint8 crosspass_flag;//通过十字标志
extern uint8 crossend_flag;//十字结束

/*环岛*/
extern uint8 r_island_cheak_flag;         //环岛第二阶段识别
extern uint8 l_island_cheak_flag;
extern uint8 left_islandout_flag;         //出环岛标志
extern uint8 right_islandout_flag;
extern uint8 left_islandin_flag;           //环岛中标志
extern uint8 right_islandin_flag;
extern uint8 left_islandend_flag;
extern uint8 right_islandend_flag;
extern uint8 ISLAND_FLAG;                //环岛第一阶段识别
extern uint8 limit_left_island;
extern uint8 limit_right_island;
extern uint8 left_islandpass_flag;       //进入环岛识别（此时开始补线）
extern uint8 right_islandpass_flag;

/*出库入库*/
extern uint8 OUT_STOCK_FLAG;
extern uint8 out_left_stock;
extern uint8 out_right_stock;
extern uint8 in_left_stock;
extern uint8 in_right_stock;

/*坡道*/
extern uint8 RAMP_FLAG;
extern uint8 ramp_pass_flag;


/*斑马线*/
extern uint8 zebra_left_flag;
extern uint8 zebra_right_flag;
extern uint8 ZEBRA_FLAG;
extern uint8 FIRST_ZEBRA_FLAG;


void all_judge();      //元素判断
void all_white_row_judge(void);
void crossroad_pass_fsm();
void lost_line_check();
void island_judge();    //环岛检测
void island_pass_fsm();


#endif



