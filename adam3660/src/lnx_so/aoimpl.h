/*
 * AoImpl.h
 *
 *  Created on: 2014-2-24
 *      Author: shen.kun
 */

#ifndef _BDAQ_AO_IMPL_H_
#define _BDAQ_AO_IMPL_H_

#include <sys/time.h>
#include <asm/byteorder.h>
#include "krnlstub.h"

//
// Supported Value Range, Refer to hardware SPEC.
VRG_INFO const s_aoVrgInfoTable[] = {
      { V_Neg10To10,          V_Neg10To10,   -10, 10, V_Neg10To10_String },
      { V_Neg5To5,            V_Neg5To5,     -5,  5,  V_Neg5To5_String   },
      { V_0To10,              V_0To10,       0,   10, V_0To10_String     },
      { V_0To5,               V_0To5,        0,   5,  V_0To5_String      },
      { mA_0To20,             mA_0To20,      0,   20, mA_0To20_String    },
      { mA_4To20,             mA_4To20,      4,   20, mA_4To20_String    },
      { mA_0To24,             mA_0To24,      0,   24, mA_0To24_String    },
};



class BDaqAoImpl : public BDaqAo
{
public:
   // --------------------------------------------------------------
   // BDaqModule interface implementation
   // --------------------------------------------------------------
   virtual ModuleType getType()   {  return DaqAo; }
   virtual int32      getNumber() {  return 0;   }
   virtual AccessMode getMode()   {  return m_kstubPtr->getMode(); }

   // Property table
   BEGIN_PROPERTY_TABLE(BDaqAoImpl, PropAccess)
      // component specified properties
      // features
      PROP_RO_VAL(CFG_FeatureResolutionInBit,      uint32, AO_RES_IN_BIT)
      PROP_RO_VAL(CFG_FeatureChannelNumberMax,     uint32, AO_CHL_COUNT - 1)
     // PROP_CUSTOM(CFG_FeatureVrgTypes,             ReadOnly, PropAccessFeatVrgTypeList)

      PROP_RO_VAL(CFG_FeatureDataMask,             uint32, AO_DATA_MASK)

      // AO channel configuration
     // PROP_CUSTOM(CFG_VrgTypeOfChannels,           Writable, PropAccessChanVrgType   )
     // PROP_CUSTOM(CFG_InitialStateOfChannels,      Writable, PropAccessChlInitState  )

   END_PROPERTY_TABLE()

public:
   // --------------------------------------------------------------
   // BDaqAo interface implementation
   // --------------------------------------------------------------
   virtual ErrorCode WriteSamples(uint32 mdlNumber, uint32 chStart, uint32 chCount, void *rawData, double *scaledData);
   virtual ErrorCode AccessAoValueRange(uint32 mdlNumber, uint32 chStart, uint32 chCount, void *valueRange, uint32 optFlag);

public:
   // --------------------------------------------------------------
   // BDaqAoImpl interface implementation
   // --------------------------------------------------------------
   BDaqAoImpl()
   {
      m_kstubPtr  = NULL;
   }

   virtual ~BDaqAoImpl()
   {
      if (!m_kstubPtr) {
         return;
      }
   }

   ErrorCode Initialize(BioKrnlStub *kstub);
   ErrorCode Reset(uint32 state);

   void RefreshChanSetting();


protected:

   ErrorCode GetValuleRange(uint32 gain, uint32 &valueRange);
   ErrorCode GetGainCode(uint32 chStart, uint32 chCount, void *valueRange, void *gainCode);
//   ErrorCode CheckUserChanVrgType(uint32 * types, uint32 count);
//   ErrorCode SetChanConfig(AO_SET_CHAN *buffer, uint32 bufLen);
//   ErrorCode SetChanVrgType(uint32 chStart, uint32 chCount, uint32 vrgTypes[]);


private:
   BioKrnlStub     *m_kstubPtr;

};

// -----------------------------------------------------------------------------------------
// inline functions
// -----------------------------------------------------------------------------------------
inline
ErrorCode BDaqAoImpl::Initialize(BioKrnlStub *kstub)
{
   assert(kstub != NULL);
   m_kstubPtr = kstub;

   return Success;
}

inline
ErrorCode BDaqAoImpl::Reset(uint32 state)
{

	ErrorCode ret = Success;

   return ret;
}

inline
void BDaqAoImpl::RefreshChanSetting()
{

}

inline
ErrorCode BDaqAoImpl::GetGainCode(uint32 chStart, uint32 chCount, void *valueRange, void *gainCode)
{
   uint32 i = 0;
   uint32 j = 0;
   uint32 *vrg = (uint32*)valueRange;
   uint32 *gain = (uint32*)gainCode;
   for(i = 0; i<chCount; i++)
   {
      for(j=0; j<ARRAY_SIZE(s_aoVrgInfoTable); j++)
      {
         if(s_aoVrgInfoTable[j].VrgType == vrg[i])
         {
            gain[i] = s_aoVrgInfoTable[j].Gain;
            break;
         }
      }
      if(j >= ARRAY_SIZE(s_aoVrgInfoTable))
         return ErrorPropNotSpted;
   }
   return Success;
}

inline
ErrorCode BDaqAoImpl::GetValuleRange(uint32 gain, uint32 &valueRange)
{
   uint32 i;
   for(i=0; i<ARRAY_SIZE(s_aoVrgInfoTable);i++)
   {
      if(s_aoVrgInfoTable[i].Gain == gain)
      {
         valueRange = s_aoVrgInfoTable[i].VrgType;
         return Success;
      }
   }
   return ErrorPropNotSpted;
}


