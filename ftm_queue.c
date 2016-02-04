#include "ftm_types.h"
#include "ftm_debug.h"
#include "ftm_queue.h"

FTM_RET FTM_QUEUE_init(FTM_QUEUE_PTR pQueue)
{
	ASSERT(pQueue != NULL);

	list_init(&pQueue->xList);
	
	return	FTM_RET_OK;
}

FTM_RET	FTM_QUEUE_destroy(FTM_QUEUE_PTR pQueue)
{
	ASSERT(pQueue != NULL);

	list_destroy(&pQueue->xList);

	return	FTM_RET_OK;
}


FTM_RET	FTM_QUEUE_push(FTM_QUEUE_PTR pQueue, FTM_VOID_PTR pItem)
{
	FTM_INT32	nRet;

	ASSERT((pQueue != NULL) && (pItem != NULL));

	nRet = list_append(&pQueue->xList, pItem);
	if (nRet < 0)
	{
		ERROR("list_append error[%d]\n", nRet);
		return	FTM_RET_ERROR;	
	}

	return	FTM_RET_OK;
}

FTM_RET	FTM_QUEUE_pop(FTM_QUEUE_PTR pQueue, FTM_VOID_PTR _PTR_ ppItem)
{
	FTM_VOID_PTR	pItem;

	ASSERT((pQueue != NULL) && (pItem != NULL));

	pItem = list_get_at(&pQueue->xList, 0);
	if (pItem == NULL)
	{
		return	FTM_RET_ERROR;	
	}

	if (list_delete_at(&pQueue->xList, 0) < 0)
	{
		return	FTM_RET_ERROR;
	}

	*ppItem = pItem;

	return	FTM_RET_OK;
}

FTM_RET	FTM_QUEUE_count(FTM_QUEUE_PTR pQueue, FTM_UINT32_PTR pulCount)
{
	ASSERT((pQueue != NULL) && (pulCount != NULL));
	
	*pulCount = list_size(&pQueue->xList);

	return	FTM_RET_OK;
}
