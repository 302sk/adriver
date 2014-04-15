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
 *    firmware-update.cpp
 *
 * Example Category:
 *    firmware download 
 *
 * Description:
 *    This example demonstrates how to use firmware download function.
 *
 * Instructions for Running:
 *    1. Set the 'deviceNumber' for opening the device.
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
#define			moduleNumber	1
void show_progress(int progress)
{
	fprintf(stderr, "%d\r", progress);
}
int main(int argc, char* argv[])
{
	ErrorCode        ret = Success;
	long		target = 0;
	PROGRESS_CALLBACK process = show_progress;
	
	if(argc < 3){
		printf("Please input the Target and File-name\n");
		printf("Example: firmwaredownload ai abc.bin\n");
		return 0;
	}
	if(!strcmp(argv[1], "ai")){
		target = AI_FIRM_WARE;
	}else if(!strcmp(argv[1], "mcu")){
		target = M4_FIRM_WARE;
	}else{
		printf("Target is invalid, it should be \"ai\" or \"mcu\"\n");
		printf("Example: firmwaredownload ai abc.bin\n");
		return 0;
	}
	BDaqDevice *device = NULL;
	//Open device with deviceNumber
	ret = BDaqDevice::Open(deviceNumber, ModeWrite, device);
	//Open bin file to download
	FILE *fp = fopen(argv[2],"r");
	if(fp == NULL){
		printf("Failed to open file!\n");
		return 0;
	}
	//Update firmware
	ret = device->UpdateFirmware(moduleNumber, fp, target, process);
	//Close device and release any allocated resource.
	if(device != NULL){
		device->Close();
	}
	// If something wrong in this execution, print the error code on screen for tracking.
	if(BioFailed(ret)){
		printf("Some error occurred. And the last error code is Ox%X.\n", ret);
	}
	fclose(fp);
	return 0;
}



