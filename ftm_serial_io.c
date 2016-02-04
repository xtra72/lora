#include <string.h>
#include "ftm_debug.h"
#include "ftm_mem.h"
#include "ftm_serial_io.h"

static void *FTM_SERIAL_IO_asyncMainProcess(void *pParams);
static void *FTM_SERIAL_IO_asyncReadProcess(void *pParams);
static void *FTM_SERIAL_IO_asyncWriteProcess(void *pParams);
static FTM_RET	FTM_SERIAL_IO_createData(FTM_SERIAL_IO_PTR pIO, FTM_UINT8_PTR pBuff, FTM_UINT32 ulLen, FTM_SERIAL_IO_DATA_PTR _PTR_ ppData);
static FTM_RET	FTM_SERIAL_IO_destroyData(FTM_SERIAL_IO_PTR pIO, FTM_SERIAL_IO_DATA_PTR pData);

FTM_RET	FTM_SERIAL_IO_init(FTM_SERIAL_IO_PTR pIO, FTM_SERIAL_IO_CONFIG_PTR pConfig)
{
	ASSERT(pIO != NULL);
	ASSERT(pConfig != NULL);

	FTM_QUEUE_init(&pIO->xInputQ);
	FTM_QUEUE_init(&pIO->xOutputQ);

	memcpy(&pIO->xConfig, pConfig, sizeof(FTM_SERIAL_IO_CONFIG));


	return	FTM_RET_OK;
}

FTM_RET FTM_SERIAL_IO_final(FTM_SERIAL_IO_PTR pIO)
{
	FTM_QUEUE_destroy(&pIO->xOutputQ);	
	FTM_QUEUE_destroy(&pIO->xInputQ);	

	return	FTM_RET_OK;
}

FTM_RET	FTM_SERIAL_IO_asyncRun(FTM_SERIAL_IO_PTR pIO)
{
	FTM_INT32	nStatus;

	ASSERT(pIO != NULL);

	pIO->nMainThreadID = pthread_create(&pIO->xMainThread, NULL, FTM_SERIAL_IO_asyncMainProcess, pIO);
	if (pIO->nMainThreadID < 0)
	{
		ERROR("Serial IO Main Thread creation error!\n");
		return	FTM_RET_ERROR;	
	}


	return	FTM_RET_OK;

}


FTM_RET FTM_SERIAL_IO_write(FTM_SERIAL_IO_PTR pIO, FTM_UINT8_PTR pBuff, FTM_UINT32 ulBuffLen)
{
	FTM_RET					xRet;
	FTM_SERIAL_IO_DATA_PTR	pData;

	ASSERT(pIO != NULL);


	xRet = FTM_SERIAL_IO_createData(pIO, pBuff, ulBuffLen, &pData);
	if (xRet != FTM_RET_OK)
	{
		return	xRet;
	}

	xRet = FTM_QUEUE_push(&pIO->xOutputQ, pData);
	if (xRet != FTM_RET_OK)
	{
		FTM_SERIAL_IO_destroyData(pIO, pData);	
		return	xRet;
	}
	
	return	FTM_RET_OK;
}

FTM_RET FTM_SERIAL_IO_read(FTM_SERIAL_IO_PTR pIO, FTM_UINT8_PTR pBuff, FTM_UINT32 ulBuffLen, FTM_UINT32_PTR pulDataLen)
{
	FTM_RET		xRet;
	FTM_UINT32	ulCount, ulWriteLen;
	FTM_SERIAL_IO_DATA_PTR	pData;

	ASSERT(pIO != NULL);
	ASSERT(pBuff != NULL);

	xRet = FTM_QUEUE_count(&pIO->xInputQ, &ulCount);
	if (xRet != FTM_RET_OK)
	{
		return	xRet;
	}

	xRet = FTM_QUEUE_pop(&pIO->xInputQ, (FTM_VOID_PTR _PTR_)&pData);
	if (xRet != FTM_RET_OK)
	{
		return	xRet;
	}

	xRet = FTM_SERIAL_write(&pIO->xSerial, pData->pBuff, pData->ulLen, &ulWriteLen);

	FTM_SERIAL_IO_destroyData(pIO, pData);

	return	xRet;
}

