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
# include "gui_core.h"
# include "gui_key.h"
# include "gui_win.h"
# include "gui_button.h"
# include "gui_menu.h"
# include "gui_progbar.h"

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

static void button_cb(GUI_KEY_INFO *pKey)
{
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
	WIN_OBJ *pWin = NULL;
	BUTTON_Handle hButton1 = 0, hButton2 = 0, hButton3 = 0, hButton4 = 0;
	MENU_Handle hMenu;
	PROGBAR_Handle hProgbar1 = 0, hProgbar2 = 0, hProgbar3 = 0;
	WIN_Handle hWin, hWin1, hWin2, hWin3;

	gui_Init();

	hWin1 = win_Create(1, (uint8_t *)"button window", GUI_CF_HALF | GUI_CF_SHOW);
	button_Create(10, 16, 40, 20, 2, (uint8_t *)"UP", GUI_KEY_UP, hWin1);
	button_Create(77, 16, 40, 20, 3, (uint8_t *)"DOWN", GUI_KEY_DOWN, hWin1);
	button_Create(10, 40, 40, 20, 4, (uint8_t *)"LEFT", GUI_KEY_LEFT, hWin1);
	button_Create(77, 40, 40, 20, 5, (uint8_t *)"UP", GUI_KEY_RIGHT, hWin1);
	
	hWin2 = win_Create(2, (uint8_t *)"progbar window", GUI_CF_HALF);
	hProgbar1 = progbar_Create(20, 20, 80, 10, 1, hWin2);
	hProgbar2 = progbar_Create(20, 35, 80, 10, 2, hWin2);
	hProgbar3 = progbar_Create(20, 50, 80, 10, 3, hWin2);
	
	hWin3 = win_Create(3, (uint8_t *)"menu window", GUI_CF_HALF);
	hMenu = menu_Create(10, 20, 60, 1, hWin3);
	menu_insertItem(hMenu, (uint8_t *)"Test Item 1", NULL);
	menu_insertItem(hMenu, (uint8_t *)"Test Item 2", NULL);
	menu_insertItem(hMenu, (uint8_t *)"Test Item 3", NULL);
	menu_insertItem(hMenu, (uint8_t *)"Test Item 4", NULL);
	menu_insertItem(hMenu, (uint8_t *)"Test Item 5", NULL);
	menu_insertItem(hMenu, (uint8_t *)"Test Item 6", NULL);
	menu_insertItem(hMenu, (uint8_t *)"Test Item 7", NULL);
	
//	menu_deleteItem(hMenu, 3);


//////	checkbox_Create(0,0,"a", 1, hWin1);
//	button_setCallback(hButton1, button_cb);
////	
//	hButton3 = button_Create(82, 30, 40, 20, 3, "RIGHT", MSG_KEY_LEFT, hWin1);
	win_boringToTop(hWin3);
//	button_setCallback(hButton2, button_cb);
	
	while(1)
	{
		gui_excute();
		
		cnt ++;
		
//		if(cnt == 2000)
//		{
//			win_boringToTop(hWin2);
//		}
//		else if(cnt == 4000)
//		{
//			win_boringToTop(hWin3);
//		}
//		else if(cnt == 6000)
//		{
//			win_boringToTop(hWin1);
//			cnt = 0;
//		}

		vTaskDelay(1);
	}
}


/********************************************  END OF FILE  *******************************************/

