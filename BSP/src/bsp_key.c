/**
  *******************************************************************************************************
  * File Name: KEY.c
  * Author: Vector
  * Version: V1.1.0
  * Date: 2018-4-23
  * Brief: ���ļ��ṩ���йز��������ĺ���,֧�ְ����Ķ���״̬���
	*					1.�������¼��
	*					2.�����������
	*					3.����������
	*					4.���������Զ�����
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-4-23
	*			Mod: �������ļ�
	*
	*		2.Author: Vector
	*			Date: 2018-9-18
	*			Mod: ����bsp_key_AddKey����,�����򰴼�ϵͳע�ᰴ��,��������Ҳ��Ϊע�����,ͬʱͳһ��ⰴ�����º���,
	*					 ��������ֲ
  *
  *******************************************************************************************************
  */	
/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "bsp.h"


/*
  *******************************************************************************************************
  *                              LOCAL DEFINITION
  *******************************************************************************************************
*/
/* �������Ŷ��� */
# define KEY_UP_PORT		GPIOA
# define KEY_UP_PIN			GPIO_Pin_6

# define KEY_DOWN_PORT	GPIOA
# define KEY_DOWN_PIN		GPIO_Pin_3

# define KEY_LEFT_PORT	GPIOB
# define KEY_LEFT_PIN		GPIO_Pin_15

# define KEY_RIGHT_PORT	GPIOB
# define KEY_RIGHT_PIN	GPIO_Pin_14

# define KEY_PWR_PORT		GPIOA
# define KEY_PWR_PIN		GPIO_Pin_0

/*  ��������ʱ��, ��λΪms,ȡ���ڶ��ʱ����һ������,
		���೤ʱ�����һ��bsp_key_Scan����*/
# define KEY_LONG_TIME		500
# define KEY_FILTER_TIME	5
# define KEY_MAX_COUNT	16
# define KEY_FIFO_SIZE	10

/*  �����ṹ��  */
typedef struct 
{
	uint16_t Fifo[KEY_FIFO_SIZE];
	uint8_t Read;
	uint8_t Write;
}keyFIFO_t;

/*  ÿ��������Ӧ1��ȫ�ֵĽṹ����� */
typedef struct
{	
	GPIO_TypeDef *port;
	uint16_t pin;
	keyMode_e mode;
	uint8_t filterCount;	/*  �����˲�������  */
	uint16_t longCount;		/*  ��������������  */
	uint16_t longTime;		/*  ��������ʱ��,0��ʾ����ⳤ��  */
	uint8_t  state;				/*  ������ǰ״̬,���»��ǵ���  */
	uint8_t repeatSpeed;	/*  ������������  */
	uint8_t repeatCount;	/*  ��������������  */
}keyCtrl_t;
	

/*
  *******************************************************************************************************
  *                              LOCAL VARIABLE
  *******************************************************************************************************
*/
/*  ����FIFO  */
static keyFIFO_t		 keyFifo = {.Read = 0, .Write = 0,};  
static keyCtrl_t		 KEY[KEY_MAX_COUNT];  
static bool isInit = false;
static uint8_t keyCount = 0;

/*
*********************************************************************************************************
*                           bsp_key_IsPress               
*
* Description: �жϰ����Ƿ���
*             
* Arguments  : ����id
*
* Reutrn     : ����״̬,������·�����
*
* Note(s)    : None.
*********************************************************************************************************
*/
static bool bsp_key_IsPress(uint8_t keyId)
{
	if(keyId > keyCount) return false;
	
	return (PIN_READ(KEY[keyId].port, KEY[keyId].pin) != (uint8_t)KEY[keyId].mode);
}


