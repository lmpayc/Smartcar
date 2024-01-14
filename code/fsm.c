#include "fsm.h"

uint8 CROSSROAD_FLAG=FLAG_OFF;//十字检测标记
//uint8 FORK_FLAG=FLAG_OFF;//岔口检测标记
uint8 ALL_WHITE_FLAG=FLAG_OFF;   //全白行标志
uint8 LOST_LEFT_FLAG=FLAG_OFF;      //左右丢线标志
uint8 LOST_RIGHT_FLAG=FLAG_OFF;
uint8 APRILTAG_FLAG=FLAG_OFF;
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
uint8 island_cnt=0;
float outdistance;

float ANGLE=0.0;
int16 imu660ra_gyro_z_temp[10]={0};

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
uint8 ZEBRA_FLAG=FLAG_OFF;     //斑马线标志



void get_angle()//角度检测
{
  int i=0;
  int sum=0;
 // imu660ra_get_gyro();

  if(imu660ra_gyro_z<30&&imu660ra_gyro_z>-30)
  {
      imu660ra_gyro_z=0;
  }
  for(i=0;i<9;i++)
  {
      imu660ra_gyro_z_temp[i]=imu660ra_gyro_z_temp[i+1];
  }
  imu660ra_gyro_z_temp[9]=imu660ra_gyro_z;
  for(i=0;i<10;i++)
    {
   sum+=imu660ra_gyro_z_temp[i];
  }
  ANGLE+=(float)((sum/(10*16.4))*PER_TIME_ERR);

}





void all_white_row_judge(void){     //判断是否存在全白行
    int i,j;
    int cnt=0;  //记录全白行数量
    ALL_WHITE_FLAG=FLAG_OFF;
    for(i=45;i>=end_row;i--){   //从第45行往上搜
        for(j=0;j<128;j++){
            if(bin_image[i][j]==0) break;
        }
        if(j>115) cnt++;

        if(cnt>1&&DISTANCE>0.1) //if(cnt>1&&DISTANCE>0.1)
           {
             ALL_WHITE_FLAG=FLAG_ON;
             break;
           }
    }

}


void out_stock(){       //出库状态
    static int flag=0;
    int out_dir=data[13];   //出库方向

    if(out_dir==0&&flag==0)//左转
     {
        OUT_STOCK_FLAG=FLAG_ON;
        out_left_stock=FLAG_ON;

     }
     if(out_dir==1&&flag==0)//右转
     {
        OUT_STOCK_FLAG=FLAG_ON;
        out_right_stock=FLAG_ON;


     }
     if(out_dir==2&&flag==0)//不动
    {
        OUT_STOCK_FLAG=FLAG_OFF;
        out_left_stock=FLAG_OFF;
        out_right_stock=FLAG_OFF;
     }
    if(OUT_STOCK_FLAG==FLAG_ON&&DISTANCE>0.5&&flag==0)
    {
      OUT_STOCK_FLAG=FLAG_OFF;
        out_left_stock=FLAG_OFF;
        out_right_stock=FLAG_OFF;
        flag=1;
    }
}


void zebra_cheak(){
    int i=0;
      int j=0;
      int temp=0;

      zebra_left_flag=FLAG_OFF;
      zebra_right_flag=FLAG_OFF;
      ZEBRA_FLAG=FLAG_OFF;
      int num1=0;
      int num2=0;

      int row_zebra=0;//具有斑马线特征的行，如果它大于某个值，则该帧具有斑马线
      int black_counter=0;      //每一行黑白跳变点个数
      int black_counter_max,black_counter_min;  /*black_counter必须在这个范围内，否则会造成误判*/

      black_counter_min=5;
      black_counter_max=8;

      for(i=start_row;i>=end_row;i--)
       {
          black_counter=0;
        for(j=3;j<120;j++){  //从左往右找黑白跳变点
            if((bin_image[i][j]==0&&bin_image[i][j+1]==255)||(bin_image[i][j]==0&&bin_image[i][j+1]==255)){
                 black_counter++;
                 if( black_counter> black_counter_min&&black_counter<black_counter_max){   //如果还存在误判，，追加均匀性判断
                     row_zebra++;
                 }
            }
        }
       }

      for(i=pro_start_row-1;i>=pro_end_row;i--)           //图像左右哪边是白色
       {
         if(bin_image[i][0]==255)
           num1++;
         if(bin_image[i][126]==255)
           num2++;
       }

      if(ALL_WHITE_FLAG==FLAG_OFF&&CROSS_ALL_FLAG==FLAG_OFF&&in_left_stock==FLAG_OFF&&in_right_stock==FLAG_OFF&&ZEBRA_FLAG==FLAG_OFF&&FIRST_ZEBRA_FLAG==FLAG_OFF&&ramp_pass_flag==FLAG_OFF)
        {
         if(row_zebra>10&&DISTANCE>0.1)
          {

            if(num1>=6&&data[13]==0)  //左转入库
            {

              zebra_left_flag=FLAG_ON;
              zebra_right_flag=FLAG_OFF;
              ZEBRA_FLAG=FLAG_ON;
            }
             if(num2>=6&&data[13]==1)   //右转入库
            {

              zebra_right_flag=FLAG_ON;
              zebra_left_flag=FLAG_OFF;
              ZEBRA_FLAG=FLAG_ON;

            }
          }
        }
}









void cross_road_judge(){
    CROSSROAD_FLAG=FLAG_OFF;//十字检测标记
    RAMP_FLAG=FLAG_OFF;   //坡道标记

    if( ALL_WHITE_FLAG==FLAG_ON && ISLAND_FLAG==FLAG_OFF && in_left_stock==FLAG_OFF&&in_right_stock==FLAG_OFF&& ramp_pass_flag==FLAG_OFF&&ZEBRA_FLAG==FLAG_OFF){
        CROSSROAD_FLAG=FLAG_ON;
    }
}


