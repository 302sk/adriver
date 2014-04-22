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
 *    mdlsearch.cpp
 *
 * Example Category:
 *   io module search 
 *
 * Description:
 *    This example demonstrates how to search io modules
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
#define		MAX_MODULE_COUNT	5

inline void waitAnyKey()
{
	do{SLEEP(1);} while(!kbhit());
} 
int main(int argc, char* argv[])
{
	ErrorCode        ret = Success;

	BDaqDevice *device = NULL;
	IO_MODULE_INFO modules_info[MAX_MODULE_COUNT];
	long actualCnt;
	int i;
	do{
		//Open device
		ret = BDaqDevice::Open(deviceNumber, ModeWrite, device);
		CHK_RESULT(ret);
		//Get ai module
		ret = device->SearchIoModules(modules_info, MAX_MODULE_COUNT, &actualCnt);
		CHK_RESULT(ret);

		for(i=0; i<actualCnt; i++){
			printf("IO Module: %d, %s\n", modules_info[i].module_id, modules_info[i].module_name);
		}
	}while(false);
	//Close device and release any allocated resource.
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



