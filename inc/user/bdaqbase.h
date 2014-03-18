/*
 * BDaqTypes.h
 *
 *  Created on: 2011-8-18
 *      Author: rocky
 */

#ifndef _BIONIC_DAQ_MODULE_BASE_H_
#define _BIONIC_DAQ_MODULE_BASE_H_

#include <stddef.h>
#include <assert.h>
#include <bdaqdef.h>

class BDaqModule
{
public:
   virtual ModuleType getType()   = 0;
   virtual int32      getNumber() = 0;    // device number or module index, depends on module type.
   virtual AccessMode getMode()   = 0;

   // Property Access
   virtual ErrorCode PropAccess(uint32 propId, uint32 &bufLen, void *buffer, uint32 *attr, uint32 opFlag) = 0;

   // Event access
   virtual ErrorCode EventGetHandle(EventId id, HANDLE &handle)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode EventGetStatus(EventId id, uint32 *lParam, uint32 *rParam)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode EventClearFlag(EventId id, uint32 lParam, uint32 rParam)
   {
      return ErrorFuncNotSpted;
   }
};

class BDaqDevice : public BDaqModule
{
public:
   virtual void       Close()             = 0;
   virtual ErrorCode  Refresh()           = 0;
   virtual ErrorCode  Reset(uint32 state) = 0;
   virtual ErrorCode  GetModule(ModuleType type, uint32 index, BDaqModule **module) = 0;

   // Undocumented functions
   virtual ErrorCode  ReadPorts(uint32 startAddr, uint32 length, void *buffer)
   {
      return ErrorFuncNotSpted;
   }

   virtual ErrorCode  WritePorts(uint32 startAddr, uint32 length, void *buffer)
   {
      return ErrorFuncNotSpted;
   }

   virtual ErrorCode  ReadPrivRgn(uint32 signature, uint32 dataLen, uint8 *dataBuf)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode  WritePrivRgn(uint32 signature, uint32 dataLen, uint8 *dataBuf)
   {
      return ErrorFuncNotSpted;
   }
};

class BDaqAi : public BDaqModule
{
public:
   virtual ErrorCode ReadSamples(uint32 mdlNumber, uint32 chStart, uint32 chCount, void *rawData, double *scaledData) = 0;

   // Not all AI modules support buffered AI operation.
   virtual ErrorCode BfdAiPrepare(uint32 dataCount, void **dataPtr)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdAiRunOnce(bool asynchronous)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdAiRun()
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdAiStop()
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdAiRelease()
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdAiScaleData(void *rawData, double *scaledData, uint32 count, uint32 *chanOffset)
   {
      return ErrorFuncNotSpted;
   }

   // Undocumented functions
   virtual ErrorCode GetScaleTable(uint32 vrgType, uint32 *tableSize, PBioMapFuncPiece tables)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode SetScaleTable(uint32 vrgType, uint32  tableSize, PBioMapFuncPiece tables)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode GenerateNewVrg(uint32 parent, uint32  tableSize, PBioMapFuncPiece tables, uint32 child)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode GetCjcScaleTable(uint32 *tableSize, PBioMapFuncPiece tables)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode SetCjcScaleTable(uint32 tableSize, PBioMapFuncPiece tables)
   {
      return ErrorFuncNotSpted;
   }
   // ------------------------------------------------------------------------
   // Helpers for instant AI read
   // ------------------------------------------------------------------------
   // Instant AI single channel methods
   ErrorCode Read(uint32 mdlNumber, uint32 channel, double& dataScaled)
   {
      return ReadSamples(mdlNumber, channel, 1, NULL, &dataScaled);
   }

   // For the device whose raw data is in 16bits format
   ErrorCode Read(uint32 mdlNumber, uint32 channel, uint16& dataRaw)
   {
      return ReadSamples(mdlNumber, channel, 1, &dataRaw, NULL);
   }

   // For the device whose raw data is in 32bits format
   ErrorCode Read(uint32 mdlNumber, uint32 channel, uint32& dataRaw)
   {
      return ReadSamples(mdlNumber, channel, 1, &dataRaw, NULL);
   }

   // Instant AI multi-channel methods
   ErrorCode Read(uint32 mdlNumber, uint32 chStart, uint32 chCount, double dataScaled[])
   {
      return ReadSamples(mdlNumber, chStart, chCount, NULL, dataScaled);
   }
   // For the device whose raw data is in 16bits format
   ErrorCode Read(uint32 mdlNumber, uint32 chStart, uint32 chCount, uint16 dataRaw[], double dataScaled[])
   {
      return ReadSamples(mdlNumber, chStart, chCount, dataRaw, dataScaled);
   }
   // For the device whose raw data is in 32bits format
   ErrorCode Read(uint32 mdlNumber, uint32 chStart, uint32 chCount, uint32 dataRaw[], double dataScaled[])
   {
      return ReadSamples(mdlNumber, chStart, chCount, dataRaw, dataScaled);
   }
};

