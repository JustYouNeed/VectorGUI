/**
  *******************************************************************************************************
  * File Name: bsp_oled.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-23
  * Brief: OLED屏幕驱动
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-4
	*			Mod: 建立本文件
  *
  *******************************************************************************************************
  */	
	
/*
    *******************************************************************************************************
    *                              INCLUDE FILES
    *******************************************************************************************************
  */	
#include "font.h" 
#include "bsp.h" 	 


/*
  *******************************************************************************************************
  *                              LOCAL DEFINITION
  *******************************************************************************************************
*/
# define OLED_SDA_PORT	GPIOB
# define OLED_SDA_PIN		GPIO_Pin_12
# define OLED_DIN_Clr()	GPIO_ResetBits(OLED_SDA_PORT, OLED_SDA_PIN)
# define OLED_DIN_Set()	GPIO_SetBits(OLED_SDA_PORT, OLED_SDA_PIN)

# define OLED_SCL_PORT	GPIOB
# define OLED_SCL_PIN		GPIO_Pin_13
# define OLED_SCK_Clr()	GPIO_ResetBits(OLED_SCL_PORT, OLED_SCL_PIN)
# define OLED_SCK_Set()	GPIO_SetBits(OLED_SCL_PORT, OLED_SCL_PIN)

# define OLED_DC_PORT		GPIOA
# define OLED_DC_PIN		GPIO_Pin_7
# define OLED_DC_Clr()	GPIO_ResetBits(OLED_DC_PORT, OLED_DC_PIN)
# define OLED_DC_Set()	GPIO_SetBits(OLED_DC_PORT, OLED_DC_PIN)

# define OLED_RST_PORT	GPIOB
# define OLED_RST_PIN		GPIO_Pin_10
# define OLED_RST_Clr()	GPIO_ResetBits(OLED_RST_PORT, OLED_RST_PIN)
# define OLED_RST_Set()	GPIO_SetBits(OLED_RST_PORT, OLED_RST_PIN)




/*
  *******************************************************************************************************
  *                              LOCAL VARIABLE
  *******************************************************************************************************
*/
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
volatile static uint8_t oledGram[128][8];
	
