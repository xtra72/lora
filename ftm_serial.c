#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "ftm_serial.h"
#include "ftm_time.h"

static int	pSpeedTables[] = 
{
	B0, B300, B600, B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800
};

FTM_RET	FTM_SERIAL_init(FTM_SERIAL_PTR pSerial, FTM_SERIAL_CONFIG_PTR pConfig)
{
	assert((pSerial != NULL) && (pConfig != NULL));

	memcpy(&pSerial->xConfig, pConfig, sizeof(FTM_SERIAL_CONFIG));

	return	FTM_RET_OK;
}

FTM_RET	FTM_SERIAL_open(FTM_SERIAL_PTR pSerial)
{
	int	nFD;
	struct termios	xNewState;

	assert(pSerial != NULL);

	if (pSerial->xConfig.xSpeed > FTM_SERIAL_SPEED_MAX)
	{
		return	FTM_RET_INVALID_ARGUMENTS;
	}

	nFD = open(pSerial->xConfig.pDeviceName, O_RDWR | O_NOCTTY);
	if (nFD < 0)
	{
		return	FTM_RET_ERROR;	
	}



	int flags = fcntl(nFD, F_GETFL, 0);
	fcntl(nFD, F_SETFL, flags | O_NONBLOCK);

	tcgetattr(nFD, &pSerial->xOldState);
	memset(&xNewState, 0, sizeof(xNewState));

	xNewState.c_cflag = pSpeedTables[pSerial->xConfig.xSpeed] | CS8 | CLOCAL | CREAD;
	xNewState.c_iflag = IGNPAR | ICRNL;
	xNewState.c_oflag = 0;

	xNewState.c_lflag = ICANON;
	memset(&xNewState.c_cc, 0, NCCS);
	xNewState.c_cc[VMIN] = 1;

	tcflush(nFD, TCIFLUSH);
	tcsetattr(nFD, TCSANOW, &xNewState);
	
	pSerial->nFD = nFD;

	return	FTM_RET_OK;
}

FTM_RET	FTM_SERIAL_close(FTM_SERIAL_PTR pSerial)
{
	assert(pSerial != NULL);

	if (pSerial->nFD <= 0)
	{
		return	FTM_RET_NOT_INITIALIZED;	
	}

	tcsetattr(pSerial->nFD, TCSANOW, &pSerial->xOldState);
	close(pSerial->nFD);

	pSerial->nFD = 0;

	return	FTM_RET_OK;
}

FTM_RET	FTM_SERIAL_final(FTM_SERIAL_PTR pSerial)
{
	assert(pSerial != NULL);

	if (pSerial->nFD <= 0)
	{
		return	FTM_RET_NOT_INITIALIZED;	
	}

	return	FTM_RET_OK;

}


FTM_RET	FTM_SERIAL_read(FTM_SERIAL_PTR pSerial, FTM_UINT8_PTR pBuffer, FTM_UINT32 ulBufferLen, FTM_UINT32_PTR pulReadLen)
{
	FTM_UINT32	ulReadLen;

	assert(pSerial != NULL);

	if (pSerial->nFD <= 0)
	{
		return	FTM_RET_NOT_INITIALIZED;	
	}

	ulReadLen = read(pSerial->nFD, pBuffer, ulBufferLen);
	if (pulReadLen != 0)
	{
		*pulReadLen  = ulReadLen;
	}


	return	FTM_RET_OK;
}

FTM_RET	FTM_SERIAL_readFrame(FTM_SERIAL_PTR pSerial, FTM_UINT8_PTR pBuffer, FTM_UINT32 ulBufferLen, FTM_UINT32 ulFrameGapTime, FTM_UINT32_PTR pulReadLen)
{
	int					bTimeout = 0;
	FTM_UINT32			ulReadLen = 0;
	FTM_TIME			xTimeout;

	assert(pSerial != NULL);

	if (pSerial->nFD <= 0)
	{
		return	FTM_RET_NOT_INITIALIZED;	
	}


	FTM_TIME_get(&xTimeout);
	FTM_TIME_add(&xTimeout, ulFrameGapTime);

	while(!FTM_TIME_isTimeout(&xTimeout) && (ulReadLen < ulBufferLen))
	{
		FTM_INT32	nLen;

		nLen = read(pSerial->nFD, &pBuffer[ulReadLen], ulBufferLen - ulReadLen);
		if (nLen > 0)
		{
			FTM_TIME_get(&xTimeout);
			FTM_TIME_add(&xTimeout, ulFrameGapTime);

			ulReadLen += nLen;
		}
		else
		{
			usleep(1000);	
		}
			
		
	}

	if (pulReadLen != 0)
	{
		*pulReadLen  = ulReadLen;
	}

	return	FTM_RET_OK;
}

FTM_RET	FTM_SERIAL_write(FTM_SERIAL_PTR pSerial, FTM_UINT8_PTR pBuffer, FTM_UINT32 ulBufferLen, FTM_UINT32_PTR pulWriteLen)
{
	assert(pSerial != NULL);

	if (pSerial->nFD <= 0)
	{
		return	FTM_RET_NOT_INITIALIZED;	
	}


	*pulWriteLen = write(pSerial->nFD, pBuffer, ulBufferLen);

	return	FTM_RET_OK;
}

