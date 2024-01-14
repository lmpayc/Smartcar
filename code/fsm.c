#include "fsm.h"

uint8 CROSSROAD_FLAG=FLAG_OFF;//ʮ�ּ����
//uint8 FORK_FLAG=FLAG_OFF;//��ڼ����
uint8 ALL_WHITE_FLAG=FLAG_OFF;   //ȫ���б�־
uint8 LOST_LEFT_FLAG=FLAG_OFF;      //���Ҷ��߱�־
uint8 LOST_RIGHT_FLAG=FLAG_OFF;
uint8 APRILTAG_FLAG=FLAG_OFF;
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
uint8 island_cnt=0;
float outdistance;

float ANGLE=0.0;
int16 imu660ra_gyro_z_temp[10]={0};

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
uint8 ZEBRA_FLAG=FLAG_OFF;     //�����߱�־



void get_angle()//�Ƕȼ��
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





void all_white_row_judge(void){     //�ж��Ƿ����ȫ����
    int i,j;
    int cnt=0;  //��¼ȫ��������
    ALL_WHITE_FLAG=FLAG_OFF;
    for(i=45;i>=end_row;i--){   //�ӵ�45��������
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


void out_stock(){       //����״̬
    static int flag=0;
    int out_dir=data[13];   //���ⷽ��

    if(out_dir==0&&flag==0)//��ת
     {
        OUT_STOCK_FLAG=FLAG_ON;
        out_left_stock=FLAG_ON;

     }
     if(out_dir==1&&flag==0)//��ת
     {
        OUT_STOCK_FLAG=FLAG_ON;
        out_right_stock=FLAG_ON;


     }
     if(out_dir==2&&flag==0)//����
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

      int row_zebra=0;//���а������������У����������ĳ��ֵ�����֡���а�����
      int black_counter=0;      //ÿһ�кڰ���������
      int black_counter_max,black_counter_min;  /*black_counter�����������Χ�ڣ�������������*/

      black_counter_min=5;
      black_counter_max=8;

      for(i=start_row;i>=end_row;i--)
       {
          black_counter=0;
        for(j=3;j<120;j++){  //���������Һڰ������
            if((bin_image[i][j]==0&&bin_image[i][j+1]==255)||(bin_image[i][j]==0&&bin_image[i][j+1]==255)){
                 black_counter++;
                 if( black_counter> black_counter_min&&black_counter<black_counter_max){   //������������У���׷�Ӿ������ж�
                     row_zebra++;
                 }
            }
        }
       }

      for(i=pro_start_row-1;i>=pro_end_row;i--)           //ͼ�������ı��ǰ�ɫ
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

            if(num1>=6&&data[13]==0)  //��ת���
            {

              zebra_left_flag=FLAG_ON;
              zebra_right_flag=FLAG_OFF;
              ZEBRA_FLAG=FLAG_ON;
            }
             if(num2>=6&&data[13]==1)   //��ת���
            {

              zebra_right_flag=FLAG_ON;
              zebra_left_flag=FLAG_OFF;
              ZEBRA_FLAG=FLAG_ON;

            }
          }
        }
}









void cross_road_judge(){
    CROSSROAD_FLAG=FLAG_OFF;//ʮ�ּ����
    RAMP_FLAG=FLAG_OFF;   //�µ����

    if( ALL_WHITE_FLAG==FLAG_ON && ISLAND_FLAG==FLAG_OFF && in_left_stock==FLAG_OFF&&in_right_stock==FLAG_OFF&& ramp_pass_flag==FLAG_OFF&&ZEBRA_FLAG==FLAG_OFF){
        CROSSROAD_FLAG=FLAG_ON;
    }
}


