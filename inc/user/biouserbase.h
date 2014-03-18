/*******************************************************************************
              Copyright (c) 1983-2009 Advantech Co., Ltd.
********************************************************************************
THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY INFORMATION
WHICH IS THE PROPERTY OF ADVANTECH CORP., ANY DISCLOSURE, USE, OR REPRODUCTION,
WITHOUT WRITTEN AUTHORIZATION FROM ADVANTECH CORP., IS STRICTLY PROHIBITED. 

================================================================================
REVISION HISTORY
--------------------------------------------------------------------------------
$Log: $
--------------------------------------------------------------------------------
$NoKeywords:  $
*/

#ifndef BIONIC_USER_MODE_LIB
#define BIONIC_USER_MODE_LIB

#include <stdlib.h>
#include <string.h>
#include <bdaqdef.h>
#include "linklist.h"

#if defined(_WIN32) || defined(WIN32)
#	if !defined(_WIN32_WCE)
#		include <setupapi.h>
#	endif
#endif

#ifdef __cplusplus
#  define BDAQ_BEGIN_C_DECLARE extern "C" {
#  define BDAQ_END_C_DECLARE   }
#else
#  define BDAQ_BEGIN_C_DECLARE 
#  define BDAQ_END_C_DECLARE
#endif

//-------------------------------------------------------------------------
// macros used by user mode modules
//-------------------------------------------------------------------------
#if defined(_WIN32_WCE) || defined(__linux__)
#  define _malloca malloc
#  define _freea   free
#endif

#if defined(__linux__)
#  define MAXIMUM_WAIT_OBJECTS 64
#endif

#if defined(_WIN32) || defined(WIN32)
//The following In/Out macro is used in DLL only, the kernel mode use AdxKeMemInX
#  define AdxMemInB( MemBase, Offset ) (  *(PUCHAR)( (PUCHAR)MemBase + Offset ) )
#  define AdxMemInW( MemBase, Offset ) ( *(PUSHORT)( (PUCHAR)MemBase + Offset ) )
#  define AdxMemInD( MemBase, Offset ) (  *(PULONG)( (PUCHAR)MemBase + Offset ) )

#  define AdxMemOutB( MemBase, Offset, Data ) (  *(PUCHAR)( (PUCHAR)MemBase + Offset ) =  (UCHAR)Data )
#  define AdxMemOutW( MemBase, Offset, Data ) ( *(PUSHORT)( (PUCHAR)MemBase + Offset ) = (USHORT)Data )
#  define AdxMemOutD( MemBase, Offset, Data ) (  *(PULONG)( (PUCHAR)MemBase + Offset ) =  (ULONG)Data )

typedef ULONGLONG PHYSICAL_ADDRESS;
#endif

// --------------------------------------------------------------------------
// Macros to verify user parameters
// --------------------------------------------------------------------------
#define CHK_EXPR(expr, err)            do { if (!(expr)) return err; } while(0)
#define CHK_HANDLE(hdl)                CHK_EXPR(hdl != NULL, ErrorHandleNotValid)
#define CHK_PRVLG(expr)                CHK_EXPR(expr,  ErrorPrivilegeNotHeld)
#define CHK_DATA_PTR(ptr)              CHK_EXPR(ptr != NULL, ErrorBufferIsNull)
#define CHK_USER_BUF(buf)              CHK_EXPR(buf != NULL, ErrorBufferIsNull)
#define CHK_USER_BUF_LEN(buf, bufLen)  do{ if (buf == NULL) return ErrorBufferIsNull; if (!(bufLen)) return ErrorBufferTooSmall; } while(0)
#ifdef __linux__
#  define CHK_HANDLE_TYPE(hdl, type)   CHK_EXPR(hdl != NULL && ((BDaqModule*)hdl)->getType() == type, ErrorHandleNotValid)
#else
#  define CHK_HANDLE_TYPE(hdl, type)   CHK_EXPR(hdl != NULL && ((BioComponent*)(hdl))->GetType() == type, ErrorHandleNotValid)
#endif

