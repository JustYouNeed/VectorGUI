/**
  *******************************************************************************************************
  * File Name: gui_mem.c
  * Author: Vector
  * Version: V1.0.0
  * Date: 2018-12-30
  * Brief: GUI�ڴ������
  *******************************************************************************************************
  * History
  *		1.Author: Vector
	*			Date: 2018-12-30
	*			Mod: �����ļ�
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

/* �ڴ����ṹ�����Ͷ��� */
typedef struct MemLink_t MemLink_t;
struct MemLink_t
{
	MemLink_t *pNextFreeMem; /* ָ����һ�����е��ڴ�� */
	uint32_t MemSize; /* ��������ڴ��Ĵ�С */
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
* Description: �ڴ�����ʼ��
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
	
	/* ����ڴ治����8�ֽڶ���ģ�����Ҫ���� */
	if((address & GUI_MEM_ALIGNMENT_MASK) != 0)
	{
		address += (GUI_MEM_ALIGNMENT - 1);
		address &= ~((uint32_t)(GUI_MEM_ALIGNMENT_MASK));
		totalHeapSize -= address - (uint32_t)memHeap;
	}
	pAlignedHeap = (uint8_t *)address;
	
	/* ָ���ڴ濪ʼ�ĵ�ַ������� */
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
* Description: ��һ���ڴ���뵽�����ڴ���������
*             
* Arguments  : pMemLink: �ڴ�ṹ��ָ��
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
	
	/* ���������ڴ������ҵ�һ����ĵ�ַ����Ҫ������ڴ�� */
	for(pIterator = &MemStart; pIterator->pNextFreeMem < pMemToInsert; pIterator = pIterator->pNextFreeMem);
	
	/* �����������������ڴ�飬��ϲ� */
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
* Description: �ڴ����뺯��
*             
* Arguments  : size: Ҫ������ڴ��С
*
* Reutrn     : ���뵽���ڴ棬����ʧ��
*
* Note(s)    : None.
*********************************************************************************************************
*/
void *gui_memAlloc(uint32_t size)
{
	void *pReturn = NULL;
	
	/* ָ���ڴ��β��ָ��Ϊ�մ����ڴ����û�г�ʼ�� */
	if(pMemEnd == NULL)
		gui_memInit();
	
	/* ������ڴ��СҪ�ں���ķ�Χ�� */
	if(size > 0 && size < GUI_MEM_SIZE)
	{
		/* ��Ϊ��Ҫ�����ڴ�͹���ṹ�壬������Ҫ���ڴ��СҪ���Ͻṹ���С */
		size += MemLinkStructSize;
		
		/* �ڴ��Сû�ж�������� */
		if((size & GUI_MEM_ALIGNMENT_MASK) != 0)
		{
			size += GUI_MEM_ALIGNMENT - (size & GUI_MEM_ALIGNMENT_MASK);
		}
		
		/* �����ڴ���� */
		struct MemLink_t *preMemLink, *lstMemLink;
		preMemLink = &MemStart;
		lstMemLink = MemStart.pNextFreeMem;
		/* �ڿ����ڴ������в��Һ��ʵ��ڴ�� */
		while((lstMemLink->MemSize < size) && (lstMemLink->pNextFreeMem != NULL))
		{
			preMemLink = lstMemLink;
			lstMemLink = lstMemLink->pNextFreeMem;
		}
		
		/* ����������һ������з��� */
		if(lstMemLink != pMemEnd)
		{
			/* ���ص��ڴ���Ҫ�����ṹ����ռ�õ� */
			pReturn = (void *)(((uint8_t *)preMemLink->pNextFreeMem) + MemLinkStructSize);
			
			/* �����ڴ��ӿ��������жϿ� */
			preMemLink->pNextFreeMem = lstMemLink->pNextFreeMem;
			
			/* ����ڴ��ж��࣬�򽫶���Ĳ��ֲ��뵽�����ڴ������� */
			if((lstMemLink->MemSize - size) > GUI_MINIUM_BLOCK_SIZE)
			{
				MemLink_t *pNewMemLink = (void *)(((uint8_t *)lstMemLink) + size);
				pNewMemLink->MemSize = pNewMemLink->MemSize - size;
				lstMemLink->MemSize = size;
				
				gui_memInsertIntoFreeList(pNewMemLink);
			}
			
			/* ��־���ڴ�Ϊʹ��״̬ */
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
* Description: �ڴ��ͷź���
*             
* Arguments  : �������ڴ��ַ
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
		pMem -= MemLinkStructSize; /* ��ȡ�ڴ���ƽṹ���ַ */
		
		pMemLink = (void *)pMem;
		
		/* �����ڴ��Ƿ񱻷����ȥ�� */
		if((pMemLink->MemSize & GUI_MEM_ALLOCATED_MASK) != 0)
		{
			if(pMemLink->pNextFreeMem == NULL)
			{
				pMemLink->MemSize &= ~GUI_MEM_ALLOCATED_MASK;
				
				/* ���ڴ���뵽�����ڴ��б��У�ͬʱ��ϲ���������������ڴ� */
				gui_memInsertIntoFreeList((MemLink_t *)pMemLink);
			}
		}
	}
}



/********************************************  END OF FILE  *******************************************/

