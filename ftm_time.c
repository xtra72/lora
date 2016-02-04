#include <assert.h>
#include "ftm_time.h"

FTM_RET	FTM_TIME_get(FTM_TIME_PTR pTime)
{
	assert(pTime != NULL);

	gettimeofday(&pTime->xTime, NULL);

	return	FTM_RET_OK;
}

FTM_RET	FTM_TIME_diff(FTM_TIME_PTR pTime1, FTM_TIME_PTR pTime2, FTM_INT32_PTR	pnDiffTime)
{
	unsigned long long	ullTime1, ullTime2, ullTimeDiff = 0;

	assert((pTime1 != NULL) && (pTime2 != NULL));
	
	ullTime1 = pTime1->xTime.tv_sec * 1000000 + pTime1->xTime.tv_usec;
	ullTime2 = pTime2->xTime.tv_sec * 1000000 + pTime2->xTime.tv_usec;

	*pnDiffTime = ullTime1 - ullTime2;

	return	FTM_RET_OK;
}

FTM_RET	FTM_TIME_add(FTM_TIME_PTR pTime, FTM_UINT32	ulTimeout)
{
	unsigned long long	ullTime = 0;

	assert(pTime != NULL);
	
	ullTime = pTime->xTime.tv_sec * 1000000 + pTime->xTime.tv_usec + ulTimeout;
	pTime->xTime.tv_sec = ullTime / 1000000;
	pTime->xTime.tv_usec = ullTime% 1000000;

	return	FTM_RET_OK;
}

FTM_BOOL FTM_TIME_isTimeout(FTM_TIME_PTR pTime)
{
	FTM_TIME	xCurrentTime;
	FTM_INT32	nDiffTime;


	FTM_TIME_get(&xCurrentTime);

	FTM_TIME_diff(pTime, &xCurrentTime, &nDiffTime);

	return	(nDiffTime <= 0);
	
}
