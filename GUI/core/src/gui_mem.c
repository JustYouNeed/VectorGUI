/**
  *******************************************************************************************************
  * File Name: gui_mem.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-30
  * Brief: GUI内存管理部分
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: 建立文件
  *******************************************************************************************************
  */
/*
  *******************************************************************************************************
  *                              INCLUDE FILES
  *******************************************************************************************************
*/
# include "gui_mem.h"

/*
  *******************************************************************************************************
  *                              LOCAL DEFINITION
  *******************************************************************************************************
*/
# define GUI_MEM_ALIGNMENT	8
# define GUI_MEM_ALIGNMENT_MASK	0x0007
# define GUI_MEM_ALLOCATED_MASK	0x80000000
# define GUI_MINIUM_BLOCK_SIZE ((uint32_t)MemLinkStructSize << 1)

/* 内存管理结构体类型定义 */
typedef struct MemLink_t MemLink_t;
struct MemLink_t
{
	MemLink_t *pNextFreeMem; /* 指向下一个空闲的内存块 */
	uint32_t MemSize; /* 这个空闲内存块的大小 */
};

/*
  *******************************************************************************************************
  *                              LOCAL VARIABLE
  *******************************************************************************************************
*/
static const uint32_t MemLinkStructSize	= ( sizeof( MemLink_t ) + ( ( size_t ) ( GUI_MEM_ALIGNMENT - 1 ) ) ) & ~( ( uint32_t ) GUI_MEM_ALIGNMENT_MASK );
static uint8_t memHeap[GUI_MEM_SIZE];
static MemLink_t MemStart, *pMemEnd = NULL;


/*
*********************************************************************************************************
*                           gui_memInit               
*
* Description: 内存管理初始化
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
static void gui_memInit(void)
{
	MemLink_t *pFirstFreeMem;
	uint8_t *pAlignedHeap;
	uint32_t address, totalHeapSize = GUI_MEM_SIZE;
	
	address  = (uint32_t)memHeap;
	
	/* 如果内存不是以8字节对齐的，则需要对齐 */
	if((address & GUI_MEM_ALIGNMENT_MASK) != 0)
	{
		address += (GUI_MEM_ALIGNMENT - 1);
		address &= ~((uint32_t)(GUI_MEM_ALIGNMENT_MASK));
		totalHeapSize -= address - (uint32_t)memHeap;
	}
	pAlignedHeap = (uint8_t *)address;
	
	/* 指向内存开始的地址，对齐后 */
	MemStart.pNextFreeMem = (void *)pAlignedHeap;
	MemStart.MemSize = (uint32_t)0;

	address = ((uint32_t)pAlignedHeap) + totalHeapSize;
	address -= sizeof(MemLink_t);
	address &= ~((uint32_t) GUI_MEM_ALIGNMENT_MASK);
	pMemEnd = (void *)address;
	pMemEnd->MemSize = 0;
	pMemEnd->pNextFreeMem = NULL;
	
	pFirstFreeMem = (void *)pAlignedHeap;
	pFirstFreeMem->MemSize = address - (uint32_t)pFirstFreeMem;
	pFirstFreeMem->pNextFreeMem = pMemEnd;
}

/*
*********************************************************************************************************
*                        gui_memInsertIntoFreeList                  
*
* Description: 将一段内存插入到空闲内存链表里面
*             
* Arguments  : pMemLink: 内存结构体指针
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
static void gui_memInsertIntoFreeList(MemLink_t *pMemToInsert)
{
	MemLink_t *pIterator = NULL;
	uint8_t *pMem;
	
	/* 遍历空闲内存链表，找到一个块的地址高于要插入的内存块 */
	for(pIterator = &MemStart; pIterator->pNextFreeMem < pMemToInsert; pIterator = pIterator->pNextFreeMem);
	
	/* 如果有连续两块空闲内存块，则合并 */
	pMem = (uint8_t *)pIterator;
	if((pMem + pIterator->MemSize) == (uint8_t *)pMemToInsert)
	{
		pIterator->MemSize += pMemToInsert->MemSize;
		pMemToInsert = pIterator;
	}
	
	pMem = (uint8_t *)pMemToInsert;
	if((pMem + pMemToInsert->MemSize) == (uint8_t *)pIterator->pNextFreeMem)
	{
		if(pIterator->pNextFreeMem != pMemEnd)
		{
			pMemToInsert->MemSize += pIterator->pNextFreeMem->MemSize;
			pMemToInsert->pNextFreeMem = pIterator->pNextFreeMem->pNextFreeMem;
		}
		else
		{
			pMemToInsert->pNextFreeMem = pMemEnd;
		}
	}
	else
	{
		pMemToInsert->pNextFreeMem = pIterator->pNextFreeMem;
	}
	
	if(pIterator != pMemToInsert)
	{
		pIterator->pNextFreeMem = pMemToInsert;
	}
}

