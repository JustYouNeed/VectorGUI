/**
  *******************************************************************************************************
  * File Name: gui_conf.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-30
  * Brief: GUI配置文件
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: 建立文件
	*
  *******************************************************************************************************
  */
# ifndef __GUI_CONF_H
# define __GUI_CONF_H

# define LCD_MAX_X	127
# define LCD_MAX_Y  63

/* 是否使用Bresenham算法画线，如果为0，则调用DDA算法 */
# define GUI_DRAW_LINE_BRESE	1u

# define GUI_OS_SUPPORT		0u

# define GUI_MEM_SIZE		16*1024

# define GUI_KEY_FIFO_SIZE		16

# endif


/********************************************  END OF FILE  *******************************************/

