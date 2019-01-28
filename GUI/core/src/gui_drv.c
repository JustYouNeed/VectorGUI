/**
  *******************************************************************************************************
  * File Name: gui_drv.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI底层驱动文件,根据用户实际情况修改，本文件参考正点原子遥控中采用的GUI结构,原作者为黄绍斌
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-29
	*			Mod: 建立文件,添加基本函数
	*		
  *******************************************************************************************************
  */	
/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "gui.h"
# include "bsp.h"
/*
*********************************************************************************************************
*                        gui_Init                  
*
* Description: 初始化GUI，包括底层屏幕驱动，清屏
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : 需要用户根据实际情况修改
*********************************************************************************************************
*/
void gui_deviceInit(void)
{
	bsp_oled_Init();
	bsp_oled_Clear();
}

/*
*********************************************************************************************************
*                          gui_fillSCR                
*
* Description: 全屏填充。直接使用数据填充显示缓冲区。
*             
* Arguments  : color: 填充颜色
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_fillSCR(GUI_COLOR color)
{
	bsp_oled_Fill(0, 0, LCD_X - 1, LCD_Y - 1, color);
}

/*
*********************************************************************************************************
*                      gui_clearSCR                    
*
* Description:  清屏函数
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : 需要根据用户实际情况修改
*********************************************************************************************************
*/
void gui_clearSCR(void)
{
	bsp_oled_Clear();
}

/*
*********************************************************************************************************
*                       gui_drawPoint                   
*
* Description: GUI打点函数
*             
* Arguments  : x,y: 点的坐标
*							 color: 点的颜色，对于单色屏,为0时灭，为1是显示
*
* Reutrn     : None.
*
* Note(s)    : 需要根据用户实际情况修改
*********************************************************************************************************
*/
void gui_drawPoint(uint8_t x, uint8_t y, GUI_COLOR color)
{
	if(x > (GUI_Context.drawRect.x0 + GUI_Context.drawRect.width)) return ;
	if(x < GUI_Context.drawRect.x0) return ;
	
	if(y > (GUI_Context.drawRect.y0 + GUI_Context.drawRect.height)) return ;
	if(y < GUI_Context.drawRect.y0) return ;
	
	bsp_oled_DrawPoint(x, y, color);
}

/*
*********************************************************************************************************
*                     gui_readPoint                     
*
* Description: GUI读点函数
*             
* Arguments  : x,y：点的坐标
*
* Reutrn     : 指定点的颜色值，返回0表示指定地址超出缓冲区范围
*
* Note(s)    : 需要根据用户实际情况修改
*********************************************************************************************************
*/
uint8_t gui_readPoint(uint8_t x, uint8_t y)
{
	return bsp_oled_ReadPoint(x, y);
}

/*
*********************************************************************************************************
*                     gui_Refresh                     
*
* Description: GUI刷新函数
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : 需要用户根据实际情况修改
*********************************************************************************************************
*/
void gui_Refresh(void)
{
	bsp_oled_Refresh();
}


/*
*********************************************************************************************************
*                           gui_keyInput               
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
void gui_keyInput(void)
{
	static uint16_t preKeyValue = 0x01;
	uint16_t nowKeyValue = 0, keyId = 0;
	MSG_KEY_INFO keyInfo;
	
	uint16_t keyValue = 0, msgKeyValue = 0;
	
	nowKeyValue = bsp_key_GetKey();
	if(nowKeyValue == KEY_NONE) return ;
	
	keyId = nowKeyValue >> 8;
	
	
	keyInfo.keyValue = 0x01 << keyId;
	/* 如果上次有按键按下这次没有，说明按键已经释放 */
	if(((preKeyValue & 0x02) != 0x02) && ((nowKeyValue & 0x02) == 0x02))
	{
		keyInfo.keyStatus = KEY_RELEASED;
		msg_inputKey(&keyInfo);
		return ;
	}
	keyInfo.keyStatus = KEY_PRESS;
	msg_inputKey(&keyInfo);
}


/********************************************  END OF FILE  *******************************************/

