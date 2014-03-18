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
   { V_Neg10To10,       0, -10,     10,   V_Neg10To10_String },
   { V_Neg5To5,         1, -5,      5,    V_Neg5To5_String },
   { V_Neg2pt5To2pt5,   2, -2.5,    2.5,  V_Neg2pt5To2pt5_String },
   { V_Neg1pt25To1pt25, 3, -1.25,   1.25, V_Neg1pt25To1pt25_String },
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

//   virtual ErrorCode EventGetHandle(EventId id, HANDLE &handle);
//   virtual ErrorCode EventGetStatus(EventId id, uint32 *lParam, uint32 *rParam);
//   virtual ErrorCode EventClearFlag(EventId id, uint32 lParam, uint32 rParam);

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
   virtual ErrorCode AccessAiValueRange(uint32 mdlNumber, uint32 chStart, uint32 chCount, void *valueRange, uint32 optFlag);
//   virtual ErrorCode 

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

   void RefreshChanSetting();

protected:
   ErrorCode PropAccessFeatVrgTypeList(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
   ErrorCode PropAccessChanCount(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
//   ErrorCode PropAccessChanVrgType(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);


protected:
   ErrorCode GetGainCode(uint32 chStart, uint32 chCount, void *valueRange, void *gainCode);
   ErrorCode GetValuleRange(uint32 gain, uint32 &valueRange);
//   ErrorCode CheckUserChanCnntType(uint32 * types, uint32 count);
//   ErrorCode CheckUserChanVrgType(uint32 * types, uint32 count);

//   ErrorCode SetChanCnntType(uint32 phyStart, uint32 phyCount, uint32 types[]);
//   ErrorCode SetChanVrgType(uint32 phyStart, uint32 phyCount, uint32 types[]);
//   ErrorCode SetChanConfig(uint32 phyStart, uint32 phyCount, uint8 types[], uint8 gains[]);


private:
   BioKrnlStub     *m_kstubPtr;

   // AI channel setting(value range info)
//   LIST_ENTRY      m_scaleListHead;                // AI scale table: each value range has an entry in the table.
//   uint32          m_nextCustomizedVrgCode;        // AI customized value range code
//   PSCALING_ENTRY  m_phyChanVrg[AI_CHL_COUNT];     // AI channel value range information for each "Physical" channel.

   // FAI
//   HANDLE          m_aiEvents[ARRAY_SIZE(s_aiSptedEvts)];
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
   uint32 *gain = (uint32*)gainCode;
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
ErrorCode BDaqAiImpl::GetValuleRange(uint32 gain, uint32 &valueRange)
{
   uint32 i;
   for(i=0; i<ARRAY_SIZE(s_aiVrgInfoTable);i++)
   {
      if(s_aiVrgInfoTable[i].Gain == gain)
      {
         valueRange = s_aiVrgInfoTable[i].VrgType;
         return Success;
      }
   }
   return ErrorPropNotSpted;
}

inline
ErrorCode BDaqAiImpl::AccessAiValueRange(uint32 mdlNumber, uint32 chStart, uint32 chCount, void *valueRange, uint32 optFlag)
{
   ErrorCode warning = Success;
   ErrorCode ret = Success;
   uint32 mdlAiChlCount = m_kstubPtr->getShared()->mdlFuncInfo[mdlNumber].funcInfo[module_func_ai].chlCount;
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
   uint32 chanGain[chCount];
   GetGainCode(chStart, chCount, valueRange, chanGain);
   AI_SET_CHAN setChan  = { mdlNumber, chStart, chCount, chanGain };
   
   if(optFlag == 1) //set value range
   {
      //SetChanConfig
      if (m_kstubPtr->Ioctl(IOCTL_AI_SET_CHAN_CFG, &setChan)) {
         ret = ErrorDeviceIoTimeOut;
      }
      
   }else{
      //get channel value range 
      uint32 chl = 0;
      uint32 *vrg = (uint32 *)valueRange;
      for(chl =0; chl<chCount; chl++)
      {
         uint32 gain = m_kstubPtr->getShared()->mdlFuncInfo[mdlNumber].funcInfo[module_func_ai].gainCode[chStart + chl];
         GetValuleRange(gain, vrg[chl]);
      }
      
   }
   return ret != Success ? ret : warning;
}


inline
ErrorCode BDaqAiImpl::ReadSamples(uint32 mdlNumber, uint32 chStart, uint32 chCount, void *rawData, double *scaledData)
{
   CHK_USER_BUF(rawData || scaledData);

//   printf("user mode IOCTL_AI_READ_SAMPLES = %x mdlNumber = %x start = %x chCount = %x rawData = %x scaled = %x\n", IOCTL_AI_READ_SAMPLES, mdlNumber, chStart, chCount, rawData, scaledData);
   uint32 rawBuff[AI_CHL_COUNT];
   if (rawData == NULL) {
      rawData = rawBuff;
   }
   ErrorCode warning = Success;
   uint32 mdlAiChlCount = m_kstubPtr->getShared()->mdlFuncInfo[mdlNumber].funcInfo[module_func_ai].chlCount;
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
   printf("user mode IOCTL_AI_READ_SAMPLES = %x\n", IOCTL_AI_READ_SAMPLES);
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
/*
inline
ErrorCode BDaqAiImpl::PropAccessChanCnntType(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
   if (IsOpGet(opFlag)) {
      uint8 const * ptrType = m_kstubPtr->getShared()->AiChanType;
      return DefCopyT(ptrType, AI_CHL_COUNT, (uint32*)buffer, bufLen);
   } else {
      CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
      CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32) * AI_CHL_COUNT);

      ErrorCode ret = CheckUserChanCnntType((uint32*)buffer, AI_CHL_COUNT);
      CHK_EXPR(ret == Success, ret);

      ret = SetChanCnntType(0, AI_CHL_COUNT, (uint32*)buffer);
      if (ret == Success && IsOpNtfNow(opFlag)) {
         m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
      }
      return ret;
   }
}
*/
/*
inline
ErrorCode BDaqAiImpl::PropAccessChanVrgType(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
   if (IsOpGet(opFlag)) {
      uint32 types[AI_CHL_COUNT];
      bool needRefresh = false;
      uint8 const * gains = m_kstubPtr->getShared()->AiChanGain;
      for (int i = 0; i < AI_CHL_COUNT; ++i) {
         if (m_phyChanVrg[i] && m_phyChanVrg[i]->Gain == gains[i]) {
            types[i] = m_phyChanVrg[i]->VrgType;
         } else {
            PSCALING_ENTRY entry = ScaleListFindEntry(&m_scaleListHead, gains[i], true);
            types[i] = entry ? entry->VrgType : V_OMIT;
            needRefresh = true;
         }
      }

      if (needRefresh) {
         RefreshChanSetting();
      }
      return DefCopyPtr(types, sizeof(types), buffer, bufLen);
   }else {
      CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
      CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32) * AI_CHL_COUNT);

      ErrorCode ret = CheckUserChanVrgType((uint32*)buffer, AI_CHL_COUNT);
      CHK_EXPR(ret == Success, ret);

      ret = SetChanVrgType(0, AI_CHL_COUNT, (uint32*)buffer);
      if (ret == Success && IsOpNtfNow(opFlag)) {
         m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
      }
      return ret;
   }
}
*/
/*
inline
ErrorCode BDaqAiImpl::CheckUserChanVrgType(uint32 * types, uint32 count)
{
   for (uint32 i = 0; i < count; ++i) {
      CHK_EXPR(ScaleListFindEntry(&m_scaleListHead, types[i], false), ErrorPropValueNotSpted);
   }
   return Success;
}
*/
/*
inline
ErrorCode BDaqAiImpl::SetChanCnntType(uint32 phyStart, uint32 phyCount, uint32 types[])
{

   uint8 ucBuf[AI_CHL_COUNT];
   uint32 len = sizeof(ucBuf);
   DefCopyT(types, phyCount * sizeof(uint32), ucBuf, len);
   return SetChanConfig(phyStart, phyCount, ucBuf, NULL);
}

inline
ErrorCode BDaqAiImpl::SetChanVrgType(uint32 phyStart, uint32 phyCount, uint32 type[])
{
   if (phyCount > AI_CHL_COUNT) {
      phyCount = AI_CHL_COUNT;
   }

   // backup current vrg type of channels
   PSCALING_ENTRY chVrgBkup[AI_CHL_COUNT];
   memcpy(chVrgBkup, m_phyChanVrg, sizeof(chVrgBkup));

   // get gain code from vrg type
   uint8 gains[AI_CHL_COUNT];
   for (uint32 i = 0; i < phyCount; ++i) {
      PSCALING_ENTRY scaleEntry = ScaleListFindEntry(&m_scaleListHead, type[i], false);
      if (scaleEntry) {
         gains[i] = (uint8)scaleEntry->Gain;
         m_phyChanVrg[(phyStart + i) & AI_CHL_MASK] = scaleEntry;
      } else {
         gains[i] = (uint8)m_phyChanVrg[(phyStart + i) & AI_CHL_MASK]->Gain;
      }
   }
   ErrorCode ret = SetChanConfig(phyStart, phyCount, NULL, gains);
   if (ret != Success) {
      memcpy(m_phyChanVrg, chVrgBkup, sizeof(chVrgBkup));
   }
   return ret;
}

inline
ErrorCode BDaqAiImpl::SetChanConfig(uint32 phyStart, uint32 phyCount, uint8 types[], uint8 gains[])
{
   AI_CHAN_CFG chanCfg[AI_CHL_COUNT];
   AI_SET_CHAN setChan  = { 0, phyStart, phyCount, chanCfg };

   uint8 const * typesPtr;
   uint8 const * gainsPtr;

   if (types) {
      setChan.SetWhich |= AI_SET_CHSCTYPE;
      typesPtr = types;
   } else {
      typesPtr = m_kstubPtr->getShared()->AiChanType;
   }

   if (gains)  {
      setChan.SetWhich |= AI_SET_CHGAIN;
      gainsPtr = gains;
   } else {
      gainsPtr = m_kstubPtr->getShared()->AiChanGain;
   }

   if (!setChan.SetWhich) {
      return Success;
   }

   for (unsigned i = 0; i < setChan.PhyChanCount; ++i) {
      chanCfg[i].SCType = typesPtr[i];
      chanCfg[i].Gain   = gainsPtr[i];
   }
   printf("user mode IOCTL_AI_SET_CHAN_CFG = %x\n", IOCTL_AI_SET_CHAN_CFG);
   if (m_kstubPtr->Ioctl(IOCTL_AI_SET_CHAN_CFG, &setChan)) {
      return ErrorDeviceIoTimeOut;
   }

   RefreshChanSetting();
   return Success;
}
*/


#endif /* _BDAQ_AI_IMPL_H_ */
