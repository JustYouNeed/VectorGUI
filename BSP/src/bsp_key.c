/**
  *******************************************************************************************************
  * File Name: KEY.c
  * Author: Vector
  * Version: V1.1.0
  * Date: 2018-4-23
  * Brief: 本文件提供了有关操作按键的函数,支持按键的多种状态检测
	*					1.按键按下检测
	*					2.按键长按检测
	*					3.按键弹起检测
	*					4.按键长按自动连发
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-4-23
	*			Mod: 建立本文件
	*
	*		2.Author: Vector
	*			Date: 2018-9-18
	*			Mod: 新增bsp_key_AddKey函数,用于向按键系统注册按键,按键机制也改为注册机制,同时统一检测按键按下函数,
	*					 更方便移植
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
/* 按键引脚定义 */
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

/*  按键长按时间, 单位为ms,取决于多久时间检测一个按键,
		即多长时间调用一个bsp_key_Scan函数*/
# define KEY_LONG_TIME		500
# define KEY_FILTER_TIME	5
# define KEY_MAX_COUNT	16
# define KEY_FIFO_SIZE	10

/*  按键结构体  */
typedef struct 
{
	uint16_t Fifo[KEY_FIFO_SIZE];
	uint8_t Read;
	uint8_t Write;
}keyFIFO_t;

/*  每个按键对应1个全局的结构体变量 */
typedef struct
{	
	GPIO_TypeDef *port;
	uint16_t pin;
	keyMode_e mode;
	uint8_t filterCount;	/*  按键滤波计数器  */
	uint16_t longCount;		/*  按键长按计数器  */
	uint16_t longTime;		/*  按键长度时间,0表示不检测长按  */
	uint8_t  state;				/*  按键当前状态,按下还是弹起  */
	uint8_t repeatSpeed;	/*  按键连发周期  */
	uint8_t repeatCount;	/*  按键连发计数器  */
}keyCtrl_t;
	

/*
  *******************************************************************************************************
  *                              LOCAL VARIABLE
  *******************************************************************************************************
*/
/*  按键FIFO  */
static keyFIFO_t		 keyFifo = {.Read = 0, .Write = 0,};  
static keyCtrl_t		 KEY[KEY_MAX_COUNT];  
static bool isInit = false;
static uint8_t keyCount = 0;

/*
*********************************************************************************************************
*                           bsp_key_IsPress               
*
* Description: 判断按键是否按下
*             
* Arguments  : 按键id
*
* Reutrn     : 按下状态,如果按下返回真
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
* Description: 向系统注册一个按键
*             
* Arguments  : 1.keyId: 按键ID,应该需要在bsp_key.h的按键ID枚举中声明
*							 2.*keyPort: 按键所使用的GPIO端口
*							 3.keyPin: 按键所使用的GPIO引脚
*							 4.keyMode: 按键模式,指定按键按下之后的电平状态,在bsp_key.h中定义
*
* Reutrn     : None.
*
* Note(s)    : 在注册按键时一定要按bsp_key.h中按键ID定义的先后顺序注册,否则出错
*********************************************************************************************************
*/
void bsp_key_AddKey(uint8_t keyId, GPIO_TypeDef *keyPort, uint16_t keyPin, keyMode_e keyMode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	bsp_rcc_GPIOClockCmd(keyPort);
	
# ifdef VECTOR_F1
	if(keyMode == KEY_MODE_A)		/*  按键按下后是高电平则设置为下拉输入  */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	else if(keyMode == KEY_MODE_K)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
# else	
	if(keyMode == KEY_MODE_A)		/*  按键按下后是高电平则设置为下拉输入  */
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
* Description: 初始化按键
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
* Description: 设置按键参数
*             
* Arguments  : 1> KeyID: 按键ID,在KEY.h中定义
*							 2> longTime: 长按事件时间
*							 3> repeatSpeed: 按键连发速度
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
* Description: 从按键FIFO中获取一个按键值
*             
* Arguments  : None.
*
* Reutrn     : 按键值,其值在KEY.h中定义
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
* Description: 推送一个按键值到按键FIFO中
*             
* Arguments  : 1.> keyValue: 按键值
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
* Description: 获取一个按键的状态
*             
* Arguments  : 1> keyId: 按键ID,其值在KEY.h中定义
*
* Reutrn     : 对应按键的状态
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
* Description: 清空按键FIFO
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
* Description: 检测按键,非阻塞
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : 该函数为本文件内部函数,外部禁止调用
*********************************************************************************************************
*/
void bsp_key_Detect(uint8_t keyId)
{
	keyCtrl_t *pKey;
	
	pKey = &KEY[keyId];	/*  先获取到按键的结构体  */
	
	/*  判断是否按下  */
	if(bsp_key_IsPress(keyId))
	{
		if(pKey->filterCount < KEY_FILTER_TIME) 
			pKey->filterCount  = KEY_FILTER_TIME;
		else if(pKey->filterCount < 2 * KEY_FILTER_TIME) 
			pKey->filterCount ++;
		else
		{
			if(pKey->state == 0)	/*  如果上个时刻的按键状态为弹起状态  */
			{
				/*  按键被按下  */				
				pKey->state = 1;
				
				/*  推送按键值到按键FIFO  */
				bsp_key_PutKey((uint16_t)((keyId << 8) + 1));
			}
			
			/*  长按时间大于零,说明开启了长按检测功能  */
			if(pKey->longTime > 0)
			{
				/*  检测到了长按  */
				if(pKey->longCount < pKey->longTime)
				{
					if(++pKey->longCount == pKey->longTime)
					{
						/*  推送长按消息到按键FIFO  */
						bsp_key_PutKey((uint16_t)((keyId << 8) + 3));
					}
				}
				else	/*  如果已经超过了长按时间,则看看是否开启了按键连发  */
				{
					/*  开启了按键连发功能  */
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
	else	/*  没有被按下则将按键状态设置为弹起状态,并推送到FIFO  */
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
* Description: 按键扫描函数
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : 本函数为按键扫描函数,在使用bsp_key_GetKey之前应先调用本函数,或者可以周期性调用,然后调用
*								bsp_key_GetKey从按键FIFO中获取扫描到的按键
*********************************************************************************************************
*/
void bsp_key_Scan(void)
{
	uint8_t i = 0;
	
	/*  循环扫描每一个按键  */
	for(i = 0; i < keyCount; i++)
	{
		bsp_key_Detect(i);
	}
}

/********************************************  END OF FILE  *******************************************/