/*����һ���뷨����ʮ�ֲ���״̬ʱ��ǰհ���ʵ���һЩ,Ѹ��ͨ��*/
void crossroad_pass_fsm(){    //ͨ��ʮ��״̬��
    static float indistance=0.0;   //��¼ʮ�־���
    static float enddistance=0.0;

    if(CROSSROAD_FLAG==FLAG_ON&&crossin_flag==FLAG_OFF&&left_point<25&&right_point<25)  //��ʮ��״̬����ʱǰհ��Ӧ����ʮ���Ϸ������ڣ�Ŀǰ���޿�������Ϊ��ת�۵�ûд�ã�
      {
        indistance=DISTANCE;
        crossin_flag=FLAG_ON;
        CROSS_ALL_FLAG=FLAG_ON;

      }
      if((DISTANCE-indistance)>0.3&&crossin_flag==FLAG_ON)    //�߹�һ�������ǿ�ƹر�ʮ��״̬��
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
    if(ALL_WHITE_FLAG==FLAG_OFF&&Search_Stop_Line==5&&DISTANCE>0.1){     //��ʱǰհ�Ƚϳ�,�Ӷ����������&&DISTANCE>0.1
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

void island_judge(){   //״̬��������
    int i,j;

/*
    for(i=start_row;i>end_row;i--){
        bin_image[i][120]=3;
    }*/



        if( zebra_left_flag==FLAG_OFF&&zebra_right_flag==FLAG_OFF&&LOST_RIGHT_FLAG==FLAG_ON&&(bin_image[55][116]==255||bin_image[55][120]==255||bin_image[55][124]==255)     //�����׺ڰ׿���������־
                &&(bin_image[25][116]==0||bin_image[25][120]==0||bin_image[25][124]==0)
                &&(bin_image[5][116]==255||bin_image[5][120]==255||bin_image[5][124]==255)){




            ISLAND_FLAG=FLAG_ON;
            r_island_cheak_flag=FLAG_ON;

    }

    if( r_island_cheak_flag==FLAG_ON){    //Ŀǰ�����ڶ��׶��ж��е���


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
            right_islandpass_flag=FLAG_ON;    //����֮��ʼ����(�����뻷������)��ֱ�������С��ĳ��ֵʱֹͣ
        }

    }





}



void island_pass_fsm(){   //����״̬��
    int i;

    if(right_islandpass_flag==FLAG_ON&&Search_Stop_Line>5&&right_islandout_flag==FLAG_OFF){
        right_islandin_flag=FLAG_ON;
        right_islandpass_flag=FLAG_OFF;
        }



    if(ALL_WHITE_FLAG==FLAG_ON&&right_islandin_flag==FLAG_ON){     //0.8   ��ʱ��ʼ������
        right_islandpass_flag=FLAG_ON;  //�Ҳ��߿���
        right_islandout_flag=FLAG_ON;
        right_islandin_flag=FLAG_OFF;
        outdistance=DISTANCE;


    }

    if(right_islandout_flag==FLAG_ON&&(DISTANCE-outdistance)>0.6&&right_islandpass_flag==FLAG_ON){  //����������һ�ξ��룬�ٿ�ʼ������//distance2.6
        right_islandpass_flag=FLAG_OFF;
        right_islandout_flag=FLAG_OFF;
        right_islandend_flag=FLAG_ON;
    }


    if(right_islandend_flag==FLAG_ON&&(DISTANCE-outdistance)>1.2){       //��һ�����ߺ�,�رջ������б�־
        r_island_cheak_flag=FLAG_OFF;
        ISLAND_FLAG=FLAG_OFF;
        right_islandend_flag=FLAG_OFF;
    }





}


void in_stock(){
    if(zebra_right_flag==FLAG_ON){  //��ת���
       in_right_stock=FLAG_ON;
    }
}





void all_judge(){

     out_stock();  //����

    zebra_cheak();        //������
    all_white_row_judge();   //ȫ���м��
  //  cross_road_judge();   //���ʮ��·��
    //crossroad_pass_fsm();  //ͨ��ʮ��·��״̬
    lost_line_check();        //���Ҷ��߼��
    island_judge();           //��������������־
    island_pass_fsm();     //���������رձ�־

    in_stock();//���״̬

}




