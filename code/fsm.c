#include "fsm.h"

uint8 CROSSROAD_FLAG=FLAG_OFF;//十字检测标记
//uint8 FORK_FLAG=FLAG_OFF;//岔口检测标记
uint8 ALL_WHITE_FLAG=FLAG_OFF;   //全白行标志
uint8 LOST_LEFT_FLAG=FLAG_OFF;      //左右丢线标志
uint8 LOST_RIGHT_FLAG=FLAG_OFF;
uint8 APRILTAG_FLAG=FLAG_OFF;
uint8 ZEBRA_FLAG=FLAG_OFF;     //斑马线标志
uint8 STOP_FLAG=FLAG_OFF;   //停车标志


/*十字*/
uint8 CROSS_ALL_FLAG=FLAG_OFF;
uint8 crossin_flag=FLAG_OFF;
uint8 crosspass_flag=FLAG_OFF;//十字中
uint8 crossend_flag=FLAG_OFF;//出十字


/*环岛*/
uint8 l_island_cheak_flag=FLAG_OFF;  //左右环岛检测标志
uint8 r_island_cheak_flag=FLAG_OFF;

uint8 ISLAND_CHEAK_FLAG=FLAG_OFF;   //环岛标志
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

/*出库入库*/
uint8 OUT_STOCK_FLAG=FLAG_OFF;   //出库
uint8 out_left_stock=FLAG_OFF;
uint8 out_right_stock=FLAG_OFF;
uint8 in_left_stock=FLAG_OFF;   //入库
uint8 in_right_stock=FLAG_OFF;

//坡道
uint8 RAMP_FLAG=FLAG_OFF;
uint8 ramp_pass_flag=FLAG_OFF;


/*斑马线*/
uint8 zebra_left_flag=FLAG_OFF;   //左右斑马线标志
uint8 zebra_right_flag=FLAG_OFF;
uint8 FIRST_ZEBRA_FLAG=FLAG_OFF;  //第一次斑马线标志






void all_white_row_judge(void){     //判断是否存在全白行
    int i,j;
    int cnt=0;  //记录全白行数量
    ALL_WHITE_FLAG=FLAG_OFF;
    for(i=50;i>=end_row;i--){   //从第50行往上搜
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
    CROSSROAD_FLAG=FLAG_OFF;//十字检测标记
    RAMP_FLAG=FLAG_OFF;   //坡道标记

    if( ALL_WHITE_FLAG==FLAG_ON && ISLAND_FLAG==FLAG_OFF && in_left_stock==FLAG_OFF&&in_right_stock==FLAG_OFF&& ramp_pass_flag==FLAG_OFF){
        CROSSROAD_FLAG=FLAG_ON;
    }
}


/*我有一个想法：打开十字补线状态时，前瞻行适当高一些,迅速通过*/
void crossroad_pass_fsm(){    //通过十字状态机
    static float indistance=0.0;   //记录十字距离
    static float enddistance=0.0;

    if(CROSSROAD_FLAG==FLAG_ON&&crossin_flag==FLAG_OFF)  //打开十字补线状态，此时前瞻行应该在补线区域内
      {
        indistance=DISTANCE;
        crossin_flag=FLAG_ON;
        CROSS_ALL_FLAG=FLAG_ON;

      }
      if((DISTANCE-indistance)>0.6&&crossin_flag==FLAG_ON)    //走过一定距离后强制关闭十字状态机
      {
        crossin_flag=FLAG_OFF;
      //crosspass_flag=FLAG_ON;
         CROSS_ALL_FLAG=FLAG_OFF;
      }
}


void lost_line_check(){
    LOST_LEFT_FLAG=FLAG_OFF;
    LOST_RIGHT_FLAG=FLAG_OFF;
    if(ALL_WHITE_FLAG==FLAG_OFF&&DISTANCE>0.1&&Search_Stop_Line<20&&ISLAND_FLAG==FLAG_OFF){     //此时前瞻比较长,从而区别于弯道
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
                    right_islandpass_flag=FLAG_ON;          //这里之后开始补线

                }
            }
    }



}




void all_judge(){
    all_white_row_judge();   //全白行检测
    cross_road_judge();   //检测十字路口
    crossroad_pass_fsm();  //通过十字路口状态
    lost_line_check();        //左右丢线检测
    island_judge();
}




