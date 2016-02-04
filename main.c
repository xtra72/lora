#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <event2/event.h>
#include "ftm_serial.h"
#include "ftm_serial_io.h"
#include "ftm_lora.h"
#include "ftm_mem.h"

FTM_SERIAL_CONFIG	xConfig =
{
	.pDeviceName = "/dev/ttyUSB0",
	.xSpeed = FTM_SERIAL_SPEED_115200
};

FTM_SERIAL_IO_CONFIG xIOConfig = 
{
	.xSerial = 
	{
		.pDeviceName = "/dev/ttyUSB0",
		.xSpeed = FTM_SERIAL_SPEED_115200
	},
	.ulIFG = 10000
};

FTM_LORA_SERVER_CONFIG	xLoRaConfig = 
{
	.xSerialIO =
	{
		.xSerial = 
		{
			.pDeviceName = "/dev/ttyUSB0",
			.xSpeed = FTM_SERIAL_SPEED_115200
		},
		.ulIFG = 10000
	}
};

void main(int nArgc, char *pArgv[])
{
	FTM_RET		xRet;
	FTM_SERIAL	xSerial;
	FTM_SERIAL_IO	xSerialIO;

	if (nArgc < 2)
	{
		fprintf(stderr, "Invalid Argument\n");	
		exit(1);
	}

	FTM_MEM_init();

#if 0
	strcpy(xConfig.pDeviceName, pArgv[1]);

	xRet = FTM_SERIAL_init(&xSerial, &xConfig);
	
	if (xRet != FTM_RET_OK)
	{
		fprintf(stderr, "Internal error!\n");	
		exit(1);
	}

	xRet = FTM_SERIAL_open(&xSerial);
	if (xRet != FTM_RET_OK)
	{
		fprintf(stderr, "Can't open device[%s]\n", xSerial.xConfig.pDeviceName);	
		exit(1);
	}

	while(1)
	{
		FTM_UINT32	ulRead = 0;

		FTM_UINT8	pBuff[255];

		if (FTM_SERIAL_readFrame(&xSerial, pBuff, sizeof(pBuff), 10000, &ulRead) == FTM_RET_OK) 
		{
			if (ulRead != 0)
			fprintf(stderr, "%u\n", ulRead);	
		}
	}
	FTM_SERIAL_close(&xSerial);
#elif 0
	FTM_SERIAL_IO_init(&xSerialIO, &xIOConfig);

	FTM_SERIAL_IO_asyncRun(&xSerialIO);

	while(1)
	{
		static int i = 0;
		char	pBuff[256];

		sprintf(pBuff, "Hello[%d]\n", i++);
		FTM_SERIAL_IO_write(&xSerialIO, pBuff, strlen(pBuff));
		sleep(1);
	}
#else
	FTM_LORA_SERVER	xLoRa;

	FTM_LORA_SERVER_init(&xLoRa, &xLoRaConfig);
	FTM_LORA_SERVER_run(&xLoRa, FTM_TRUE);
#endif
	while(1)
	{
	}
}