/*
*********************************************************************************************************
*                    bsp_oled_WriteByte                      
*
* Description: 向OLED中写入一个字节的数据或者命令
*             
* Arguments  : 1.dat:要写入的数据.
*							 2.cmd:要写入的数据或者命令.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
static void bsp_oled_WriteByte(uint8_t dat,uint8_t cmd)
{		  
	uint8_t i;
	
	if(cmd)					/*  命令  */
	  OLED_DC_Set();
	else 						/*  数据  */
	  OLED_DC_Clr();		  
	
	
	for(i=0;i<8;i++)	/*  循环发送  */
	{			  
		OLED_SCK_Clr();
		if(dat&0x80)
		   OLED_DIN_Set();
		else 
		   OLED_DIN_Clr();
		OLED_SCK_Set();
		dat<<=1;   
	}				 		  
	OLED_DC_Set();     	  
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
void bsp_oled_SetDisplayFlip(oledDirFilp_e flip)
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
void bsp_oled_SetBrightness(uint8_t level)
{
	bsp_oled_WriteByte(0x81,OLED_CMD); //对比度设置
	bsp_oled_WriteByte(level,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
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
void bsp_oled_Refresh(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		bsp_oled_WriteByte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		bsp_oled_WriteByte (0x00,OLED_CMD);      //设置显示位置—列低地址
		bsp_oled_WriteByte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
			bsp_oled_WriteByte(oledGram[n][i],OLED_DATA); 
	}   
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
void bsp_oled_DisplayEnabled(bool enabled)
{
	if(!enabled)
	{
		bsp_oled_WriteByte(0X8D,OLED_CMD);  //SET DCDC命令
		bsp_oled_WriteByte(0X10,OLED_CMD);  //DCDC OFF
		bsp_oled_WriteByte(0XAE,OLED_CMD);  //DISPLAY OFF
	}
	else
	{
		bsp_oled_WriteByte(0X8D,OLED_CMD);  //SET DCDC命令
		bsp_oled_WriteByte(0X14,OLED_CMD);  //DCDC ON
		bsp_oled_WriteByte(0XAF,OLED_CMD);  //DISPLAY ON
	}
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
void bsp_oled_Clear(void)  
{  
	uint8_t i,n;  
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			oledGram[n][i]=0X00;  
	bsp_oled_Refresh();//更新显示
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
void bsp_oled_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)oledGram[x][pos]|=temp;
	else oledGram[x][pos]&=~temp;	    
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
uint8_t bsp_oled_ReadPoint(uint8_t x,uint8_t y)
{
	uint8_t pos,bx,temp=0x00;
	//x = 127-x;
	y = 63-y;
	pos=y/8;
	bx=y%8;
	temp=1<<bx;
  if(temp&oledGram[x][pos]) return 1;
	return 0;
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
void bsp_oled_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
		for(y=y1;y<=y2;y++)
			bsp_oled_DrawPoint(x,y,dot);											    
//	bsp_oled_Refresh();//更新显示
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
void bsp_oled_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t f_w,uint8_t f_h,uint8_t mode)
{      			    
//	uint8_t temp,t,t1;
//	uint8_t y0=y;	
//	uint8_t csize=(f_h/8+((f_h%8)?1:0))*f_w;//得到自由分辨字符所占的字节数
//	chr=chr-' ';//得到偏移后的值		 
//	for(t=0;t<csize;t++)
//	{   
//		if(f_w==6&&f_h==8)temp=asc2_0608[chr][t];		//调用0608ascii字体
//		else if(f_w==6&&f_h==12)temp=asc2_0612[chr][t];	//调用0612ascii字体
//		else if(f_w==12&&f_h==24)temp=asc2_1224[chr][t];//调用1224ascii字体
//		else return;	//没有的字库
//		for(t1=0;t1<8;t1++)
//		{
//			if(temp&0x80)bsp_oled_DrawPoint(x,y,mode);
//			else bsp_oled_DrawPoint(x,y,!mode);
//			temp<<=1;
//			y++;
//			if((y-y0)==f_h)
//			{
//				y=y0;
//				x++;
//				break;
//			}
//		}  	 
//	}     
}
//m^n函数
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
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
void bsp_oled_ShowNum(uint8_t x,uint8_t y,int32_t num,uint8_t len,uint8_t f_w,uint8_t f_h)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;				

	if(num < 0) 
	{
		num = -num;
//		bsp_oled_ShowChar(x, y, '-', f_w, f_h, 1);
//		x += f_w;
	}
	
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				bsp_oled_ShowChar(x+(f_w)*t,y,' ',f_w,f_h,1);
				continue;
			}
			else 
				enshow=1; 
		}
	 	bsp_oled_ShowChar(x+(f_w)*t,y,temp+'0',f_w,f_h,1); 
	}
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
void bsp_oled_ShowString(uint8_t x, uint8_t y, uint8_t *str, uint8_t f_w, uint8_t f_h)
{	
    while((*str<='~')&&(*str>=' '))//判断是不是非法字符!
    {       
        if(x>(128-(f_w))){x=0;y+=f_h;}
        if(y>(64-f_h)){y=x=0;bsp_oled_Clear();}
        bsp_oled_ShowChar(x,y,*str,f_w,f_h,1);	 
        x+=f_w;
        str++;
    }  
	
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
void bsp_oled_ShowPicture(uint8_t x,uint8_t y,const uint8_t *p,uint8_t p_w,uint8_t p_h)
{	
	uint8_t temp,i,col,row;
	uint8_t y0=y;
	uint8_t width=p_w;
	uint8_t high=p_h;
	uint8_t exp_col_bytes;
	uint8_t act_col_bytes;
	
	if(x+p_w>128)width=128-p_w;//实际显示宽度
	
	if(y+p_h>64)high=64-p_h;//实际显示高度
	exp_col_bytes=(p_h/8+((p_h%8)?1:0));//显示一列的字节数
	act_col_bytes=(high/8+((high%8)?1:0));//实际显示一列的字节数
	
	for(row=0;row<width;row++)//列++
	{
		for(col=0;col<act_col_bytes;col++)//显示一列
		{   
			temp = p[col+row*exp_col_bytes];
			for(i=0;i<8;i++)
			{
				if(temp&0x80)bsp_oled_DrawPoint(x,y,1);
				else bsp_oled_DrawPoint(x,y,0);
				temp<<=1;
				y++;
				if((y-y0)==high)
				{
					y=y0;
					x++;
					break;
				}		
			} 
		}
	}		
}


/*
*********************************************************************************************************
*                      bsp_oled_GPIOInit                    
*
* Description: OLED显示屏引脚初始化
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
static void bsp_oled_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	bsp_rcc_GPIOClockCmd(OLED_SDA_PORT);
	bsp_rcc_GPIOClockCmd(OLED_SCL_PORT);
	bsp_rcc_GPIOClockCmd(OLED_DC_PORT);
	bsp_rcc_GPIOClockCmd(OLED_RST_PORT);

	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	
	GPIO_InitStructure.GPIO_Pin = OLED_SDA_PIN;
	GPIO_Init(OLED_SDA_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = OLED_SCL_PIN;
	GPIO_Init(OLED_SCL_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = OLED_RST_PIN;
	GPIO_Init(OLED_RST_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = OLED_DC_PIN;
	GPIO_Init(OLED_DC_PORT, &GPIO_InitStructure);
}
void bsp_oled_SetPos(uint8_t x, uint8_t y) 
{ 
	bsp_oled_WriteByte(0xb0+y,OLED_CMD);
	bsp_oled_WriteByte(((x&0xf0)>>4)|0x10,OLED_CMD);
	bsp_oled_WriteByte((x&0x0f)|0x01,OLED_CMD); 
}   	
/*
*********************************************************************************************************
*                                     bsp_oled_Config     
*
* Description: 初始化OLED屏幕
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void bsp_oled_Init(void)
{
	bsp_oled_GPIOInit();
	
	OLED_RST_Set();
	vTaskDelay(100);
	OLED_RST_Clr();
	vTaskDelay(100);
	OLED_RST_Set(); 
					  
	
	bsp_oled_WriteByte(0xAE,OLED_CMD); //关闭显示
	bsp_oled_WriteByte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	bsp_oled_WriteByte(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
	bsp_oled_WriteByte(0xA8,OLED_CMD); //设置驱动路数
	bsp_oled_WriteByte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	bsp_oled_WriteByte(0xD3,OLED_CMD); //设置显示偏移
	bsp_oled_WriteByte(0X00,OLED_CMD); //默认为0

	bsp_oled_WriteByte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
													    
	bsp_oled_WriteByte(0x8D,OLED_CMD); //电荷泵设置
	bsp_oled_WriteByte(0x14,OLED_CMD); //bit2，开启/关闭
	bsp_oled_WriteByte(0x20,OLED_CMD); //设置内存地址模式
	bsp_oled_WriteByte(0x00,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	bsp_oled_WriteByte(0xA0,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	bsp_oled_WriteByte(0xCF,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	bsp_oled_WriteByte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	bsp_oled_WriteByte(0x12,OLED_CMD); //[5:4]配置
		 
	bsp_oled_WriteByte(0x81,OLED_CMD); //对比度设置
	bsp_oled_WriteByte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	bsp_oled_WriteByte(0xD9,OLED_CMD); //设置预充电周期
	bsp_oled_WriteByte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	bsp_oled_WriteByte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	bsp_oled_WriteByte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	bsp_oled_WriteByte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	bsp_oled_WriteByte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	bsp_oled_WriteByte(0xAF,OLED_CMD); //开启显示	 
	bsp_oled_Clear();
	bsp_oled_SetPos(0,0); 	
}

/********************************************  END OF FILE  *******************************************/
