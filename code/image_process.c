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
uint8 g_midline[64];

uint8 g_road_width[64]={0};//路宽
uint8 g_last_road_width[64]={0};//上次路宽

uint8 Search_Stop_Line;  //搜索截至行,最长白列的最高点所在行数

uint8 center_line=63;    //理论中心线为63


uint8 left_point=63;    //十字转折点
uint8 right_point=63;

float left_Correlation=0.0;   //左右边界相关系数
float right_Correlation=0.0;


 float g_image_err =0 ;    //当前误差
 float g_iamge_last_err=0; //上次误差
 float g_lastimg_err[5]={0};  //记录5次误差，4为当前误差，误差也要做限幅



 const int row_road_width[64]=
 {
  21, 22, 23, 25, 25, 27, 27, 28, 29, 31, 31, 32, 34, 34,
  36, 37, 38, 39, 40, 42, 42, 44, 45, 45, 47, 49, 49, 51,
  51, 51, 53, 54, 55, 56, 57, 57, 59, 61, 61, 61, 63, 65,
  65, 67, 69, 69, 70, 72, 72, 74, 75, 76, 78, 78, 79, 80,
  82, 83, 83, 85, 87, 87, 89, 89
 };




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

/*
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



*/
    Threshold=data[15];

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



/************************************
  * @brief    过滤噪点
  * @param
  * @return
  * @note
  * @date     2023/11/2 Thur
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

       if( Longest_White_Column_r_len==Longest_White_Column_l_len){
            if(((start_column+end_column)/2-Longest_White_Column_l_j)>(Longest_White_Column_r_j-(start_column+end_column)/2)){
                Longest_White_Column_l_j=Longest_White_Column_r_j;
            }
       }

       if(Longest_White_Column_r_len>Longest_White_Column_l_len){    //右边大时，把右边的值赋给左边
           Longest_White_Column_l_len=Longest_White_Column_r_len;
           Longest_White_Column_l_j=Longest_White_Column_r_j;
       }







       Search_Stop_Line= 60-Longest_White_Column_l_len;
       if(Search_Stop_Line<=5) Search_Stop_Line=5;

       for(i=start_row;i>Search_Stop_Line;i--){
           for(j=0;j<bin_image_W;j++){
               if(j==Longest_White_Column_l_j){
                    bin_image[i][j]=2;       //蓝色最长白列线
                    break;
               }

           }
        }


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
      for(i =end_row;i <= start_row;i++){
             if(i == end_row)
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
               temp1=Left_Line[i-1]+15 ;
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
               temp2=Right_Line[i-1]-15;
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



              g_last_road_width[i]=g_road_width[i];
              g_road_width[i]=abs((int)(Right_Line[i]-Left_Line[i]));   //更新路宽

             if( found_flag==1&&bin_image[i][center_line]==255)
               found_num=i;                    //就是在找最长白列的最高点
             if(bin_image[i][center_line]==0)
             found_flag=0;      //
             if(found_num<5)
               found_num=5;
       }

              Correlation_get();     //左右边界相关系数判定

              jump_point();
             // crossroad_pass();      //十字补线
               // island_pass();
              get_center();   //计算得到的图像中线

}

void get_center(){
    int i,j;
    for(i=start_row;i>=end_row;i--){
        midline_pre[i]=(Left_Line [i]+Right_Line [i])/2;     //中线
 }

  centerline_change();//补中线

 //MedianFilter( midline_pre,g_midline,3);  //中线滤波
  for(i=start_row;i>=end_row;i--){
      g_midline[i]=midline_pre[i];     //中线
   }







    //屏幕显示
    for(i=start_row;i>end_row;i--){
               for(j=2;j<bin_image_W-3;j++){
                   /*if (j == Left_Line[i]) {
                       bin_image[i][j-1]=4;     //左边界
                   }*/
                   if(j==midline_pre[i]){
                       bin_image[i][j]=4;        //red中线
                   }
                   /*if (j == Right_Line[i]) {
                       bin_image[i][j+1]=4;       //red右边界
                       break;
                   }*/
               }
           }
}

