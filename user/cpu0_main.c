/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu0_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.4
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
#include "user_headfile.h"
#include "image_process.h"
#include "all_param.h"
#include "pid.h"
#include "VOFA.h"


// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��
// *************************** ����Ӳ������˵�� ***************************
// ����С���Ӳ����ֵ������ͷ ��Ӧ��������ͷ�ӿ� ��ע������
//      ģ��ܽ�            ��Ƭ���ܽ�
//      TXD/SCL             �鿴 zf_device_ov7725.h �� OV7725_COF_UART_TX �� OV7725_COF_IIC_SCL �궨��
//      RXD/SDA             �鿴 zf_device_ov7725.h �� OV7725_COF_UART_RX �� OV7725_COF_IIC_SDA �궨��
//      PCLK                �鿴 zf_device_ov7725.h �� OV7725_PCLK_PIN �궨��
//      VSY                 �鿴 zf_device_ov7725.h �� OV7725_VSYNC_PIN �궨��
//      D0-D7               �鿴 zf_device_ov7725.h �� OV7725_DATA_PIN �궨�� �Ӹö��忪ʼ�������˸�����
//      GND                 ���İ��Դ�� GND
//      3V3                 ���İ� 3V3 ��Դ
// ����1.8��TFTģ��
//      ģ��ܽ�            ��Ƭ���ܽ�
//      SCL                 �鿴 zf_device_tft180.h �� TFT180_SCL_PIN �궨�� Ĭ�� P15_3
//      SDA                 �鿴 zf_device_tft180.h �� TFT180_SDA_PIN �궨�� Ĭ�� P15_5
//      RES                 �鿴 zf_device_tft180.h �� TFT180_RES_PIN �궨�� Ĭ�� P15_1
//      DC                  �鿴 zf_device_tft180.h �� TFT180_DC_PIN  �궨�� Ĭ�� P15_0
//      CS                  �鿴 zf_device_tft180.h �� TFT180_CS_PIN  �궨�� Ĭ�� P15_2
//      BL                  �鿴 zf_device_tft180.h �� TFT180_BL_PIN  �궨�� Ĭ�� P15_4
//      GND                 ���İ��Դ�� GND
//      3V3                 ���İ� 3V3 ��Դ



// *************************** ���̲���˵�� ***************************
// 1.���İ���¼��ɱ����� �����İ���������� �嵽��
// 2.����ͷ�������������ͷ�ӿ� ע������1.8��TFTģ�����������Ļ�ӿ�
// 3.�����ϵ� ���ߺ��İ�������Ϻ��ϵ� ���İ尴�¸�λ����
// 4.��Ļ����ʾ��ʼ����ϢȻ����ʾ����ͷͼ��
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�

// **************************** �������� ****************************



int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������


    tft180_set_dir(TFT180_CROSSWISE);    // ��Ҫ�Ⱥ��� ��Ȼ��ʾ����
    mt9v03x_init();
    tft180_init();





    pwm_init( ATOM2_CH0_P00_9,50,820);

    pwm_init( ATOM0_CH2_P21_4,17000,100);  //����
    pwm_init( ATOM0_CH3_P21_5,17000,100);

    pwm_init( ATOM3_CH4_P33_12,17000,100);  //����
    pwm_init( ATOM3_CH5_P33_13,17000,100);

    pit_ms_init(CCU60_CH0,5);
    pit_ms_init(CCU60_CH1,30);

    int page=0;   //��ǰҳ��
    int row=1;    //��ǰ��
    int flag=0;    //��ǰģʽ

    wireless_uart_init();

    EncoderInit();

    init_data();
   //  try_data();

    // �˴���д�û����� ���������ʼ�������
	cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
	while (TRUE)
	{
        // �˴���д��Ҫѭ��ִ�еĴ���

	    Vofa_4data(LEFT_SPEED,RIGHT_SPEED,DISTANCE,0);


	    if(!page){
	      //  tft180_show_gray_image(0, 0, mt9v03x_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W / 2, MT9V03X_H / 2, 0);
	        tft180_show_rgb565_image(0,bin_image_H/2+2, bin_image[0], bin_image_W,bin_image_H, bin_image_W, bin_image_H,0);
	      tft180_show_int(0,110,delta_servo_pwm,3);
	    tft180_show_float(0,0 ,g_image_err,3,1);
	    //  tft180_show_int(70,0,LOST_RIGHT_FLAG,3);
	     // tft180_show_float(0,110 ,left_Correlation,3,2);
	    //  tft180_show_float(50,110 ,right_Correlation,3,2);
	    }



	     page_change(&page);

	     if(page){
	       page_show(page);
	       change_int(page,row);  //ҳ����ʾ
	       row_change(&row,&page);
	       row_show(row);
	      }





        // �˴���д��Ҫѭ��ִ�еĴ���
	}
}






#pragma section all restore
// **************************** �������� ****************************
// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// ����1����Ļ����ʾ
//      ���ʹ��������ԣ��������Ҫ�õ�ع��� �����Ļ�������ŵ�ѹ
//      �����Ļ�ǲ��ǲ��λ���� ������Ŷ�Ӧ��ϵ
//      �����Ӧ���Ŷ���ȷ ���һ���Ƿ������Ų��β��� ��Ҫ��ʾ����
//      �޷���ɲ��β�������һ��GPIO���̽���Ļ����IO��ʼ��ΪGPIO��ת��ƽ �����Ƿ��ܿ�
// ����2����ʾ reinit ����
//      �������Ƿ�����
//      ���幩���Ƿ�ʹ�õ�������ĵ�ع���
// ����2����ʾͼ������ ��λ
//      �������ͷ�ź����Ƿ����ɶ�
