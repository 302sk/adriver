
#ifdef _WIN32
#include "stdafx.h"
#endif
#include <wchar.h>
#include <stdlib.h>
#include <map>
#include <bdaqdef.h>
#include <enumstrings.h>

typedef wchar_t const * PCWSTR;

#ifdef _WIN32
#  define StrOf(type)   L#type
#  define swprintf      swprintf_s
#else
#  define StrOf(type)   L""#type
#endif

#define DECLARE_MAP_AND_GETTER(type, field)                 \
private:                                                    \
   static std::map<type, PCWSTR> field;                     \
public:                                                     \
   static std::map<type, PCWSTR> const& GetMapFor##type()   \
   {                                                        \
      if (field.empty()) {                                  \
         InitializeMapFor##type();                          \
      }                                                     \
                                                            \
      return field;                                         \
   }

#define INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(map, val) \
   map.insert(std::make_pair(val, val##_String))

class Maps
{
   DECLARE_MAP_AND_GETTER(AccessMode,        _acs_mode_map)
   DECLARE_MAP_AND_GETTER(AiSignalType,      _ai_sig_type_map)
   DECLARE_MAP_AND_GETTER(DioPortType,       _dio_port_type_map)
   DECLARE_MAP_AND_GETTER(SamplingMethod,    _sampling_method_map)
   DECLARE_MAP_AND_GETTER(ValueRange,        _vrg_map)
   DECLARE_MAP_AND_GETTER(FreqMeasureMethod, _freq_measure_method_map)
   DECLARE_MAP_AND_GETTER(ActiveSignal,      _active_signal_map)
   DECLARE_MAP_AND_GETTER(TriggerAction,     _trig_action_map)
   DECLARE_MAP_AND_GETTER(SignalDrop,        _signal_drop_map)
   DECLARE_MAP_AND_GETTER(ErrorCode,         _error_code_map)
   DECLARE_MAP_AND_GETTER(BurnoutRetType,    _burnout_ret_type_map)
	DECLARE_MAP_AND_GETTER(DioPortDir,        _dio_port_dir)

private:
   static void InitializeMapForAccessMode() 
   {
      _acs_mode_map.clear();

      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_acs_mode_map, ModeRead);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_acs_mode_map, ModeWrite);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_acs_mode_map, ModeWriteWithReset);
   }

   static void InitializeMapForAiSignalType()
   {
      _ai_sig_type_map.clear();

      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_ai_sig_type_map, SingleEnded);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_ai_sig_type_map, Differential);
   }

   static void InitializeMapForDioPortType()
   {
      _dio_port_type_map.clear();

      _dio_port_type_map.insert(std::make_pair(PortDi,        L"PortDi"));
      _dio_port_type_map.insert(std::make_pair(PortDo,        L"PortDo"));
      _dio_port_type_map.insert(std::make_pair(PortDio,       L"PortDio"));
      _dio_port_type_map.insert(std::make_pair(Port8255A,     L"Port8255A"));
      _dio_port_type_map.insert(std::make_pair(Port8255C,     L"Port8255C"));
      _dio_port_type_map.insert(std::make_pair(PortIndvdlDio, L"PortIndvdlDio"));
   }

   static void InitializeMapForSamplingMethod()
   {
      _sampling_method_map.clear();

      _sampling_method_map.insert(std::make_pair(EqualTimeSwitch, L"EqualTimeSwitch"));
      _sampling_method_map.insert(std::make_pair(Simultaneous,    L"Simultaneous"));
   }

   static void InitializeMapForValueRange()
   {
      _vrg_map.clear();

      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_Neg15To15);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_Neg10To10);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_Neg5To5);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_Neg2pt5To2pt5);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_Neg1pt25To1pt25);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_Neg1To1);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_0To15);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_0To10);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_0To5);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_0To2pt5);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_0To1pt25);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_0To1);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_Neg625To625);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_Neg500To500);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_Neg312pt5To312pt5);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_Neg200To200);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_Neg150To150);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_Neg100To100);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_Neg50To50);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_Neg30To30);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_Neg20To20);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_Neg15To15);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_Neg10To10);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_Neg5To5);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_0To625);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_0To500);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_0To150);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_0To100);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_0To50);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_0To20);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_0To15);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mV_0To10);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mA_Neg20To20);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mA_0To20);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mA_4To20);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, mA_0To24);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_Neg2To2);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_Neg4To4);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_Neg20To20);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Jtype_0To760C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Ktype_0To1370C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Ttype_Neg100To400C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Etype_0To1000C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Rtype_500To1750C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Stype_500To1750C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Btype_500To1800C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt392_Neg50To150);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt385_Neg200To200);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt385_0To400);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt385_Neg50To150);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt385_Neg100To100);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt385_0To100);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt385_0To200);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt385_0To600);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt392_Neg100To100);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt392_0To100);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt392_0To200);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt392_0To600);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt392_0To400);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt392_Neg200To200);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Pt1000_Neg40To160);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Balcon500_Neg30To120);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Ni518_Neg80To100);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Ni518_0To100);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Ni508_0To100);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Ni508_Neg50To200);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Thermistor_3K_0To100);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Thermistor_10K_0To100);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Jtype_Neg210To1200C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Ktype_Neg270To1372C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Ttype_Neg270To400C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Etype_Neg270To1000C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Rtype_Neg50To1768C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Stype_Neg50To1768C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Btype_40To1820C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Jtype_Neg210To870C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Rtype_0To1768C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Stype_0To1768C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, Ttype_Neg20To135C);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_ExternalRefBipolar);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_vrg_map, V_ExternalRefUnipolar);
   }

   static void InitializeMapForFreqMeasureMethod()
   {
      _freq_measure_method_map.clear();

      _freq_measure_method_map.insert(std::make_pair(AutoAdaptive,           L"AutoAdaptive"));
      _freq_measure_method_map.insert(std::make_pair(CountingPulseBySysTime, L"CountingPulseBySysTime"));
      _freq_measure_method_map.insert(std::make_pair(CountingPulseByDevTime, L"CountingPulseByDevTime"));
      _freq_measure_method_map.insert(std::make_pair(PeriodInverse,          L"PeriodInverse"));
   }

   static void InitializeMapForActiveSignal()
   {
      _active_signal_map.clear();

      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_active_signal_map, ActiveNone);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_active_signal_map, RisingEdge);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_active_signal_map, FallingEdge);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_active_signal_map, BothEdge);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_active_signal_map, HighLevel);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_active_signal_map, LowLevel);
   }

   static void InitializeMapForTriggerAction()
   {
      _trig_action_map.clear();

      _trig_action_map.insert(std::make_pair(ActionNone,   L"ActionNone"));
      _trig_action_map.insert(std::make_pair(DelayToStart, L"DelayToStart"));
      _trig_action_map.insert(std::make_pair(DelayToStop,  L"DelayToStop"));
   }

   static void InitializeMapForSignalDrop()
   {
      _signal_drop_map.clear();

      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SignalNone);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternalClock);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal1KHz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal10KHz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal100KHz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal1MHz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal10MHz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal20MHz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal30MHz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal40MHz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal50MHz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal60MHz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigDiPatternMatch);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigDiStatusChange);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigExtAnaClock);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigExtAnaScanClock);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigExtAnaTrigger);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigExtDigClock);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigExtDigTrigger0);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigExtDigTrigger1);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigExtDigTrigger2);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigExtDigTrigger3);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigCHFrzDo);

      // the description strings for the SignalDrop values between SigCHFrzDo and SigInternal2Hz will be generated as needed

      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal2Hz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal20Hz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal200Hz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal2KHz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal20KHz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal200KHz);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_signal_drop_map, SigInternal2MHz);
   }

   static void InitializeMapForErrorCode()
   {
      _error_code_map.clear();

      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, Success);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, WarningIntrNotAvailable);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, WarningParamOutOfRange);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, WarningPropValueOutOfRange);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, WarningPropValueNotSpted);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, WarningPropValueConflict);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, WarningVrgOfGroupNotSame);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorHandleNotValid);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorParamOutOfRange);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorParamNotSpted);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorParamFmtUnexpted);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorMemoryNotEnough);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorBufferIsNull);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorBufferTooSmall);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorDataLenExceedLimit);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorFuncNotSpted);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorEventNotSpted);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorPropNotSpted);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorPropReadOnly);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorPropValueConflict);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorPropValueOutOfRange);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorPropValueNotSpted);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorPrivilegeNotHeld);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorPrivilegeNotAvailable);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorDriverNotFound);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorDriverVerMismatch);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorDriverCountExceedLimit);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorDeviceNotOpened);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorDeviceNotExist);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorDeviceUnrecognized);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorConfigDataLost);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorFuncNotInited);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorFuncBusy);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorIntrNotAvailable);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorDmaNotAvailable);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorDeviceIoTimeOut);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorSignatureNotMatch);
      INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_error_code_map, ErrorUndefined);
   }

	static void InitializeMapForBurnoutRetType()
	{
		_burnout_ret_type_map.clear();
		
		INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_burnout_ret_type_map, Current);
		INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_burnout_ret_type_map, ParticularValue);
		INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_burnout_ret_type_map, UpLimit);
		INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_burnout_ret_type_map, LowLimit);
		INSERT_ENUM_VALUE_WITH_PRE_DEFINED_STRING(_burnout_ret_type_map, LastCorrectValue);	
	}

	static void InitializeMapForDioPortDir()
	{
		_dio_port_dir.clear();

		_dio_port_dir.insert(std::make_pair(Input,   L"Input"));
		_dio_port_dir.insert(std::make_pair(LoutHin, L"LoutHin"));
		_dio_port_dir.insert(std::make_pair(LinHout, L"LinHout"));
		_dio_port_dir.insert(std::make_pair(Output,  L"Output"));
	}
};


