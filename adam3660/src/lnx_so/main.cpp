/*
 * main.cpp
 *
 *  Created on: 2014-2-24
 *      Author: shen.kun
 */

#include "deviceimpl.h"

extern "C"
ErrorCode BDaqDeviceObjectCreate(int32 deviceNumber, AccessMode mode, BDaqDevice **device)
{
   CHK_USER_BUF(device);
   *device = NULL;

   BDaqDeviceImpl *impl = new BDaqDeviceImpl();
   if (impl == NULL){
      return ErrorMemoryNotEnough;
   }

   ErrorCode ret = impl->Initialize(deviceNumber, mode);
   if (ret != Success){
      delete impl;
      return ret;
   }

   *device = impl;
   return ret;
}

extern "C"
ErrorCode AdxDeviceOpen(int32 deviceNumber, AccessMode mode, BDaqDevice **device)
{
	CHK_USER_BUF(device);
   *device = NULL;

   BDaqDeviceImpl *impl = new BDaqDeviceImpl();
   if (impl == NULL){
      return ErrorMemoryNotEnough;
   }

   ErrorCode ret = impl->Initialize(deviceNumber, mode);
   if (ret != Success){
      delete impl;
      return ret;
   }

   *device = impl;
   return ret;
}


extern "C"
ErrorCode AdxDeviceReset(BDaqDevice* device, uint32 state)
{
	CHK_USER_BUF(device);
	return ((BDaqDeviceImpl *)device) -> Reset( state );
}

extern "C"
ErrorCode AdxDeviceGetModuleHandle(BDaqDevice* device, ModuleType type, uint32 index, BDaqModule **module)
{
	CHK_USER_BUF(device);
	return ((BDaqDeviceImpl *)device)->GetModule(type, index, module);
}

extern "C"
ErrorCode AdxDeviceClose(BDaqDevice* device)
{
	CHK_USER_BUF(device);
	BDaqDeviceImpl *impl = (BDaqDeviceImpl *)device;
	delete impl;
	return Success;
}

extern "C"
ErrorCode AdxDeviceRefreshProperties(BDaqDevice* device)
{
	CHK_USER_BUF(device);
	return ((BDaqDeviceImpl *)device)->Refresh();
}

extern "C"
ErrorCode AdxAiReadSamples(BDaqAi* ai, uint32 mdlNumber, uint32 channelStart, uint32 channelCount, void * rawData, double * scaledData)
{
	CHK_USER_BUF(ai);
	CHK_USER_BUF(rawData);
	CHK_USER_BUF(scaledData);
	return ((BDaqAiImpl *)ai)->ReadSamples(mdlNumber, channelStart, channelCount, rawData, scaledData);
}

extern "C"
ErrorCode AdxAiAccessValueRange(BDaqAi* ai, uint32 mdlNumber, uint32 channelStart, uint32 channelCount, void *valueRange, uint32 optFlag)
{
	CHK_USER_BUF(ai);
	CHK_USER_BUF(valueRange);
	return ((BDaqAiImpl *)ai)->AccessAiValueRange(mdlNumber, channelStart, channelCount, valueRange, optFlag);
}

extern "C"
ErrorCode AdxAoWriteSamples(BDaqAo* ao, uint32 mdlNumber, uint32 channelStart, uint32 channelCount, void * rawData, double * scaledData)
{
	CHK_USER_BUF(ao);
	//CHK_USER_BUF(rawData);
	//CHK_USER_BUF(scaledData);
	if((rawData == NULL) && (scaledData == NULL) )
	{
		return ErrorBufferIsNull;
	}
	return ((BDaqAoImpl *)ao)->WriteSamples(mdlNumber, channelStart, channelCount, rawData, scaledData);
}

extern "C"
ErrorCode AdxAoAccessValueRange(BDaqAo* ao, uint32 mdlNumber, uint32 channelStart, uint32 channelCount, void *valueRange, uint32 optFlag)
{
	CHK_USER_BUF(ao);
	CHK_USER_BUF(valueRange);
	return ((BDaqAoImpl *)ao)->AccessAoValueRange(mdlNumber, channelStart, channelCount, valueRange, optFlag);
}


extern "C"
ErrorCode AdxDiReadPorts(BDaqDio* dio, uint32 mdlNumber, uint32 portStart, uint32 portCount, uint8 * dataBuffer)
{
	CHK_USER_BUF(dio);
	CHK_USER_BUF(dataBuffer);
	return ((BDaqDioImpl *)dio)->ReadDiPorts(mdlNumber, portStart, portCount, dataBuffer);
}

extern "C"
ErrorCode AdxDoWritePorts(BDaqDio* dio, uint32 mdlNumber, uint32 portStart, uint32 portCount, uint8 * dataBuffer)
{
	CHK_USER_BUF(dio);
	CHK_USER_BUF(dataBuffer);
	return ((BDaqDioImpl *)dio)->WriteDoPorts(mdlNumber, portStart, portCount, dataBuffer);
}

extern "C"
ErrorCode AdxDoReadBackPorts(BDaqDio* dio, uint32 mdlNumber, uint32 portStart, uint32 portCount, uint8 * dataBuffer)
{
	CHK_USER_BUF(dio);
	CHK_USER_BUF(dataBuffer);
	return ((BDaqDioImpl *)dio)->ReadDoPorts(mdlNumber, portStart, portCount, dataBuffer);
}

extern "C"
ErrorCode AdxPropertyRead(void* handle, uint32 propertyId, uint32 bufferSize, void * buffer, uint32 * dataLength, uint32 * attribute)
{
	CHK_USER_BUF(handle);
	CHK_USER_BUF(buffer);
//	return ((BDaqModule *)handle)->PropAccess(propertyId, dataLength, buffer, attribute, 0);
	return Success;
}

extern "C"
ErrorCode AdxPropertyWrite(void* handle, uint32 propertyId, uint32 dataLength, void * buffer, uint32 notifyNow)
{
	CHK_USER_BUF(handle);
	CHK_USER_BUF(buffer);
//	return ((BDaqModule *)handle)->PropAccess(propertyId, dataLength, buffer, attribute, 0);
	return Success;
}