void centerline_change()//补中线
{
    int i;
    if(r_island_cheak_flag == FLAG_ON ==FLAG_ON&&in_left_stock==FLAG_OFF&&in_right_stock==FLAG_OFF&& right_islandpass_flag==FLAG_OFF)
      {
         for(i = start_row;i >= end_row;i--)
        {
          //if(g_road_width[i]>row_road_width[i]+10){
              midline_pre[i]=row_road_width[i]/2+Left_Line[i]+3;
         // }
         }
      }

    if(LOST_LEFT_FLAG==FLAG_ON&&in_left_stock==FLAG_OFF&&in_right_stock==FLAG_OFF)
         {
            for(i = start_row;i >= end_row;i--)
           {
             //if(g_road_width[i]>row_road_width[i]+10){
                 midline_pre[i]=Right_Line[i]-(row_road_width[i]/2+3);
            // }
            }
         }

    if (right_islandpass_flag == FLAG_ON && in_left_stock == FLAG_OFF && in_right_stock == FLAG_OFF)
    {
        for (i = start_row; i >= end_row; i--)
        {
            //if(g_road_width[i]>row_road_width[i]+10){
            midline_pre[i] = Right_Line[i] - (row_road_width[i] / 2 + 6);
            // }
        }
    }




}


unsigned char GetMedianNum(uint8 * bArray, uint8 iFilterLen)//获取中值
{
    uint8 i,j;// 循环变量
    uint8 bTemp;

    // 用冒泡法对数组进行排序
    for (j = 0; j < iFilterLen - 1; j ++)
    {
        for (i = 0; i < iFilterLen - j - 1; i ++)
        {
            if (bArray[i] > bArray[i + 1])
            {
                // 互换
                bTemp = bArray[i];
                bArray[i] = bArray[i + 1];
                bArray[i + 1] = bTemp;
            }
        }
    }

    // 计算中值
    if ((iFilterLen & 1) > 0)
    {
        // 数组有奇数个元素，返回中间一个元素
        bTemp = bArray[(iFilterLen + 1) / 2];
    }
    else
    {
        // 数组有偶数个元素，返回中间两个元素平均值
        bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
    }
    return bTemp;
}

void MedianFilter( uint8*line_pre,uint8 *line_now,uint8 Width)//中值滤波
{
   uint8  *lpSrc;                           // 指向源中线的指针
   uint8  *lpDst;                           // 指向要复制区域的指针
   uint8 i;
   for(i =end_row;i<=start_row; i++)
  {
   lpSrc=line_pre+i;
   lpDst=line_now+i;
    *lpDst=*lpSrc;
  }
  for(i = end_row+Width;i<start_row-Width; i+=Width)
  {
     lpSrc=line_pre+i-Width/2;
     lpDst=line_now+i;
     *lpDst=GetMedianNum(lpSrc, Width);
  }
}







