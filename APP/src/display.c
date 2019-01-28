/**
  *******************************************************************************************************
  * File Name: display.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: ��ʾ����
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-29
	*			Mod: �����ļ�
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
* Description: ��ʼ����ʾӲ����
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
* Description: ��ʾ���ֳ�ʼ��
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
		bsp_led_Toggle(LED_BLUE);
	}
	else if(key->keyValue == MSG_KEY_LEFT)
	{
		bsp_led_Toggle(LED_RED);
	}
}

/*
*********************************************************************************************************
*                        task_displayTask                  
*
* Description: ��ʾ����
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
	BUTTON_Handle hButton1 = 0, hButton2 = 0;
	
	gui_Init();

	hWin1 = wm_Create(0, 0, 127, 63, 1);
	hWindow = window_Create(0, 0, 127, 63, "window", 1, hWin1);
	hButton1 = button_Create(5, 30, 40, 20, 2, "LEFT", MSG_KEY_PWR, hWin1);
	button_setCallback(hButton1, button_cb);
	
	hButton2 = button_Create(82, 30, 40, 20, 3, "RIGHT", MSG_KEY_LEFT, hWin1);
	button_setCallback(hButton2, button_cb);
	
	while(1)
	{
		gui_excute();
		
		vTaskDelay(5);
	}
}


/********************************************  END OF FILE  *******************************************/

