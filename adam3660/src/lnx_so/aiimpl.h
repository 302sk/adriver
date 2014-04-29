/*
 * AiImpl.h
 *
 *  Created on: 2014-2-24
 *      Author: shen.kun
 */

#ifndef _BDAQ_AI_IMPL_H_
#define _BDAQ_AI_IMPL_H_

#include <sys/time.h>
#include <asm/byteorder.h>
#include "krnlstub.h"

#define AD_MAX_DIGITAL          ((double)0xFFFFFFFF + 1.0)


//
// Supported Value Range, Refer to hardware SPEC.
VRG_INFO const s_aiVrgInfoTable[] = {
   { V_Neg10To10,       0x08, -10,     10,   V_Neg10To10_String },
   { V_Neg2pt5To2pt5,   0x05, -2.5,    2.5,  V_Neg2pt5To2pt5_String },
   { mA_0To20,          0x4D, 0,       20,   mA_0To20_String },
   { mA_4To20,          0x07, 4,       20,   mA_4To20_String },
};

// AI supported events
uint32 const s_aiSptedEvts[] = {
   EvtBufferedAiOverrun,
   EvtBufferedAiDataReady,
   EvtBufferedAiStopped,
   EvtBufferedAiCacheOverflow
};

class BDaqAiImpl : public BDaqAi
{
public:
   // --------------------------------------------------------------
   // BDaqModule interface implementation
   // --------------------------------------------------------------
   virtual ModuleType getType()   {  return DaqAi; }
   virtual int32      getNumber() {  return 0;   }
   virtual AccessMode getMode()   {  return m_kstubPtr->getMode(); }

   // Property table
   BEGIN_PROPERTY_TABLE(BDaqAiImpl, PropAccess)
      // component specified properties
      // features
      PROP_RO_VAL(CFG_FeatureResolutionInBit,      uint32, AI_RESOLUTION_IN_BIT)
      PROP_RO_VAL(CFG_FeatureDataSize,             uint32, AI_DATA_SIZE )
      PROP_RO_VAL(CFG_FeatureChannelNumberMax,     uint32, AI_CHL_COUNT - 1)
      PROP_RO_VAL(CFG_FeatureChannelConnectionType,uint32, AllDifferential)

      PROP_RO_VAL(CFG_FeatureDataMask,             uint32, AI_DATA_MASK)
      PROP_RO_VAL(CFG_FeatureOverallVrgType,       uint32, 0)
      PROP_RO_VAL(CFG_FeatureScanChannelStartBase, uint32, 1)
      PROP_RO_VAL(CFG_FeatureScanChannelCountBase, uint32, 1)
      
      // AI channel configuration
      PROP_CUSTOM(CFG_FeatureVrgTypes,             ReadOnly, PropAccessFeatVrgTypeList)
      PROP_RO_VAL(CFG_ChannelCount,                uint32, AI_CHL_COUNT/*PropAccessChanCount*/)
//      PROP_CUSTOM(CFG_VrgTypeOfChannels,           Writable,   PropAccessChanVrgType)
   END_PROPERTY_TABLE()

public:
   // --------------------------------------------------------------
   // BDaqAi interface implementation
   // --------------------------------------------------------------
   virtual ErrorCode ReadSamples(uint32 mdlNumber, uint32 chStart, uint32 chCount, void *rawData, double *scaledData);

public:
   // --------------------------------------------------------------
   // BDaqAiImpl interface implementation
   // --------------------------------------------------------------
   BDaqAiImpl()
   {
      m_kstubPtr  = NULL;
   }

   virtual ~BDaqAiImpl()
   {
      if (!m_kstubPtr) {
         return;
      }
   }