#define CREATE_MAP(type, field)           \
   std::map<type, PCWSTR> Maps::field

CREATE_MAP(AccessMode,        _acs_mode_map);
CREATE_MAP(AiSignalType,      _ai_sig_type_map);
CREATE_MAP(DioPortType,       _dio_port_type_map);
CREATE_MAP(SamplingMethod,    _sampling_method_map);
CREATE_MAP(ValueRange,        _vrg_map);
CREATE_MAP(FreqMeasureMethod, _freq_measure_method_map);
CREATE_MAP(ActiveSignal,      _active_signal_map);
CREATE_MAP(TriggerAction,     _trig_action_map);
CREATE_MAP(SignalDrop,        _signal_drop_map);
CREATE_MAP(ErrorCode,         _error_code_map);
CREATE_MAP(BurnoutRetType,    _burnout_ret_type_map);
CREATE_MAP(DioPortDir,        _dio_port_dir);

#define DEFINE_STRING_GETTER(type)                                            \
   ErrorCode GetStringFor##type(type val, int32 str_len, wchar_t* str)        \
   {                                                                          \
      std::map<type, PCWSTR> const& map = Maps::GetMapFor##type();            \
                                                                              \
      std::map<type, PCWSTR>::const_iterator itor = map.find(val);            \
      if (itor == map.end()) return ErrorUndefined; /* unknown enum value */  \
                                                                              \
      if (str_len <= (int32)wcslen(itor->second)) return ErrorBufferTooSmall; \
                                                                              \
      wcsncpy(str, itor->second, str_len);                                    \
                                                                              \
      return Success;                                                         \
   }

