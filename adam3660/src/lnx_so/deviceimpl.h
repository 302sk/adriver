/*
 * BDaqDeviceImpl.h
 *
 *  Created on: 2014-2-24
 *      Author: shen.kun
 */

#ifndef _BDAQ_DEVICE_IMPL_H_
#define _BDAQ_DEVICE_IMPL_H_

#include <wchar.h>

#include "aiimpl.h"
#include "aoimpl.h"
#include "dioimpl.h"
#include "krnlstub.h"
#include "version.h"

const uint32 s_devSptedEvts[] = {EvtPropertyChanged};

class BDaqDeviceImpl : public BDaqDevice
{
public:
   // --------------------------------------------------------------
   // BDaqModule interface implementation
   // --------------------------------------------------------------
   virtual ModuleType getType()   {  return DaqDevice; }
   virtual int32      getNumber() {  return m_kstub.getDeviceNumber(); }
   virtual AccessMode getMode()   {  return m_kstub.getMode(); }

   virtual ErrorCode EventGetHandle(EventId id, HANDLE &handle);
   virtual ErrorCode EventGetStatus(EventId id, uint32 *lParam, uint32 *rParam);
   virtual ErrorCode EventClearFlag(EventId id, uint32 lParam, uint32 rParam);

   // Property table
   BEGIN_PROPERTY_TABLE(BDaqDeviceImpl, PropAccess)
      PROP_RO_VAL(CFG_Number,                    uint32,         m_kstub.getDeviceNumber())
      PROP_RO_VAL(CFG_ComponentType,             uint32,         DaqDevice)
      PROP_RO_VAL(CFG_DeviceProductID,           uint32,         m_kstub.getShared()->product_id)
      PROP_RO_VAL(CFG_DeviceBoardID,             uint32,         m_kstub.getShared()->BoardId)
      PROP_RO_PTR(CFG_DeviceSupportedEvents,     s_devSptedEvts, sizeof(s_devSptedEvts))
//      PROP_RO_VAL( CFG_DeviceBoardVersion,       uint32,         m_kstub.getShared()->HardwareVer   )

      PROP_CUSTOM(CFG_Description,               Writable,         PropAccessDescription)
      PROP_CUSTOM(CFG_DeviceDriverVersion,       ReadOnly,         PropAccessDriverVersion)
      PROP_CUSTOM(CFG_DeviceDllVersion,          ReadOnly,         PropAccessDriverVersion)
      PROP_CUSTOM(CFG_DeviceLocation,            ReadOnly,         PropAccessDeviceLocation)

   END_PROPERTY_TABLE()

public:
   // --------------------------------------------------------------
   // BDaqDevice interface implementation
   // --------------------------------------------------------------
   virtual void Close() {  delete this;  }

   virtual ErrorCode Refresh();
   virtual ErrorCode Reset(uint32 state);
   virtual ErrorCode GetModule(ModuleType type, uint32 index, BDaqModule **module);
   virtual ErrorCode DeviceFirmwareUpdate(uint32 mdlNumber, FILE *fp);

   virtual ErrorCode  ReadPorts(uint32 startAddr, uint32 length, void *buffer)
   {
      return DeviceDbgReadRegisters(m_kstub, startAddr, length, buffer);
   }
   virtual ErrorCode  WritePorts(uint32 startAddr, uint32 length, void *buffer)
   {
      return DeviceDbgWriteRegisters(m_kstub, startAddr, length, buffer);
   }

public:
   // --------------------------------------------------------------
   // BDaqDeviceImpl interface implementation
   // --------------------------------------------------------------
   BDaqDeviceImpl()
   {
      memset(m_devEvents, 0, sizeof(m_devEvents));
   }
   virtual ~BDaqDeviceImpl()
   {

   }