/*
*********************************************************************************************************
*                     bsp_key_AddKey                     
*
* Description: ��ϵͳע��һ������
*             
* Arguments  : 1.keyId: ����ID,Ӧ����Ҫ��bsp_key.h�İ���IDö��������
*							 2.*keyPort: ������ʹ�õ�GPIO�˿�
*							 3.keyPin: ������ʹ�õ�GPIO����
*							 4.keyMode: ����ģʽ,ָ����������֮��ĵ�ƽ״̬,��bsp_key.h�ж���
*
* Reutrn     : None.
*
* Note(s)    : ��ע�ᰴ��ʱһ��Ҫ��bsp_key.h�а���ID������Ⱥ�˳��ע��,�������
*********************************************************************************************************
*/
void bsp_key_AddKey(uint8_t keyId, GPIO_TypeDef *keyPort, uint16_t keyPin, keyMode_e keyMode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	bsp_rcc_GPIOClockCmd(keyPort);
	
# ifdef VECTOR_F1
	if(keyMode == KEY_MODE_A)		/*  �������º��Ǹߵ�ƽ������Ϊ��������  */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	else if(keyMode == KEY_MODE_K)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
# else	
	if(keyMode == KEY_MODE_A)		/*  �������º��Ǹߵ�ƽ������Ϊ��������  */
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	else if(keyMode == KEY_MODE_K)
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
# endif
	
	GPIO_Init(keyPort, &GPIO_InitStructure);
	
	KEY[keyId].port = keyPort;
	KEY[keyId].pin = keyPin;
	KEY[keyId].mode = keyMode;
	KEY[keyId].longTime = KEY_LONG_TIME;
	KEY[keyId].filterCount = KEY_FILTER_TIME / 2;
	KEY[keyId].state = KEY_NONE;
	KEY[keyId].repeatCount = 0;
	KEY[keyId].repeatSpeed = 10;
	
	keyCount++;
}


/*
*********************************************************************************************************
*                          bsp_key_Init                
*
* Description: ��ʼ������
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_key_Init(void)
{
	if(isInit) return ;
	bsp_key_AddKey(KEY_UP, KEY_UP_PORT, KEY_UP_PIN, KEY_MODE_K);
	bsp_key_AddKey(KEY_DOWN, KEY_DOWN_PORT, KEY_DOWN_PIN, KEY_MODE_K);
	bsp_key_AddKey(KEY_LEFT, KEY_LEFT_PORT, KEY_LEFT_PIN, KEY_MODE_K);
	bsp_key_AddKey(KEY_RIGHT, KEY_RIGHT_PORT, KEY_RIGHT_PIN, KEY_MODE_K);
	bsp_key_AddKey(KEY_PWR, KEY_PWR_PORT, KEY_PWR_PIN, KEY_MODE_K);
	isInit = true;
}

/*
*********************************************************************************************************
*                          bsp_key_SetPara                
*
* Description: ���ð�������
*             
* Arguments  : 1> KeyID: ����ID,��KEY.h�ж���
*							 2> longTime: �����¼�ʱ��
*							 3> repeatSpeed: ���������ٶ�
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_key_SetPara(uint8_t keyId, uint16_t longTime, uint16_t repeatSpeed)
{
	KEY[keyId].longTime = longTime;
	KEY[keyId].longCount = 0;
	
	KEY[keyId].repeatSpeed = repeatSpeed;
	KEY[keyId].repeatCount = 0;
}


/*
*********************************************************************************************************
*                          bsp_GetKey                
*
* Description: �Ӱ���FIFO�л�ȡһ������ֵ
*             
* Arguments  : None.
*
* Reutrn     : ����ֵ,��ֵ��KEY.h�ж���
*
* Note(s)    : None.
*********************************************************************************************************
*/
uint16_t bsp_key_GetKey(void)
{
	if(!isInit) return 0;
	
	uint16_t key;
	if(keyFifo.Read == keyFifo.Write) 
	{
		return KEY_NONE;
	}
	else
	{
		key = keyFifo.Fifo[keyFifo.Read];
		if( ++keyFifo.Read >= KEY_FIFO_SIZE) keyFifo.Read = 0;
		return key;
	}
}

