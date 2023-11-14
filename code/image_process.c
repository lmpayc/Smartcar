#include"image_process.h"
 uint8  bin_image[bin_image_H][bin_image_W];


 uint8 start_column = 1;
 uint8 end_column = bin_image_W-1;
 uint8 start_row = 60 ;   //搜线开始行
uint8 end_row =5;      //搜线截止行
uint8 pro_start_row=60; //前瞻开始行
uint8 pro_end_row=54;

uint8 found_num=63;//找到的有效行数，最长白列最高点所在的行数
uint8 found_flag=0; //有效行判断标志


uint8  Left_Line[bin_image_H];    //左右边界
uint8  Right_Line[bin_image_H];
uint8 midline_pre[64];    //计算中线
uint8 Search_Stop_Line;  //搜索截至行

uint8 center_line=63;    //理论中心线为63





 float g_image_err =0 ;
 float g_iamge_last_err=0; //上次误差







void Downsampling(){   //下采样
    for (int i = 0; i < bin_image_H; i++) {
           for (int j = 0; j < bin_image_W; j++) {
               // 计算双线性插值坐标
               float x = (float)(j * (MT9V03X_W - 1)) / (bin_image_W - 1);
               float y = (float)(i * (MT9V03X_H - 1)) / (bin_image_H - 1);

               int x0 = (int)x;
               int y0 = (int)y;
               int x1 = x0 + 1;
               int y1 = y0 + 1;

               float dx = x - x0;
               float dy = y - y0;

               // 执行双线性插值
               uint8 q11 = mt9v03x_image[y0][x0];
               uint8 q12 = mt9v03x_image[y0][x1];
               uint8 q21 = mt9v03x_image[y1][x0];
               uint8 q22 = mt9v03x_image[y1][x1];

               float result = (1 - dx) * (1 - dy) * q11 +
                              dx * (1 - dy) * q12 +
                              (1 - dx) * dy * q21 +
                              dx * dy * q22;

               // 四舍五入并存储结果
               bin_image[i][j] = (uint8)(result + 0.5);
           }
       }
}



int otsu()   //大津法
{
    uint16 i,j;
    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; //类间方差;
    uint16 MinValue, MaxValue;
    int Threshold = 0;
    uint8 HistoGram[256];



    for (j = 0; j < 256; j++)  HistoGram[j] = 0; //初始化灰度直方图

    for (j = 0; j < bin_image_H; j++)
    {
        for (i = 0; i < bin_image_W; i++)
        {
            HistoGram[bin_image[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255;MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)
        {
            return MaxValue;          // 图像中只有一个颜色
        }
        if (MinValue + 1 == MaxValue)
        {
            return MinValue;      // 图像中只有二个颜色
        }


    for (j = MinValue; j < MaxValue; j++)    Amount += HistoGram[j];                   //像素总数

    PixelIntegral = 0;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j; //灰度值总数
    }

    SigmaB = -1;

        for (j = MinValue; j < MaxValue; j++){

            PixelBack = PixelBack + HistoGram[j];                   //前景像素点数
            PixelFore = Amount - PixelBack;                         //背景像素点数



            OmegaBack = (float)PixelBack / Amount;                  //前景像素百分比
            OmegaFore = (float)PixelFore / Amount;                  //背景像素百分比

            PixelIntegralBack += HistoGram[j] * j;                  //前景灰度值
            PixelIntegralFore = PixelIntegral - PixelIntegralBack;  //背景灰度值


            MicroBack = (float)PixelIntegralBack / PixelBack;       //前景灰度百分比
            MicroFore = (float)PixelIntegralFore / PixelFore;       //背景灰度百分比



            Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore); //计算类间方差

            if (Sigma > SigmaB) //找出最大类间方差以及对应的阈值
            {
            SigmaB = Sigma;
            Threshold = j;
            }
        }


        for (i = 0; i < bin_image_H; i++) {
                for (j = 0; j < bin_image_W; j++) {
                    if (bin_image[i][j] <= Threshold) {
                        bin_image[i][j] = 0;
                    } else {
                        bin_image[i][j] = 255;
                    }
                }
            }

    return Threshold;                        //返回最佳阈值;
}



/*void img_bin(uint8 image_binary[bin_image_H][bin_image_W]){
    for (int i = 0; i < bin_image_H; i++) {
          for (int j = 0; j < bin_image_W; j++) {
              if (image_binary[i][j] <= otsu(image_binary)) {
                  bin_image[i][j] = 0;
              } else {
                  bin_image[i][j] = 255;
              }
          }
      }
}*/
/************************************
  * @brief    过滤噪点
  * @param
  * @return
  * @note
  * @date     2022/11/2 Thur
 ************************************/
void Pixle_Filter()
{
    int nr; //行
    int nc; //列

    for(nr=1; nr<bin_image_H-1; nr++)
    {
        for(nc=1; nc<bin_image_W-1; nc=nc+1)
        {
            if((bin_image[nr][nc]==0xff)&&(((bin_image[nr-1][nc]==0xff)&&(bin_image[nr+1][nc]==0xff))||((bin_image[nr][nc+1]==0xff)&&(bin_image[nr][nc-1]==0xff))))
            {
                bin_image[nr][nc]=0xff;
            }
            else if((bin_image[nr][nc]==0x00)&&(((bin_image[nr-1][nc]==0x00)&&(bin_image[nr+1][nc]==0x00))||((bin_image[nr][nc+1]==0x00)&&(bin_image[nr][nc-1]==0x00))))
            {
                bin_image[nr][nc]=0x00;
            }
        }
    }
}



uint8 longest_white(){
    uint8 i,j;
    uint8 White_Column[bin_image_W];
    uint8 Longest_White_Column_l_len;
    uint8 Longest_White_Column_l_j;
    uint8 Longest_White_Column_r_len;
    uint8 Longest_White_Column_r_j;



    // uint8 right_border,left_border;
   //  uint8  Left_Line[bin_image_H];
    // uint8  Right_Line[bin_image_H];
   //  uint8 Search_Stop_Line;//搜索截止行选取左或者右区别不大，他们两个理论上是一样的，最长白列的长度
/*
     uint8 start_column = 5;
     uint8 end_column = bin_image_W-5;
     uint8 start_row = bin_image_H-5;
     uint8 end_row =5;*/

     /*uint8 pro_start_row; //前瞻
     uint8 pro_end_row;*/


     uint8 wet_sum=0;

     int16 error_sum=0;
     //float error=0;

     for(j=0;j<bin_image_W;j++){       //白列长度置零
      White_Column[j]=0;
     }




    for(j=0;j<bin_image_W;j++){   //从图像最低端往上计算每一列长度
        for(i=start_row;i>end_row;i--){
            if(bin_image[i][j]==0){    //遇到黑色就停止
               break;
            }
            else White_Column[j]++;
        }
    }

        //从中间往左边找左边最长白列
          Longest_White_Column_l_len =0;

        for(i=(start_column+end_column)/2;i>start_column;i--)
        {
            if (Longest_White_Column_l_len < White_Column[i])//找最长的那一列
            {
                Longest_White_Column_l_len = White_Column[i];//【0】是白列长度
                Longest_White_Column_l_j = i;              //【1】是下标，第j列
            }
        }


     //从中间往右边 找右最长白列
        Longest_White_Column_r_len = 0;//【0】是白列长度
        for(i=(start_column+end_column)/2;i<end_column;i++)//从右往左，注意条件，找到左边最长白列位置就可以停了
        {

            if (Longest_White_Column_r_len < White_Column[i])//找最长的那一列
            {
                Longest_White_Column_r_len = White_Column[i];//【0】是白列长度
                Longest_White_Column_r_j = i;              //【1】是下标，第j列

            }
        }

       if( Longest_White_Column_l_j==Longest_White_Column_l_len){
            if(((start_column+end_column)/2-Longest_White_Column_l_j)>(Longest_White_Column_r_j-(start_column+end_column)/2)){
                Longest_White_Column_l_j=Longest_White_Column_r_j;
            }
       }

       if(Longest_White_Column_r_len>Longest_White_Column_l_len){    //右边大时，把右边的值赋给左边
           Longest_White_Column_l_len=Longest_White_Column_r_len;
           Longest_White_Column_l_j=Longest_White_Column_r_j;
       }




     /*   for (i = start_row; i >end_row; i--)//搜左右边界线
            {
            //从最下面一行，访问到有效是视野行
            for (j = Longest_White_Column_l_j; j <= bin_image_W-1; j++) //
            {
                if (bin_image[i][j] ==255 && bin_image[i][j + 1] == 0 )//白黑黑，找到右边界
                {
                     right_border = j;

                    break;
                }
                else if(j>=bin_image_W-1)//没找到右边界，把屏幕最右赋值给右边界   //
                {
                    right_border = j;

                    break;
                }
            }


            for (j = Longest_White_Column_l_j; j >= 1 ; j--)//往左边扫描
            {//从最下面一行，访问到我的有效是视野行
                if (bin_image[i][j] ==255 && bin_image[i][j - 1] == 0 )//黑黑白认为到达左边界
                {
                    left_border = j;

                    break;
                }
                else if(j<=1)
                {
                    left_border = j;//找到头都没找到边，就把屏幕最左右当做边界

                    break;
                }
            }
            Left_Line [i] = left_border;       //左边线线数组
            Right_Line[i] = right_border;      //右边线线数组
        }



        for(i=start_row;i>=end_row;i--){
              midline[i]=(Left_Line [i]+Right_Line [i])/2;     //中线

       }



       for(i=start_row;i>end_row;i--){
            for(j=2;j<bin_image_W-3;j++){

                if(j==Left_Line [i]){
                    bin_image[i][j-1]=4;     //左边界


                }


                if(j==midline[i]){
                    bin_image[i][j]=4;        //red中线
                }

                if(j==Right_Line [i]){
                    bin_image[i][j+1]=4;       //red右边界

                    break;
                }
            }
        }
*/


       Search_Stop_Line= 60-Longest_White_Column_l_len;
       if(Search_Stop_Line<0) Search_Stop_Line=0;

       for(i=start_row;i>Search_Stop_Line;i--){
           for(j=0;j<bin_image_W;j++){
               if(j==Longest_White_Column_l_j){
                    bin_image[i][j]=2;       //蓝色最长白列线
                    break;
               }

           }
        }
/*

       pro_end_row=Search_Stop_Line-3;
       pro_start_row=pro_end_row+6;

       if(pro_end_row<25) pro_end_row=25;                 //前瞻行阈值设定
       if(pro_start_row>62) pro_start_row=62;

       wet_sum=0;
       for(i=pro_start_row;i>=pro_end_row;i--){   //计算误差（i为前瞻行）
           bin_image[i][bin_image_W/2]=3;
            error_sum+=(bin_image_W/2-midline[i])*(Right_Line[i]-Left_Line[i]);
            wet_sum+=Right_Line[i]-Left_Line[i];
        }

       for(j=0;j<bin_image_W;j++){   //显示前瞻行
           bin_image[pro_start_row][j]=3;
           bin_image[pro_end_row][j]=3;
       }

       g_image_err=(float)error_sum/(wet_sum);*/

        //tft180_show_rgb565_image(0,bin_image_H/2+2, bin_image[0], bin_image_W,bin_image_H, bin_image_W, bin_image_H,0);
        // tft180_show_float(0,0 ,error,3,1);


       return Longest_White_Column_l_j;



}


void get_edge(){
    int i;
    static int16 j_l;//记录上一次边线位置
    static int16 j_r;


    int temp1=63;
    int temp2=63;

    found_num=63;//重置有效行和标志位
    found_flag=1;




      //从上一次的中点往两边搜,起始行为最近处的一行（第64行）
        for(i = start_row;i >= end_row;i--)
           {
             if(i == start_row)
             {
                for(j_l = center_line; j_l >= 1; j_l--)
                 {

                    if(bin_image[i][j_l]==255&&bin_image[i][j_l-1]==0)  //左边为黑白即为找到
                    {
                        Left_Line[i] = j_l;
                                break;
                    }
                 }
             if(j_l == 0)//没搜到
             {
                 Right_Line[i] = 0;
              }
          //右边线
             for(j_r = center_line; j_r <=126; j_r++)//从上一行的中线开始向两边搜线
              {

               if(bin_image[i][j_r]==255&&bin_image[i][j_r+1]==0)
                {
                   Right_Line[i] = j_r;
                  break;
                }
              }
             if(j_r == 127)//没搜到
              {
                 Right_Line[i] = 127;
               }
             }
             else
             {
               temp1=Left_Line[i+1]+10 ;
               if(temp1>127)
                 temp1=127;
               for(j_l =temp1 ; j_l >= 1; j_l--)
                 {

                    if(bin_image[i][j_l]==255&&bin_image[i][j_l-1]==0)       //左边为依次黑白即为找到
                    {
                        Left_Line[i] = j_l;
                                break;
                    }
                 }
                 if(j_l == 0||abs(Left_Line[i]-Left_Line[i+1]>10))//没搜到
                 {
                   for(j_l = center_line; j_l >= 1; j_l--)
                   {

                     if(bin_image[i][j_l]==255&&bin_image[i][j_l-1]==0)
                     {
                         Left_Line[i] = j_l;
                                break;
                     }
                   }
                   if(j_l == 0)//没搜到
                    {
                       Left_Line[i] = 0;
                    }
                  }
               temp2=Right_Line[i+1]-10;
               if(temp2<0)
                 temp2=0;
               for(j_r = temp2; j_r <=126; j_r++)
                {
                  if(bin_image[i][j_r]==255&&bin_image[i][j_r+1]==0)
                 {
                      Right_Line[i] = j_r;
                  break;
                 }
                 }
              if(j_r == 127||abs(Right_Line[i]-Right_Line[i+1]>10))//没搜到
                {
                   for(j_r = center_line; j_r <=126; j_r++)
                   {

                   if(bin_image[i][j_r]==255&&bin_image[i][j_r+1]==0)
                   {
                       Right_Line[i] = j_r;
                  break;
                   }
                   }
                    if(j_r == 127)//没搜到
                    {
                        Right_Line[i] = 127;
                    }
                }
             }

              //g_road_width[i]=abs((int)(g_right_edge[i]-g_left_edge[i]));  //路宽有关

             if( found_flag==1&&bin_image[i][center_line]==255)
               found_num=i;
             if(bin_image[i][center_line]==0)
             found_flag=0;
             if(found_num<5)
               found_num=5;
           }
            get_center();
}

void get_center(){
    int i,j;
    for(i=start_row;i>=end_row;i--){
        midline_pre[i]=(Left_Line [i]+Right_Line [i])/2;     //中线
           }

    for(i=start_row;i>end_row;i--){
               for(j=2;j<bin_image_W-3;j++){

                   if(j==Left_Line [i]){
                       bin_image[i][j-1]=4;     //左边界


                   }


                   if(j==midline_pre[i]){
                       bin_image[i][j]=4;        //red中线
                   }

                   if(j==Right_Line [i]){
                       bin_image[i][j+1]=4;       //red右边界

                       break;
                   }
               }
           }
}

void caluate_err(){
    int i,j;
    uint8 wet_sum=0;
    int16 error_sum=0;


          pro_end_row=Search_Stop_Line+2;       //前瞻结束选在截止行下面一些
          if(pro_end_row<25) pro_end_row=25;                 //前瞻行阈值设定
          pro_start_row=pro_end_row+6;
          if(pro_start_row>60) pro_start_row=60;


          for(i=pro_start_row;i>=pro_end_row;i--){   //计算误差（i为前瞻行）
             // bin_image[i][bin_image_W/2]=3;
               error_sum+=(63-midline_pre[i])*(Right_Line[i]-Left_Line[i]);
               wet_sum+=Right_Line[i]-Left_Line[i];
           }

           g_image_err=(float)error_sum/(wet_sum);
          // tft180_show_float(0,0 ,g_image_err,3,1);

          for(j=0;j<bin_image_W;j++){   //显示前瞻行
              bin_image[pro_start_row][j]=3;
              bin_image[pro_end_row][j]=3;
          }


}



