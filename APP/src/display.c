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
	
//	if(key->keyValue == MSG_KEY_UP)
//	{
//		bsp_led_Toggle(LED_BLUE);
//	}
//	else if(key->keyValue == MSG_KEY_LEFT)
//	{
		bsp_led_Toggle(LED_RED);
//	}
	
//	int32_t i = 0;
//	
//	while(i++ < 6553600);
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
	BUTTON_Handle hButton1 = 0, hButton2 = 0, hButton3 = 0, hButton4 = 0;
	MENU_Handle hMenu;
	PROGBAR_Handle hProgbar1 = 0, hProgbar2 = 0, hProgbar3 = 0;
	WIN_Handle hWin, hWin1, hWin2, hWin3;
	WIN_OBJ *pWin;
	int16_t err;
	gui_Init();

	hWin1 = win_Create(1, "window 1", GUI_CF_HALF | GUI_CF_SHOW);
	hProgbar1 = progbar_Create(10, 20, 80, 10, 1, hWin1);
	hProgbar2 = progbar_Create(10, 35, 80, 10, 2, hWin1);
	hProgbar3 = progbar_Create(10, 50, 80, 10, 3, hWin1);
//	hMenu = menu_Create(10, 20, 60, 1, hWin1);
//	
//	menu_insertItem(hMenu, "Test Item 1", button_cb);
//	menu_insertItem(hMenu, "Test Item 2", button_cb);
//	menu_insertItem(hMenu, "Test Item 3", button_cb);
//	menu_insertItem(hMenu, "Test Item 4", button_cb);
//	menu_insertItem(hMenu, "Test Item 5", button_cb);
//	menu_insertItem(hMenu, "Test Item 6", button_cb);
//	menu_insertItem(hMenu, "Test Item 7", button_cb);
	
//	menu_deleteItem(hMenu, 3);
	
//	hButton1 = button_Create(5, 17, 40, 20, 2, "UP", MSG_KEY_UP, hWin1);
//	hButton2 = button_Create(5, 40, 40, 20, 3, "DOWN", MSG_KEY_DOWN, hWin1);
//	hButton3 = button_Create(80, 17, 40, 20, 4, "LEFT", MSG_KEY_LEFT, hWin1);
//	hButton4 = button_Create(80, 40, 40, 20, 5, "RIGHT", MSG_KEY_RIGHT, hWin1);
////	checkbox_Create(0,0,"a", 1, hWin1);
//	button_setCallback(hButton1, button_cb);
////	
//	hButton3 = button_Create(82, 30, 40, 20, 3, "RIGHT", MSG_KEY_LEFT, hWin1);
////	win_boringToTop(hWin1);
//	button_setCallback(hButton2, button_cb);
	
	while(1)
	{
		gui_excute();
		
		cnt ++;
		
		if(cnt %100 == 0)
		{
			progbar_increaseRatio(hProgbar1, 1);
			progbar_increaseRatio(hProgbar2, 2);
			progbar_increaseRatio(hProgbar3, 3);
		}
		
		vTaskDelay(1);
	}
}


/********************************************  END OF FILE  *******************************************/

