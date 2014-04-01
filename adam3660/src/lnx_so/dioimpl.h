/*
 * BioDio.h
 *
 *  Created on: 2014-2-24
 *      Author: shen.kun
 */

#ifndef _BDAQ_DIO_IMPL_H_
#define _BDAQ_DIO_IMPL_H_

#include "krnlstub.h"

//const uint8  s_dioPortType[] = { PortDio, PortDio}; // DIO port type
const uint8  s_dioPortType[] = { Port8255A, Port8255A}; // DIO port type

const uint8  s_diDataMask[] = {0xff, 0xff};
const uint8  s_doDataMask[] = {0x00, 0x00};


class BDaqDioImpl : public BDaqDio
{
public:
   // --------------------------------------------------------------
   // BDaqModule interface implementation
   // --------------------------------------------------------------
   virtual ModuleType getType()   { return DaqDio;   }
   virtual int32      getNumber() {  return 0; }
   virtual AccessMode getMode()   { return m_kstubPtr->getMode(); }

   // Property table
   BEGIN_PROPERTY_TABLE(BioDioImpl, PropAccess)
      // component specified properties
      // features
      PROP_RO_VAL(CFG_FeatureChannelNumberMax, uint32,        DIO_CHL_COUNT - 1)
      PROP_RO_VAL(CFG_FeaturePortsCount,       uint32,        DIO_PORT_COUNT)
      PROP_RO_PTR(CFG_FeaturePortsType,        s_dioPortType, sizeof(s_dioPortType))

      // DIO configuration
//      PROP_CUSTOM(CFG_DiDataMaskOfPorts,       ReadOnly,  PropAccessPortsDataMask)
//      PROP_CUSTOM(CFG_DoDataMaskOfPorts,       ReadOnly,  PropAccessPortsDataMask)
//      PROP_CUSTOM(CFG_DirectionOfPorts,        Writable, PropAccessPortDirection)
//      PROP_CUSTOM(CFG_DoInitialStateOfPorts,   Writable, PropAccessDoChlInitState)
   END_PROPERTY_TABLE()

public:
   // --------------------------------------------------------------
   // BDaqDio interface implementation
   // --------------------------------------------------------------
   virtual ErrorCode ReadDiPorts(uint32 mdlNumber, uint32 portStart, uint32 portCount, uint8 buffer[]);
   virtual ErrorCode WriteDoPorts(uint32 mdlNumber, uint32 portStart, uint32 portCount, uint8 buffer[]);
   virtual ErrorCode ReadDoPorts(uint32 mdlNumber, uint32 portStart, uint32 portCount, uint8 buffer[]);

public:
   // --------------------------------------------------------------
   // BDaqDioImpl interface implementation
   // --------------------------------------------------------------
   BDaqDioImpl()
   {
      m_kstubPtr = NULL;
   }

   virtual ~BDaqDioImpl()
   {
   }

   ErrorCode Initialize(BioKrnlStub *kstub);
   ErrorCode Reset(uint32 state);

protected:
     uint32 get_chl_count(uint32 mdlNumber);
//   ErrorCode PropAccessPortDirection(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
//   ErrorCode PropAccessDoChlInitState(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
//   ErrorCode PropAccessPortsDataMask( uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag );

//   ErrorCode CheckUserPortDir(uint8 *dirs, uint32 count);
//   ErrorCode SetDioPortDir(uint32 portStart, uint32 portCount, uint8 *dirs);
private:
   BioKrnlStub  *m_kstubPtr;
};

// -----------------------------------------------------------------------------------------
// inline functions
// -----------------------------------------------------------------------------------------
inline
ErrorCode BDaqDioImpl::Initialize(BioKrnlStub *kstub)
{
   assert(kstub != NULL);
   m_kstubPtr = kstub;
   return Success;
}

inline
ErrorCode BDaqDioImpl::Reset(uint32 state)
{
   CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);


   return Success;
}
/*
inline
ErrorCode BDaqDioImpl::PropAccessPortDirection(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
   if (IsOpGet(opFlag)) {
      uint8 const * ptr = m_kstubPtr->getShared()->DioPortDir;
      uint32       size = sizeof(m_kstubPtr->getShared()->DioPortDir);
      return DefCopyT(ptr, size, (uint8 *)buffer, bufLen);
   } else {
      CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
      CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint8) * DIO_PORT_COUNT);

		ErrorCode ret = Success;
      ret = SetDioPortDir(0, DIO_PORT_COUNT, (uint8*)buffer);
      if (ret == Success && IsOpNtfNow(opFlag)) {
         m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
      }
      return ret;
   }
}

inline
ErrorCode BDaqDioImpl::PropAccessPortsDataMask( uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag )
{
    if ( IsOpGet( opFlag ) )
    {
        uint8 dirs[DIO_PORT_COUNT];
        uint8 const * ptr = m_kstubPtr->getShared()->DioPortDir;
        uint32       size = sizeof(m_kstubPtr->getShared()->DioPortDir);
        
        DefCopyT(ptr, size, (uint8 *)buffer, size);
        
        if ( id == CFG_DiDataMaskOfPorts )
        {
            // DI ports mask is just the opposite to the port direction value.
            for ( uint32 i = 0; i < DIO_PORT_COUNT; ++i )
            {
                dirs[i] = ~dirs[i];
            }
        } // else : CFG_DoPortsDataMask : it is just the same as the port direction value.
        return DefCopyT( dirs, sizeof(dirs), (uint8*)buffer, bufLen );
    } 
    return ErrorPropReadOnly;
}


inline
ErrorCode BDaqDioImpl::PropAccessDoChlInitState(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
   if (IsOpGet(opFlag)) {
      uint8 state[DIO_PORT_COUNT];
      std::fill_n(state, DIO_PORT_COUNT, DEF_DO_STATE);

      return DefCopyT(state, sizeof(state), (uint8 *)buffer, bufLen);
   } else {
      CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
      CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint8) * DIO_PORT_COUNT);

      ErrorCode ret = Success;
      return ret;
   }
}
*/

