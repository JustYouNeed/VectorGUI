/**
  *******************************************************************************************************
  * File Name: gui_key.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2019-2-1
  * Brief: GUI的按键输入部分
  *******************************************************************************************************
  * History
	*		1.Author: Vector
	*			Date: 2019-2-1
	*			Mod: 建立文件
  *
  *******************************************************************************************************
  */
/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "gui_key.h"
# include "gui_widget.h"
# include "gui_win.h"
# include "bsp.h"

/*
  *******************************************************************************************************
  *                              LOCAL VARIABLE
  *******************************************************************************************************
*/
static GUI_KEY_INFO guiKeyFifo[GUI_KEY_FIFO_SIZE];
static uint8_t guiKeyFifo_r = 0;
static uint8_t guiKeyFifo_w = 0;


/*
*********************************************************************************************************
*                         gui_inputKey                 
*
* Description: 向GUI输入一个按键消息
*             
* Arguments  : keyValue: 按键值
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_inputKey(uint32_t keyValue)
{
	static uint32_t preKeyValue = 0x01;
	uint16_t keyId = 0;
	
	if(keyValue == GUI_KEY_NULL) return ;
	
	keyId = keyValue >> 8;
	
	guiKeyFifo[guiKeyFifo_w].keyValue = 0x01 << keyId;	
	guiKeyFifo[guiKeyFifo_w].keyStatus = GUI_KEY_PRESS;
	
	/* 如果上次有按键按下这次没有，说明按键已经释放 */
	if(((preKeyValue & 0x02) != 0x02) && ((keyValue & 0x02) == 0x02))
	{
		guiKeyFifo[guiKeyFifo_w].keyStatus = GUI_KEY_RELEASED;
	}	
	if(guiKeyFifo_w++ >= GUI_KEY_FIFO_SIZE) guiKeyFifo_w = 0;
}

/*
*********************************************************************************************************
*                            gui_onKey              
*
* Description: 处理按键按下
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_onKey(void)
{
	/* 没有按键按下 */
	if(guiKeyFifo_r == guiKeyFifo_w) return ;
	
	int16_t err = ERR_NONE;
	WIDGET_OBJ *pWidget = WIN_Desktop->pWidget;
	GUI_KEY_INFO *pKey = &guiKeyFifo[guiKeyFifo_r];
	if(guiKeyFifo_r++ >= GUI_KEY_FIFO_SIZE) guiKeyFifo_r = 0;
	
	while(pWidget)
	{
		if(pKey->keyValue & pWidget->actKey) /* 判断控件是否设置了响应该按键 */
		{
			/* 如果是就调用回调函数，对于一般控件来说，都有一个默认的回调函数 */
			pWidget->_cb(pWidget->id, pKey);
		}
		
		pWidget = pWidget->pNext;
	}
}

/********************************************  END OF FILE  *******************************************/