   ErrorCode Initialize(int32 devNumber, AccessMode mode);

protected:
   ErrorCode PropAccessDescription(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
   ErrorCode PropAccessDriverVersion(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
   ErrorCode PropAccessDeviceLocation(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);

private:
   BioKrnlStub m_kstub;
   BDaqAiImpl  m_ai;
   BDaqAoImpl   m_ao;
   BDaqDioImpl  m_dio;
   HANDLE      m_devEvents[ARRAY_SIZE(s_devSptedEvts)]; // device events
};

// -----------------------------------------------------------------------------------------
// inline functions
// -----------------------------------------------------------------------------------------
inline
ErrorCode BDaqDeviceImpl::Refresh()
{
   // Need do nothing.
   if (m_kstub.getMode() == ModeRead) {
      m_ai.RefreshChanSetting();
      m_ao.RefreshChanSetting();
   }

   return Success;
}

inline
ErrorCode BDaqDeviceImpl::Reset(uint32 state)
{
   m_ai.Reset(state);
   return Success;
}

inline
ErrorCode BDaqDeviceImpl::Initialize(int32 devNumber, AccessMode mode)
{
   ErrorCode ret;
   do{
      if ((ret = m_kstub.Initialize(devNumber, mode)) != Success) {
         break;
      }

      if ((ret = m_ai.Initialize(&m_kstub)) != Success){
         break;
      }

      if ((ret = m_ao.Initialize(&m_kstub)) != Success){
         break;
      }

      if ((ret = m_dio.Initialize(&m_kstub)) != Success){
         break;
      }


   }while(0);

   return ret;
}

inline
ErrorCode BDaqDeviceImpl::GetModule(ModuleType type, uint32 index, BDaqModule **module)
{
   CHK_EXPR(index == 0, ErrorFuncNotSpted);
   CHK_USER_BUF(module);
   switch (type)
   {
   case DaqAi:  *module = &m_ai;   break;
   case DaqAo:  *module = &m_ao;    break;
   case DaqDio:  *module = &m_dio;   break;
   default:
      return ErrorFuncNotSpted;
   }
   return Success;
}

inline
static __u16 CalcCheckSum(__u8* data, __u16 len)
{
   int i = 0;
   __u16 chk_sum = 0;
   for(i = 0; i< len; i++)
   {
      chk_sum += data[i];
   }
   return chk_sum;
}

inline
ErrorCode BDaqDeviceImpl::DeviceFirmwareUpdate(uint32 mdlNumber, FILE *fp)
{
   __u8 data[68];
   DEVICE_FIRMWARE_DOWNLOAD test;
   __u16 chk_sum = 0;
   test.mdlNumber = mdlNumber;
   test.index = 0;
   test.data = data;
   test.len = 2;
   test.cmd = 1;  //start command
   
   m_kstub.Ioctl(IOCTL_DEVICE_FIRMWARE_DOWNLOAD, &test); //start 1
   usleep(4000000);
   m_kstub.Ioctl(IOCTL_DEVICE_FIRMWARE_DOWNLOAD, &test); //start 2
   usleep(4000000);

   test.index = 1;
   test.cmd = 2;
   while(!feof(fp))
   {
      test.len = fread(data, 1, 64, fp);
      chk_sum += CalcCheckSum(data, test.len);
      if(feof(fp))
      {
         test.index |= 0x8000;
         *(__u16 *)(&data[test.len]) = chk_sum;
         test.len += 2;
      }
      m_kstub.Ioctl(IOCTL_DEVICE_FIRMWARE_DOWNLOAD, &test);  //dwonload
      test.index ++;
   }
   test.cmd = 3; //reset
   test.index = 0;
   m_kstub.Ioctl(IOCTL_DEVICE_FIRMWARE_DOWNLOAD, &test);    //reset
   return Success;
}


inline
ErrorCode BDaqDeviceImpl::EventGetHandle(EventId id, HANDLE &handle)
{
   handle = NULL;

   // Look for the event in supported event list
   int index = ARRAY_INDEX_OF(s_devSptedEvts, (uint32)id);
   if (index == -1) {
      return ErrorEventNotSpted;
   }

   // Create the event if it hasn't been created.
   if (m_devEvents[index] == NULL) {
      USER_EVENT_INFO info = { id };
      m_kstub.Ioctl(IOCTL_DEVICE_REGISTER_USER_EVENT, &info);
      m_devEvents[index] = info.Handle;
   }

   handle = m_devEvents[index];
   return handle != NULL ? Success : ErrorMemoryNotEnough;
}

inline
ErrorCode BDaqDeviceImpl::EventGetStatus(EventId id, uint32 *lParam, uint32 *rParam)
{
   CHK_EXPR(ARRAY_INDEX_OF(s_devSptedEvts, (uint32)id) != -1, ErrorEventNotSpted);
   return Success;
}

inline
ErrorCode BDaqDeviceImpl::EventClearFlag(EventId id, uint32 lParam, uint32 rParam)
{
   CHK_EXPR(ARRAY_INDEX_OF(s_devSptedEvts, (uint32)id) != -1, ErrorEventNotSpted);
   return Success;
}

inline
ErrorCode BDaqDeviceImpl::PropAccessDescription(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  if (IsOpGet(opFlag))
  {
     // get description from kernel
     char name[64];
     m_kstub.Ioctl(IOCTL_DEVICE_GET_DESC, name);

     // convert to wide-char
     wchar_t wname[64];
     int len = mbstowcs(wname, name, 64);

     // copy to user buffer
     return DefCopyPtr(wname, (len + 1) * sizeof(wchar_t), buffer, bufLen);
  } else {
     CHK_PRVLG(m_kstub.getMode() != ModeRead);
     return Success;
  }
}

inline
ErrorCode BDaqDeviceImpl::PropAccessDriverVersion(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  if (IsOpGet(opFlag))
  {
     if (id == CFG_DeviceDriverVersion)
     {
        const wchar_t *ver = DEV_DRIVER_VER;
        return DefCopyPtr(ver, (wcslen(ver) + 1) * sizeof(wchar_t), buffer, bufLen);
     } else {
        assert(id == CFG_DeviceDllVersion);
        const wchar_t *ver = DEV_DRIVER_VER;
        return DefCopyPtr(ver, (wcslen(ver) + 1) * sizeof(wchar_t), buffer, bufLen);
     }
  }
  return ErrorPropReadOnly;
}

inline
ErrorCode BDaqDeviceImpl::PropAccessDeviceLocation(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
   if (IsOpGet(opFlag))
   {
      // get description from kernel
      char loc[64];
      m_kstub.Ioctl(IOCTL_DEVICE_GET_LOCATION_INFO, loc);

      // convert to wide-char
      wchar_t wloc[64];
      int len = mbstowcs(wloc, loc, 64);

      // copy to user buffer
      return DefCopyPtr(wloc, (len + 1) * sizeof(wchar_t), buffer, bufLen);
   }
   return ErrorPropReadOnly;
}


#endif /* BDAQDEVICEIMPL_H_ */
