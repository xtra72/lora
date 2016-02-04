#ifndef	__FTM_DEBUG_H__
#define	__FTM_DEBUG_H__

#include <stdio.h>
#include "ftm_types.h"

#define		FTM_PATH_LEN		1024
#define		FTM_FILENAME_LEN	256

#define	MSG_NORMAL		(1 << 0)	
#define	MSG_TRACE		(1 << 1)
#define	MSG_ERROR		(1 << 2)

#define	MSG_ALL			(MSG_NORMAL | MSG_TRACE | MSG_ERROR)

typedef	struct
{
	FTM_UINT32				ulMode;
	struct
	{
		FTM_BOOL		bToFile;
		FTM_UINT8		pPath[FTM_PATH_LEN + 1];
		FTM_UINT8		pPrefix[FTM_FILENAME_LEN + 1];
		FTM_BOOL		bLine;
	}	xTrace;

	struct
	{
		FTM_BOOL		bToFile;
		FTM_UINT8		pPath[FTM_PATH_LEN + 1];
		FTM_UINT8		pPrefix[FTM_FILENAME_LEN + 1];
		FTM_BOOL		bLine;
	}	xError;
}	FTM_DEBUG_CFG, _PTR_ FTM_DEBUG_CFG_PTR;

FTM_RET	FTM_DEBUG_configSet(FTM_DEBUG_CFG_PTR pCfg);
FTM_RET	FTM_DEBUG_configLoad(FTM_DEBUG_CFG_PTR pCfg, FTM_UINT8_PTR pFileName);

FTM_RET	FTM_DEBUG_printModeSet(FTM_UINT32		nMode);
FTM_RET	FTM_DEBUG_printModeGet(FTM_UINT32_PTR	pMode);
FTM_RET	FTM_DEBUG_printOut
(
	unsigned long 	nLevel, 
	const char		*pFunction,
	int				nLine,
	char 			*pFormat, 
	...
);

FTM_RET	FTM_DEBUG_consoleCmd(FTM_INT32 nArgc, FTM_UINT8_PTR pArgv[]);


#define	ASSERT(x)	{ if (!(x)) FTM_DEBUG_printOut(MSG_ERROR, __func__, __LINE__, "%s", #x); }

FTM_RET	FTM_DEBUG_packetDump
(
	FTM_UINT8_PTR	pName,
	FTM_UINT8_PTR	pPacket,
	FTM_INT32			nLen
);

#define MESSAGE(format, ...) FTM_DEBUG_printOut(MSG_NORMAL, NULL, 0, format, ## __VA_ARGS__)

#ifdef	TRACE_OFF
#define	TRACE(format, ...) 
#else
#define	TRACE(format, ...) FTM_DEBUG_printOut(MSG_TRACE, __func__, __LINE__, format, ## __VA_ARGS__)
#endif
#define	ERROR(format, ...) FTM_DEBUG_printOut(MSG_ERROR, __func__, __LINE__, format, ## __VA_ARGS__)

#endif
