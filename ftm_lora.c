#include <assert.h>
#include <string.h>
#include "ftm_debug.h"
#include "ftm_lora.h"
#include "ftm_lora_wan.h"
#include "ftm_serial_io.h"

static	FTM_RET	FTM_LORA_SERVER_in(FTM_SERIAL_IO_PTR pSerialIO, FTM_UINT8_PTR pBuff, FTM_UINT32 ulLen, void *pParams);

FTM_RET	FTM_LORA_SERVER_init(FTM_LORA_SERVER_PTR pServer, FTM_LORA_SERVER_CONFIG_PTR pConfig)
{
	ASSERT(pServer != NULL);
	ASSERT(pConfig != NULL);

	
	memcpy(&pServer->xConfig, pConfig, sizeof(FTM_LORA_SERVER_CONFIG));

	pServer->xConfig.xSerialIO.fIn = FTM_LORA_SERVER_in;
	pServer->xConfig.xSerialIO.pInParams = pServer;

	FTM_SERIAL_IO_init(&pServer->xSerialIO, &pServer->xConfig.xSerialIO);

	return	FTM_RET_OK;
}

FTM_RET	FTM_LORA_SERVER_run(FTM_LORA_SERVER_PTR pServer, FTM_BOOL bAsyncMode)
{
	assert(pServer != NULL);

	
	FTM_SERIAL_IO_asyncRun(&pServer->xSerialIO);
}


FTM_RET	FTM_LORA_SERVER_in(FTM_SERIAL_IO_PTR pSerialIO, FTM_UINT8_PTR pBuff, FTM_UINT32 ulLen, void *pParams)
{
	FTM_INT32	i;
	FTM_LORA_MAC_HEADER_PTR	pHeader = (FTM_LORA_MAC_HEADER_PTR)pBuff;
	FTM_LORA_SERVER_PTR		pServer = (FTM_LORA_SERVER_PTR)pParams;

	ASSERT(pServer != NULL);
	ASSERT(pSerialIO != NULL);
	ASSERT(pBuff != NULL);

	if (ulLen == 0)
	{
		return	FTM_RET_ERROR;	
	}

	printf("PKT : %d - ", ulLen);
	for(i = 0 ; i < ulLen ; i++)
	{
		printf("%02x ", pBuff[i]);	
	}
	printf("\n");


	switch(pHeader->xBits.bMType)
	{
	case	FTM_LORA_MAC_FRAME_TYPE_JOIN_REQ:
		{
			FTM_UINT32	ulMic;
			FTM_LORA_MAC_FRAME_JOIN_REQUEST_PTR pFrame = (FTM_LORA_MAC_FRAME_JOIN_REQUEST_PTR)pBuff;

			if (ulLen != 23)
			{
				return	FTM_RET_COMM_INVALID_FRAME;
			}


			FTM_LOR_MAC_computeJoinMic((FTM_UINT8_PTR)pFrame, ulLen - 4, pServer->xConfig.pAppKey, &ulMic);
			if (ulMic != pFrame->ulMic)
			{
				return	FTM_RET_COMM_INVALID_FRAME;
			}

		}
		break;
	}

	return	FTM_RET_OK;
}
