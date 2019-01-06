/**
  *******************************************************************************************************
  * File Name: main.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-25
  * Brief: 标准C程序入口文件
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-25
	*			Mod:	建立文件
  *
  *******************************************************************************************************
  */	
/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "bsp.h"
# include "app.h"
/*
  *******************************************************************************************************
  *                              LOCAL VARIABLE
  *******************************************************************************************************
*/
# define START_TASK_PRIO		2
# define START_TASK_STK_SIZE		256
TaskHandle_t startTaskHandle = NULL;
void task_StartTask(void *p_arg);


/*
*********************************************************************************************************
*                          main                
*
* Description: 标准C程序入口函数
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
int main(void)
{ 
	powerManger_Init();
	power_Enabled(true);
	
	bsp_Init();
	
	xTaskCreate(task_StartTask, "Start Task", START_TASK_STK_SIZE, NULL, START_TASK_PRIO, &startTaskHandle);
	vTaskStartScheduler();
	while(1);
}


/*
*********************************************************************************************************
*                                          
*
* Description: 
*             
* Arguments  : 
*
* Reutrn     : 
*
* Note(s)    : 
*********************************************************************************************************
*/
void task_StartTask(void *p_arg)
{
	taskENTER_CRITICAL();
//	bsp_led_StartFlash(LED_ALL, 5, 50, LED_KEEP_FLASH);
	
	
	xTaskCreate(task_LEDTask, "LED Task", LED_TASK_STK_SIZE, NULL, 2, NULL);
	xTaskCreate(task_displayTask, "Display Task", 256, NULL, 4, NULL);

	
	vTaskDelete(startTaskHandle);
	taskEXIT_CRITICAL();
}

/********************************************  END OF FILE  *******************************************/