#define SET_WARN( w, newWarn )         do{ if ((w) == Success) { w = newWarn; } } while(0)

// --------------------------------------------------------------------------
// Macro to calculate actual section length for buffered operation.
// --------------------------------------------------------------------------
#define CALC_ACTUAL_SECT_SIZE(SectionSize, SampleCount) \
   ( SectionSize > SampleCount ? SampleCount : ( SectionSize ? SectionSize : ( SampleCount + 1 ) / 2 ) )

//-----------------------------------------------------------------------------------------
// Helper function to get event index in the specified array.
//-----------------------------------------------------------------------------------------
template<class T>
inline int ArrayIndexOf(T const *array, uint32 len, T target)
{
   for (T const *curr = array; curr < array + len; ++curr){
      if (*curr == target) return (int)(curr - array);
   }
   return -1; // Not found.
}

template<>
inline int ArrayIndexOf(uint32 const *array, uint32 len, uint32 target)
{
   for (uint32 const *curr = array; curr < array + len; ++curr){
      if (*curr == target) return (int)(curr - array);
   }
   return -1; // Not found.
}

#define ARRAY_INDEX_OF(ar, target)  ArrayIndexOf(ar, ARRAY_SIZE(ar), target)

// -------------------------------------------------------------------------------
// debug helper
// -------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(DEBUG) || defined(DBG) || defined(DAQ_TRACING) || defined(BIO_TRACING)
#	if defined(_WIN32_WCE) || defined(__linux__)
#		define daq_trace(_x_)  printf _x_  
#	else
#     include <tchar.h>
		inline void DbgPrint(TCHAR * fmt,...)
		{
		   TCHAR msg[256];
         va_list args;

         va_start(args, fmt);
         _vsntprintf_s(msg, _TRUNCATE, fmt, args);
         va_end(args);

         OutputDebugString(msg);
		}
#		define daq_trace( _x_ )  DbgPrint _x_
#	endif
#else
#  define daq_trace( _x_ )  do{}while(0)
#endif

#define BioTrace  daq_trace // for compatible with our linux driver

//-------------------------------------------------------------------------
// AI/AO function macros and structures
//-------------------------------------------------------------------------
//
// AI/AO Value range and gain code mapping table.
typedef struct _VRG_INFO{
   uint32  VrgType;
   uint32  Gain;
   double  RangeLow;
   double  RangeHigh;
   wchar_t const *VrgName;
} VRG_INFO;
typedef VRG_INFO const * PCVRG_INFO;

//
// AI/AO scaling info entry(polynomial for data conversion)
// !!!Internal use only!!!!
typedef struct _BioMapFuncPiece{
   uint32 size;         /* the size of the structure  */
   uint32 degree;       /* the polynomial degree */
   double upperLimit;   /* the upper limit for this scaling polynomial. */
   double coef[2];      /* variable length array for the coefficient of polynomial, in increasing degree. */
}BioMapFuncPiece,*PBioMapFuncPiece;

typedef struct _SCALING_ENTRY{
   LIST_ENTRY      ListEntry;
   uint32          VrgType;     // Predefined or user customized value range type.
   uint32          Gain;        // device specified gain code; 
   uint32          PieceCount;  // polynomial count.
   BioMapFuncPiece Pieces[1];   // variable length array, each of it is a scaling polynomial.
} SCALING_ENTRY, *PSCALING_ENTRY;

// default piece count of map function
#define AI_DEF_PIECE_COUNT       1
#define AO_DEF_PIECE_COUNT       2

// Allocator and Deallocator of SCALING_ENTRY
#define AllocScalingEntry( size )  ((PSCALING_ENTRY)new uint32[(size + 3)/4])
#define FreeScalingEntry( entry )  (delete [] entry)

