#ifndef	_FTM_SERIAL_IO_H_
#define	_FTM_SERIAL_IO_H_

#include <pthread.h>
#include "ftm_serial.h"
#include "ftm_queue.h"

typedef struct FTM_SERIAL_IO_STRUCT _PTR_ FTM_SERIAL_IO_PTR;

typedef	FTM_RET	(*FTM_SERIAL_IO_CB_IN)(FTM_SERIAL_IO_PTR pIO, FTM_UINT8_PTR pBuff, FTM_UINT32 ulLen, void *pParams); 
typedef	FTM_RET	(*FTM_SERIAL_IO_CB_OUT)(FTM_SERIAL_IO_PTR pIO, void *pParams); 

typedef struct
{
	FTM_UINT32				ulLen;
	FTM_UINT8				pBuff[];
}	FTM_SERIAL_IO_DATA, _PTR_ FTM_SERIAL_IO_DATA_PTR;

typedef	struct
{
	FTM_SERIAL_CONFIG		xSerial;
	FTM_UINT32				ulIFG;

	FTM_SERIAL_IO_CB_IN		fIn;
	void					*pInParams;
	FTM_SERIAL_IO_CB_OUT	fOut;
	void					*pOutParams;
}	FTM_SERIAL_IO_CONFIG, _PTR_ FTM_SERIAL_IO_CONFIG_PTR;

typedef	struct FTM_SERIAL_IO_STRUCT
{
	FTM_SERIAL_IO_CONFIG	xConfig;

	FTM_QUEUE				xInputQ;
	FTM_QUEUE				xOutputQ;

	pthread_t				xMainThread;
	pthread_t				xReadThread;
	pthread_t				xWriteThread;
	FTM_INT32				nMainThreadID;
	FTM_INT32				nReadThreadID;
	FTM_INT32				nWriteThreadID;

	FTM_SERIAL				xSerial;

} FTM_SERIAL_IO;

FTM_RET	FTM_SERIAL_IO_init(FTM_SERIAL_IO_PTR pIO, FTM_SERIAL_IO_CONFIG_PTR pConfig);
FTM_RET	FTM_SERIAL_IO_final(FTM_SERIAL_IO_PTR pIO);

FTM_RET	FTM_SERIAL_IO_asyncRun(FTM_SERIAL_IO_PTR pIO);

FTM_RET FTM_SERIAL_IO_write(FTM_SERIAL_IO_PTR pIO, FTM_UINT8_PTR pData, FTM_UINT32 ulDataLen);
FTM_RET FTM_SERIAL_IO_read(FTM_SERIAL_IO_PTR pIO, FTM_UINT8_PTR pBuff, FTM_UINT32 ulBuffLen, FTM_UINT32_PTR pulDataLen);

#endif