/*
*********************************************************************************************************
*                           gui_memAlloc               
*
* Description: 内存申请函数
*             
* Arguments  : size: 要申请的内存大小
*
* Reutrn     : 申请到的内存，或者失败
*
* Note(s)    : None.
*********************************************************************************************************
*/
void *gui_memAlloc(uint32_t size)
{
	void *pReturn = NULL;
	
	/* 指向内存堆尾的指针为空代表内存管理没有初始化 */
	if(pMemEnd == NULL)
		gui_memInit();
	
	/* 申请的内存大小要在合理的范围内 */
	if(size > 0 && size < GUI_MEM_SIZE)
	{
		/* 因为需要放置内存和管理结构体，所以需要的内存大小要加上结构体大小 */
		size += MemLinkStructSize;
		
		/* 内存大小没有对齐则对齐 */
		if((size & GUI_MEM_ALIGNMENT_MASK) != 0)
		{
			size += GUI_MEM_ALIGNMENT - (size & GUI_MEM_ALIGNMENT_MASK);
		}
		
		/* 进行内存分配 */
		struct MemLink_t *preMemLink, *lstMemLink;
		preMemLink = &MemStart;
		lstMemLink = MemStart.pNextFreeMem;
		/* 在空闲内存链表中查找合适的内存块 */
		while((lstMemLink->MemSize < size) && (lstMemLink->pNextFreeMem != NULL))
		{
			preMemLink = lstMemLink;
			lstMemLink = lstMemLink->pNextFreeMem;
		}
		
		/* 如果不是最后一个则进行分配 */
		if(lstMemLink != pMemEnd)
		{
			/* 返回的内存需要跳过结构体所占用的 */
			pReturn = (void *)(((uint8_t *)preMemLink->pNextFreeMem) + MemLinkStructSize);
			
			/* 将该内存块从空闲链表中断开 */
			preMemLink->pNextFreeMem = lstMemLink->pNextFreeMem;
			
			/* 如果内存有多余，则将多余的部分插入到空闲内存链表中 */
			if((lstMemLink->MemSize - size) > GUI_MINIUM_BLOCK_SIZE)
			{
				MemLink_t *pNewMemLink = (void *)(((uint8_t *)lstMemLink) + size);
				pNewMemLink->MemSize = pNewMemLink->MemSize - size;
				lstMemLink->MemSize = size;
				
				gui_memInsertIntoFreeList(pNewMemLink);
			}
			
			/* 标志该内存为使用状态 */
			lstMemLink->MemSize |= GUI_MEM_ALLOCATED_MASK;
			lstMemLink->pNextFreeMem = NULL;
		}
	}
	
	return pReturn;
}

/*
*********************************************************************************************************
*                         gui_memFree                 
*
* Description: 内存释放函数
*             
* Arguments  : 申请人内存地址
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void gui_memFree(void *memAddr)
{
	uint8_t *pMem = (uint8_t *)memAddr;
	MemLink_t *pMemLink = NULL;
	
	if(pMem != NULL)
	{
		pMem -= MemLinkStructSize; /* 获取内存控制结构体地址 */
		
		pMemLink = (void *)pMem;
		
		/* 检查该内存是否被分配出去了 */
		if((pMemLink->MemSize & GUI_MEM_ALLOCATED_MASK) != 0)
		{
			if(pMemLink->pNextFreeMem == NULL)
			{
				pMemLink->MemSize &= ~GUI_MEM_ALLOCATED_MASK;
				
				/* 将内存插入到空闲内存列表中，同时会合并连续的两块空闲内存 */
				gui_memInsertIntoFreeList((MemLink_t *)pMemLink);
			}
		}
	}
}



/********************************************  END OF FILE  *******************************************/

