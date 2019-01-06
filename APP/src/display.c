/**
  *******************************************************************************************************
  * File Name: display.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: 显示部分
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-29
	*			Mod: 建立文件
	*
  *******************************************************************************************************
  */	
/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "app.h"
# include "gui.h"

/*
  *******************************************************************************************************
  *                              LOCAL VARIABLE
  *******************************************************************************************************
*/
static bool isInit = false;

/*
*********************************************************************************************************
*                                display_deviceInit          
*
* Description: 初始化显示硬件层
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
static void display_deviceInit(void)
{
	gui_Init();
}

/*
*********************************************************************************************************
*                       display_Init                   
*
* Description: 显示部分初始化
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void display_Init(void)
{
	if(isInit) return;
	
	display_deviceInit();
	
	isInit = true;
}

static void button_cb(WM_MESSAGE *pMsg)
{
	MSG_KEY_INFO *key = NULL;
	
	key = &pMsg->key;
	
	if(key->keyValue == MSG_KEY_UP)
	{
		bsp_led_Toggle(LED_RED);
	}
	else if(key->keyValue == MSG_KEY_DOWN)
	{
		bsp_led_Toggle(LED_BLUE);
	}
}

/*
*********************************************************************************************************
*                        task_displayTask                  
*
* Description: 显示任务
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void task_displayTask(void *p_arg)
{
	uint16_t cnt = 0, flag = 1;
	WM_HWIN hWin1;
	WINDOW_Handle hWindow = 0;
	BUTTON_Handle hButton1 = 0;
//	uint8_t f = 0;
	
	gui_Init();

	hWin1 = wm_Create(0, 0, 127, 63, 1);
	hWindow = window_Create(0, 0, 127, 63, "ok", 1, hWin1);
	hButton1 = button_Create(5, 30, 20, 20, 2, "A", MSG_KEY_UP, hWin1);
	button_setCallback(hButton1, button_cb);
	
	button_Create(30, 30, 20, 20, 3, "B", MSG_KEY_DOWN, hWin1);
	button_Create(55, 30, 20, 20, 4, "C", MSG_KEY_LEFT, hWin1);
	button_Create(80, 30, 20, 20, 5, "D", MSG_KEY_RIGHT, hWin1);
//	button_Delete(&hButton1);
	while(1)
	{
		gui_excute();
		
//		cnt++;
//		
//		if(cnt%1 == 0)
//		{
//			if(flag)
//			{
//				button_setText(hButton1, "B");
//				flag = 0;
//			}
//			else
//			{
//				button_setText(hButton1, "C");
//				flag = 1;
//			}
//			cnt = 0;
//		}
		vTaskDelay(5);
	}
}


/********************************************  END OF FILE  *******************************************/

