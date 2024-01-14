#include"image_process.h"
 uint8  bin_image[bin_image_H][bin_image_W];


 uint8 start_column = 1;
 uint8 end_column = bin_image_W-1;
 uint8 start_row = 60 ;   //���߿�ʼ��
uint8 end_row =5;      //���߽�ֹ��
uint8 pro_start_row=60; //ǰհ��ʼ��
uint8 pro_end_row=54;

uint8 found_num=63;//�ҵ�����Ч�������������ߵ����ڵ�����
uint8 found_flag=0; //��Ч���жϱ�־


uint8  Left_Line[bin_image_H];    //���ұ߽�
uint8  Right_Line[bin_image_H];
uint8 midline_pre[64];    //��������
uint8 g_midline[64];

uint8 g_road_width[64]={0};//·��
uint8 g_last_road_width[64]={0};//�ϴ�·��

uint8 Search_Stop_Line;  //����������,����е���ߵ���������

uint8 center_line=63;    //����������Ϊ63


uint8 left_point=63;    //ʮ��ת�۵�
uint8 right_point=63;

float left_Correlation=0.0;   //���ұ߽����ϵ��
float right_Correlation=0.0;


 float g_image_err =0 ;    //��ǰ���
 float g_iamge_last_err=0; //�ϴ����
 float g_lastimg_err[5]={0};  //��¼5����4Ϊ��ǰ�����ҲҪ���޷�



 const int row_road_width[64]=
 {
  21, 22, 23, 25, 25, 27, 27, 28, 29, 31, 31, 32, 34, 34,
  36, 37, 38, 39, 40, 42, 42, 44, 45, 45, 47, 49, 49, 51,
  51, 51, 53, 54, 55, 56, 57, 57, 59, 61, 61, 61, 63, 65,
  65, 67, 69, 69, 70, 72, 72, 74, 75, 76, 78, 78, 79, 80,
  82, 83, 83, 85, 87, 87, 89, 89
 };




void Downsampling(){   //�²���
    for (int i = 0; i < bin_image_H; i++) {
           for (int j = 0; j < bin_image_W; j++) {
               // ����˫���Բ�ֵ����
               float x = (float)(j * (MT9V03X_W - 1)) / (bin_image_W - 1);
               float y = (float)(i * (MT9V03X_H - 1)) / (bin_image_H - 1);

               int x0 = (int)x;
               int y0 = (int)y;
               int x1 = x0 + 1;
               int y1 = y0 + 1;

               float dx = x - x0;
               float dy = y - y0;

               // ִ��˫���Բ�ֵ
               uint8 q11 = mt9v03x_image[y0][x0];
               uint8 q12 = mt9v03x_image[y0][x1];
               uint8 q21 = mt9v03x_image[y1][x0];
               uint8 q22 = mt9v03x_image[y1][x1];

               float result = (1 - dx) * (1 - dy) * q11 +
                              dx * (1 - dy) * q12 +
                              (1 - dx) * dy * q21 +
                              dx * dy * q22;

               // �������벢�洢���
               bin_image[i][j] = (uint8)(result + 0.5);
           }
       }
}



