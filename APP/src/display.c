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
	WM_HWIN hWin1, hWin2, hWin3, hWin4, hWin5, hWin6, hWin7;
	BUTTON_Handle hButton1 = 0, hButton2 = 0, hButton3 = 0, hButton4 = 0, hButton5 = 0;
	uint8_t f = 0;
	
	gui_Init();
	hWin1 = wm_Create(0, 0, 127, 63, 1);
	hWin2 = wm_Create(0, 0, 127, 63, 0);
	hWin3 = wm_Create(0, 0, 127, 63, 0);
	hWin4 = wm_Create(0, 0, 127, 63, 0);
//	hButton5 = button_Create(105, 30, 20, 20, 6, "E", hWin7);
	window_Create(0, 0, 127, 63, "ok", 7, hWin4);

	button_Delete(&hButton2);
	button_Delete(&hButton3);
	button_Delete(&hButton4);
	wm_Delete(&hWin1);
	wm_Delete(&hWin2);
	wm_Delete(&hWin3);
	wm_Delete(&hWin4);
	while(1)
	{
		gui_excute();
		
//		cnt++;
		
//		if(cnt%1 == 0)
//		{
			if(flag)
			{
				hButton1 = button_Create(5, 30, 20, 20, 2, "A", hWin4);
				hButton2 = button_Create(30, 30, 20, 20, 3, "B", hWin4);
				hButton3 = button_Create(55, 30, 20, 20, 4, "C", hWin4);
				hButton4 = button_Create(80, 30, 20, 20, 5, "D", hWin4);
				hButton5 = button_Create(105, 30, 20, 20, 6, "E", hWin4);
				flag = 0;
			}
			else
			{
				button_Delete(&hButton1);
				button_Delete(&hButton2);
				button_Delete(&hButton3);
				button_Delete(&hButton4);
				button_Delete(&hButton5);
				flag = 1;
			}
//			cnt = 0;
//		}
		vTaskDelay(5);
	}
}


/********************************************  END OF FILE  *******************************************/

