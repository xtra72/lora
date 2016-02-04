#ifndef	__FTM_LIST_H__
#define __FTM_LIST_H__

#include "ftm_types.h"
#include "simclist.h"
typedef	struct
{
	list_t		xList;	
	FTM_BOOL	bIterator;
}	FTM_LIST, _PTR_ FTM_LIST_PTR;

typedef	FTM_INT32	(*FTM_LIST_ELEM_seeker)(const FTM_VOID_PTR pElement, const FTM_VOID_PTR pIndicator);
typedef	FTM_INT32	(*FTM_LIST_ELEM_comparator)(const FTM_VOID_PTR pElement, const FTM_VOID_PTR pIndicator);

FTM_RET FTM_LIST_init(FTM_LIST_PTR pList);
FTM_RET	FTM_LIST_destroy(FTM_LIST_PTR pList);

FTM_RET	FTM_LIST_append(FTM_LIST_PTR pList, FTM_VOID_PTR pItem);

FTM_RET	FTM_LIST_remove(FTM_LIST_PTR pList, FTM_VOID_PTR pItem);
FTM_RET	FTM_LIST_removeAt(FTM_LIST_PTR pList, FTM_UINT32 ulPosition);

FTM_RET	FTM_LIST_get(FTM_LIST_PTR pList, FTM_VOID_PTR pKey, FTM_VOID_PTR _PTR_ ppElement);
FTM_RET	FTM_LIST_getAt(FTM_LIST_PTR pList, FTM_UINT32 ulPosition, FTM_VOID_PTR _PTR_ ppElement);

FTM_RET	FTM_LIST_iteratorStart(FTM_LIST_PTR pList);
FTM_RET	FTM_LIST_iteratorNext(FTM_LIST_PTR pList, FTM_VOID_PTR _PTR_ ppElement);

FTM_RET	FTM_LIST_count(FTM_LIST_PTR pList, FTM_UINT32_PTR pulCount);

FTM_RET FTM_LIST_setSeeker(FTM_LIST_PTR pList, FTM_LIST_ELEM_seeker fSeeker);
FTM_RET FTM_LIST_setComparator(FTM_LIST_PTR pList, FTM_LIST_ELEM_comparator fComparator);
#endif

