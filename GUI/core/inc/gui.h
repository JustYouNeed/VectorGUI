/**
  *******************************************************************************************************
  * File Name: gui.h
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-29
  * Brief: GUI底层总头文件
  *******************************************************************************************************
  * History
  *   1.Author: Vetor
  *		Date: 2018-12-29
  *		Mod: 建立文件
  *******************************************************************************************************
  */	
# ifndef __GUI_H
# define __GUI_H
 

# define LCD_X	128
# define LCD_Y	64

/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "math.h"
# include "stdio.h"
# include <stdint.h>
# include "stdbool.h"
# include "bsp_malloc.h"


# include "gui_conf.h"
# include "gui_types.h"
# include "gui_font.h"
# include "gui_drv.h"
# include "gui_basic.h"
# include "gui_msg.h"
# include "gui_graph2.h"
# include "gui_core.h"
# include "gui_widget.h"
# include "gui_wm.h"
# include "gui_text.h"

# include "gui_menu.h"
# include "gui_window.h"
# include "gui_scrollbar.h"
# include "gui_progbar.h"
# include "gui_button.h"
# include "gui_checkbox.h"


extern GUI_CONTEXT GUI_Context;
# endif
 
 
/********************************************  END OF FILE  *******************************************/


