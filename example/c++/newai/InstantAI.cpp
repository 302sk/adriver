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
* Windows Example:
*    InstantAI.cpp
*
* Example Category:
*    AI
*
* Description:
*    This example demonstrates how to use Instant AI function.
*
* Instructions for Running:
*    1. Set the 'deviceDescription' for opening the device.
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
//#include "../../../inc/bdaqctrl.h"
#include "bdaqcl.h"
using namespace Automation::BDaq;
//-----------------------------------------------------------------------------------
// Configure the following three parameters before running the example
//-----------------------------------------------------------------------------------
#define      deviceDescription  L"DemoDevice,BID#0"
#define         deviceNumber    0
int32        startChannel = 0;
const int32  channelCount = 3;
int32	startPort = 0;
const int32	portCount = 2;

inline void waitAnyKey()
{
   do{SLEEP(1);} while(!kbhit());
} 
int main(int argc, char* argv[])
{
   ErrorCode        ret = Success;

   BDaqDevice *device = NULL;
   BDaqAi *ai = NULL;
   BDaqAo	*ao = NULL;
   BDaqDio	*dio = NULL;
   
    printf("1\n");
   // Step 1: Create a 'instantAiCtrl' for InstantAI function.
   //InstantAiCtrl * instantAiCtrl = AdxInstantAiCtrlCreate();
   ret = BDaqDevice::Open(deviceNumber, ModeWrite, device);
    printf("2. device = %x ret = %x\n", device, ret);
    FILE *fp = fopen("ADAM3660M4_V16.bin","r");
   do
   {
      // Step 2: Select a device by device number or device description and specify the access mode.
      // in this example we use AccessWriteWithReset(default) mode so that we can 
      // fully control the device, including configuring, sampling, etc.
      //DeviceInformation devInfo(deviceNumber);
      ret = device->GetModule(0, ai);
      ret = device->GetModule(0, ao);
      ret = device->GetModule(0, dio);
      printf("3\n");
      //ret = instantAiCtrl->setSelectedDevice(devInfo);
      CHK_RESULT(ret);

      // Step 3: Read samples and do post-process, we show data here.
      printf("Acquisition is in progress, any key to quit!\n\n");
      double   scaledData[channelCount] = {0};//the count of elements in this array should not be less than the value of the variable channelCount
      int16		rawData[channelCount] = {0};
      BYTE		dioData[portCount] = {0};
      int32 channelCountMax = 4;//instantAiCtrl->getFeatures()->getChannelCountMax();

      do
      {
         //read samples and save to buffer 'scaledData'.
        // ret = ai->Read(1,startChannel,channelCount,rawData, scaledData);
   		// ret = ao->Write( startChannel, channelCount, rawData);
   		// ret = dio->DiRead(startPort, portCount, dioData);
   		// ret = dio->DoRead( startPort, portCount, dioData);
   		// ret = dio->DoWrite( startPort, portCount, dioData);
    //     printf("++++++++++++\n");

			if(fp == NULL)
			{
				printf("failed to open file\n");
				break;
			}else{
				printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ fp = %x\n", fp);
			}
    		ret = device->UpdateFirmware(1, fp);
    		break;
         CHK_RESULT(ret);

         // process the acquired data. only show data here.
         for (int32 i = startChannel; i< startChannel+channelCount;++i)
         {
            printf("Channel %d data: %10.6f\n", i % channelCountMax, scaledData[i-startChannel]);
         }
         printf("\n");
         //SLEEP(1);
         usleep(1000000);
      } while(!kbhit());
   }while(false);

	// Step 4 : Close device and release any allocated resource.
	//instantAiCtrl->Dispose();
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
	fclose(fp);
   return 0;
}