/*我有一个想法：打开十字补线状态时，前瞻行适当高一些,迅速通过*/
void crossroad_pass_fsm(){    //通过十字状态机
    static float indistance=0.0;   //记录十字距离
    static float enddistance=0.0;

    if(CROSSROAD_FLAG==FLAG_ON&&crossin_flag==FLAG_OFF&&left_point<25&&right_point<25)  //打开十字状态，此时前瞻行应该在十字上方区域内（目前暂无开启，因为搜转折点没写好）
      {
        indistance=DISTANCE;
        crossin_flag=FLAG_ON;
        CROSS_ALL_FLAG=FLAG_ON;

      }
      if((DISTANCE-indistance)>0.3&&crossin_flag==FLAG_ON)    //走过一定距离后强制关闭十字状态机
      {
        crossin_flag=FLAG_OFF;
      //crosspass_flag=FLAG_ON;
         CROSS_ALL_FLAG=FLAG_OFF;
         left_point=63;
         right_point=63;
      }
}

void lost_line_check(){
    LOST_LEFT_FLAG=FLAG_OFF;
    LOST_RIGHT_FLAG=FLAG_OFF;
    if(ALL_WHITE_FLAG==FLAG_OFF&&Search_Stop_Line==5&&DISTANCE>0.1){     //此时前瞻比较长,从而区别于弯道&&DISTANCE>0.1
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

void island_judge(){   //状态开启条件
    int i,j;

/*
    for(i=start_row;i>end_row;i--){
        bin_image[i][120]=3;
    }*/



        if( zebra_left_flag==FLAG_OFF&&zebra_right_flag==FLAG_OFF&&LOST_RIGHT_FLAG==FLAG_ON&&(bin_image[55][116]==255||bin_image[55][120]==255||bin_image[55][124]==255)     //遇到白黑白开启环岛标志
                &&(bin_image[25][116]==0||bin_image[25][120]==0||bin_image[25][124]==0)
                &&(bin_image[5][116]==255||bin_image[5][120]==255||bin_image[5][124]==255)){




            ISLAND_FLAG=FLAG_ON;
            r_island_cheak_flag=FLAG_ON;

    }

    if( r_island_cheak_flag==FLAG_ON){    //目前环岛第二阶段判断有点早


      if((bin_image[47][122]==255&&bin_image[51][122]==0)||(bin_image[47][120]==255&&bin_image[51][120]==0)||(bin_image[47][121]==255&&bin_image[51][121]==0)
                        ||(bin_image[47][124]==255&&bin_image[51][124]==0)||(bin_image[47][125]==255&&bin_image[51][125]==0)
                        ||(bin_image[47][126]==255&&bin_image[51][126]==0)||(bin_image[47][127]==255&&bin_image[51][127]==0)

      /*if((bin_image[45][122]==255&&bin_image[49][122]==0)||(bin_image[45][120]==255&&bin_image[49][120]==0)||(bin_image[45][121]==255&&bin_image[49][121]==0)
       ||(bin_image[45][124]==255&&bin_image[49][124]==0)||(bin_image[45][125]==255&&bin_image[49][125]==0)
       ||(bin_image[45][126]==255&&bin_image[49][126]==0)||(bin_image[45][127]==255&&bin_image[49][127]==0)*/


                ){
                    island_cnt++;
                  }

        if(island_cnt==1){
            right_islandpass_flag=FLAG_ON;    //这里之后开始补线(补进入环岛的线)，直到最长白列小于某个值时停止
        }

    }





}



void island_pass_fsm(){   //环岛状态机
    int i;

    if(right_islandpass_flag==FLAG_ON&&Search_Stop_Line>5&&right_islandout_flag==FLAG_OFF){
        right_islandin_flag=FLAG_ON;
        right_islandpass_flag=FLAG_OFF;
        }



    if(ALL_WHITE_FLAG==FLAG_ON&&right_islandin_flag==FLAG_ON){     //0.8   此时开始出环岛
        right_islandpass_flag=FLAG_ON;  //右补线开启
        right_islandout_flag=FLAG_ON;
        right_islandin_flag=FLAG_OFF;
        outdistance=DISTANCE;


    }

    if(right_islandout_flag==FLAG_ON&&(DISTANCE-outdistance)>0.6&&right_islandpass_flag==FLAG_ON){  //出环岛后走一段距离，再开始补中线//distance2.6
        right_islandpass_flag=FLAG_OFF;
        right_islandout_flag=FLAG_OFF;
        right_islandend_flag=FLAG_ON;
    }


    if(right_islandend_flag==FLAG_ON&&(DISTANCE-outdistance)>1.2){       //补一段中线后,关闭环岛所有标志
        r_island_cheak_flag=FLAG_OFF;
        ISLAND_FLAG=FLAG_OFF;
        right_islandend_flag=FLAG_OFF;
    }





}


void in_stock(){
    if(zebra_right_flag==FLAG_ON){  //右转入库
       in_right_stock=FLAG_ON;
    }
}





void all_judge(){

     out_stock();  //出库

    zebra_cheak();        //斑马线
    all_white_row_judge();   //全白行检测
  //  cross_road_judge();   //检测十字路口
    //crossroad_pass_fsm();  //通过十字路口状态
    lost_line_check();        //左右丢线检测
    island_judge();           //环岛条件开启标志
    island_pass_fsm();     //环岛条件关闭标志

    in_stock();//入库状态

}




