/**
  *******************************************************************************************************
  * File Name: gui_key.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2019-2-1
  * Brief: GUI�İ������벿��
  *******************************************************************************************************
  * History
	*		1.Author: Vector
	*			Date: 2019-2-1
	*			Mod: �����ļ�
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
* Description: ��GUI����һ��������Ϣ
*             
* Arguments  : keyValue: ����ֵ
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
	
	/* ����ϴ��а����������û�У�˵�������Ѿ��ͷ� */
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
* Description: ����������
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
	/* û�а������� */
	if(guiKeyFifo_r == guiKeyFifo_w) return ;
	
	int16_t err = ERR_NONE;
	WIDGET_OBJ *pWidget = WIN_Desktop->pWidget;
	GUI_KEY_INFO *pKey = &guiKeyFifo[guiKeyFifo_r];
	if(guiKeyFifo_r++ >= GUI_KEY_FIFO_SIZE) guiKeyFifo_r = 0;
	
	while(pWidget)
	{
		if(pKey->keyValue & pWidget->actKey) /* �жϿؼ��Ƿ���������Ӧ�ð��� */
		{
			/* ����Ǿ͵��ûص�����������һ��ؼ���˵������һ��Ĭ�ϵĻص����� */
			pWidget->_cb(pWidget->id, pKey);
		}
		
		pWidget = pWidget->pNext;
	}
}

/********************************************  END OF FILE  *******************************************/