/*
*********************************************************************************************************
*                        bsp_PutKey                  
*
* Description: ����һ������ֵ������FIFO��
*             
* Arguments  : 1.> keyValue: ����ֵ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_key_PutKey(uint16_t keyValue)
{
	keyFifo.Fifo[keyFifo.Write] = keyValue;
	
	if(++keyFifo.Write >= KEY_FIFO_SIZE)
		keyFifo.Write = 0;
}


/*
*********************************************************************************************************
*                          bsp_GetKeyState                
*
* Description: ��ȡһ��������״̬
*             
* Arguments  : 1> keyId: ����ID,��ֵ��KEY.h�ж���
*
* Reutrn     : ��Ӧ������״̬
*
* Note(s)    : None.
*********************************************************************************************************
*/
uint8_t bsp_key_GetState(keyId_e keyId)
{
	return KEY[keyId].state;
}

/*
*********************************************************************************************************
*                         bsp_key_Clear                 
*
* Description: ��հ���FIFO
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_key_Clear(void)
{
	keyFifo.Read = keyFifo.Write;
}

/*
*********************************************************************************************************
*                        bsp_key_Detect                  
*
* Description: ��ⰴ��,������
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : �ú���Ϊ���ļ��ڲ�����,�ⲿ��ֹ����
*********************************************************************************************************
*/
void bsp_key_Detect(uint8_t keyId)
{
	keyCtrl_t *pKey;
	
	pKey = &KEY[keyId];	/*  �Ȼ�ȡ�������Ľṹ��  */
	
	/*  �ж��Ƿ���  */
	if(bsp_key_IsPress(keyId))
	{
		if(pKey->filterCount < KEY_FILTER_TIME) 
			pKey->filterCount  = KEY_FILTER_TIME;
		else if(pKey->filterCount < 2 * KEY_FILTER_TIME) 
			pKey->filterCount ++;
		else
		{
			if(pKey->state == 0)	/*  ����ϸ�ʱ�̵İ���״̬Ϊ����״̬  */
			{
				/*  ����������  */				
				pKey->state = 1;
				
				/*  ���Ͱ���ֵ������FIFO  */
				bsp_key_PutKey((uint16_t)((keyId << 8) + 1));
			}
			
			/*  ����ʱ�������,˵�������˳�����⹦��  */
			if(pKey->longTime > 0)
			{
				/*  ��⵽�˳���  */
				if(pKey->longCount < pKey->longTime)
				{
					if(++pKey->longCount == pKey->longTime)
					{
						/*  ���ͳ�����Ϣ������FIFO  */
						bsp_key_PutKey((uint16_t)((keyId << 8) + 3));
					}
				}
				else	/*  ����Ѿ������˳���ʱ��,�򿴿��Ƿ����˰�������  */
				{
					/*  �����˰�����������  */
					if(pKey->repeatSpeed > 0)
					{
						if(++pKey->repeatCount >= pKey->repeatSpeed)
						{
							pKey->repeatCount = 0;
							bsp_key_PutKey((uint16_t)((keyId << 8) + 1));
						}
					}
				}
			}
		}
	}
	else	/*  û�б������򽫰���״̬����Ϊ����״̬,�����͵�FIFO  */
	{
		if(pKey->filterCount > KEY_FILTER_TIME)
			pKey->filterCount  = KEY_FILTER_TIME;
		else if(pKey->filterCount != 0)
			pKey->filterCount -- ;
		else
		{
			if(pKey->state == 1)
			{
				pKey->state = 0;
				bsp_key_PutKey((uint16_t)((keyId << 8) + 2));
			}
		}
		
		pKey->repeatCount = 0;
		pKey->longCount = 0;
	}
}

/*
*********************************************************************************************************
*                            bsp_key_Scan              
*
* Description: ����ɨ�躯��
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : ������Ϊ����ɨ�躯��,��ʹ��bsp_key_GetKey֮ǰӦ�ȵ��ñ�����,���߿��������Ե���,Ȼ�����
*								bsp_key_GetKey�Ӱ���FIFO�л�ȡɨ�赽�İ���
*********************************************************************************************************
*/
void bsp_key_Scan(void)
{
	uint8_t i = 0;
	
	/*  ѭ��ɨ��ÿһ������  */
	for(i = 0; i < keyCount; i++)
	{
		bsp_key_Detect(i);
	}
}

/********************************************  END OF FILE  *******************************************/