int otsu()   //���
{
    uint16 i,j;
    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; //��䷽��;
    uint16 MinValue, MaxValue;
    int Threshold = 0;
    uint8 HistoGram[256];

/*
    for (j = 0; j < 256; j++)  HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ

    for (j = 0; j < bin_image_H; j++)
    {
        for (i = 0; i < bin_image_W; i++)
        {
            HistoGram[bin_image[j][i]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
    for (MaxValue = 255;MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ

    if (MaxValue == MinValue)
        {
            return MaxValue;          // ͼ����ֻ��һ����ɫ
        }
        if (MinValue + 1 == MaxValue)
        {
            return MinValue;      // ͼ����ֻ�ж�����ɫ
        }


    for (j = MinValue; j < MaxValue; j++)    Amount += HistoGram[j];                   //��������

    PixelIntegral = 0;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j; //�Ҷ�ֵ����
    }

    SigmaB = -1;

        for (j = MinValue; j < MaxValue; j++){

            PixelBack = PixelBack + HistoGram[j];                   //ǰ�����ص���
            PixelFore = Amount - PixelBack;                         //�������ص���



            OmegaBack = (float)PixelBack / Amount;                  //ǰ�����ذٷֱ�
            OmegaFore = (float)PixelFore / Amount;                  //�������ذٷֱ�

            PixelIntegralBack += HistoGram[j] * j;                  //ǰ���Ҷ�ֵ
            PixelIntegralFore = PixelIntegral - PixelIntegralBack;  //�����Ҷ�ֵ


            MicroBack = (float)PixelIntegralBack / PixelBack;       //ǰ���ҶȰٷֱ�
            MicroFore = (float)PixelIntegralFore / PixelFore;       //�����ҶȰٷֱ�



            Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore); //������䷽��

            if (Sigma > SigmaB) //�ҳ������䷽���Լ���Ӧ����ֵ
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

    return Threshold;                        //���������ֵ;
}



/************************************
  * @brief    �������
  * @param
  * @return
  * @note
  * @date     2023/11/2 Thur
 ************************************/
void Pixle_Filter()
{
    int nr; //��
    int nc; //��

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
   //  uint8 Search_Stop_Line;//������ֹ��ѡȡ����������𲻴�����������������һ���ģ�����еĳ���
/*
     uint8 start_column = 5;
     uint8 end_column = bin_image_W-5;
     uint8 start_row = bin_image_H-5;
     uint8 end_row =5;*/

     /*uint8 pro_start_row; //ǰհ
     uint8 pro_end_row;*/




     for(j=0;j<bin_image_W;j++){       //���г�������
      White_Column[j]=0;
     }




    for(j=0;j<bin_image_W;j++){   //��ͼ����Ͷ����ϼ���ÿһ�г���
        for(i=start_row;i>end_row;i--){
            if(bin_image[i][j]==0){    //������ɫ��ֹͣ
               break;
            }
            else White_Column[j]++;
        }
    }

        //���м����������������
          Longest_White_Column_l_len =0;

        for(i=(start_column+end_column)/2;i>start_column;i--)
        {
            if (Longest_White_Column_l_len < White_Column[i])//�������һ��
            {
                Longest_White_Column_l_len = White_Column[i];//��0���ǰ��г���
                Longest_White_Column_l_j = i;              //��1�����±꣬��j��
            }
        }


     //���м����ұ� ���������
        Longest_White_Column_r_len = 0;//��0���ǰ��г���
        for(i=(start_column+end_column)/2;i<end_column;i++)//��������ע���������ҵ���������λ�þͿ���ͣ��
        {

            if (Longest_White_Column_r_len < White_Column[i])//�������һ��
            {
                Longest_White_Column_r_len = White_Column[i];//��0���ǰ��г���
                Longest_White_Column_r_j = i;              //��1�����±꣬��j��

            }
        }

       if( Longest_White_Column_r_len==Longest_White_Column_l_len){
            if(((start_column+end_column)/2-Longest_White_Column_l_j)>(Longest_White_Column_r_j-(start_column+end_column)/2)){
                Longest_White_Column_l_j=Longest_White_Column_r_j;
            }
       }

       if(Longest_White_Column_r_len>Longest_White_Column_l_len){    //�ұߴ�ʱ�����ұߵ�ֵ�������
           Longest_White_Column_l_len=Longest_White_Column_r_len;
           Longest_White_Column_l_j=Longest_White_Column_r_j;
       }







       Search_Stop_Line= 60-Longest_White_Column_l_len;
       if(Search_Stop_Line<=5) Search_Stop_Line=5;

       for(i=start_row;i>Search_Stop_Line;i--){
           for(j=0;j<bin_image_W;j++){
               if(j==Longest_White_Column_l_j){
                    bin_image[i][j]=2;       //��ɫ�������
                    break;
               }

           }
        }


       return Longest_White_Column_l_j;



}


void get_edge(){
    int i;
    static int16 j_l;//��¼��һ�α���λ��
    static int16 j_r;


    int temp1=63;
    int temp2=63;

    found_num=63;//������Ч�кͱ�־λ
    found_flag=1;




      //����һ�ε��е���������,��ʼ��Ϊ�������һ�У���64�У�
      for(i =end_row;i <= start_row;i++){
             if(i == end_row)
             {
                for(j_l = center_line; j_l >= 1; j_l--)
                 {

                    if(bin_image[i][j_l]==255&&bin_image[i][j_l-1]==0)  //���Ϊ�ڰ׼�Ϊ�ҵ�
                    {
                        Left_Line[i] = j_l;
                                break;
                    }
                 }
             if(j_l == 0)//û�ѵ�
             {
                 Right_Line[i] = 0;
              }
          //�ұ���
             for(j_r = center_line; j_r <=126; j_r++)//����һ�е����߿�ʼ����������
              {

               if(bin_image[i][j_r]==255&&bin_image[i][j_r+1]==0)
                {
                   Right_Line[i] = j_r;
                  break;
                }
              }
             if(j_r == 127)//û�ѵ�
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

                    if(bin_image[i][j_l]==255&&bin_image[i][j_l-1]==0)       //���Ϊ���κڰ׼�Ϊ�ҵ�
                    {
                        Left_Line[i] = j_l;
                                break;
                    }
                 }
                 if(j_l == 0||abs(Left_Line[i]-Left_Line[i+1]>10))//û�ѵ�
                 {
                   for(j_l = center_line; j_l >= 1; j_l--)
                   {

                     if(bin_image[i][j_l]==255&&bin_image[i][j_l-1]==0)
                     {
                         Left_Line[i] = j_l;
                                break;
                     }
                   }
                   if(j_l == 0)//û�ѵ�
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
              if(j_r == 127||abs(Right_Line[i]-Right_Line[i+1]>10))//û�ѵ�
                {
                   for(j_r = center_line; j_r <=126; j_r++)
                   {

                   if(bin_image[i][j_r]==255&&bin_image[i][j_r+1]==0)
                   {
                       Right_Line[i] = j_r;
                  break;
                   }
                   }
                    if(j_r == 127)//û�ѵ�
                    {
                        Right_Line[i] = 127;
                    }
                }
             }



              g_last_road_width[i]=g_road_width[i];
              g_road_width[i]=abs((int)(Right_Line[i]-Left_Line[i]));   //����·��

             if( found_flag==1&&bin_image[i][center_line]==255)
               found_num=i;                    //������������е���ߵ�
             if(bin_image[i][center_line]==0)
             found_flag=0;      //
             if(found_num<5)
               found_num=5;
       }

              Correlation_get();     //���ұ߽����ϵ���ж�

              jump_point();
             // crossroad_pass();      //ʮ�ֲ���
               // island_pass();
              get_center();   //����õ���ͼ������

}

void get_center(){
    int i,j;
    for(i=start_row;i>=end_row;i--){
        midline_pre[i]=(Left_Line [i]+Right_Line [i])/2;     //����
 }

  centerline_change();//������

 //MedianFilter( midline_pre,g_midline,3);  //�����˲�
  for(i=start_row;i>=end_row;i--){
      g_midline[i]=midline_pre[i];     //����
   }







    //��Ļ��ʾ
    for(i=start_row;i>end_row;i--){
               for(j=2;j<bin_image_W-3;j++){
                   /*if (j == Left_Line[i]) {
                       bin_image[i][j-1]=4;     //��߽�
                   }*/
                   if(j==midline_pre[i]){
                       bin_image[i][j]=4;        //red����
                   }
                   /*if (j == Right_Line[i]) {
                       bin_image[i][j+1]=4;       //red�ұ߽�
                       break;
                   }*/
               }
           }
}

void centerline_change()//������
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


unsigned char GetMedianNum(uint8 * bArray, uint8 iFilterLen)//��ȡ��ֵ
{
    uint8 i,j;// ѭ������
    uint8 bTemp;

    // ��ð�ݷ��������������
    for (j = 0; j < iFilterLen - 1; j ++)
    {
        for (i = 0; i < iFilterLen - j - 1; i ++)
        {
            if (bArray[i] > bArray[i + 1])
            {
                // ����
                bTemp = bArray[i];
                bArray[i] = bArray[i + 1];
                bArray[i + 1] = bTemp;
            }
        }
    }

    // ������ֵ
    if ((iFilterLen & 1) > 0)
    {
        // ������������Ԫ�أ������м�һ��Ԫ��
        bTemp = bArray[(iFilterLen + 1) / 2];
    }
    else
    {
        // ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
        bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
    }
    return bTemp;
}

void MedianFilter( uint8*line_pre,uint8 *line_now,uint8 Width)//��ֵ�˲�
{
   uint8  *lpSrc;                           // ָ��Դ���ߵ�ָ��
   uint8  *lpDst;                           // ָ��Ҫ���������ָ��
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

    /*ǰհ���趨Ŀǰ��7��,end_row_max=36�»����ȶ�����������15�����һ��߼���ѹ��ǰհ��*/

                 //ǰհ����ֵ�趨



    if(Search_Stop_Line<=end_row_max){ //ǰհ����ѡ�ڽ�ֹ������
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


    if(crossin_flag==FLAG_ON&&left_point<25&&right_point<25){              //����ʮ�־Ͱ�ǰհ�и���ʮ������
        if(pro_start_row>left_point)  pro_start_row=left_point;
        pro_end_row=pro_start_row-7;
    }




    for(i=pro_start_row;i>=pro_end_row;i--){   //������iΪǰհ�У�

          error_sum+=(63-midline_pre[i])*(Right_Line[i]-Left_Line[i]);  //�������޸������˲��������(Right_Line[i]-Left_Line[i])
           wet_sum+=(Right_Line[i]-Left_Line[i]);//               //
       }

      g_image_err=(float)((float)(error_sum)/(float)(wet_sum));


      /*�������*/
       g_lastimg_err[0]=g_lastimg_err[1];
       g_lastimg_err[1]=g_lastimg_err[2];
       g_lastimg_err[2]=g_lastimg_err[3];
       g_lastimg_err[3]=g_lastimg_err[4];
       g_lastimg_err[4]=g_image_err;

       g_image_err=(g_lastimg_err[4]+g_lastimg_err[3]+g_lastimg_err[2]+g_lastimg_err[1])/4.0;


      /*����޷�*/



     for(j=0;j<bin_image_W;j++){   //��ʾǰհ��
              bin_image[pro_start_row][j]=3;
              bin_image[pro_end_row][j]=3;
          }
}

float limit(float x,float min,float max){
    if(x<min) x=min;
    if(x>max) x=max;
    return x;
}

void crossroad_pass()//ʮ�ֲ���(λ���ѱ߽����󣬶Ա߽����´���)
{
    uint8 i;
    uint8 leftnear_row = 63;      //��һ����ת�۵�(�������ĵ�)
    uint8 leftfar_row = 63;       //�ڶ�����ת�۵�
    uint8 leftfar_flag =  FLAG_OFF;     //����ת�۵��Ƿ��ҵ���־
    uint8 rightfar_flag = FLAG_OFF;
    uint8 leftnear_flag = FLAG_OFF;    //�ڶ���ת�۵㿪����־
    uint8 rightnear_flag = FLAG_OFF;
    uint8 rightnear_row = 63;
    uint8 rightfar_row = 63;


   //crossin_flag=FLAG_ON;
    if(crossin_flag==FLAG_ON){
    //��߽����۵�
    for(i = start_row; i >= Search_Stop_Line+3 ; i--){
        if(Left_Line[i] - Left_Line[i-3] > 2)//�ҵ���һ�����۵�
        {
            leftnear_flag = FLAG_ON;
            leftnear_row = i;
            left_point = i;
            break;
        }
    }

    if(leftnear_flag == FLAG_ON){
       //�ҵ��ڶ������۵�
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

        if(leftfar_flag == FLAG_ON)//���ǰ����۵㶼�ҵ�����������
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

   /*    else//ֻ�ҵ��·������۵㣬ǿ�в���
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



    //�ұ߽����۵�
    for(i = start_row; i >= Search_Stop_Line+3; i--)
    {
        if(Right_Line[i] - Right_Line[i-3] < -2)//�ҵ���һ�����۵�
        {
            rightnear_flag = FLAG_ON;
            rightnear_row = i;
            break;
        }
    }


    if(rightnear_flag == FLAG_ON)
    {
        //�ҵ��ڶ������۵�
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
        if(rightfar_flag == FLAG_ON)//���ǰ����۵㶼�ҵ�����������
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

    /*  else//ֻ�ҵ��·������۵㣬ǿ�в���
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




void Correlation_get()//���ұ߽����ϵ���ж�
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
        //��߽����۵�
        for(i = pro_start_row; i >= end_row+5 ; i--){
            if(Left_Line[i] - Left_Line[i+2] > 3)//�ҵ���һ�����۵�
            {

                left_point = i;
                break;
            }
        }



        for(i = pro_start_row; i >= end_row+5; i--)
           {
               if(Right_Line[i] - Right_Line[i+2] < -2)//�ҵ���һ�����۵�
               {
                   right_point = i;
                   break;
               }
           }


    }


}