#define RUN_STRING_GETTER(type)                                                  \
   if (wcscmp(enumTypeName, StrOf(type)) == 0) {                                 \
      return GetStringFor##type((type)enumValue, enumStringLength, enumString);  \
   }

#define RUN_DYN_STRING_GETTER(type)                                                      \
   if (wcscmp(enumTypeName, StrOf(type)) == 0) {                                         \
      ErrorCode err = GetStringFor##type((type)enumValue, enumStringLength, enumString); \
      if ((err != Success) && (err != ErrorBufferTooSmall)) {                       \
         err = GetExtraStringFor##type(enumValue, enumStringLength, enumString);    \
      }                                                                             \
                                                                                    \
      return err;                                                                   \
   }

#define DEFINE_ENUM_GETTER(type)                                                                     \
   ErrorCode GetValueFor##type(PCWSTR str, uint32* val)                                               \
   {                                                                                                 \
      std::map<type, PCWSTR> const& map = Maps::GetMapFor##type();                                   \
      for (std::map<type, PCWSTR>::const_iterator itor = map.begin(); itor != map.end(); ++itor) {   \
         if (wcscmp(str, itor->second) == 0) {                                                       \
            (*val) = (int32)itor->first;                                                             \
            return Success;                                                                          \
         }                                                                                           \
      }                                                                                              \
                                                                                                     \
      return ErrorUndefined; /* unknown enum string */                                               \
   }

