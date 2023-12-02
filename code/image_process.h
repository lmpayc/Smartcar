#ifndef _image_process_h_
#define _image_process_h_

#include "user_headfile.h"
#include "zf_common_headfile.h"
#include "all_define.h"
#include "all_param.h"

extern uint8  bin_image[bin_image_H][bin_image_W];  //��ֵ��������

extern  uint8 start_column ;    //���������������
extern  uint8 end_column ;      //���������������
extern  uint8 start_row ;       //����������ʼ�У���ͣ�
extern  uint8 end_row ;          //�������������У���ߣ�
extern  uint8 pro_start_row; //ǰհ��ʼ�У����±ߣ�
extern  uint8 pro_end_row;   //ǰհ�����У����ϱߣ�

extern uint8  Left_Line[64];    //���ұ߽�
extern uint8  Right_Line[64];
extern uint8 g_road_width[64];//·��
extern uint8 g_last_road_width[64];//�ϴ�·��
extern uint8 midline_pre[64];       //��������
extern uint8 g_midline[64];

extern uint8 found_num;//�ҵ�����Ч�������������ߵ����ڵ�����
extern uint8 found_flag;



extern uint8 center_line;    //���������ߣ�������������У�


extern float g_image_err;    //��ǰ���
extern float g_iamge_last_err; //�ϴ����
extern float g_lastimg_err[5];//[4]�ǵ�ǰ���

extern float left_Correlation;
extern float right_Correlation;

extern uint8 Search_Stop_Line;



void Downsampling();
int otsu();
uint8 longest_white();
void Pixle_Filter();
void get_edge();
void get_center();
void caluate_err();
float limit(float x,float min,float max);
void crossroad_pass();
void Correlation_get();//���ұ߽����ϵ���ж�
unsigned char GetMedianNum(uint8 * bArray, uint8 iFilterLen);//��ȡ��ֵ
void MedianFilter( uint8*line_pre,uint8 *line_now,uint8 Width);  //�������˲�

void centerline_change();//������

#endif
