#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
//#include "timer.h"
#include "lcd.h"
///#include "key.h"
//#include "beep.h"
#include "string.h"
#include "malloc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "limits.h"
//#include "24cxx.h"

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"
#include "usb_dcd_int.h"



#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;
/************************************************
 ALIENTEK ̽����STM32F407������ FreeRTOSʵ��17-3
 FreeRTOS����֪ͨģ����Ϣ����-�⺯���汾
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		256  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define TASK1_TASK_PRIO		2
//�����ջ��С	
#define TASK1_STK_SIZE 		256  
//������
TaskHandle_t Task1Task_Handler;
//������
void task1_task(void *pvParameters);

//�������ȼ�
#define TASK2_TASK_PRIO 3
//�����ջ��С	
#define TASK2_STK_SIZE  256 
//������
TaskHandle_t Task2Task_Handler;
//������
void task2_task(void *pvParameters);

//�������ȼ�
#define TASK3_TASK_PRIO 4
//�����ջ��С	
#define TASK3_STK_SIZE  256 
//������
TaskHandle_t Task3Task_Handler;
//������
void task3_task(void *pvParameters);

//�������ȼ�
#define TASK4_TASK_PRIO 5
//�����ջ��С	
#define TASK4_STK_SIZE  256 
//������
TaskHandle_t Task4Task_Handler;
//������
void task4_task(void *pvParameters);


int main(void)
{ 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	delay_init(168);					//��ʼ����ʱ����
	uart_init(115200);     				//��ʼ������
	LED_Init();		        			//��ʼ��LED�˿�
//	KEY_Init();							//��ʼ������
//	BEEP_Init();						//��ʼ��������
	LCD_Init();							//��ʼ��LCD
//	AT24CXX_Init();			//IIC��ʼ�� 
//	my_mem_init(SRAMIN);            	//��ʼ���ڲ��ڴ��
     USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb); 
 	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"IIC TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2014/5/6");	 
	LCD_ShowString(30,130,200,16,16,"KEY1:Write  KEY0:Read");	//��ʾ��ʾ��Ϣ	
	
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���

    //����TASK1����
    xTaskCreate((TaskFunction_t )task1_task,             
                (const char*    )"task1_task",           
                (uint16_t       )TASK1_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK1_TASK_PRIO,        
                (TaskHandle_t*  )&Task1Task_Handler);  
					 
    //����������������
    xTaskCreate((TaskFunction_t )task2_task,     
                (const char*    )"task2_task",   
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_TASK_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler); 
					 
    xTaskCreate((TaskFunction_t )task3_task,     
                (const char*    )"task3_task",   
                (uint16_t       )TASK3_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK3_TASK_PRIO,
                (TaskHandle_t*  )&Task3Task_Handler); 
					 
    xTaskCreate((TaskFunction_t )task4_task,     
                (const char*    )"task4_task",   
                (uint16_t       )TASK4_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK4_TASK_PRIO,
                (TaskHandle_t*  )&Task4Task_Handler); 
					 
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();             //�˳��ٽ���
}


//task1������
void task1_task(void *pvParameters)
{

	while(1)
	{

			  if(USB_USART_RX_BUF[0]=='2')
			  {
				  xTaskNotify((TaskHandle_t	)Task2Task_Handler,		//��������֪ͨ��������
							    (uint32_t		)2,						//����ֵ֪ͨ
								 (eNotifyAction	)eSetValueWithOverwrite);	//��д�ķ�ʽ��������֪ͨ
				  USB_USART_RX_BUF[0]='0';
			  }
			  else if(USB_USART_RX_BUF[0]=='3')
			  {
				  xTaskNotify((TaskHandle_t	)Task3Task_Handler,		//��������֪ͨ��������
							    (uint32_t		)3,						//����ֵ֪ͨ
								 (eNotifyAction	)eSetValueWithOverwrite);	//��д�ķ�ʽ��������֪ͨ
				  USB_USART_RX_BUF[0]='0';
			  }else if(USB_USART_RX_BUF[0]=='4')
			  {
				  xTaskNotify((TaskHandle_t	)Task4Task_Handler,		//��������֪ͨ��������
							    (uint32_t		)4,						//����ֵ֪ͨ
								 (eNotifyAction	)eSetValueWithOverwrite);	//��д�ķ�ʽ��������֪ͨ
				  USB_USART_RX_BUF[0]='0';
			  }
			
			
			USB_USART_RX_STA=0;
		


		  
			
		}

        vTaskDelay(100);           //��ʱ10ms��Ҳ����10��ʱ�ӽ���	
	}



//task2_task����
void task2_task(void *pvParameters)
{

	uint32_t NotifyValue;
	while(1)
	{
	  xTaskNotifyWait((uint32_t	)0x00,				//���뺯����ʱ���������bit
							(uint32_t	)ULONG_MAX,			//�˳�������ʱ��������е�bit
							(uint32_t*	)&NotifyValue,		//��������ֵ֪ͨ
							(TickType_t	)portMAX_DELAY);	//����ʱ��
	if(USB_USART_RX_BUF[0]=='2')
	{
			usb_printf("����2�յĴ�����ϢΪ:%d   \n",(u8)NotifyValue);
	}
	        vTaskDelay(1000);           //��ʱ10ms��Ҳ����10��ʱ�ӽ���			
	}
}

//task3_task����
void task3_task(void *pvParameters)
{
	uint32_t NotifyValue;
	while(1)
	{
	  xTaskNotifyWait((uint32_t	)0x00,				//���뺯����ʱ���������bit
							(uint32_t	)ULONG_MAX,			//�˳�������ʱ��������е�bit
							(uint32_t*	)&NotifyValue,		//��������ֵ֪ͨ
							(TickType_t	)portMAX_DELAY);	//����ʱ��
	if(USB_USART_RX_BUF[0]=='3')
	{

			usb_printf("����3�յĴ�����ϢΪ:%d   \n",(u8)NotifyValue);

	}
	       vTaskDelay(100);           //��ʱ10ms��Ҳ����10��ʱ�ӽ���				
	}
}

//task4_task����
void task4_task(void *pvParameters)
{
	uint32_t NotifyValue;
	while(1)
	{
	  xTaskNotifyWait((uint32_t	)0x00,				//���뺯����ʱ���������bit
							(uint32_t	)ULONG_MAX,			//�˳�������ʱ��������е�bit
							(uint32_t*	)&NotifyValue,		//��������ֵ֪ͨ
							(TickType_t	)portMAX_DELAY);	//����ʱ��
	if(USB_USART_RX_BUF[0]=='4')
	{

			usb_printf("����4�յĴ�����ϢΪ:%d   \n",(u8)NotifyValue);
	}
	        vTaskDelay(100);           //��ʱ10ms��Ҳ����10��ʱ�ӽ���				
	}
}