void caluate_err(){
    int i,j;
    int wet_sum=1;
    int error_sum=0;
    int end_row_max;
    end_row_max=data[14];

    /*前瞻行设定目前是7行,end_row_max=36下基本稳定，建议明天15行左右或者继续压低前瞻行*/

                 //前瞻行阈值设定



    if(Search_Stop_Line<=end_row_max){ //前瞻结束选在截止行下面
        pro_end_row=end_row_max;
        pro_start_row=pro_end_row+15;
    }



    else{
        pro_end_row=Search_Stop_Line;
        pro_start_row=pro_end_row+15;
        if(pro_start_row>62)
            {
            pro_start_row=62;
            pro_end_row=pro_start_row-15;
            }

    }


    if(pro_start_row>63) pro_start_row=63;


    if(crossin_flag==FLAG_ON&&left_point<25&&right_point<25){              //遇到十字就把前瞻行刚在十字上面
        if(pro_start_row>left_point)  pro_start_row=left_point;
        pro_end_row=pro_start_row-7;
    }




    for(i=pro_start_row;i>=pro_end_row;i--){   //计算误差（i为前瞻行）

          error_sum+=(63-midline_pre[i])*(Right_Line[i]-Left_Line[i]);  //在这里修改中线滤波后的数组(Right_Line[i]-Left_Line[i])
           wet_sum+=(Right_Line[i]-Left_Line[i]);//               //
       }

      g_image_err=(float)((float)(error_sum)/(float)(wet_sum));


      /*更新误差*/
       g_lastimg_err[0]=g_lastimg_err[1];
       g_lastimg_err[1]=g_lastimg_err[2];
       g_lastimg_err[2]=g_lastimg_err[3];
       g_lastimg_err[3]=g_lastimg_err[4];
       g_lastimg_err[4]=g_image_err;

       g_image_err=(g_lastimg_err[4]+g_lastimg_err[3]+g_lastimg_err[2]+g_lastimg_err[1])/4.0;


      /*误差限幅*/



     for(j=0;j<bin_image_W;j++){   //显示前瞻行
              bin_image[pro_start_row][j]=3;
              bin_image[pro_end_row][j]=3;
          }
}

float limit(float x,float min,float max){
    if(x<min) x=min;
    if(x>max) x=max;
    return x;
}

void crossroad_pass()//十字补线(位于搜边界的最后，对边界重新处理)
{
    uint8 i;
    uint8 leftnear_row = 63;      //第一个左转折点(靠近车的点)
    uint8 leftfar_row = 63;       //第二个左转折点
    uint8 leftfar_flag =  FLAG_OFF;     //两个转折点是否都找到标志
    uint8 rightfar_flag = FLAG_OFF;
    uint8 leftnear_flag = FLAG_OFF;    //第二个转折点开启标志
    uint8 rightnear_flag = FLAG_OFF;
    uint8 rightnear_row = 63;
    uint8 rightfar_row = 63;


   //crossin_flag=FLAG_ON;
    if(crossin_flag==FLAG_ON){
    //左边界搜折点
    for(i = start_row; i >= Search_Stop_Line+3 ; i--){
        if(Left_Line[i] - Left_Line[i-3] > 2)//找到第一个左折点
        {
            leftnear_flag = FLAG_ON;
            leftnear_row = i;
            left_point = i;
            break;
        }
    }

    if(leftnear_flag == FLAG_ON){
       //找到第二个左折点
        if(leftnear_row<=(4+Search_Stop_Line))
          leftnear_row=(4+Search_Stop_Line);
        for(i = leftnear_row - 4; i >=Search_Stop_Line; i--)
        {
            if(Left_Line[i+3] - Left_Line[i] < -20)
            {
                leftfar_flag = FLAG_ON;
                leftfar_row = i ;
                break;
            }
        }

        if(leftfar_flag == FLAG_ON)//如果前后的折点都找到，进行连线
        {
            for(i = leftnear_row; i >= leftfar_row; i--)
            {
                Left_Line[i] = (leftnear_row - i)*(Left_Line[leftfar_row] - Left_Line[leftnear_row])/(leftnear_row - leftfar_row)+Left_Line[leftnear_row];
                if(Left_Line[i]<=0)
                    Left_Line[i]=0;
                 if(Left_Line[i]>=127)
                     Left_Line[i]=127;
            }
        }

   /*    else//只找到下方的左折点，强行补线
        {
            for(i = leftnear_row; i >= Search_Stop_Line; i--)
            {
                Left_Line[i] = (leftnear_row - i)*(Left_Line[leftnear_row] - Left_Line[63])/(63 - leftnear_row)+Left_Line[leftnear_row];
                 if(Left_Line[i]<=0)
                     Left_Line[i]=0;
                 if(Left_Line[i]>=127)
                     Left_Line[i]=127;
            }
        }*/
    }



    //右边界搜折点
    for(i = start_row; i >= Search_Stop_Line+3; i--)
    {
        if(Right_Line[i] - Right_Line[i-3] < -2)//找到第一个右折点
        {
            rightnear_flag = FLAG_ON;
            rightnear_row = i;
            break;
        }
    }


    if(rightnear_flag == FLAG_ON)
    {
        //找到第二个右折点
      if( rightnear_row<= (4+Search_Stop_Line))
         rightnear_row=(4+Search_Stop_Line);
        for(i = rightnear_row - 4; i >= Search_Stop_Line; i--)
        {
            if(Right_Line[i+3] - Right_Line[i] > 20)
            {
                rightfar_flag = FLAG_ON;
                rightfar_row = i ;
                break;
            }
        }
        if(rightfar_flag == FLAG_ON)//如果前后的折点都找到，进行连线
        {
            for(i = rightnear_row; i >= rightfar_row; i--)
            {
                Right_Line[i] = Right_Line[rightnear_row] - (rightnear_row - i)*(Right_Line[rightnear_row] - Right_Line[rightfar_row])/(rightnear_row - rightfar_row);
                if(Right_Line[i]<=0)
                    Right_Line[i]=0;
                 if(Right_Line[i]>=127)
                     Right_Line[i]=127;
            }
        }

    /*  else//只找到下方的右折点，强行补线
        {
            for(i = rightnear_row; i >= Search_Stop_Line; i--)
            {
                Right_Line[i] = Right_Line[rightnear_row] - (rightnear_row - i)*(Right_Line[start_row] - Right_Line[rightnear_row])/(start_row - rightnear_row);
                 if(Right_Line[i]<=0)
                     Right_Line[i]=0;
                 if(Right_Line[i]>=127)
                     Right_Line[i]=127;
            }
        }*/
    }
}
}




