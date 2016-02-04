#ifndef	_FTM_TYPES_H_
#define	_FTM_TYPES_H_

#define	PACKED	__attribute__((packed))

#define	_PTR_	*

typedef	unsigned int	FTM_RET;

typedef	unsigned char	FTM_UINT8, 	_PTR_ FTM_UINT8_PTR;
typedef	char			FTM_INT8, 	_PTR_ FTM_INT8_PTR;
typedef	unsigned short	FTM_UINT16, _PTR_ FTM_UINT16_PTR;
typedef	short			FTM_INT16, 	_PTR_ FTM_INT16_PTR;
typedef	unsigned int	FTM_UINT32, _PTR_ FTM_UINT32_PTR;
typedef	long			FTM_INT32, 	_PTR_ FTM_INT32_PTR;
typedef	int				FTM_BOOL,	_PTR_ FTM_BOOL_PTR;

#define	FTM_VOID_PTR	void *

#define	FTM_FALSE		0
#define	FTM_TRUE		(!FTM_FALSE)


#include "ftm_error.h"
#endif