class BDaqAo : public BDaqModule
{
public:
   virtual ErrorCode WriteSamples(uint32 mdlNumber, uint32 chStart, uint32 chCount, void *rawData, double *scaledData) = 0;

   // Not all AO modules support buffered AO operation.
   virtual ErrorCode BfdAoPrepare(uint32 dataCount, void **dataPtr)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdAoRunOnce(bool asynchronous)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdAoRun()
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdAoStop(uint32 action)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdAoRelease()
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdAoScaleData(double *scaledData, void *rawData, uint32 count, uint32 *chanOffset)
   {
      return ErrorFuncNotSpted;
   }

   // Undocumented functions
   virtual ErrorCode GetScaleTable(uint32 vrgType, uint32 *tableSize, PBioMapFuncPiece tables)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode SetScaleTable(uint32 vrgType, uint32  tableSize, PBioMapFuncPiece tables)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode GenerateNewVrg(uint32 parent, uint32  tableSize, PBioMapFuncPiece tables, uint32 child)
   {
      return ErrorFuncNotSpted;
   }

   // ------------------------------------------------------------------------
   // Helpers for instant AO write
   // ------------------------------------------------------------------------
   // Instant AO single channel methods
   ErrorCode Write(uint32 mdlNumber, uint32 channel, double dataScaled)
   {
      return WriteSamples(mdlNumber, channel, 1, NULL, &dataScaled);
   }

   // For the device whose raw data is in 16bits format
   ErrorCode Write(uint32 mdlNumber, uint32 channel, uint16 dataRaw)
   {
      return WriteSamples(mdlNumber, channel, 1, &dataRaw, NULL);
   }

   // For the device whose raw data is in 32bits format
   ErrorCode Write(uint32 mdlNumber, uint32 channel, uint32 dataRaw)
   {
      return WriteSamples(mdlNumber, channel, 1, &dataRaw, NULL);
   }

   // Instant AO multi-channel methods
   ErrorCode Write(uint32 mdlNumber, uint32 chStart, uint32 chCount, double dataScaled[])
   {
      return WriteSamples(mdlNumber, chStart, chCount, NULL, dataScaled);
   }
   // For the device whose raw data is in 16bits format
   ErrorCode Write(uint32 mdlNumber, uint32 chStart, uint32 chCount, uint16 dataRaw[])
   {
      return WriteSamples(mdlNumber, chStart, chCount, dataRaw, NULL);
   }
   // For the device whose raw data is in 32bits format
   ErrorCode Write(uint32 mdlNumber, uint32 chStart, uint32 chCount, uint32 dataRaw[])
   {
      return WriteSamples(mdlNumber, chStart, chCount, dataRaw, NULL);
   }
};

class BDaqDio : public BDaqModule
{
public:
   // Not all DIO modules support all these operations.
   virtual ErrorCode ReadDiPorts(uint32 portStart, uint32 portCount, uint8 buffer[])
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode WriteDoPorts(uint32 portStart, uint32 portCount, uint8 buffer[])
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode ReadDoPorts(uint32 portStart, uint32 portCount, uint8 buffer[])
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode StartDiSnap(EventId id, uint32 portStart, uint32 portCount, uint8 **dataPtr)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode StopDiSnap(EventId id)
   {
      return ErrorFuncNotSpted;
   }

   // Not all DIO modules support buffered DI operation.
   virtual ErrorCode BfdDiPrepare(uint32 dataCount, void **dataPtr)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdDiRunOnce(bool asynchronous)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdDiRun()
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdDiStop()
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdDiRelease()
   {
      return ErrorFuncNotSpted;
   }

   // Not all DIO modules support buffered DO operation.
   virtual ErrorCode BfdDoPrepare(uint32 dataCount, void **dataPtr)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdDoRunOnce(bool asynchronous)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdDoRun()
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdDoStop(uint32 action)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode BfdDoRelease()
   {
      return ErrorFuncNotSpted;
   }

   // Undocumented functions
   // 0 -- start, 1 -- stop, 2 -- feed
   virtual ErrorCode WdtExec(int action)
   {
      return ErrorFuncNotSpted;
   }

