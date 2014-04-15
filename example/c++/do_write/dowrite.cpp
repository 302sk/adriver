/*******************************************************************************
Copyright (c) 1983-2012 Advantech Co., Ltd.
********************************************************************************
THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY INFORMATION
WHICH IS THE PROPERTY OF ADVANTECH CORP., ANY DISCLOSURE, USE, OR REPRODUCTION,
WITHOUT WRITTEN AUTHORIZATION FROM ADVANTECH CORP., IS STRICTLY PROHIBITED.

================================================================================
REVISION HISTORY
--------------------------------------------------------------------------------
$Log:  $
--------------------------------------------------------------------------------
$NoKeywords:  $
*/
/******************************************************************************
*
* Linux Example:
*    doread.cpp
*
* Example Category:
*    DO 
*
* Description:
*    This example demonstrates how to use DI function.
*
* Instructions for Running:
*    1. Set the 'deviceNumber' for opening the device.
*    2. Set the 'startChannel' as the first channel for scan analog samples
*    3. Set the 'channelCount' to decide how many sequential channels to scan analog samples.
*
* I/O Connections Overview:
*    Please refer to your hardware reference manual.
*
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "../inc/compatibility.h"
#include "../inc/bdaqcl.h"
using namespace Automation::BDaq;
//-----------------------------------------------------------------------------------
// Configure the following three parameters before running the example
//-----------------------------------------------------------------------------------
#define         deviceNumber    0
#define		moduleNumber	1
int32        startChannel = 0;
const int32  channelCount = 4;


inline void waitAnyKey()
{
   do{SLEEP(1);} while(!kbhit());
} 
int main(int argc, char* argv[])
{
	ErrorCode        ret = Success;

	BDaqDevice *device = NULL;
	BDaqDio	*dio = NULL;

	//Open device
	ret = BDaqDevice::Open(deviceNumber, ModeWrite, device);

	do
	{
		//Get dio module
		ret = device->GetModule(0, dio);
		CHK_RESULT(ret);

		BYTE	dioData[ 1 ] = {0};
		//Input do value
		printf("Please input DO value:");
		scanf("%x", &dioData[0]);
		//Write do value	
		ret = dio->DoWrite(moduleNumber,startChannel, channelCount, dioData);
		CHK_RESULT(ret);
		//Read do value 
		ret = dio->DoRead(moduleNumber,startChannel, channelCount, dioData);
		CHK_RESULT(ret);
		printf("do value: %2x\n", dioData[0]);

	}while(false);

	//Close device
	if(device != NULL)
	{
		device->Close();
	}
	// If something wrong in this execution, print the error code on screen for tracking.
	if(BioFailed(ret))
	{
		printf("Some error occurred. And the last error code is Ox%X.\n", ret);
		waitAnyKey();// wait any key to quit!
	}
	return 0;
}



