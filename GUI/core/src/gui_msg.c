/**
  *******************************************************************************************************
  * File Name: gui_msg.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2019-1-2
  * Brief: GUI����Ϣ���Ʋ���
  *******************************************************************************************************
  * History
	*		1.Author: Vector
	*			Date: 2019-1-2
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


/*
  *******************************************************************************************************
  *                              LOCAL VARIABLE
  *******************************************************************************************************
*/
static WM_MESSAGE msg[MSG_FIFO_SIZE];
extern WM_HWIN hDesk;
extern WM_OBJ *WM_Desktop;
static uint16_t msgCnt = 0;

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
void msg_inputKey(MSG_KEY_INFO *keyInfo)
{
	if(msgCnt >= MSG_FIFO_SIZE) return ;
	
	
	msg[msgCnt].msgId = MSG_KEY;
	msg[msgCnt].key.keyStatus = keyInfo->keyStatus;
	msg[msgCnt].key.keyValue = keyInfo->keyValue;
	msg[msgCnt].data.p = (void *)keyInfo;
	msg[msgCnt].hWin = WM_Desktop->hWin;
	msgCnt++;
//	msg_sendMessage(hDesk, &msg[msgCnt]);
//	msgCnt++;
}

/*
*********************************************************************************************************
*                               msg_sendMsgNoData           
*
* Description: ����û�����ݵ���Ϣ
*             
* Arguments  : hWin: ���ڴ��������
*							 msgId: ��ϢID
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void msg_sendMsgNoData(WM_HWIN hWin, uint32_t msgId)
{
	if(msgCnt >= MSG_FIFO_SIZE) return ;
//	WM_MESSAGE pMsg;
//	pMsg.hWin = hWin;
//	pMsg.msgId = msgId;
//	
//	msg_sendMessage(hWin, &pMsg);	if(msgCnt > MSG_FIFO_SIZE) return ;
	
	msg[msgCnt].msgId = msgId;
	msg[msgCnt].hWin = hWin;
	msgCnt++;
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
void msg_sendMessage(WM_HWIN hWin, WM_MESSAGE *pMsg)
{
	if(msgCnt >= MSG_FIFO_SIZE) return ;
	
	msg[msgCnt].data.p = pMsg->data.p;
	msg[msgCnt].data.v = pMsg->data.v;
	msg[msgCnt].hWin = hWin;
	msg[msgCnt].msgId = pMsg->msgId;
	msgCnt++;
//	uint8_t hParent = GUI_GET_HPARENT(hWin);
//	uint8_t widgetType = GUI_GET_WIDGET_TYPE(hWin);
//	uint8_t hWidget = GUI_GET_HWIDGET(hWin);
//	
//	WIDGET_OBJ *pObj = NULL;
//	int16_t err;
//	
//	pObj = widget_getWidget(hWin, &err);
//	if(!pObj) return ;
//	
//	if(pObj->_cb != NULL)
//		(*pObj->_cb)(pMsg);
//	else
//		wm_defaultProc(pMsg);	
//	msgCnt--;
}

void msg_notifyWidget(WIDGET_HANDLE hWidget, WM_MESSAGE *pMsg)
{
	
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
void msg_process(void)
{
	if(msgCnt <= 0) return ; /* û����Ϣ��Ҫ���� */
	int16_t err = ERR_NONE;
	WIDGET_OBJ *pObj = WM_Desktop->pWidget;
	
	msgCnt--;
	
	WM_MESSAGE *pMsg = &msg[msgCnt];
	
	switch(pMsg->msgId)
	{
		case MSG_KEY:	/* ���ڰ�����Ϣ����Ҫ����ÿһ���ؼ� */
		{
			MSG_KEY_INFO *key = (MSG_KEY_INFO *)&(pMsg->key);
			while(pObj)
			{
				/* ����ÿؼ�ע������Ӧ�����¼�����Ӧ */
				if(key->keyValue & pObj->actKey)
				{
					pMsg->hWin = pObj->id;
					pObj->_cb(pMsg);
				}
				pObj = pObj->pNext;
			}
		};break;
		case MSG_PAINT:
		{
			pObj = widget_getWidget(pMsg->hWin, &err);
			pObj->_cb(pMsg);
		}
	}
}

/********************************************  END OF FILE  *******************************************/

