/*
 * flash.c
 *
 *  Created on: 2023��10��3��
 *      Author: The Beheaded
 */
#include"flash.h"
#include"all_define.h"


//������
 //����״̬����
extern uint8 key1_status;
extern uint8 key2_status;
extern uint8 key3_status;
extern uint8 key4_status;

//��һ�ο���״̬����
extern uint8 key1_last_status;
extern uint8 key2_last_status;
extern uint8 key3_last_status;
extern uint8 key4_last_status;

//���ر�־λ
extern uint8 key1_flag;
extern uint8 key2_flag;
extern uint8 key3_flag;
extern uint8 key4_flag;

extern uint16 duty1;

extern int data[data_number];
extern uint32 Data[data_number];
extern int x[data_number][5];
extern int p[5];
extern int state;






void key_judge()            //����״̬�ж�
{
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;
    key1_status = gpio_get_level(KEY1);
    key2_status = gpio_get_level(KEY2);
    key3_status = gpio_get_level(KEY3);
    key4_status = gpio_get_level(KEY4);
    if(key1_status && !key1_last_status)    key1_flag = 1;
    if(key2_status && !key2_last_status)    key2_flag = 1;
    if(key3_status && !key3_last_status)    key3_flag = 1;
    if(key4_status && !key4_last_status)    key4_flag = 1;
}



void combine_int(int i)    //�ϲ�ÿһλ����
{
    data[i]=x[i][0]*10000+x[i][1]*1000+x[i][2]*100+x[i][3]*10+x[i][4];
}

void row_change(int *row,int *page)    //�л�ѡ�����
{

    key_judge();
    if (*page>0)  //����ҳ
    {
        if (key3_flag==1)       //key3����
        {
            key3_flag=0;
            *row+=1;
            tft180_clear();

        }
        if(*row>4){
            *row=1;
            tft180_clear();
        }
}
}


void page_change(int *page)    //�л�ѡ���ҳ
{

    key_judge();
    if (key1_flag==1)     //key1 page -1
            {
                key1_flag=0;
               *page += 1;
               tft180_clear();
            }

    if (key2_flag==1)     //key2 page -1
               {
                  key2_flag=0;
                  *page -= 1;
                  tft180_clear();
               }

    if(*page<0){
        *page=4;
    }
    if(*page>4){
        *page=0;
    }
}


void change_int(int page,int row)   //�޸�����
{
    int line=1;     //λ��
    key_judge();
    if(x[0][0]>1){    //��ֹ�������ռ�ձ�
          x[0][0]=1;
     }
    if (key4_flag==1&&page>0){
                    key4_flag=0;
                    while(1){

                        key_judge();
                        tft180_draw_line((line-1)*8+72,(row-1)*32+15,(line-1)*8+76,(row-1)*32+15,RGB565_RED);
                        page_show(page);
                        row_show(row);

                        if(key3_flag==1){  //��λ
                            key3_flag=0;
                            line++;
                            if(line>5){
                                line=1;
                            }
                        }

                        if (key1_flag==1){
                            key1_flag=0;
                            x[(page-1)*4+row-1][line-1]++;   //λ��+1
                            if(x[0][0]>1){    //��ֹ�������ռ�ձ�
                                x[0][0]=1;
                            }

                            combine_int((page-1)*4+row-1);

                        }
                        if (key2_flag==1){
                                  key2_flag=0;
                                  x[(page-1)*4+row-1][line-1]--;
                                  combine_int((page-1)*4+row-1);
                                        if (x[(page-1)*4+row-1][line-1]<0){
                                            x[(page-1)*4+row-1][line-1]=0;
                                            combine_int((page-1)*4+row-1);
                                        }
                        }

                        if(key4_flag==1){
                        key4_flag=0;

                        combine_int((page-1)*4+row-1);
                        save_int(page,row);
                            break;
                        }

                    }
      }
    if(data[0]>10000){
        data[0]=10000;
    }
}








void save_int (int p,int r)      //�����pageҳ�ĵ�r�в���
{
    int i;
    Data[(p-1)*4+r-1]=data[(p-1)*4+r-1];
    for(i=0;i<data_number;i++)
    {
        flash_union_buffer[i].uint32_type=Data[i];
    }
    if (flash_check(0, 11))
    {
        flash_erase_page(0, 11);
    }
    flash_write_page_from_buffer(0, 11);
}

void seperate_int(int i)    //data[i]ÿһλ���һλ����
{
       x[i][0]=data[i]/10000;
       x[i][1]=data[i]/1000 %10;
       x[i][2]=data[i]%1000 / 100;
       x[i][3]=data[i]%100/10;
       x[i][4]=data[i]%10;
}


void show_int(int k,int high)    //��ӡ����  k:�������,��ʾ��k������  high�������꣬
{
    int i;
    seperate_int(k);
    for (i=0;i<5;i++)
    {
       tft180_show_int((uint16)(70+i*8), (uint16)high, x[k][i], (uint8)1);
    }
}

void page_show(int page)
{
    int i;
    if (page == 0)
    {


    }
    else if (page == 1)
    {
        tft180_show_string(10, 0, "duty:");
        tft180_show_string(10, 32, "kp:");
        tft180_show_string(10, 64, "ki:");
        tft180_show_string(10, 96, "kd:");
        for(i=0;i<4;i++)
                {
                    show_int((page-1)*4+i, i*32);
                }

    }
    else if (page == 2)
    {
        tft180_show_string(10, 0, "psta_r:");
        tft180_show_string(10, 32, "pend_r:");
        tft180_show_string(10, 64, "par7:");
        tft180_show_string(10, 96, "par8:");
        for(i=0;i<4;i++)
                        {
                            show_int((page-1)*4+i, i*32);
                        }


    }
    else if (page == 3)
    {
        tft180_show_string(10, 0, "par9:");
        tft180_show_string(10, 32, "par10:");
        tft180_show_string(10, 64, "par11:");
        tft180_show_string(10, 96, "par12:");
        for(i=0;i<4;i++)
             {
                  show_int((page-1)*4+i, i*32);
            }

    }
    else if (page == 4)
    {
        tft180_show_string(10, 0, "par13:");
        tft180_show_string(10, 32, "par14:");
        tft180_show_string(10, 64, "par15:");
        tft180_show_string(10, 96, "par16:");
        for(i=0;i<4;i++)
          {
              show_int((page-1)*4+i, i*32);
          }

    }

}

void row_show(int row)     //��ʾѡ�����
{


    switch(row)
    {
    case 1:
        tft180_show_char(0, 0, '*');
        break;
    case 2:
        tft180_show_char(0, 32, '*');
        break;
    case 3:
        tft180_show_char(0, 64, '*');
        break;
    case 4:
        tft180_show_char(0, 96, '*');
        break;

    }
}

void try_data()       //д���һ�β�������
{
    int i;
    for(i=0;i<data_number;i++)
    {
        Data[i]=data[i];
        flash_union_buffer[i].uint32_type=Data[i];
    }
    if (flash_check(0, 11))
    {
        flash_erase_page(0, 11);
    }
    flash_write_page_from_buffer(0, 11);
}

void init_data()   //��������
{
    int j;
    flash_buffer_clear();
    for(j=0;j<data_number;j++)
    {
        flash_read_page_to_buffer(0, 11);
        if (Data[j]!=flash_union_buffer[j].uint32_type)
        {
            Data[j]=flash_union_buffer[j].uint32_type;
        }
        data[j]=Data[j];
    }
}






