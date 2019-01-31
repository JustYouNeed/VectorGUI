/**
  *******************************************************************************************************
  * File Name: gui_core.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-30
  * Brief: GUI�ں��ļ�
  *******************************************************************************************************
  * History
	*		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: �����ļ�
  *
  *******************************************************************************************************
  */

/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "gui.h"


GUI_CONTEXT GUI_Context;

/*
*********************************************************************************************************
*                          gui_gotoXY                
*
* Description: ��ת��ָ������
*             
* Arguments  : x,y: ָ������
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_gotoXY(uint16_t x, uint16_t y)
{
	GUI_Context.dispPosX = x;
	GUI_Context.dispPosY = y;
}

/*
*********************************************************************************************************
*                        gui_Init                  
*
* Description: GUI��ʼ������
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_Init(void)
{
	gui_deviceInit();
//	wm_Init();
	
	GUI_Context.curFont = GUI_FONT_ASCII_6X8;
	GUI_Context.dispPosX = 0;
	GUI_Context.dispPosY = 0;
	GUI_Context.drawPosX = 0;
	GUI_Context.drawPosY = 0;
	GUI_Context.drawRect.x0 = 0;
	GUI_Context.drawRect.y0 = 0;
	GUI_Context.drawRect.width = 127;
	GUI_Context.drawRect.height = 63;
}

/*
*********************************************************************************************************
*                              gui_clear            
*
* Description: GUI��������
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_clear(void)
{
	gui_fillSCR(0);
}

/*
*********************************************************************************************************
*                         gui_clearRect                 
*
* Description: ���һ���������
*             
* Arguments  : x0,y0: �����������Ͻǵ������
*							 width, height: �������򳤿�
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_clearRect(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height)
{
	gui_fillRectangle(x0, y0, x0 + width, y0 + height, 0);
}

void gui_onPaint()
{
	if(WIN_Desktop == NULL) return ;
	WIDGET_OBJ *pWidget = NULL;
	/* ֻ�������� */
	if(WIN_Desktop->isInvalid)	/* ֻ���ڴ�����Ч��ʱ�����Ҫ���� */
	{
		gui_clear();
		win_onPaint(WIN_Desktop);	/* �Ȼ��ƴ��� */
		pWidget = WIN_Desktop->pWidget;	/* ��ȡ�ؼ������ͷ */
		while(pWidget)		/* �����ô��ڵĿؼ�,���ػ� */
		{
			widget_onPaint(pWidget);
			pWidget = (pWidget->pNext);	/* ��ȡ��һ���ؼ� */
		}
		WIN_Desktop->isInvalid = false;	/* ��������Ϊ��Ч */
		gui_Refresh();	/* ������Ļ */
	}
}

/*
*********************************************************************************************************
*                      gui_excute                    
*
* Description: GUI�߳�ִ�к���������GUI���ػ��Լ���Ϣ�Ĵ���
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_excute(void)
{
//	gui_clear();
	gui_onPaint();
	msg_process();
//	gui_Refresh();	/* ������Ļ */
}


/********************************************  END OF FILE  *******************************************/

