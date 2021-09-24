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
 ALIENTEK 探索者STM32F407开发板 FreeRTOS实验17-3
 FreeRTOS任务通知模拟消息邮箱-库函数版本
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		256  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define TASK1_TASK_PRIO		2
//任务堆栈大小	
#define TASK1_STK_SIZE 		256  
//任务句柄
TaskHandle_t Task1Task_Handler;
//任务函数
void task1_task(void *pvParameters);

//任务优先级
#define TASK2_TASK_PRIO 3
//任务堆栈大小	
#define TASK2_STK_SIZE  256 
//任务句柄
TaskHandle_t Task2Task_Handler;
//任务函数
void task2_task(void *pvParameters);

//任务优先级
#define TASK3_TASK_PRIO 4
//任务堆栈大小	
#define TASK3_STK_SIZE  256 
//任务句柄
TaskHandle_t Task3Task_Handler;
//任务函数
void task3_task(void *pvParameters);

//任务优先级
#define TASK4_TASK_PRIO 5
//任务堆栈大小	
#define TASK4_STK_SIZE  256 
//任务句柄
TaskHandle_t Task4Task_Handler;
//任务函数
void task4_task(void *pvParameters);


int main(void)
{ 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);					//初始化延时函数
	uart_init(115200);     				//初始化串口
	LED_Init();		        			//初始化LED端口
//	KEY_Init();							//初始化按键
//	BEEP_Init();						//初始化蜂鸣器
	LCD_Init();							//初始化LCD
//	AT24CXX_Init();			//IIC初始化 
//	my_mem_init(SRAMIN);            	//初始化内部内存池
     USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb); 
 	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"IIC TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2014/5/6");	 
	LCD_ShowString(30,130,200,16,16,"KEY1:Write  KEY0:Read");	//显示提示信息	
	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区

    //创建TASK1任务
    xTaskCreate((TaskFunction_t )task1_task,             
                (const char*    )"task1_task",           
                (uint16_t       )TASK1_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK1_TASK_PRIO,        
                (TaskHandle_t*  )&Task1Task_Handler);  
					 
    //创建按键处理任务
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
					 
    vTaskDelete(StartTask_Handler); //删除开始任务
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();             //退出临界区
}


//task1任务函数
void task1_task(void *pvParameters)
{

	while(1)
	{

			  if(USB_USART_RX_BUF[0]=='2')
			  {
				  xTaskNotify((TaskHandle_t	)Task2Task_Handler,		//接收任务通知的任务句柄
							    (uint32_t		)2,						//任务通知值
								 (eNotifyAction	)eSetValueWithOverwrite);	//覆写的方式发送任务通知
				  USB_USART_RX_BUF[0]='0';
			  }
			  else if(USB_USART_RX_BUF[0]=='3')
			  {
				  xTaskNotify((TaskHandle_t	)Task3Task_Handler,		//接收任务通知的任务句柄
							    (uint32_t		)3,						//任务通知值
								 (eNotifyAction	)eSetValueWithOverwrite);	//覆写的方式发送任务通知
				  USB_USART_RX_BUF[0]='0';
			  }else if(USB_USART_RX_BUF[0]=='4')
			  {
				  xTaskNotify((TaskHandle_t	)Task4Task_Handler,		//接收任务通知的任务句柄
							    (uint32_t		)4,						//任务通知值
								 (eNotifyAction	)eSetValueWithOverwrite);	//覆写的方式发送任务通知
				  USB_USART_RX_BUF[0]='0';
			  }
			
			
			USB_USART_RX_STA=0;
		


		  
			
		}

        vTaskDelay(100);           //延时10ms，也就是10个时钟节拍	
	}



//task2_task函数
void task2_task(void *pvParameters)
{

	uint32_t NotifyValue;
	while(1)
	{
	  xTaskNotifyWait((uint32_t	)0x00,				//进入函数的时候不清除任务bit
							(uint32_t	)ULONG_MAX,			//退出函数的时候清除所有的bit
							(uint32_t*	)&NotifyValue,		//保存任务通知值
							(TickType_t	)portMAX_DELAY);	//阻塞时间
	if(USB_USART_RX_BUF[0]=='2')
	{
			usb_printf("任务2收的串口信息为:%d   \n",(u8)NotifyValue);
	}
	        vTaskDelay(1000);           //延时10ms，也就是10个时钟节拍			
	}
}

//task3_task函数
void task3_task(void *pvParameters)
{
	uint32_t NotifyValue;
	while(1)
	{
	  xTaskNotifyWait((uint32_t	)0x00,				//进入函数的时候不清除任务bit
							(uint32_t	)ULONG_MAX,			//退出函数的时候清除所有的bit
							(uint32_t*	)&NotifyValue,		//保存任务通知值
							(TickType_t	)portMAX_DELAY);	//阻塞时间
	if(USB_USART_RX_BUF[0]=='3')
	{

			usb_printf("任务3收的串口信息为:%d   \n",(u8)NotifyValue);

	}
	       vTaskDelay(100);           //延时10ms，也就是10个时钟节拍				
	}
}

//task4_task函数
void task4_task(void *pvParameters)
{
	uint32_t NotifyValue;
	while(1)
	{
	  xTaskNotifyWait((uint32_t	)0x00,				//进入函数的时候不清除任务bit
							(uint32_t	)ULONG_MAX,			//退出函数的时候清除所有的bit
							(uint32_t*	)&NotifyValue,		//保存任务通知值
							(TickType_t	)portMAX_DELAY);	//阻塞时间
	if(USB_USART_RX_BUF[0]=='4')
	{

			usb_printf("任务4收的串口信息为:%d   \n",(u8)NotifyValue);
	}
	        vTaskDelay(100);           //延时10ms，也就是10个时钟节拍				
	}
}