#define ANALOG_LSB_SPAN(low, high, bin_span)    (((high) - (low)) / (bin_span))
#define ANALOG_LSB_BITS(low, high, bits)        ANALOG_LSB_SPAN(low, high, (1<<bits))

/*-------------------------------------------------------------------
* Thermocouple type -- for internal using only!
*-------------------------------------------------------------------*/
typedef enum _BioTempSensorType {
   TCType_J,      /* Thermocouple type J */
   TCType_K,      /* Thermocouple type K */
   TCType_S,      /* Thermocouple type S */
   TCType_T,      /* Thermocouple type T */
   TCType_B,      /* Thermocouple type B */
   TCType_R,      /* Thermocouple type R */
   TCType_E,      /* Thermocouple type E */
   TCType_N,      /* Thermocouple type N */
} BioTempSensorType;

//-----------------------------------------------------------------------------------------
// Macros & functions for Simplify property access.
//-----------------------------------------------------------------------------------------
//
// prototype for user customized Get/Set functions.
// LONG T::Accessor( ULONG propId, ULONG& bufLen, PVOID buffer, ULONG opFlag );
// OpFlag :
// bit 0: Get/set. 1 -- get property, otherwise, set property
// bit 1: data source. 1 -- from/to registry, otherwise, from/to device
// bit 2: notify bit. 1 -- notify the user now.
#define BuildOpFlag(isGet, isReg, isNtfNow )  (isGet | (isReg << 1) | (isNtfNow << 2))
#define OPFLAG_READ         0x1
#define OPFLAG_REG          0x2
#define OPFLAG_NTFNOW       0x4
#define IsOpGet(opFlag)     (opFlag & 0x1)
#define IsOpReg(opFlag)     (opFlag & 0x2)
#define IsOpNtfNow(opFlag)  (opFlag & 0x4)