void *FTM_SERIAL_IO_asyncMainProcess(void *pParams)
{
	FTM_INT32	nStatus;
	FTM_SERIAL_IO_PTR	pIO = (FTM_SERIAL_IO_PTR)pParams;

	ASSERT(pIO != NULL);


	if (FTM_SERIAL_init(&pIO->xSerial, &pIO->xConfig.xSerial) != FTM_RET_OK)
	{
		return	NULL;	
	}
	
	FTM_SERIAL_open(&pIO->xSerial);

	pIO->nReadThreadID = pthread_create(&pIO->xReadThread, NULL, FTM_SERIAL_IO_asyncReadProcess, pIO);
	if (pIO->nReadThreadID < 0)
	{
		ERROR("Serial IO Read Thread creation error!\n");
		return	NULL;	
	}

	pIO->nWriteThreadID = pthread_create(&pIO->xMainThread, NULL, FTM_SERIAL_IO_asyncWriteProcess, pIO);
	if (pIO->nWriteThreadID < 0)
	{
		ERROR("Serial IO Write Thread creation error!\n");
		return	NULL;	
	}

	while(1)
	{
		

		printf("TEST!\n");	
		usleep(1000000);
	}

	pthread_kill(pIO->nReadThreadID, NULL);
	pIO->nReadThreadID = 0;
	pthread_kill(pIO->nWriteThreadID, NULL);
	pIO->nWriteThreadID = 0;

	FTM_SERIAL_close(&pIO->xSerial);

	return	0;
}

void *FTM_SERIAL_IO_asyncReadProcess(void *pParams)
{
	FTM_SERIAL_IO_PTR	pIO = (FTM_SERIAL_IO_PTR)pParams;
	FTM_UINT8			pBuff[4096];
	FTM_UINT32			ulReadLen;

	ASSERT(pIO != NULL);


	while(1)
	{
		ulReadLen = 0;

		if (FTM_SERIAL_readFrame(&pIO->xSerial, pBuff, sizeof(pBuff), 10000/*pIO->xConfig.ulIFG*/, &ulReadLen) == FTM_RET_OK)
		{
			if (ulReadLen != 0)
			{
				if (pIO->xConfig.fIn == NULL)
				{
					FTM_SERIAL_IO_DATA_PTR	pData = NULL;

					FTM_SERIAL_IO_createData(pIO, pBuff, ulReadLen, &pData);
					FTM_QUEUE_push(&pIO->xInputQ, pData);
				}
				else
				{
					pIO->xConfig.fIn(pIO, pBuff, ulReadLen, pIO->xConfig.pInParams);	
				}
			}
		}
	}

	return	0;
}

void *FTM_SERIAL_IO_asyncWriteProcess(void *pParams)
{
	FTM_SERIAL_IO_PTR	pIO = (FTM_SERIAL_IO_PTR)pParams;

	ASSERT(pIO != NULL);


	while(1)
	{
		FTM_UINT32	ulCount, ulWriteLen;

		FTM_QUEUE_count(&pIO->xOutputQ, &ulCount);
		
		if (ulCount != 0)
		{
			FTM_SERIAL_IO_DATA_PTR	pData = NULL;

			if (FTM_QUEUE_pop(&pIO->xOutputQ, (FTM_VOID_PTR _PTR_)&pData) == FTM_RET_OK)
			{
				FTM_SERIAL_write(&pIO->xSerial, pData->pBuff, pData->ulLen, &ulWriteLen);
				FTM_SERIAL_IO_destroyData(pIO, pData);
			}
		}
		else
		{
			printf("3");
			usleep(1000000);	
		}
	}

	return	0;
}

FTM_RET	FTM_SERIAL_IO_createData(FTM_SERIAL_IO_PTR pIO, FTM_UINT8_PTR pBuff, FTM_UINT32 ulLen, FTM_SERIAL_IO_DATA_PTR _PTR_ ppData)
{
	FTM_SERIAL_IO_DATA_PTR pData;

	pData = (FTM_SERIAL_IO_DATA_PTR)FTM_MEM_malloc(sizeof(FTM_SERIAL_IO_DATA) + ulLen);
	if (pData == NULL)
	{
		return	FTM_RET_NOT_ENOUGH_MEMORY;	
	}

	if (pBuff != NULL)
	{
		memcpy(pData->pBuff, pBuff, ulLen);
	}
	pData->ulLen = ulLen;


	*ppData = pData;

	return	FTM_RET_OK;

}

FTM_RET	FTM_SERIAL_IO_destroyData(FTM_SERIAL_IO_PTR pIO, FTM_SERIAL_IO_DATA_PTR pData)
{
	FTM_MEM_free(pData);

	return	FTM_RET_OK;
}
