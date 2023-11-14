#ifndef _image_process_h_
#define _image_process_h_

#include "zf_common_headfile.h"
#include "all_define.h"
#include "all_param.h"

extern uint8  bin_image[bin_image_H][bin_image_W];  //二值化后数组

extern  uint8 start_column ;    //理论最长白列最左列
extern  uint8 end_column ;      //理论最长白列最右列
extern  uint8 start_row ;       //白列搜索开始行（最低）
extern  uint8 end_row ;          //白列搜索截至行（最高）
extern  uint8 pro_start_row; //前瞻开始行（最下边）
extern  uint8 pro_end_row;   //前瞻结束行（最上边）

extern uint8  Left_Line[64];    //左右边界
extern uint8  Right_Line[64];
extern uint8 midline_pre[64];       //计算中线

extern uint8 found_num;//找到的有效行数，最长白列最高点所在的行数
extern uint8 found_flag;



extern uint8 center_line;    //理论中心线（即最长白列所在列）


extern float g_image_err;    //平均误差
extern float g_iamge_last_err; //上次误差

extern uint8 Search_Stop_Line;



void Downsampling();
int otsu();
uint8 longest_white();
void Pixle_Filter();
void get_edge();
void get_center();
void caluate_err();







#endif
