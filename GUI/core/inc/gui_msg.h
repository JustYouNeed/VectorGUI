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
# ifndef __GUI_MSG_H
# define __GUI_MSG_H

# define MSG_CREATE													1
# define MSG_DELETE													2
# define MSG_PAINT													3
# define MSG_KEY														4
# define MSG_GET_ID													5
# define MSG_SET_ID													6
# define MSG_GET_CLIENT_WINDOW							7
# define MSG_INIT_DIALOG										8
# define MSG_USER														40


# define MSG_FIFO_SIZE 		20
typedef struct WM_MESSAGE WM_MESSAGE;
typedef enum MSG_KEY_ENUM	MSG_KEY_ENUM;
typedef struct MSG_KEY_INFO MSG_KEY_INFO;


struct MSG_KEY_INFO
{
	uint16_t keyValue;
	uint8_t keyStatus;
};

typedef enum
{
	KEY_PRESS = 0x01,
	KEY_RELEASED = 0x02,
}GUI_KEY_STATUS;

/* ���뵽GUI�İ�����Ϣ */
enum MSG_KEY_ENUM
{
	MSG_KEY_NULL = 0x00,
	MSG_KEY_UP = 0x01 << 0,
	MSG_KEY_DOWN = 0x01 << 1,
	MSG_KEY_LEFT = 0x01 << 2,
	MSG_KEY_RIGHT = 0x01 << 3,
	MSG_KEY_PWR = 0x01 << 4,
};


typedef struct 
{
	uint16_t msgCnt;
	uint16_t msg[MSG_FIFO_SIZE];
}MSG_FIFO;

/* ������Ϣ�ṹ�� */
struct WM_MESSAGE
{
	int16_t msgId;		/* ��ϢID */
	WM_HWIN hWin;	/* ��Ϣ����Ŀ�괰�� */
	WM_HWIN hWinSrc;  /* ��Ϣ����Դ���� */
	MSG_KEY_INFO key;
	union
	{
		void *p;
		int16_t v;
	}data;
};

void msg_inputKey(MSG_KEY_INFO *keyInfo);
void msg_process(void);
void msg_sendMessage(WM_HWIN hWin, WM_MESSAGE *pMsg);
void msg_sendMsgNoData(WM_HWIN hWin, uint32_t msgId);
# endif

