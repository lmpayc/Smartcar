#include "fsm.h"

uint8 CROSSROAD_FLAG=FLAG_OFF;//ʮ�ּ����
//uint8 FORK_FLAG=FLAG_OFF;//��ڼ����
uint8 ALL_WHITE_FLAG=FLAG_OFF;   //ȫ���б�־
uint8 LOST_LEFT_FLAG=FLAG_OFF;      //���Ҷ��߱�־
uint8 LOST_RIGHT_FLAG=FLAG_OFF;
uint8 APRILTAG_FLAG=FLAG_OFF;
uint8 ZEBRA_FLAG=FLAG_OFF;     //�����߱�־
uint8 STOP_FLAG=FLAG_OFF;   //ͣ����־


/*ʮ��*/
uint8 CROSS_ALL_FLAG=FLAG_OFF;
uint8 crossin_flag=FLAG_OFF;
uint8 crosspass_flag=FLAG_OFF;//ʮ����
uint8 crossend_flag=FLAG_OFF;//��ʮ��


/*����*/
uint8 l_island_cheak_flag=FLAG_OFF;  //���һ�������־
uint8 r_island_cheak_flag=FLAG_OFF;

uint8 ISLAND_CHEAK_FLAG=FLAG_OFF;   //������־
uint8 left_islandin_flag=FLAG_OFF;
uint8 right_islandin_flag=FLAG_OFF;

uint8 left_islandout_flag=FLAG_OFF;
uint8 right_islandout_flag=FLAG_OFF;
uint8 left_islandend_flag=FLAG_OFF;
uint8 right_islandend_flag=FLAG_OFF;
uint8 ISLAND_FLAG=FLAG_OFF;
uint8 limit_left_island=FLAG_OFF;
uint8 limit_right_island=FLAG_OFF;
uint8 left_islandpass_flag=FLAG_OFF;
uint8 right_islandpass_flag=FLAG_OFF;

/*�������*/
uint8 OUT_STOCK_FLAG=FLAG_OFF;   //����
uint8 out_left_stock=FLAG_OFF;
uint8 out_right_stock=FLAG_OFF;
uint8 in_left_stock=FLAG_OFF;   //���
uint8 in_right_stock=FLAG_OFF;

//�µ�
uint8 RAMP_FLAG=FLAG_OFF;
uint8 ramp_pass_flag=FLAG_OFF;


/*������*/
uint8 zebra_left_flag=FLAG_OFF;   //���Ұ����߱�־
uint8 zebra_right_flag=FLAG_OFF;
uint8 FIRST_ZEBRA_FLAG=FLAG_OFF;  //��һ�ΰ����߱�־






void all_white_row_judge(void){     //�ж��Ƿ����ȫ����
    int i,j;
    int cnt=0;  //��¼ȫ��������
    ALL_WHITE_FLAG=FLAG_OFF;
    for(i=50;i>=end_row;i--){   //�ӵ�50��������
        for(j=0;j<128;j++){
            if(bin_image[i][j]==0) break;
        }
        if(j>115) cnt++;

        if(cnt>1) //if(cnt>1&&DISTANCE>0.1)
           {
             ALL_WHITE_FLAG=FLAG_ON;
             break;
           }
    }

}


void cross_road_judge(){
    CROSSROAD_FLAG=FLAG_OFF;//ʮ�ּ����
    RAMP_FLAG=FLAG_OFF;   //�µ����

    if( ALL_WHITE_FLAG==FLAG_ON && ISLAND_FLAG==FLAG_OFF && in_left_stock==FLAG_OFF&&in_right_stock==FLAG_OFF&& ramp_pass_flag==FLAG_OFF){
        CROSSROAD_FLAG=FLAG_ON;
    }
}


/*����һ���뷨����ʮ�ֲ���״̬ʱ��ǰհ���ʵ���һЩ,Ѹ��ͨ��*/
void crossroad_pass_fsm(){    //ͨ��ʮ��״̬��
    static float indistance=0.0;   //��¼ʮ�־���
    static float enddistance=0.0;

    if(CROSSROAD_FLAG==FLAG_ON&&crossin_flag==FLAG_OFF)  //��ʮ�ֲ���״̬����ʱǰհ��Ӧ���ڲ���������
      {
        indistance=DISTANCE;
        crossin_flag=FLAG_ON;
        CROSS_ALL_FLAG=FLAG_ON;

      }
      if((DISTANCE-indistance)>0.6&&crossin_flag==FLAG_ON)    //�߹�һ�������ǿ�ƹر�ʮ��״̬��
      {
        crossin_flag=FLAG_OFF;
      //crosspass_flag=FLAG_ON;
         CROSS_ALL_FLAG=FLAG_OFF;
      }
}


void lost_line_check(){
    LOST_LEFT_FLAG=FLAG_OFF;
    LOST_RIGHT_FLAG=FLAG_OFF;
    if(ALL_WHITE_FLAG==FLAG_OFF&&DISTANCE>0.1&&Search_Stop_Line<20&&ISLAND_FLAG==FLAG_OFF){     //��ʱǰհ�Ƚϳ�,�Ӷ����������
        if(right_Correlation>=0.99)
            {
              LOST_LEFT_FLAG=FLAG_ON;
            }
            if(left_Correlation<=-0.99)
            {
             LOST_RIGHT_FLAG=FLAG_ON;
            }
            if(right_Correlation>0.99&&left_Correlation<-0.99)
               {
              LOST_LEFT_FLAG=FLAG_OFF;
              LOST_RIGHT_FLAG=FLAG_OFF;
            }
    }
}

void island_judge(){
    ISLAND_FLAG=FLAG_OFF;
    r_island_cheak_flag=FLAG_OFF;


    if(LOST_RIGHT_FLAG==FLAG_ON&&ISLAND_FLAG==FLAG_OFF){
            ISLAND_FLAG==FLAG_ON;
            r_island_cheak_flag=FLAG_ON;
            if(r_island_cheak_flag==FLAG_ON&&ISLAND_FLAG==FLAG_ON){
                if(bin_image[60][125]==0&&bin_image[61][125]==0){
                    right_islandpass_flag=FLAG_ON;          //����֮��ʼ����

                }
            }
    }



}




void all_judge(){
    all_white_row_judge();   //ȫ���м��
    cross_road_judge();   //���ʮ��·��
    crossroad_pass_fsm();  //ͨ��ʮ��·��״̬
    lost_line_check();        //���Ҷ��߼��
    island_judge();
}




