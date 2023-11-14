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
uint8 Search_Stop_Line;  //����������

uint8 center_line=63;    //����������Ϊ63





 float g_image_err =0 ;
 float g_iamge_last_err=0; //�ϴ����







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
  * @brief    �������
  * @param
  * @return
  * @note
  * @date     2022/11/2 Thur
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


     uint8 wet_sum=0;

     int16 error_sum=0;
     //float error=0;

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

       if( Longest_White_Column_l_j==Longest_White_Column_l_len){
            if(((start_column+end_column)/2-Longest_White_Column_l_j)>(Longest_White_Column_r_j-(start_column+end_column)/2)){
                Longest_White_Column_l_j=Longest_White_Column_r_j;
            }
       }

       if(Longest_White_Column_r_len>Longest_White_Column_l_len){    //�ұߴ�ʱ�����ұߵ�ֵ�������
           Longest_White_Column_l_len=Longest_White_Column_r_len;
           Longest_White_Column_l_j=Longest_White_Column_r_j;
       }




     /*   for (i = start_row; i >end_row; i--)//�����ұ߽���
            {
            //��������һ�У����ʵ���Ч����Ұ��
            for (j = Longest_White_Column_l_j; j <= bin_image_W-1; j++) //
            {
                if (bin_image[i][j] ==255 && bin_image[i][j + 1] == 0 )//�׺ںڣ��ҵ��ұ߽�
                {
                     right_border = j;

                    break;
                }
                else if(j>=bin_image_W-1)//û�ҵ��ұ߽磬����Ļ���Ҹ�ֵ���ұ߽�   //
                {
                    right_border = j;

                    break;
                }
            }


            for (j = Longest_White_Column_l_j; j >= 1 ; j--)//�����ɨ��
            {//��������һ�У����ʵ��ҵ���Ч����Ұ��
                if (bin_image[i][j] ==255 && bin_image[i][j - 1] == 0 )//�ںڰ���Ϊ������߽�
                {
                    left_border = j;

                    break;
                }
                else if(j<=1)
                {
                    left_border = j;//�ҵ�ͷ��û�ҵ��ߣ��Ͱ���Ļ�����ҵ����߽�

                    break;
                }
            }
            Left_Line [i] = left_border;       //�����������
            Right_Line[i] = right_border;      //�ұ���������
        }



        for(i=start_row;i>=end_row;i--){
              midline[i]=(Left_Line [i]+Right_Line [i])/2;     //����

       }



       for(i=start_row;i>end_row;i--){
            for(j=2;j<bin_image_W-3;j++){

                if(j==Left_Line [i]){
                    bin_image[i][j-1]=4;     //��߽�


                }


                if(j==midline[i]){
                    bin_image[i][j]=4;        //red����
                }

                if(j==Right_Line [i]){
                    bin_image[i][j+1]=4;       //red�ұ߽�

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
                    bin_image[i][j]=2;       //��ɫ�������
                    break;
               }

           }
        }
/*

       pro_end_row=Search_Stop_Line-3;
       pro_start_row=pro_end_row+6;

       if(pro_end_row<25) pro_end_row=25;                 //ǰհ����ֵ�趨
       if(pro_start_row>62) pro_start_row=62;

       wet_sum=0;
       for(i=pro_start_row;i>=pro_end_row;i--){   //������iΪǰհ�У�
           bin_image[i][bin_image_W/2]=3;
            error_sum+=(bin_image_W/2-midline[i])*(Right_Line[i]-Left_Line[i]);
            wet_sum+=Right_Line[i]-Left_Line[i];
        }

       for(j=0;j<bin_image_W;j++){   //��ʾǰհ��
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
    static int16 j_l;//��¼��һ�α���λ��
    static int16 j_r;


    int temp1=63;
    int temp2=63;

    found_num=63;//������Ч�кͱ�־λ
    found_flag=1;




      //����һ�ε��е���������,��ʼ��Ϊ�������һ�У���64�У�
        for(i = start_row;i >= end_row;i--)
           {
             if(i == start_row)
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
               temp1=Left_Line[i+1]+10 ;
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

              //g_road_width[i]=abs((int)(g_right_edge[i]-g_left_edge[i]));  //·���й�

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
        midline_pre[i]=(Left_Line [i]+Right_Line [i])/2;     //����
           }

    for(i=start_row;i>end_row;i--){
               for(j=2;j<bin_image_W-3;j++){

                   if(j==Left_Line [i]){
                       bin_image[i][j-1]=4;     //��߽�


                   }


                   if(j==midline_pre[i]){
                       bin_image[i][j]=4;        //red����
                   }

                   if(j==Right_Line [i]){
                       bin_image[i][j+1]=4;       //red�ұ߽�

                       break;
                   }
               }
           }
}

void caluate_err(){
    int i,j;
    uint8 wet_sum=0;
    int16 error_sum=0;


          pro_end_row=Search_Stop_Line+2;       //ǰհ����ѡ�ڽ�ֹ������һЩ
          if(pro_end_row<25) pro_end_row=25;                 //ǰհ����ֵ�趨
          pro_start_row=pro_end_row+6;
          if(pro_start_row>60) pro_start_row=60;


          for(i=pro_start_row;i>=pro_end_row;i--){   //������iΪǰհ�У�
             // bin_image[i][bin_image_W/2]=3;
               error_sum+=(63-midline_pre[i])*(Right_Line[i]-Left_Line[i]);
               wet_sum+=Right_Line[i]-Left_Line[i];
           }

           g_image_err=(float)error_sum/(wet_sum);
          // tft180_show_float(0,0 ,g_image_err,3,1);

          for(j=0;j<bin_image_W;j++){   //��ʾǰհ��
              bin_image[pro_start_row][j]=3;
              bin_image[pro_end_row][j]=3;
          }


}