inline
ErrorCode BDaqAoImpl::AccessAoValueRange(uint32 mdlNumber, uint32 chStart, uint32 chCount, void *valueRange, uint32 optFlag)
{
   ErrorCode warning = Success;
   ErrorCode ret = Success;
   uint32 mdlAoChlCount = m_kstubPtr->getShared()->mdlFuncInfo[mdlNumber].funcInfo[module_func_ao].chlCount;
   if( mdlAoChlCount == 0 )  //there is no ao function on the module
      return ErrorFuncNotSpted;
   if(chStart >= mdlAoChlCount)
   {
      chStart = mdlAoChlCount - 1;
      SET_WARN(warning, WarningParamOutOfRange );
   }
   if( chCount == 0)
   {
      return Success;
   }
   if(chStart + chCount > mdlAoChlCount)
   {
      chCount = mdlAoChlCount - chStart;
      SET_WARN(warning, WarningParamOutOfRange );
   }
   uint32 chanGain[chCount];
   GetGainCode(chStart, chCount, valueRange, chanGain);
   AI_SET_CHAN setChan  = { 0, chStart, chCount, 1, chanGain };
   
   if(optFlag == 1) //set value range
   {
      //SetChanConfig
      if (m_kstubPtr->Ioctl(IOCTL_AO_SET_CHAN_CFG, &setChan)) {
         ret = ErrorDeviceIoTimeOut;
      }
      
   }else{
      //get channel value range 
      uint32 chl = 0;
      for(chl =0; chl<chCount; chl++)
      {
         uint32 *vrg = (uint32 *)valueRange;
         uint32 gain = m_kstubPtr->getShared()->mdlFuncInfo[mdlNumber].funcInfo[module_func_ao].gainCode[chStart + chl];
         GetValuleRange(gain, vrg[chl]);
      }
      
   }
   return ret;
}


inline
ErrorCode BDaqAoImpl::WriteSamples(uint32 mdlNumber, uint32 chStart, uint32 chCount, void *rawData, double *scaledData)
{
   uint16 raw_buf[MODULE_MAX_CHANNEL];
   if (rawData == NULL) {
      rawData = raw_buf;
//      ScaleDataToBinary(m_chanVrg, chStart, chStart + chCount, scaledData, raw_buf);
   }

   ErrorCode warning = Success;
   uint32 mdlAiChlCount = m_kstubPtr->getShared()->mdlFuncInfo[mdlNumber].funcInfo[module_func_ao].chlCount;
   if( mdlAiChlCount == 0 )  //there is no ao function on the module
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

   printf("usr mode: AO write samples\n");
   ErrorCode ret = Success;
   AO_WRITE_SAMPLES xbuf = { chStart, chCount, rawData};
   if (m_kstubPtr->Ioctl(IOCTL_AO_WRITE_SAMPLES, &xbuf)){
      ret = ErrorDeviceIoTimeOut;
   }

   return ret != Success ? ret : warning;
}

/*
inline
ErrorCode BDaqAoImpl::CheckUserChanVrgType(uint32 * types, uint32 count)
{
   for (uint32 i = 0; i < count; ++i) {
      CHK_EXPR(ScaleListFindEntry(&m_scaleListHead, types[i], false), ErrorPropValueNotSpted);
   }
   return Success;
}

inline
ErrorCode BDaqAoImpl::SetChanConfig(AO_SET_CHAN *buffer, uint32 bufLen)
{
   ErrorCode ret = Success;
   if (m_kstubPtr->Ioctl(IOCTL_AO_SET_CHAN_CFG, buffer)) {
      ret = ErrorDeviceIoTimeOut;
   }

   if (ret == Success && (buffer->SetWhich & AO_SET_CHVRG) != 0) {
      RefreshChanSetting();
   }
   return ret;
}

inline
ErrorCode BDaqAoImpl::SetChanVrgType(uint32 chStart, uint32 chCount, uint32 vrgTypes[])
{
   // backup current vrg type of channels
   PSCALING_ENTRY chVrgBkup[ ARRAY_SIZE(m_chanVrg) ];
   memcpy(chVrgBkup, m_chanVrg, sizeof(chVrgBkup));

   // get kernel vrg type from user vrg type
   uint32 gains[AO_CHL_COUNT];
   AO_SET_CHAN chCfg = { AO_SET_CHVRG, 0, 0,
      chStart & AO_CHL_MASK, __min(chCount, AO_CHL_COUNT), gains
   };
   for (unsigned i = 0; i < chCfg.ChanCount; ++i) {
      PSCALING_ENTRY scaleEntry = ScaleListFindEntry(&m_scaleListHead, vrgTypes[i], false);
      if (scaleEntry) {
         gains[i] = scaleEntry->Gain;
         m_chanVrg[(chStart + i) & AO_CHL_MASK] = scaleEntry;
      } else {
         gains[i] = m_chanVrg[(chStart + i) & AO_CHL_MASK]->Gain;
      }
   }

   ErrorCode ret = SetChanConfig(&chCfg, sizeof(chCfg));
   if (ret != Success) {
      memcpy(m_chanVrg, chVrgBkup, sizeof(chVrgBkup));
   }

   return ret;
}
*/
#endif /* _BDAQ_AO_IMPL_H_ */