inline 
uint32 BDaqDioImpl::get_chl_count(uint32 mdlNumber)
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
ErrorCode BDaqDioImpl::ReadDiPorts(uint32 mdlNumber, uint32 portStart, uint32 portCount, uint8 buffer[])
{
   ErrorCode warning = Success;
   uint32 mdlDiChlCount = get_chl_count(mdlNumber);
   if( mdlDiChlCount == 0 )  //there is no ai function on the module
      return ErrorFuncNotSpted;
   if(portStart >= mdlDiChlCount)
   {
      portStart = mdlDiChlCount - 1;
      SET_WARN(warning, WarningParamOutOfRange );
   }
   if( portCount == 0)
   {
      return Success;
   }
   if(portStart + portCount > mdlDiChlCount)
   {
      portCount = mdlDiChlCount - portStart;
      SET_WARN(warning, WarningParamOutOfRange );
   }
   
   printf("user mode: DIO read DI ports\n");
   DIO_RW_PORTS xbuf = { mdlNumber, portStart, portCount, buffer };
   if (m_kstubPtr->Ioctl(IOCTL_DIO_READ_DI_PORTS, &xbuf)){
      return ErrorDeviceIoTimeOut;
   }
   return warning;
}

inline
ErrorCode BDaqDioImpl::WriteDoPorts(uint32 mdlNumber, uint32 portStart, uint32 portCount, uint8 buffer[])
{
   ErrorCode warning = Success;
   uint32 mdlDoChlCount = get_chl_count(mdlNumber);
   if( mdlDoChlCount == 0 )  //there is no do function on the module
      return ErrorFuncNotSpted;
   if(portStart >= mdlDoChlCount)
   {
      portStart = mdlDoChlCount - 1;
      SET_WARN(warning, WarningParamOutOfRange );
   }
   if( portCount == 0)
   {
      return Success;
   }
   if(portStart + portCount > mdlDoChlCount)
   {
      portCount = mdlDoChlCount - portStart;
      SET_WARN(warning, WarningParamOutOfRange );
   }

   printf("user mode: DIO write DO ports\n");
   DIO_RW_PORTS xbuf = { mdlNumber, portStart, portCount, buffer };
   if (m_kstubPtr->Ioctl(IOCTL_DIO_WRITE_DO_PORTS, &xbuf)){
      return ErrorDeviceIoTimeOut;
   }
   return warning;
}

inline
ErrorCode BDaqDioImpl::ReadDoPorts(uint32 mdlNumber, uint32 portStart, uint32 portCount, uint8 buffer[])
{
   CHK_USER_BUF(buffer);
   ErrorCode warning = Success;
   uint32 mdlDoChlCount = get_chl_count(mdlNumber);
   if( mdlDoChlCount == 0 )  //there is no ai function on the module
      return ErrorFuncNotSpted;
   if(portStart >= mdlDoChlCount)
   {
      portStart = mdlDoChlCount - 1;
      SET_WARN(warning, WarningParamOutOfRange );
   }
   if( portCount == 0)
   {
      return Success;
   }
   if(portStart + portCount > mdlDoChlCount)
   {
      portCount = mdlDoChlCount - portStart;
      SET_WARN(warning, WarningParamOutOfRange );
   }

   printf("user mode: DIO read DO ports\n");
   DIO_RW_PORTS xbuf = { mdlNumber, portStart, portCount, buffer };
   if (m_kstubPtr->Ioctl(IOCTL_DIO_READ_DO_PORTS, &xbuf)){
     return ErrorDeviceIoTimeOut;
   }
   return warning;
}
/*
inline
ErrorCode BDaqDioImpl::CheckUserPortDir(uint8 *dirs, uint32 count)
{
   for (unsigned i = 0; i < count; ++i) {
      CHK_EXPR(dirs[i] == DIR_IN || dirs[i] == DIR_OUT, ErrorPropValueNotSpted);
   }
   return Success;
}
*/
/*
inline
ErrorCode BDaqDioImpl::SetDioPortDir(uint32 portStart, uint32 portCount, uint8 *dirs )
{
   CHK_USER_BUF(dirs);

   DIO_SET_PORT_DIR xbuf = {portStart, portCount, dirs};
   if (m_kstubPtr->Ioctl(IOCTL_DIO_SET_PORT_DIR, &xbuf)){
      return ErrorDeviceIoTimeOut;
   }
   return Success;
}
*/
#endif /* _BDAQ_DIO_IMPL_H_ */
