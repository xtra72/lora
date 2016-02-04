#ifndef	_FTM_TIME_H_
#define	_FTM_TIME_H_

#include <sys/time.h>
#include <time.h>
#include "ftm_types.h"

typedef struct	
{
	struct timeval	xTime;
}	FTM_TIME, _PTR_ FTM_TIME_PTR;

FTM_RET		FTM_TIME_get(FTM_TIME_PTR pTime);
FTM_RET		FTM_TIME_diff(FTM_TIME_PTR pTime1, FTM_TIME_PTR pTime2, FTM_INT32_PTR pnTimeDiff);
FTM_RET		FTM_TIME_add(FTM_TIME_PTR pTime, FTM_UINT32	ulTimeout);
FTM_BOOL	FTM_TIME_isTimeout(FTM_TIME_PTR pTime);

#endif