   // ------------------------------------------------------------------------
   // Helpers for instant DI/O methods
   // ------------------------------------------------------------------------
   ErrorCode DiRead(uint32 port, uint8 &data)
   {
      return ReadDiPorts(port, 1, &data);
   }
   ErrorCode DiRead(uint32 portStart, uint32 portCount, uint8 data[])
   {
      return ReadDiPorts(portStart, portCount, data);
   }
   ErrorCode DoWrite(uint32 port, uint8 data)
   {
      return WriteDoPorts(port, 1, &data);
   }
   ErrorCode DoWrite(uint32 portStart, uint32 portCount, uint8 data[])
   {
      return WriteDoPorts(portStart, portCount, data);
   }
   ErrorCode DoRead(uint32 port, uint8& data)
   {
      return ReadDoPorts(port, 1, &data);
   }
   ErrorCode DoRead(uint32 portStart, uint32 portCount, uint8 data[])
   {
      return ReadDoPorts(portStart, portCount, data);
   }
};

class BDaqCntr : public BDaqModule
{
public:
   // Common methods
   virtual ErrorCode CounterReset(uint32 cntrStart, uint32 cntrCount) = 0;

   // Not all counter modules support the following operation.
   virtual ErrorCode CounterRead(uint32 cntrStart, uint32 cntrCount, uint32 loadVal[], uint32 holdVal[])
   {
      return ErrorFuncNotSpted;
   }

   // Primary counter
   virtual ErrorCode CounterStart(uint32 cntrStart, uint32 cntrCount)
   {
      return ErrorFuncNotSpted;
   }

   // Event Counting methods
   virtual ErrorCode EventCountStart(uint32 cntrStart, uint32 cntrCount)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode EventCountRead(uint32 cntrStart, uint32 cntrCount, uint32 cntrValue[])
   {
      return ErrorFuncNotSpted;
   }

   // One-Shot methods
   virtual ErrorCode OneShotStart(uint32 cntrStart, uint32 cntrCount)
   {
      return ErrorFuncNotSpted;
   }

   // Timer/Pulse methods
   virtual ErrorCode TimerPulseStart(uint32 cntrStart, uint32 cntrCount)
   {
      return ErrorFuncNotSpted;
   }

   // Frequency Measurement methods
   virtual ErrorCode FreqMeasureStart(uint32 cntrStart, uint32 cntrCount)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode FreqMeasureRead(uint32 cntrStart, uint32 cntrCount, double frequency[])
   {
      return ErrorFuncNotSpted;
   }

   // Pulse width measurement
   virtual ErrorCode PwmInStart(uint32 cntrStart, uint32 groupCount)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode PwmInRead(uint32 cntrStart, uint32 groupCount, double hiPeriod[], double lowPeriod[])
   {
      return ErrorFuncNotSpted;
   }

   // Pulse width modulation
   virtual ErrorCode PwmOutStart(uint32 cntrStart, uint32 cntrCount)
   {
      return ErrorFuncNotSpted;
   }

   // Up-down count
   virtual ErrorCode UpDownCountStart(uint32 cntrStart, uint32 cntrCount, uint32 resetCounter)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode UpDownCountValueReset(uint32 cntrStart, uint32 cntrCount)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode UpDownCountRead(uint32 cntrStart, uint32 cntrCount, uint32 cntrValue[])
   {
      return ErrorFuncNotSpted;
   }

   virtual ErrorCode CounterSnapStart(EventId id, uint32 cntrStart, uint32 cntrCount, uint32 **loadValues, uint32 **holdValues)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode CounterSnapStop(EventId id)
   {
      return ErrorFuncNotSpted;
   }

   virtual ErrorCode CounterCompareSetTable(uint32 counter, uint32 dataCount, uint32 buffer[])
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode CounterCompareSetInterval(uint32 counter, uint32 firstValue, uint32 increment, uint32 count)
   {
      return ErrorFuncNotSpted;
   }
   virtual ErrorCode CounterCompareClear(uint32 counter)
   {
      return ErrorFuncNotSpted;
   }

   // ------------------------------------------------------------------------
   // Helpers
   // ------------------------------------------------------------------------
   ErrorCode EventCountRead(uint32 cntr, uint32& cntrValue)
   {
      return EventCountRead(cntr, 1, &cntrValue);
   }
   ErrorCode FreqMeasureRead(uint32 cntr, double& frequency)
   {
      return FreqMeasureRead(cntr, 1, &frequency);
   }
   ErrorCode PwmInRead(uint32 cntr, double& hiPeriod, double& lowPeriod)
   {
      return PwmInRead(cntr, 1, &hiPeriod, &lowPeriod);
   }
   ErrorCode UpDownCountRead(uint32 cntr, uint32& cntrValue)
   {
      return UpDownCountRead(cntr, 1, &cntrValue);
   }
};

#endif /* _BIONIC_DAQ_MODULE_BASE_H_ */