   ErrorCode Initialize(BioKrnlStub *kstub);
   ErrorCode Reset(uint32 state);
   ErrorCode AccessAiValueRange(uint32 mdlNumber, uint32 chStart, uint32 chCount, void *valueRange, uint32 optFlag);
   ErrorCode Calibrate(uint32 mdlNumber, uint32 caliType);
   void RefreshChanSetting();

protected:
   ErrorCode PropAccessFeatVrgTypeList(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
   ErrorCode PropAccessChanCount(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
   uint32    get_chl_count(uint32 mdlNumber);
   ErrorCode GetGainCode(uint32 chStart, uint32 chCount, void *valueRange, void *gainCode);
   ErrorCode GetValuleRange(uint32 chStart, uint32 chCount, uint8* gainCode, uint32 *valueRange);

private:
   BioKrnlStub     *m_kstubPtr;
};

// -----------------------------------------------------------------------------------------
// inline functions
// -----------------------------------------------------------------------------------------
inline
ErrorCode BDaqAiImpl::Initialize(BioKrnlStub *kstub)
{
   assert(kstub != NULL);
   m_kstubPtr = kstub;
   

   return Success;
}

inline
ErrorCode BDaqAiImpl::Reset(uint32 state)
{
   
   return Success;
}

inline
void BDaqAiImpl::RefreshChanSetting()
{

}

inline
ErrorCode BDaqAiImpl::GetGainCode(uint32 chStart, uint32 chCount, void *valueRange, void *gainCode)
{
   uint32 i = 0;
   uint32 j = 0;
   uint32 *vrg = (uint32*)valueRange;
   uint8 *gain = (uint8*)gainCode;
   for(i = 0; i<chCount; i++)
   {
      for(j=0; j<ARRAY_SIZE(s_aiVrgInfoTable); j++)
      {
         if(s_aiVrgInfoTable[j].VrgType == vrg[i])
         {
            gain[i] = s_aiVrgInfoTable[j].Gain;
            break;
         }
      }
      if(j >= ARRAY_SIZE(s_aiVrgInfoTable))
         return ErrorPropNotSpted;
   }
   return Success;
}

inline
ErrorCode BDaqAiImpl::GetValuleRange(uint32 chStart, uint32 chCount, uint8* gainCode, uint32 *valueRange)
{
   uint32 i = 0;
   uint32 j = 0;
   uint32 *vrg = (uint32*)valueRange;
   uint8 *gain = (uint8*)gainCode;
   for(i = 0; i < chCount; i++)
   {
      for(j=0; j<ARRAY_SIZE(s_aiVrgInfoTable);j++)
      {
         if(s_aiVrgInfoTable[j].Gain == (uint32)gain[i])
         {
            vrg[i] = s_aiVrgInfoTable[j].VrgType;
            break;
         }
      }
      if(j >= ARRAY_SIZE(s_aiVrgInfoTable))
      {
         return ErrorPropNotSpted;
      }
   }
   return Success;
}

inline 
ErrorCode BDaqAiImpl::Calibrate(uint32 mdlNumber, uint32 caliType)
{
   AI_CALI_CMD caliCmd;
   int ret = 0;
   caliCmd.mdlNumber = mdlNumber;
   caliCmd.caliType = (unsigned char)caliType;
   
   caliCmd.getResult = 0;  //send command
   m_kstubPtr->Ioctl(IOCTL_AI_CALIBRATE, &caliCmd);
   //wait 100ms for calibration completed 
   usleep(500000);
   
   caliCmd.getResult = 1; //get result
   ret = m_kstubPtr->Ioctl(IOCTL_AI_CALIBRATE, &caliCmd);
   if(ret){
      return ErrorDeviceIoTimeOut;
   }
   return Success;
}

inline
ErrorCode BDaqAiImpl::AccessAiValueRange(uint32 mdlNumber, uint32 chStart, uint32 chCount, void *valueRange, uint32 optFlag)
{
   ErrorCode warning = Success;
   ErrorCode ret = Success;
   uint32 mdlAiChlCount = get_chl_count(mdlNumber);
   if( mdlAiChlCount == 0 )  //there is no ai function on the module
      return ErrorFuncNotSpted;
   if(chStart >= mdlAiChlCount)
   {
      chStart = mdlAiChlCount - 1;
      SET_WARN(warning, WarningParamOutOfRange );
   }
   if( chCount == 0)
   {
      return Success;
   }
   if(chStart + chCount > mdlAiChlCount)
   {
      chCount = mdlAiChlCount - chStart;
      SET_WARN(warning, WarningParamOutOfRange );
   }
   uint8 chanGain[chCount];
   
   
   if(optFlag == 1) //set value range
   {
      //SetChanConfig
      GetGainCode(chStart, chCount, valueRange, chanGain); //translate value range to gain code
      AI_SET_CHAN setChan  = { mdlNumber, chStart, chCount, 1, chanGain };
      if (m_kstubPtr->Ioctl(IOCTL_AI_SET_CHAN_CFG, &setChan)) {
         ret = ErrorDeviceIoTimeOut;
      }
      
   }else{
      //get channel value range 
      AI_SET_CHAN getChan  = { mdlNumber, chStart, chCount, 0, chanGain };
      if (m_kstubPtr->Ioctl(IOCTL_AI_SET_CHAN_CFG, &getChan)) {
         ret = ErrorDeviceIoTimeOut;
      }

      //translate gain code to value range
      GetValuleRange(chStart, chCount, chanGain, (uint32*)valueRange);
   }
   return ret != Success ? ret : warning;
}


inline 
uint32 BDaqAiImpl::get_chl_count(uint32 mdlNumber)
{
   int i = 0;
   for(i = 0; i < MODULE_MAX_COUNT; i++)
   {
      if(m_kstubPtr->getShared()->mdlProfile[i].module_id == mdlNumber)
      {
         return m_kstubPtr->getShared()->mdlProfile[i].module_resource.ai_chl_num;
      }
   }
   return 0;
}

inline
ErrorCode BDaqAiImpl::ReadSamples(uint32 mdlNumber, uint32 chStart, uint32 chCount, void *rawData, double *scaledData)
{
   CHK_USER_BUF(rawData || scaledData);

   uint32 rawBuff[AI_CHL_COUNT];
   if (rawData == NULL) {
      rawData = rawBuff;
   }
   ErrorCode warning = Success;
   uint32 mdlAiChlCount = get_chl_count(mdlNumber);
   if( mdlAiChlCount == 0 )  //there is no ai on the module
      return ErrorFuncNotSpted;
   if(chStart >= mdlAiChlCount)
   {
      chStart = mdlAiChlCount - 1;
      SET_WARN(warning, WarningParamOutOfRange );
   }
   if(chStart + chCount > mdlAiChlCount)
   {
      chCount = mdlAiChlCount - chStart;
      SET_WARN(warning, WarningParamOutOfRange );
   }
   

   // read and scale data
   ErrorCode ret = Success;
   AI_READ_SAMPLES xbuf = { mdlNumber, chStart, chCount, rawData };
   if (m_kstubPtr->Ioctl(IOCTL_AI_READ_SAMPLES, &xbuf)){
      ret = ErrorDeviceIoTimeOut;
   }

   if (ret == Success && scaledData) { // conversion data from binary to floating
        //add code to convert scaled data
   }

   return ret != Success ? ret : warning;
}


inline
ErrorCode BDaqAiImpl::PropAccessFeatVrgTypeList(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
   if (IsOpGet(opFlag)) {
      return ErrorPropReadOnly;
   }
   return ErrorPropReadOnly;
}


inline
ErrorCode BDaqAiImpl::PropAccessChanCount(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
   if (IsOpGet(opFlag)) {
//      uint32 ch_count = m_kstubPtr->getShared()->AiLogChanCount;
//      return DefCopyT(ch_count, buffer, bufLen);
         return ErrorPropReadOnly;
   }
   return ErrorPropReadOnly;
}



#endif /* _BDAQ_AI_IMPL_H_ */
