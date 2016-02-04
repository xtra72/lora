#ifndef	__FTM_QUEUE_H__
#define __FTM_QUEUE_H__

#include "ftm_types.h"
#include "simclist.h"
typedef	struct
{
	list_t		xList;	
}	FTM_QUEUE, _PTR_ FTM_QUEUE_PTR;

FTM_RET FTM_QUEUE_init(FTM_QUEUE_PTR pQueue);
FTM_RET	FTM_QUEUE_destroy(FTM_QUEUE_PTR pQueue);

FTM_RET	FTM_QUEUE_push(FTM_QUEUE_PTR pQueue, FTM_VOID_PTR pItem);
FTM_RET	FTM_QUEUE_pop(FTM_QUEUE_PTR pQueue, FTM_VOID_PTR _PTR_ ppItem);

FTM_RET	FTM_QUEUE_count(FTM_QUEUE_PTR pQueue, FTM_UINT32_PTR pulCount);
#endif