void Correlation_get()//左右边界相关系数判定
{
  int i=0;
  float sumx=0.0;
  float sumy_l=0.0;
  float sumy_r=0.0;
  float avex=0.0;
  float avey_l=0.0;
  float avey_r=0.0;
  float R1=0.0;
  float R2=0.0;
  float R3=0.0;
  float R4=0.0;
  float R5=0.0;
  for(i = start_row; i >=end_row  ; i--)
  {
    sumx+=i*1.0;
    sumy_l+=(127.0-Left_Line[i]*1.0);
    sumy_r+=Right_Line[i]*1.0;
  }
  avex=sumx/(float)(start_row-end_row +1);
  avey_l=sumy_l/(float)(start_row-end_row +1);
  avey_r=sumy_r/(float)(start_row-end_row +1);
  for(i = start_row; i >= end_row ;i--)
  {
    R1+=(i*1.0-avex)*(avey_l-(127.0-Left_Line[i]*1.0));
    R2+=(i*1.0-avex)*(Right_Line[i]*1.0-avey_r);
    R3+=pow((i*1.0-avex),2);
    R4+=pow(((127.0-Left_Line[i]*1.0)-avey_l),2);
    R5+=pow((Right_Line[i]*1.0-avey_r),2);
  }
  R3+=1.0;
  R4+=1.0;
  R5+=1.0;
  left_Correlation=R1/sqrtf(R3*R4);
  right_Correlation=R2/sqrtf(R3*R5);


}


void jump_point(){
    int i;


    if(crossin_flag==FLAG_ON||CROSSROAD_FLAG==FLAG_ON){
        //左边界搜折点
        for(i = pro_start_row; i >= end_row+5 ; i--){
            if(Left_Line[i] - Left_Line[i+2] > 3)//找到第一个左折点
            {

                left_point = i;
                break;
            }
        }



        for(i = pro_start_row; i >= end_row+5; i--)
           {
               if(Right_Line[i] - Right_Line[i+2] < -2)//找到第一个右折点
               {
                   right_point = i;
                   break;
               }
           }


    }


}