#if defined(_WIN32) || defined(WIN32)
#  define BEGIN_PROPERTY_TABLE(classT, FunctionName) \
   LONG FunctionName(ULONG propId, ULONG &bufLen, void *buffer, ULONG *attr, ULONG opFlag) \
   {\
      switch (propId) {
#else
#  define BEGIN_PROPERTY_TABLE(classT, FunctionName) \
   ErrorCode FunctionName(uint32 propId, uint32 &bufLen, void *buffer, uint32 *attr, uint32 opFlag) \
   {\
      switch (propId) {
#endif

#define PROP_RO_VAL( propId, type, value )        \
   case propId:                                   \
      if (attr) *attr = propId##_Attr | ReadOnly; \
      return IsOpGet(opFlag) ? DefCopyT<type>(value, buffer, bufLen) : ErrorPropReadOnly;

#define PROP_RO_PTR( propId, ptr, length )         \
   case propId:                                    \
      if (attr) *attr = propId##_Attr | ReadOnly;  \
      return IsOpGet(opFlag) ? DefCopyPtr(ptr, length, buffer, bufLen) : ErrorPropReadOnly;

#define PROP_MONO_VAL( propId, type, value, errcode ) \
   case propId:                                       \
      if (attr) *attr = propId##_Attr | Writable;     \
      if (IsOpGet(opFlag)) return DefCopyT<type>(value, buffer, bufLen);\
      if (!buffer || bufLen < sizeof(type)) return ErrorBufferTooSmall; \
      return value == *(type*)buffer ? Success : errcode;

// For modal type property value
#define PROP_MONO_MDL_VAL(propId, type, value)   PROP_MONO_VAL(propId, type, value, ErrorPropValueNotSpted)

// For nature type property value
#define PROP_MONO_NAT_VAL(propId, type, value)   PROP_MONO_VAL(propId, type, value, ErrorPropValueOutOfRange)

#define PROP_CUSTOM(propId, access, fnGetSet)    \
   case propId:                                  \
      if (attr) *attr= propId##_Attr | access;   \
      return fnGetSet(propId, bufLen, buffer, opFlag);

#define END_PROPERTY_TABLE( )            \
      default: return ErrorPropNotSpted; \
   }                                     \
}

__inline ErrorCode DefCopyPtr(const void *src, uint32 srcLen, void *dst, uint32& dstLen)
{
   uint32 orgLen = dstLen;
   dstLen = srcLen;
   if (dst) {
      memcpy(dst, src, __min(srcLen, orgLen));
      if (orgLen < dstLen) return ErrorBufferTooSmall;
   }
   return Success;
}

template<class T>
inline ErrorCode DefCopyT(T const & src, void *dst, uint32 &dstLen)
{
   return DefCopyPtr(&src, sizeof(T), dst, dstLen);
}

template<class Tin, class Tout>
inline ErrorCode DefCopyT(Tin const *src, uint32 srcLen, Tout *dst, uint32& dstLen)
{
   uint32 orgLen = dstLen;
   dstLen = (srcLen / sizeof(Tin)) * sizeof(Tout) ;
   if ( dst ) {
      Tin const *srcEnd = src + __min(srcLen / sizeof(Tin), orgLen / sizeof(Tout));
      while (src < srcEnd) *dst++ = (Tout)*src++;
      if (orgLen < dstLen) return ErrorBufferTooSmall;
   }
   return Success;
}

#if defined(_WIN32) && !defined(_WIN64)
__inline ErrorCode DefCopyPtr(const void *src, ULONG srcLen, void *dst, ULONG& dstLen)
{
   return DefCopyPtr(src, (uint32)srcLen, dst, (uint32&)dstLen);
}
template<class T>
inline ErrorCode DefCopyT(T const & src, void *dst, ULONG &dstLen)
{
   return DefCopyPtr(&src, sizeof(T), dst, (uint32&)dstLen);
}
template<class Tin, class Tout>
inline ErrorCode DefCopyT(Tin const *src, ULONG srcLen, Tout *dst, ULONG &dstLen)
{
   return DefCopyT(src, (uint32)srcLen, dst, (uint32&)dstLen);
}
#endif

//-------------------------------------------------------------------------
// common functions
//-------------------------------------------------------------------------
BDAQ_BEGIN_C_DECLARE

ErrorCode EnumToString(
   wchar_t const *enumTypeName,
   uint32        enumValue,
   uint32        enumStringLength,
   wchar_t       *enumString);

ErrorCode StringToEnum(
   wchar_t const *enumTypeName,
   wchar_t const *enumString,
   uint32        *enumValue);

void UserFreqToCounterDivider(
   IN  uint32   baseFreq,
   IN  double   userFreq,
   OUT uint16   *div1,
   OUT uint16   *div2 );

#if defined(_WIN32) || defined(WIN32)

BOOL InitUserLibThreadCtx(DWORD tlsIndex);

VOID DeinitUserLibThreadCtx(void);

BOOL GetExeFileVersion(
   IN  PCWSTR fileName,
   OUT PWSTR  version);

#  ifndef _WIN32_WCE
   BOOL QueryDeviceIntfDetail (
      IN HDEVINFO          devInfoSet,
      IN PSP_DEVINFO_DATA  devInfoData,
      IN GUID const      * devIntfGUID,
      OUT PSP_DEVICE_INTERFACE_DETAIL_DATA* detailData);

   ULONG QueryDeviceNumberByIntfDetail(
      IN PSP_DEVICE_INTERFACE_DETAIL_DATA detailData,
      IN ULONG                            fieldOffsetInKObj);

   ULONG QueryDeviceNumberByIntfGUID(
      IN HDEVINFO          devInfoSet,
      IN PSP_DEVINFO_DATA  devInfoData,
      IN GUID const      * devIntfGUID,
      IN ULONG             fieldOffsetInKObj);
#  endif

#endif

BDAQ_END_C_DECLARE


//-------------------------------------------------------------------------
// device functions
//-------------------------------------------------------------------------
BDAQ_BEGIN_C_DECLARE

#if defined(_WIN32) || defined(WIN32)
   LONG DeviceOpenKrnlDevice(
      IN ULONG deviceNumber, 
      IN ULONG accessMode, 
      OUT HANDLE * devHdl);

   LONG DeviceGetKrnlShared(
      IN HANDLE deviceHandle,
      IN ULONG  deviceNumber,
      IN ULONG  sizeofShared,
      OUT void  **kshared);

   void DeviceReleaseKrnlShared(
      IN HANDLE deviceHandle, 
      IN void *kshared);

   LONG DeviceDbgReadRegisters(
      IN HANDLE	devHdl,
      IN ULONG	   startAddr,
      IN ULONG	   length,
      IN PVOID	   buffer);

   LONG DeviceDbgWriteRegisters(
      IN HANDLE   devHdl,
      IN ULONG    startAddr,
      IN ULONG    length, 
      IN PVOID    buffer);

   LONG DeviceReadRegistry(
      IN  HANDLE  devHdl,
      IN  PCWSTR  name, 
      IN  ULONG   length, 
      OUT PVOID   buffer);

   LONG DeviceWriteRegistry(
      IN HANDLE  devHdl, 
      IN PCWSTR  name, 
      IN ULONG   type, 
      IN ULONG   length, 
      IN PVOID   buffer);

   LONG DeviceGetInterfaceName(
      IN     ULONG  deviceNumber, 
      IN OUT ULONG *length,
      OUT    PWSTR  intfName);

   LONG DeviceGetDescription(
      IN ULONG           deviceNumber, 
      IN OUT PULONG      length, 
      OUT OPTIONAL PVOID buffer);

   LONG DeviceSetDescription(
      IN  HANDLE  devHdl, 
      IN  ULONG   length, 
      IN  PVOID   buffer);

#  ifdef _WIN32_WCE
      LONG DeviceGetKrnlDriverVer(
         IN  PWSTR          templateKeyName, 
         IN OUT PULONG      length, 
         OUT OPTIONAL PVOID buffer);
#  else
      LONG DeviceGetKrnlDriverVer(
         IN  ULONG          deviceNumber, 
         IN OUT PULONG      length, 
         OUT OPTIONAL PVOID buffer);
#  endif

   LONG DeviceGetDllDriverVer(
      IN  HMODULE        instHdl, 
      IN OUT PULONG      length, 
      OUT OPTIONAL PVOID Buffer);

   LONG SysErrorToBioError(IN ULONG sysErrorCode);

   LONG DeviceIoctlWrapper(
      IN HANDLE krnlHdl, 
      IN DWORD  ctlCode, 
      IN PVOID  inBuf, 
      IN DWORD  inBufSize, 
      IN PVOID  outBuf, 
      IN DWORD  outBufSize,
      OUT DWORD * bytesReturned);
#else
   ErrorCode DeviceDbgReadRegisters(int fd, uint32 startAddr, uint32 length, void *buffer);
   ErrorCode DeviceDbgWriteRegisters(int fd, uint32 startAddr, uint32 length, void *buffer);
#endif 

BDAQ_END_C_DECLARE

//-------------------------------------------------------------------------
// ai&ao functions
//-------------------------------------------------------------------------
BDAQ_BEGIN_C_DECLARE

PCVRG_INFO VrgFindItemByGain(
   IN uint32     gain,
   IN PCVRG_INFO entries,
   IN uint32     count);

PCVRG_INFO VrgFindItemByVrgType(
   IN uint32     vrgType,
   IN PCVRG_INFO entries,
   IN uint32     count);

SCALING_ENTRY * ScaleListFindEntry(
   IN LIST_ENTRY *listHead,
   IN uint32     tag,
   IN bool       byGain);

uint32 SizeOfMapFuncPieces(
   IN BioMapFuncPiece const *pieces,
   IN uint32                 count);

uint32 PieceCountOfMapFuncPieces(
   IN BioMapFuncPiece const *pieces,
   IN uint32                 tableSize);

uint32 AiCalcLogicalChanCount( 
   IN uint8 const chType[], 
   IN uint32 count);

void AiBuildDefaultMapFunc(
   IN VRG_INFO const * vrg,
   IN uint32         resInBit,
   OUT SCALING_ENTRY *entry);

void AoBuildVoltageDefaultMapFunc(
   IN VRG_INFO const *vrg,
   IN uint32         resInBit,
   IN uint32         extRefAntiPolar, /*ignored if not external reference*/
   IN double         extRefVoltage,   /*ignored if not external reference*/
   OUT SCALING_ENTRY *entry);

void AoBuildCurrentDefaultMapFunc(
   IN VRG_INFO const *vrg,
   IN uint32         resInBit,
   IN uint32         offset,
   OUT SCALING_ENTRY *entry);

BDAQ_END_C_DECLARE

inline double ScaleDataUsePolynomial(
   IN BioMapFuncPiece const *pieces,
   IN uint32                count,
   IN double                inData)
{
   uint32 i;
   BioMapFuncPiece const *lastPiece = pieces;
   //
   // find the map function piece according to the input data
   for (i = 0; i < count && inData > pieces->upperLimit; ++i)
   {
      lastPiece = pieces;
      pieces = (BioMapFuncPiece*)((uint8*)pieces + pieces->size);
   }
   //
   // the input data is out of range
   if (i >= count)
   {
      pieces = lastPiece;              // Roll back to the last piece
      inData = lastPiece->upperLimit;  // limit the input to a valid value
   }
   //
   // calculate the polynomial using method:
   // F(x) = (...(Cn * x + Cn-1) * x + Cn-2).....) * x + C1) * x + C0
   int          j      = pieces->degree;
   double const *coef  = pieces->coef;
   double       result = coef[j];
   while (j)
   {
      result = result * inData + coef[--j];
   }
   return result;
}

//--------------------------------------------------------------------------
// Thermo functions
//--------------------------------------------------------------------------
BDAQ_BEGIN_C_DECLARE

double GetThermoTemperature(double thermoVolt, uint32 thermoType);
double GetCjcVoltage(double cjcTemperature, uint32 thermoType);

double ThermoTemperature(double fV1, uint32 type);
double TemperatureScales(double tmp, uint32 unit);
double GetCjcErrorVoltage(double cjc, uint32 type);
double GetActualThermoVolt(double thermoVolt, uint32 vrgType);
double GetActualThermoValue(double thermoValue, uint32 vrgType);

BDAQ_END_C_DECLARE

// Helper method used to clean up a scaling list.
inline void CleanupScalingList(LIST_ENTRY *listHead)
{
   // clear AO scale list
   PLIST_ENTRY listEntry;
   while ((listEntry = RemoveHeadList(listHead)) != listHead) {
      PSCALING_ENTRY scaleEntry = container_of(listEntry, SCALING_ENTRY, ListEntry);
      FreeScalingEntry(scaleEntry);
   }
}

template<class Tin, class Tout>
inline uint32 ScaleDataToBinary(
   IN PSCALING_ENTRY logCHVrg[], 
   IN uint32         logCHStart,
   IN uint32         logCHEnd,
   IN Tin *          inBuf,
   OUT Tout *        outBuf)
{
   for (; logCHStart < logCHEnd; ++logCHStart, ++inBuf, ++outBuf)
   {
      *outBuf = (Tout)(uint32)ScaleDataUsePolynomial(
                              logCHVrg[logCHStart]->Pieces, 
                              logCHVrg[logCHStart]->PieceCount, 
                              *inBuf);
   }
   return logCHStart;
}

template<class Tin, class Tout>
inline uint32 ScaleDataToFloating(
   IN PSCALING_ENTRY logCHVrg[], 
   IN uint32         logCHStart,
   IN uint32         logCHEnd,
   IN Tin          * inBuf,
   OUT Tout        * outBuf)
{
   for (; logCHStart < logCHEnd; ++logCHStart, ++inBuf, ++outBuf)
   {
      *outBuf = (Tout)ScaleDataUsePolynomial(
                        logCHVrg[logCHStart]->Pieces, 
                        logCHVrg[logCHStart]->PieceCount, 
                        *inBuf);
   }
   return logCHStart;
}

template< class Tin, class Tout >
inline uint32 ScaleDataToFloatingWithMask (
   IN PSCALING_ENTRY logCHVrg[], 
   IN uint32         logCHStart,
   IN uint32         logCHEnd,
   IN Tin            dataMask,
   IN Tin          * inBuf,
   OUT Tout        * outBuf)
{
   for (; logCHStart < logCHEnd; ++logCHStart, ++inBuf, ++outBuf)
   {
      *outBuf = (Tout)ScaleDataUsePolynomial(
                        logCHVrg[logCHStart]->Pieces, 
                        logCHVrg[logCHStart]->PieceCount, 
                        *inBuf & dataMask);
   }
   return logCHStart;
}

#if defined(_WIN32) || defined(WIN32)
// ------------------------------------------------------------------------------------------
// the 'BioPropPage' is a subset of standard interface 'IPropertyPage'.
// Do we really need this ?
// ------------------------------------------------------------------------------------------
class __declspec(novtable) BioPropPageBase 
{
public:
   virtual ~BioPropPageBase(){}; // Make the destructor as virtual

public: 
   virtual void Init(void * device, ULONG dataSource) = 0;
   virtual HWND Activate(HWND parentWnd, RECT &wndPos, BOOL modal) = 0;
   virtual void Deactivate(void) = 0;
   virtual void Show(UINT cmdShow) = 0;
   virtual void Move(RECT &wndPos) = 0;
   virtual void Refresh(ULONG dataSource) = 0;
   virtual BOOL Apply(ULONG saveTo) = 0;
   virtual void SetSaveOption(ULONG option) = 0;
};

// ------------------------------------------------------------------------------------------
// the default implement of interface 'BioPropPage'
// ------------------------------------------------------------------------------------------
template< class DeviceCmpnt, class PropPage >
class __declspec(novtable) BioPropPageImpl : public BioPropPageBase
{
public:
   BioPropPageImpl() : m_device(NULL){ };
   virtual void Init(void * device, ULONG dataSource)
   {
      m_device = (DeviceCmpnt*)device;
      m_dataSrc= dataSource;
   }
   virtual HWND Activate(HWND parentWnd, RECT &wndPos, BOOL /*Modal*/)
   {
      PropPage* pT = static_cast<PropPage*>(this);
      if (pT->Create(parentWnd)) { Move(wndPos);}
      return pT->m_hWnd;
   }
   virtual void Deactivate(void)
   {
      PropPage* pT = static_cast<PropPage*>(this);
      if (pT->m_hWnd && ::IsWindow(pT->m_hWnd)) { pT->DestroyWindow(); }
   }
   virtual void Show(UINT cmdShow)
   {
      PropPage* pT = static_cast<PropPage*>(this);
      if (pT->m_hWnd) { ShowWindow(pT->m_hWnd, cmdShow); }
   }
   virtual void Move(RECT &wndPos)
   {
      PropPage* pT = static_cast<PropPage*>(this);
      if (pT->m_hWnd) {
         MoveWindow(pT->m_hWnd, 
            wndPos.left, wndPos.top, 
            wndPos.right - wndPos.left,
            wndPos.bottom - wndPos.top, TRUE);
      }      
   }
   virtual void Refresh(ULONG dataSource) { }
   virtual BOOL Apply(ULONG saveTo) { return TRUE; }
   virtual void SetSaveOption(ULONG option) { }

protected:
   DeviceCmpnt * m_device;
   ULONG         m_dataSrc; // from where to load the init value: 0 -- device, 1 -- registry
};

#endif // defined(_WIN32) || defined(WIN32)

#endif // BIONIC_USER_MODE_LIB