#define RUN_ENUM_GETTER(type)                            \
   if (wcscmp(enumTypeName, StrOf(type)) == 0) {         \
      return GetValueFor##type(enumString, enumValue);   \
   }

#define RUN_DYN_ENUM_GETTER(type)                                 \
   if (wcscmp(enumTypeName, StrOf(type)) == 0) {                  \
      if (GetValueFor##type(enumString, enumValue) != Success) {  \
         return GetExtraValueFor##type(enumString, enumValue);    \
      }                                                           \
                                                                  \
      return Success;                                             \
   }

DEFINE_STRING_GETTER(AccessMode)
DEFINE_ENUM_GETTER(AccessMode)

DEFINE_STRING_GETTER(AiSignalType)
DEFINE_ENUM_GETTER(AiSignalType)

DEFINE_STRING_GETTER(DioPortType)
DEFINE_ENUM_GETTER(DioPortType)

DEFINE_STRING_GETTER(SamplingMethod)
DEFINE_ENUM_GETTER(SamplingMethod)

DEFINE_STRING_GETTER(ValueRange)
DEFINE_ENUM_GETTER(ValueRange)

DEFINE_STRING_GETTER(FreqMeasureMethod)
DEFINE_ENUM_GETTER(FreqMeasureMethod)

DEFINE_STRING_GETTER(ActiveSignal)
DEFINE_ENUM_GETTER(ActiveSignal)

DEFINE_STRING_GETTER(TriggerAction)
DEFINE_ENUM_GETTER(TriggerAction)

DEFINE_STRING_GETTER(SignalDrop)
DEFINE_ENUM_GETTER(SignalDrop)

DEFINE_STRING_GETTER(ErrorCode)
DEFINE_ENUM_GETTER(ErrorCode)

DEFINE_STRING_GETTER(BurnoutRetType)
DEFINE_ENUM_GETTER(BurnoutRetType)

DEFINE_STRING_GETTER(DioPortDir)
DEFINE_ENUM_GETTER(DioPortDir)

ErrorCode GetExtraStringForSignalDrop(uint32 val, int32 str_len, wchar_t* str)
{
#define GENERATE_STRING(tar_str_len, num_str_len, base_val, str_prefix, str_suffix) \
   if (str_len <= tar_str_len) return ErrorBufferTooSmall;                          \
                                                                                    \
   wchar_t tar_str[tar_str_len];                                                    \
   swprintf(tar_str, tar_str_len, L"%s%d%s", str_prefix, val - base_val, str_suffix); \
                                                                                    \
   wcsncpy(str, tar_str, str_len);                                                  \
   return Success

   if ((val >= (int32)SigAi0) && (val <= (int32)SigAi63)) {
      GENERATE_STRING(14, 3, SigAi0, L"AI Channel ", L"");
   }

   if ((val >= (int32)SigAo0) && (val <= (int32)SigAo31)) {
      GENERATE_STRING(14, 3, SigAo0, L"AO Channel ", L"");
   }

   if ((val >= (int32)SigDi0) && (val <= (int32)SigDi255)) {
      GENERATE_STRING(15, 4, SigDi0, L"DI Channel ", L"");
   }

   if ((val >= (int32)SigDio0) && (val <= (int32)SigDio255)) {
      GENERATE_STRING(17, 4, SigDio0, L"DI/O Channel ", L"");
   }

   if ((val >= (int32)SigCntClk0) && (val <= (int32)SigCntClk7)) {
      GENERATE_STRING(16, 2, SigCntClk0, L"Counter ", L"'s CLK");
   }

   if ((val >= (int32)SigCntGate0) && (val <= (int32)SigCntGate7)) {
      GENERATE_STRING(17, 2, SigCntGate0, L"Counter ", L"'s GATE");
   }

   if ((val >= (int32)SigCntOut0) && (val <= (int32)SigCntOut7)) {
      GENERATE_STRING(17, 2, SigCntOut0, L"Counter ", L"'s OUT");
   }

   if ((val >= (int32)SigCntFout0) && (val <= (int32)SigCntFout7)) {
      GENERATE_STRING(17, 2, SigCntFout0, L"Counter ", L"'s FOUT");
   }

   if ((val >= (int32)SigAmsiPin0) && (val <= (int32)SigAmsiPin19)) {
      GENERATE_STRING(8, 2, SigAmsiPin0, L"AMSI ", L"");
   }

   return ErrorUndefined; /* unknown enum string */
}

ErrorCode GetExtraValueForSignalDrop(PCWSTR str, uint32* val)
{
#define PARSE_STRING(flag_str, num_str_pos, base_val) \
   if (wcsstr(str, flag_str) != NULL) {               \
      int num = (int)wcstol(str + num_str_pos, NULL, 10);\
                                                      \
      (*val) = (int32)base_val + num;                 \
                                                      \
      return Success;                                 \
   }

#define PARSE_STRING_FOR_COUNTER_SIGNAL(flag_str, num_str_pos, base_val)\
   if (wcsstr(str, flag_str) != NULL) {                                 \
      int num = (int)wcstol(&str[num_str_pos], NULL, 10);               \
      (*val) = (int32)base_val + num;                                   \
      return Success;                                                   \
   }

   PARSE_STRING(L"AI Channel", 11, SigAi0);

   PARSE_STRING(L"AO Channel", 11, SigAo0);

   PARSE_STRING(L"DI Channel", 11, SigDi0);

   PARSE_STRING(L"DI/O Channel", 13, SigDio0);

   PARSE_STRING_FOR_COUNTER_SIGNAL(L"'s CLK", 8, SigCntClk0);

   PARSE_STRING_FOR_COUNTER_SIGNAL(L"'s GATE", 8, SigCntGate0);

   PARSE_STRING_FOR_COUNTER_SIGNAL(L"'s OUT", 8, SigCntOut0);

   PARSE_STRING_FOR_COUNTER_SIGNAL(L"'s FOUT", 8, SigCntFout0);

   PARSE_STRING(L"AMSI", 5, SigAmsiPin0);

   return ErrorUndefined; /* unknown enum string */
}

ErrorCode EnumToString(wchar_t const *enumTypeName, uint32 enumValue, uint32 enumStringLength, wchar_t *enumString)
{
   if (enumString == NULL) return ErrorBufferIsNull;

   RUN_STRING_GETTER(AccessMode);

   RUN_STRING_GETTER(AiSignalType);

   RUN_STRING_GETTER(DioPortType);

   RUN_STRING_GETTER(SamplingMethod);

   RUN_STRING_GETTER(ValueRange);

   RUN_STRING_GETTER(FreqMeasureMethod);

   RUN_STRING_GETTER(ActiveSignal);

   RUN_STRING_GETTER(TriggerAction);

   RUN_DYN_STRING_GETTER(SignalDrop);
	
   RUN_STRING_GETTER(ErrorCode);

	RUN_STRING_GETTER(BurnoutRetType);

	RUN_STRING_GETTER(DioPortDir);

   // TODO : add your implementation here

   return ErrorUndefined; /* unknown enum type */
}

ErrorCode StringToEnum(wchar_t const *enumTypeName, wchar_t const *enumString, uint32* enumValue)
{
   if (enumValue == NULL) return ErrorBufferIsNull;

   RUN_ENUM_GETTER(AccessMode);

   RUN_ENUM_GETTER(AiSignalType);

   RUN_ENUM_GETTER(DioPortType);

   RUN_ENUM_GETTER(SamplingMethod);

   RUN_ENUM_GETTER(ValueRange);

   RUN_ENUM_GETTER(FreqMeasureMethod);

   RUN_ENUM_GETTER(ActiveSignal);

   RUN_ENUM_GETTER(TriggerAction);

   RUN_DYN_ENUM_GETTER(SignalDrop);	
	
   RUN_ENUM_GETTER(ErrorCode);

	RUN_ENUM_GETTER(BurnoutRetType);

	RUN_ENUM_GETTER(DioPortDir);

   // TODO : add your implementation here

   return ErrorUndefined; /* unknown enum type */
}
