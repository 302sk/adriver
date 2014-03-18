/*
 * BioCounter.h
 *
 *  Created on: 2011-8-26
 *      Author: rocky
 */

#ifndef _BDAQ_COUNTER_IMPL_H_
#define _BDAQ_COUNTER_IMPL_H_

#include "krnlstub.h"

uint32 const s_cntrSptedEvts[] = {
    EvtCntTerminalCount0, EvtCntTerminalCount1,
    EvtCntTimer0,         EvtCntTimer1,
    EvtCntOneShot0,       EvtCntOneShot1
};



uint32 const s_capsOfCntr[] = { // Counter capabilities
  Primary, InstantEventCount, OneShot, TimerPulse, InstantFreqMeter, InstantPwmIn, InstantPwmOut
};

uint32 const s_sptedClkSrc0[] = { // Counter0 clock source
  SigCntClk0,       SigCntClk1,       SigCntOut1,        SigCntGate0,
  SigInternal20MHz, SigInternal2MHz,  SigInternal200KHz, SigInternal20KHz, 
  SigInternal2KHz,  SigInternal200Hz, SigInternal20Hz,   SigInternal2Hz,  
};

uint32 const s_sptedClkSrc1[] = { // Counter1 clock source
  SigCntClk0,       SigCntClk1,       SigCntOut0,        SigCntGate1,
  SigInternal20MHz, SigInternal2MHz,  SigInternal200KHz, SigInternal20KHz, 
  SigInternal2KHz,  SigInternal200Hz, SigInternal20Hz,   SigInternal2Hz,  
};

uint32 const s_sptedGateSrc0[] = { // Counter0 Gate clock source
  SignalNone,       SigCntGate0,      SigCntGate1,		 SigCntOut1,
  SigInternal2KHz,  SigInternal200Hz, SigInternal20Hz,   SigInternal2Hz,  
};

uint32 const s_sptedGateSrc1[] = { // Counter1 Gate clock source
  SignalNone,       SigCntGate0,      SigCntGate1,		 SigCntOut0,
  SigInternal2KHz,  SigInternal200Hz, SigInternal20Hz,   SigInternal2Hz,  
};

uint32 const s_sptedChipOpModes[] = {// Primal counter features & properties
  C1780_MA, C1780_MB, C1780_MC, C1780_MD, 
  C1780_ME, C1780_MF, C1780_MG, C1780_MH, 
  C1780_MI, C1780_MJ, C1780_MK, C1780_ML,
  C1780_MX
};
uint32 const s_cntrChipCntTypes[] = {// Primal counter features & properties
  DownCount, UpCount
};
uint32 const s_cntrChipClkPolarities[] = { // Primal counter features & properties
  Positive, Negative
};
uint32 const s_cntrChipGatePolarities[] = { // Primal counter features & properties
  Positive, Negative
};
uint32 const s_cntrChipOutSignals[] = {// Primal counter features & properties
  SignalOutNone, PositivePulse, NegativePulse, ToggledFromLow, ToggledFromHigh
};
uint32 const s_cntrChipValRegs[] = {// Primal counter features & properties
  CntLoad, CntHold
};

//////////////////////////////////////////////////////////////////////////
// EventCounting features
uint32 const s_cntrEcClkPolarities[] = { 
  Positive, Negative
};

uint32 const s_cntrEcGateCtrl[] = {
  1, 1
};

uint32 const s_cntrEcGatePolarities[] = { 
  Positive, Negative
};
//////////////////////////////////////////////////////////////////////////
// One Shot
uint32 const s_cntrOsClkSrcOfCntr0[] = {
  SigInternal20MHz, SigInternal2MHz, SigCntClk0, SigCntClk1, SigCntOut1, SigCntGate0
};
uint32 const s_cntrOsClkSrcOfCntr1[] = {
  SigInternal20MHz, SigInternal2MHz, SigCntClk1, SigCntClk0, SigCntOut0, SigCntGate1
};

uint32 const s_cntrOsClkPolarities[] = { 
  Positive, Negative
};

uint32 const s_cntrOsGateSrcOfCntr0[] = { 
  SignalNone, SigCntGate0, SigCntGate1, SigCntOut1
};
uint32 const s_cntrOsGateSrcOfCntr1[] = { 
  SignalNone, SigCntGate1, SigCntGate0, SigCntOut0
};

uint32 const s_cntrOsGatePolarities[] = { 
  Positive, Negative
};

uint32 const s_cntrOsOutSignals[] = {
  PositivePulse, NegativePulse, ToggledFromLow, ToggledFromHigh
};

MathInterval const s_cntrOsDelayCountRange = { 
  LCBRCB, 2, 0xffffffff
};
//////////////////////////////////////////////////////////////////////////
// TimerPulse Features

uint32 const s_cntrTmrGateCtrl[] = {
  1, 1
};

uint32 const s_cntrTmrGatePolarities[] = { 
  Positive, Negative
};

uint32 const s_cntrTmrOutSignals[] = {
  PositivePulse, NegativePulse, ToggledFromLow, ToggledFromHigh
};

MathInterval const s_cntrTmrFreqRange = {
  LCBRCB, 1/200.0, 1e6
}; // 1/200 Hz to 1MHz

//////////////////////////////////////////////////////////////////////////
// Freq Measurement features

uint32 const s_cntrFmMethods[] = {
  AutoAdaptive, CountingPulseByDevTime, PeriodInverse
};

//////////////////////////////////////////////////////////////////////////
// PWM_Out features
uint32 const s_cntrPoGateCtrl[] = {
  1, 1
};

uint32 const s_cntrPoGatePolarities[] = { 
  Positive, Negative
};

MathInterval const s_cntrPoHiLoPeriodRange = {
  LCBRCB, 5e-7, 200
}; // 1e-7 to 200s

//////////////////////////////////////////////////////////////////////////


class BDaqCntrImpl : public BDaqCntr
{
  public:
	 // --------------------------------------------------------------
	 // BDaqModule interface implementation
	 // --------------------------------------------------------------
	 virtual ModuleType getType()   {  return DaqCounter; }
	 virtual int32      getNumber() {  return 0;   }
	 virtual AccessMode getMode()   {  return m_kstubPtr->getMode(); }

	 virtual ErrorCode EventGetHandle(EventId id, HANDLE &handle);
	 virtual ErrorCode EventGetStatus(EventId id, uint32 *lParam, uint32 *rParam);
	 virtual ErrorCode EventClearFlag(EventId id, uint32 lParam, uint32 rParam);


	 // Property Table
	 BEGIN_PROPERTY_TABLE(BDaqCntrImpl, PropAccess)
		// common
		PROP_RO_VAL(CFG_FeatureChannelNumberMax,       uint32, CNTR_CHL_COUNT -1)
		PROP_RO_VAL(CFG_FeatureResolutionInBit,        uint32, CNTR_RES_IN_BIT)
		PROP_RO_VAL(CFG_FeatureDataSize,               uint32, CNTR_DATA_SIZE)	
		
		PROP_RO_PTR(CFG_FeatureCapabilitiesOfCounter0, s_capsOfCntr, sizeof(s_capsOfCntr))
		PROP_RO_PTR(CFG_FeatureCapabilitiesOfCounter1, s_capsOfCntr, sizeof(s_capsOfCntr))

		// primal counter
		PROP_RO_PTR(CFG_FeatureChipClkSourceOfCounter0,  s_sptedClkSrc0, sizeof(s_sptedClkSrc0))
		PROP_RO_PTR(CFG_FeatureChipClkSourceOfCounter1,  s_sptedClkSrc1, sizeof(s_sptedClkSrc1))

		PROP_RO_PTR(CFG_FeatureChipGateSourceOfCounter0,  s_sptedGateSrc0,          sizeof(s_sptedGateSrc0))
		PROP_RO_PTR(CFG_FeatureChipGateSourceOfCounter1,  s_sptedGateSrc1,          sizeof(s_sptedGateSrc1))
		PROP_RO_PTR(CFG_FeatureChipOperationModes,        s_sptedChipOpModes,       sizeof(s_sptedChipOpModes))
		PROP_RO_PTR(CFG_FeatureChipSignalCountingTypes,   s_cntrChipCntTypes, 	    sizeof(s_cntrChipCntTypes) )
		PROP_RO_PTR(CFG_FeatureChipClkPolarities, 		  s_cntrChipClkPolarities,  sizeof(s_cntrChipClkPolarities) )
		PROP_RO_PTR(CFG_FeatureChipGatePolarities,		  s_cntrChipGatePolarities, sizeof(s_cntrChipGatePolarities))
		PROP_RO_PTR(CFG_FeatureChipOutSignals,			  s_cntrChipOutSignals,	    sizeof(s_cntrChipOutSignals))
		PROP_RO_PTR(CFG_FeatureChipValueRegisters,		  s_cntrChipValRegs,		sizeof(s_cntrChipValRegs) )

		PROP_CUSTOM(CFG_ChipOperationModeOfCounters,	  Writable,	PropAccessChipOpMode )
		PROP_CUSTOM(CFG_ChipSignalCountingTypeOfCounters, Writable,	PropAccessChipSigCntType )
		PROP_CUSTOM(CFG_ChipClkSourceOfCounters,		  Writable,	PropAccessChipClkSrc )
		PROP_CUSTOM(CFG_ChipClkPolarityOfCounters,		  Writable,	PropAccessChipClkPolarity )
		PROP_CUSTOM(CFG_ChipGateSourceOfCounters, 		  Writable,	PropAccessChipGateSrc )
		PROP_CUSTOM(CFG_ChipGatePolarityOfCounters,		  Writable,	PropAccessChipGatePolarity )
		PROP_CUSTOM(CFG_ChipOutSignalOfCounters,		  Writable,	PropAccessChipOutSignal )
		PROP_CUSTOM(CFG_ChipLoadValueOfCounters,		  Writable,	PropAccessChipLoadValue )
		PROP_CUSTOM(CFG_ChipHoldValueOfCounters,		  Writable,	PropAccessChipHoldValue )

		// Event counting
		
		PROP_RO_PTR( CFG_FeatureEcClkPolarities,		  s_cntrEcClkPolarities,  sizeof(s_cntrEcClkPolarities) )
		PROP_RO_PTR( CFG_FeatureEcGateControlOfCounters,  s_cntrEcGateCtrl,		  sizeof(s_cntrEcGateCtrl) )
		PROP_RO_PTR( CFG_FeatureEcGatePolarities,		  s_cntrEcGatePolarities, sizeof(s_cntrEcGatePolarities) )
		 
		PROP_CUSTOM( CFG_EcClkPolarityOfCounters,		Writable,		PropAccessEcClockPolarity )
		PROP_CUSTOM( CFG_EcGateEnabledOfCounters,		Writable,		PropAccessEcGateEnabled )
		PROP_CUSTOM( CFG_EcGatePolarityOfCounters, 		Writable,		PropAccessEcGatePolarity )

		// One-shot
		PROP_RO_PTR( CFG_FeatureOsClkSourceOfCounter0,  s_cntrOsClkSrcOfCntr0,    sizeof(s_cntrOsClkSrcOfCntr0))
	    PROP_RO_PTR( CFG_FeatureOsClkSourceOfCounter1, 	s_cntrOsClkSrcOfCntr1,	  sizeof(s_cntrOsClkSrcOfCntr1) )
		PROP_RO_PTR( CFG_FeatureOsClkPolarities,		s_cntrOsClkPolarities,	  sizeof(s_cntrOsClkPolarities) )
		PROP_RO_PTR( CFG_FeatureOsGateSourceOfCounter0,	s_cntrOsGateSrcOfCntr0,   sizeof(s_cntrOsGateSrcOfCntr0) )
		PROP_RO_PTR( CFG_FeatureOsGateSourceOfCounter1,	s_cntrOsGateSrcOfCntr1,   sizeof(s_cntrOsGateSrcOfCntr1) )
		PROP_RO_PTR( CFG_FeatureOsGatePolarities,		s_cntrOsGatePolarities,   sizeof(s_cntrOsGatePolarities) )
		PROP_RO_PTR( CFG_FeatureOsOutSignals,			s_cntrOsOutSignals, 	  sizeof(s_cntrOsOutSignals) )
		PROP_RO_VAL( CFG_FeatureOsDelayCountRange, 		MathInterval,			  s_cntrOsDelayCountRange)
	 
		PROP_CUSTOM(CFG_OsClkSourceOfCounters,         Writable, PropAccessOsClkSource)
	    PROP_CUSTOM(CFG_OsClkPolarityOfCounters,	   Writable, PropAccessOsClkPolarity )
		PROP_CUSTOM(CFG_OsDelayCountOfCounters,        Writable, PropAccessOsDelayCount)
		PROP_CUSTOM(CFG_OsGateSourceOfCounters,        Writable, PropAccessOsGateSource)
		PROP_CUSTOM(CFG_OsGatePolarityOfCounters,      Writable, PropAccessOsGatePolarity)
		PROP_CUSTOM(CFG_OsOutSignalOfCounters,         Writable, PropAccessOsOutSignal)

		// Timer/Pulse
		PROP_RO_PTR(CFG_FeatureTmrGateControlOfCounters,s_cntrTmrGateCtrl,		  sizeof(s_cntrTmrGateCtrl) )
		PROP_RO_PTR(CFG_FeatureTmrGatePolarities, 		s_cntrTmrGatePolarities,  sizeof(s_cntrTmrGatePolarities) )
		PROP_RO_PTR(CFG_FeatureTmrOutSignals, 			s_cntrTmrOutSignals,	  sizeof(s_cntrTmrOutSignals) )
		PROP_RO_VAL(CFG_FeatureTmrFrequencyRange, 		MathInterval,			  s_cntrTmrFreqRange )
		 
		PROP_CUSTOM(CFG_TmrGateEnabledOfCounters, 		Writable,				  PropAccessTmrGateEnabled )
		PROP_CUSTOM(CFG_TmrGatePolarityOfCounters,		Writable,				  PropAccessTmrGatePolarity )
		PROP_CUSTOM(CFG_TmrOutSignalOfCounters,			Writable,				  PropAccessTmrOutSignal )
		PROP_CUSTOM(CFG_TmrFrequencyOfCounters,			Writable,				  PropAccessTmrFrequency )
 
		// Frequency measurement
		PROP_RO_PTR(CFG_FeatureFmMethods,             s_cntrFmMethods, sizeof(s_cntrFmMethods))
		
		PROP_CUSTOM(CFG_FmMethodOfCounters,           Writable, PropAccessFmMethod)
		PROP_CUSTOM(CFG_FmCollectionPeriodOfCounters, Writable, PropAccessFmCollPeriodOfCntrs)
		
		// PWMOut
		PROP_RO_PTR(CFG_FeaturePoGateControlOfCounters,	s_cntrPoGateCtrl,		  sizeof(s_cntrPoGateCtrl) )
		PROP_RO_PTR(CFG_FeaturePoGatePolarities,		s_cntrPoGatePolarities,   sizeof(s_cntrPoGatePolarities) )
		PROP_RO_VAL(CFG_FeaturePoHiPeriodRange,			MathInterval,			  s_cntrPoHiLoPeriodRange )
		PROP_RO_VAL(CFG_FeaturePoLoPeriodRange,			MathInterval,			  s_cntrPoHiLoPeriodRange )

	 
		PROP_CUSTOM(CFG_PoHiPeriodOfCounters,        Writable, PropAccessPoHiPeriod)
		PROP_CUSTOM(CFG_PoLoPeriodOfCounters,        Writable, PropAccessPoLoPeriod)
		PROP_CUSTOM(CFG_PoGateEnabledOfCounters,     Writable, PropAccessPoGateEnabled)
		PROP_CUSTOM(CFG_PoGatePolarityOfCounters,    Writable, PropAccessPoGatePolarity)

		END_PROPERTY_TABLE()

  public:
		// --------------------------------------------------------------
		// BDaqCntr interface implementation
		// --------------------------------------------------------------
		// common methods
		virtual ErrorCode CounterReset(uint32 start, uint32 count);
		virtual ErrorCode CounterRead(uint32 start, uint32 count, uint32 *loadVals, uint32 *holdVals);

		// primal counter methods
		virtual ErrorCode CounterStart(uint32 start, uint32 count);

		// event counting methods
		virtual ErrorCode EventCountStart(uint32 start, uint32 count);
		virtual ErrorCode EventCountRead(uint32 start, uint32 count, uint32 *vals);

		// one-shot methods
		virtual ErrorCode OneShotStart(uint32 start, uint32 count);

		// timer/pulse methods
		virtual ErrorCode TimerPulseStart(uint32 start, uint32 count);

		// frequency measurement methods
		virtual ErrorCode FreqMeasureStart(uint32 start, uint32 count);
		virtual ErrorCode FreqMeasureRead(uint32 start, uint32 count, double * freq);

		// pulse width measurement methods
		virtual ErrorCode PwmInStart(uint32 cntrStart, uint32 groupCount);
		virtual ErrorCode PwmInRead(uint32 cntrStart, uint32 groupCount, double hiPeriod[], double lowPeriod[]);
		virtual ErrorCode PwmOutStart(uint32 cntrStart, uint32 cntrCount);

  public:
		// --------------------------------------------------------------
		// BDaqCntrImpl interface implementation
		// --------------------------------------------------------------
		BDaqCntrImpl()
		{
		  m_kstubPtr = NULL;
		  memset(m_cntrEvents,   0, sizeof(m_cntrEvents));
		}

		virtual ~BDaqCntrImpl()
		{
		}

		ErrorCode Initialize(BioKrnlStub *kstub);
		ErrorCode Reset(uint32 state);

  protected:

		ErrorCode PropAccessChipClkSrc(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessChipOpMode(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessChipSigCntType(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessChipClkPolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessChipGateSrc(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessChipGatePolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessChipOutSignal(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessChipLoadValue(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);	
		ErrorCode PropAccessChipHoldValue(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);

		//EventCount
		ErrorCode PropAccessEcGatePolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessEcClockPolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessEcGateEnabled(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);	

		//OneShot
		ErrorCode PropAccessOsDelayCount(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessOsClkSource(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessOsGateSource(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessOsGatePolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessOsOutSignal(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessOsClkPolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		
		//TimePulse
		ErrorCode PropAccessTmrFrequency(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessTmrGateEnabled(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessTmrGatePolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessTmrOutSignal(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);

        //Frequency measurement
        ErrorCode PropAccessFmMethod(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessFmCollPeriodOfCntrs(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		
		//PWMOut
		ErrorCode PropAccessPoHiPeriod(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessPoLoPeriod(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessPoGateEnabled(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);
		ErrorCode PropAccessPoGatePolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag);

		ErrorCode ConfigCounter(uint32 id, uint32 start, uint32 count, void *val, uint32 len);
		ErrorCode StartCounter(uint32 start, uint32 count, uint32 operation, void *param = NULL, uint32 paramLen = 0);
		ErrorCode ReadCounter(uint32 start, uint32 count, void *buffer, uint32 bufLen);

		ErrorCode CheckClkSource(uint32 *clkSrcs, uint32 count);
		ErrorCode CheckChipMode(uint32 *modes, uint32 count);
		ErrorCode CheckPropSpted(uint32 * Prop, uint32 PropCount, uint32* FeatureList, uint32 FeatureItemCount);
		
		ErrorCode AdjustLoadValue(uint32 *loadVals, uint32 count, uint32 *actualVals);
  		ErrorCode AdjustHoldValue(uint32 *holdVals, uint32 count, uint32 *actualVals);		
		ErrorCode AdjustOsDelayCount(uint32 *delays, uint32 count, uint32 *actualVals);

  private:
		BioKrnlStub  *m_kstubPtr;
		HANDLE m_cntrEvents[ ARRAY_SIZE(s_cntrSptedEvts) ];
};

// -----------------------------------------------------------------------------------------
// inline functions
// -----------------------------------------------------------------------------------------
  inline
ErrorCode BDaqCntrImpl::Initialize(BioKrnlStub *kstub)
{
  assert(kstub != NULL);
  m_kstubPtr = kstub;
  return Success;
}

  inline
ErrorCode BDaqCntrImpl::Reset(uint32 state)
{
  CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);

  CounterReset(0, CNTR_CHL_COUNT);

  struct {
	 uint32 id;
	 uint32 def_val;
  } prop_map[] = {
	 {CFG_ChipClkSourceOfCounters,     DEF_CNTR_CHIP_CLKSRC},
	 {CFG_ChipOperationModeOfCounters, DEF_CNTR_CHIP_OPMODE},
	 {CFG_ChipLoadValueOfCounters,     DEF_CNTR_CHIP_LOADVAL},
	 {CFG_OsClkSourceOfCounters,       DEF_CNTR_OST_CLKSRC},
	 {CFG_OsDelayCountOfCounters,      DEF_CNTR_OST_DELAYCNT},
	 {CFG_TmrFrequencyOfCounters,      DEF_CNTR_TMR_DIVISOR},
	 {CFG_FmCollectionPeriodOfCounters,DEF_CNTR_FM_PERIOD},
  };

  if (state){
	 uint32 xbuf[CNTR_CHL_COUNT];
	 if (state){
		for (unsigned i = 0; i < ARRAY_SIZE(prop_map); ++i){
		  std::fill_n(xbuf, CNTR_CHL_COUNT, prop_map[i].def_val);
		  ConfigCounter(prop_map[i].id, 0, CNTR_CHL_COUNT, xbuf, sizeof(xbuf));
		}
	 }
  }

  return Success;
}

  inline
ErrorCode BDaqCntrImpl::CounterReset(uint32 start, uint32 count)
{
  CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
  CHK_EXPR(start < CNTR_CHL_COUNT, ErrorParamOutOfRange);
  CHK_EXPR(count <= CNTR_CHL_COUNT, ErrorParamOutOfRange);

  if (count == 0) {
	 return Success;
  }

  CNTR_RESET cntrs = { start, count };
  m_kstubPtr->Ioctl(IOCTL_CNTR_RESET, &cntrs);

  return Success;
}

  inline
ErrorCode BDaqCntrImpl::CounterRead(uint32 start, uint32 count, uint32 *loadVals, uint32 *holdVals)
{
  CHK_USER_BUF(loadVals);
  if (holdVals){
	 memset(holdVals, 0, __min(count, CNTR_CHL_COUNT) * sizeof(uint32));
  }

  ErrorCode warning = Success;
  if (start >= CNTR_CHL_COUNT) {
	 start %= CNTR_CHL_COUNT;
	 SET_WARN(warning, WarningParamOutOfRange);
  }
  if (count > CNTR_CHL_COUNT) {
	 count = CNTR_CHL_COUNT;
	 SET_WARN(warning, WarningParamOutOfRange);
  }

  // read the counter value
  CNTR_VALUE rawVal[CNTR_CHL_COUNT];
  ErrorCode ret = ReadCounter(start, count, rawVal, sizeof(rawVal));
  if (ret == Success) {
	 // transfer data to user buffer
	 CNTR_VALUE *rawPtr = rawVal;
	 for(; count; --count, ++rawPtr, ++loadVals) {
		*loadVals = rawPtr->Value;
	 }
  }

  return ret != Success ? ret : warning;
}

  inline
ErrorCode BDaqCntrImpl::CounterStart(uint32 start, uint32 count)
{
  CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
  CHK_EXPR(start < CNTR_CHL_COUNT, ErrorParamOutOfRange);
  CHK_EXPR(count <= CNTR_CHL_COUNT, ErrorParamOutOfRange);

  return StartCounter(start, count, Primary);
}

  inline
ErrorCode BDaqCntrImpl::EventCountStart(uint32 start, uint32 count)
{
  CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
  CHK_EXPR(start < CNTR_CHL_COUNT, ErrorParamOutOfRange);
  CHK_EXPR(count <= CNTR_CHL_COUNT, ErrorParamOutOfRange);

  return StartCounter(start, count, InstantEventCount);
}

  inline
ErrorCode BDaqCntrImpl::EventCountRead(uint32 start, uint32 count, uint32 *vals)
{
  CHK_USER_BUF(vals);

  ErrorCode warning = Success;
  if (start >= CNTR_CHL_COUNT) {
	 start %= CNTR_CHL_COUNT;
	 SET_WARN(warning, WarningParamOutOfRange);
  }
  if (count > CNTR_CHL_COUNT) {
	 count = CNTR_CHL_COUNT;
	 SET_WARN(warning, WarningParamOutOfRange);
  }

  // read the counter value
  CNTR_VALUE rawVal[CNTR_CHL_COUNT];
  ErrorCode ret = ReadCounter(start, count, rawVal, sizeof(rawVal));
  if (ret == Success) {
	 // transfer data to user buffer
	 CNTR_VALUE *rawPtr = rawVal;
	 for(; count; --count, ++rawPtr, ++vals) {
		// convert the raw count value to event counting number
		if (rawPtr->CanRead) {  
		  *vals  = rawPtr->Value;
		} else { // the counter is waiting for a valid clock pulse to load the init value.
		  *vals = 0;
		}
	 }
  }

  return ret != Success ? ret : warning;
}

  inline
ErrorCode BDaqCntrImpl::OneShotStart(uint32 start, uint32 count)
{
  CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
  CHK_EXPR(start < CNTR_CHL_COUNT, ErrorParamOutOfRange);
  CHK_EXPR(count <= CNTR_CHL_COUNT, ErrorParamOutOfRange);

  return StartCounter(start, count, OneShot);
}

  inline
ErrorCode BDaqCntrImpl::TimerPulseStart(uint32 start, uint32 count)
{
  CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
  CHK_EXPR(start < CNTR_CHL_COUNT, ErrorParamOutOfRange);
  CHK_EXPR(count <= CNTR_CHL_COUNT, ErrorParamOutOfRange);

  return StartCounter(start, count, TimerPulse);
}

  inline
ErrorCode BDaqCntrImpl::FreqMeasureStart(uint32 start, uint32 count)
{
  CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
  CHK_EXPR(start < CNTR_CHL_COUNT, ErrorParamOutOfRange);
  CHK_EXPR(count <= CNTR_CHL_COUNT, ErrorParamOutOfRange);

  return StartCounter(start, count, InstantFreqMeter);
}

  inline
ErrorCode BDaqCntrImpl::FreqMeasureRead(uint32 start, uint32 count, double * freq)
{
  CHK_USER_BUF(freq);

  ErrorCode warning = Success;
  if (start >= CNTR_CHL_COUNT) {
	 start %= CNTR_CHL_COUNT;
	 SET_WARN(warning, WarningParamOutOfRange);
  }
  if (count > CNTR_CHL_COUNT) {
	 count = CNTR_CHL_COUNT;
	 SET_WARN(warning, WarningParamOutOfRange);
  }

  CNTR_STATE const * cntrState;
  CNTR_CONFIG const * cntrConfig;
  uint32 baseFreq;

  while (count--) {
	 cntrState = &m_kstubPtr->getShared()->CntrState[start];
	 cntrConfig = &m_kstubPtr->getShared()->CntrConfig;
	 baseFreq = cntrConfig->BaseFreq[start];


	 if (cntrState->CanRead) {
		if((m_kstubPtr->getShared()->CntrConfig.FmMethod[start] == PeriodInverse )&&(cntrState->SummedValue > 0)){
		  *freq = ((double)baseFreq/cntrState->SummedValue); 
		}else{
		  *freq = ((double)baseFreq*cntrState->SummedValue); 
		}
	 } else {
		*freq = 0;
	 }
	 //printf("BaseFreq = %u, FmMethod = %u,SummedValue = %u",baseFreq,cntrConfig->FmMethod[start],cntrState->SummedValue);	 
	 ++freq;
	 ++start;
	 start %= CNTR_CHL_COUNT;
  }

  return warning;
}
  inline
ErrorCode BDaqCntrImpl::PwmInStart(uint32 start, uint32 count)
{
  CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
  CHK_EXPR(start < CNTR_CHL_COUNT, ErrorParamOutOfRange);
  CHK_EXPR(count <= CNTR_CHL_COUNT, ErrorParamOutOfRange);

  return StartCounter(start, count, InstantPwmIn);
}

  inline
ErrorCode BDaqCntrImpl::PwmInRead(uint32 start, uint32 count, double *hiPeriod, double *lowPeriod)
{
  CHK_USER_BUF(hiPeriod);
  CHK_USER_BUF(lowPeriod);

  ErrorCode warning = Success;
  if (start >= CNTR_CHL_COUNT) {
	 start %= CNTR_CHL_COUNT;
	 SET_WARN(warning, WarningParamOutOfRange);
  }
  if (count > CNTR_CHL_COUNT) {
	 count = CNTR_CHL_COUNT;
	 SET_WARN(warning, WarningParamOutOfRange);
  }

  CNTR_STATE const * cntrState;
  while (count--) {
	 cntrState = &m_kstubPtr->getShared()->CntrState[start];
	 if (cntrState->CanRead) {
		*lowPeriod = ((double)m_kstubPtr->getShared()->CntrConfig.PoLowDiv[start]) / CNTR_CLK_BASE;
		*hiPeriod = ((double)m_kstubPtr->getShared()->CntrConfig.PoHighDiv[start]) / CNTR_CLK_BASE;
	 } else {
		*lowPeriod = ((double)0);
		*hiPeriod = ((double)0);
	 }
	 ++start;
  }

  return warning;	
}

  inline
ErrorCode BDaqCntrImpl::PwmOutStart(uint32 start, uint32 count)
{
  CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
  CHK_EXPR(start < CNTR_CHL_COUNT, ErrorParamOutOfRange);
  CHK_EXPR(count <= CNTR_CHL_COUNT, ErrorParamOutOfRange);

  return StartCounter(start, count, InstantPwmOut);
}

inline
ErrorCode BDaqCntrImpl::EventGetHandle(EventId id, HANDLE &handle)
{
   handle = NULL;
   
   // Look for the event in supported event list
   int index = ARRAY_INDEX_OF(s_cntrSptedEvts, (uint32)id);
   if (index == -1) {
      return ErrorEventNotSpted;
   }

   // Create the event if it hasn't been created.
   if (m_cntrEvents[index] == NULL) {
      USER_EVENT_INFO info = { id };
      m_kstubPtr->Ioctl(IOCTL_DEVICE_REGISTER_USER_EVENT, &info);
      m_cntrEvents[index] = info.Handle;
      
   }

   handle = m_cntrEvents[index];
   return handle != NULL ? Success : ErrorMemoryNotEnough;
}

inline
ErrorCode BDaqCntrImpl::EventGetStatus(EventId id, uint32 *lParam, uint32 *rParam)
{
   // Note: Don't disturb the kernel mode driver if we can handle the query here.
   EVENT_GET_STATUS  evtStatus = {0};
   uint32 evtIdx = GetEventKIndex( id );
  
 
	if ( evtIdx < KrnlSptedEventCount )
	{	
		evtStatus.LParam = m_kstubPtr->getShared()->IsEvtSignaled[evtIdx];
	}  else { // other not supported event type
		return ErrorEventNotSpted;
	}
   
	if ( lParam )
	{
		*lParam = evtStatus.LParam;
	}
	if ( rParam )
	{
		*rParam = evtStatus.RParam;
	}

   return Success;
}

inline
ErrorCode BDaqCntrImpl::EventClearFlag(EventId id, uint32 lParam, uint32 rParam)
{
   // Only the owner of the FAI operation can clear flag, others can only listen.
   CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);

   // For performance reason, I had to implement the function for
   // clearing event flag here and don't disturb the kernel.
   uint32 evtIdx = GetEventKIndex( id );
   
   if ( evtIdx < KrnlSptedEventCount )
   {   
       m_kstubPtr->getShared()->IsEvtSignaled[evtIdx] = 0;

   }  else { // other not supported event type
	   return ErrorEventNotSpted;
   }

   return Success;
}

inline
ErrorCode BDaqCntrImpl::PropAccessChipClkSrc(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.ChipClkSource;

	 return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32) * CNTR_CHL_COUNT);

	 ErrorCode ret = CheckClkSource((uint32 *)buffer, CNTR_CHL_COUNT);
	 CHK_EXPR(ret == Success, ret);

	 ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, buffer, sizeof(uint32) * CNTR_CHL_COUNT);
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret;
  }
}

inline
ErrorCode BDaqCntrImpl::PropAccessChipOpMode(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.ChipOpMode;

	 return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32) * CNTR_CHL_COUNT);

	 ErrorCode ret = CheckChipMode((uint32 *)buffer, CNTR_CHL_COUNT);
	 CHK_EXPR(ret == Success, ret);

	 ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, buffer, sizeof(uint32) * CNTR_CHL_COUNT);
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret;
  }
}

inline
ErrorCode BDaqCntrImpl::PropAccessChipSigCntType(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
	if ( IsOpGet( opFlag ) )
	{
		uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.ChipCntType;

		return DefCopyT( ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32*)buffer, bufLen );
	} else {
		CHK_PRVLG( m_kstubPtr->getMode() != ModeRead );
		CHK_USER_BUF_LEN( buffer, bufLen >= sizeof(uint32) * CNTR_CHL_COUNT );
	
		ErrorCode ret = CheckPropSpted(
			(uint32 *)buffer, CNTR_CHL_COUNT,
			(uint32 *)s_cntrChipCntTypes, 
			 ARRAY_SIZE(s_cntrChipCntTypes));
	
		CHK_EXPR( ret == Success, ret );

		ret = ConfigCounter( id, 0, CNTR_CHL_COUNT, buffer, sizeof(uint32) * CNTR_CHL_COUNT );
		if ( ret == Success && IsOpNtfNow(opFlag) ){
				m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
		}
		return ret;
	}

}
inline
ErrorCode BDaqCntrImpl::PropAccessChipClkPolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
	if ( IsOpGet( opFlag ) )
	{
		uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.ChipClkPolarity;

		return DefCopyT( ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32*)buffer, bufLen );
	} else {
		CHK_PRVLG( m_kstubPtr->getMode() != ModeRead );
		CHK_USER_BUF_LEN( buffer, bufLen >= sizeof(uint32) * CNTR_CHL_COUNT );
	
		ErrorCode  ret = CheckPropSpted(
			(uint32 *)buffer, CNTR_CHL_COUNT, 
			(uint32 *)s_cntrChipClkPolarities, ARRAY_SIZE(s_cntrChipClkPolarities));
		
		CHK_EXPR( ret == Success, ret );

		ret = ConfigCounter( id, 0, CNTR_CHL_COUNT, buffer, sizeof(uint32) * CNTR_CHL_COUNT );
		if ( ret == Success && IsOpNtfNow(opFlag) ){
			m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
		}

		return ret;
	}

}

inline
ErrorCode BDaqCntrImpl::PropAccessChipGateSrc(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
	if ( IsOpGet( opFlag ) )
	{
		uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.ChipGateSrc;

		return DefCopyT( ptr, sizeof(uint32)* CNTR_CHL_COUNT, (uint32*)buffer, bufLen );
	} else {
		CHK_PRVLG( m_kstubPtr->getMode() != ModeRead );
		CHK_USER_BUF_LEN( buffer, bufLen >= sizeof(uint32) * CNTR_CHL_COUNT );
	
		ErrorCode  ret = Success;
		
		ret = CheckPropSpted( (uint32*)buffer, 1, 
			(uint32 *)s_sptedGateSrc0, ARRAY_SIZE(s_sptedGateSrc0));
		CHK_EXPR( ret == Success, ret );
		
		ret = CheckPropSpted( (uint32*)buffer+1, 1, 
		(uint32 *)s_sptedGateSrc1, ARRAY_SIZE(s_sptedGateSrc1));

		CHK_EXPR( ret == Success, ret );
	
		ret = ConfigCounter( id, 0, CNTR_CHL_COUNT, buffer, sizeof(uint32) * CNTR_CHL_COUNT );
		
		if ( ret == Success && IsOpNtfNow(opFlag) ){
			m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);

		}

		return ret;
	}

}

inline
ErrorCode BDaqCntrImpl::PropAccessChipGatePolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
	if ( IsOpGet( opFlag ) )
	{
		uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.ChipGatePolarity;
 
		return DefCopyT( ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32*)buffer, bufLen );
	} else {
		CHK_PRVLG( m_kstubPtr->getMode() != ModeRead );
		CHK_USER_BUF_LEN( buffer, bufLen >= sizeof(uint32) * CNTR_CHL_COUNT );
	
		ErrorCode  ret = CheckPropSpted(
			(uint32 *)buffer, CNTR_CHL_COUNT,
			(uint32 *)s_cntrChipGatePolarities, ARRAY_SIZE(s_cntrChipGatePolarities));
	
		CHK_EXPR( ret == Success, ret );

		ret = ConfigCounter( id, 0, CNTR_CHL_COUNT, buffer, sizeof(uint32) * CNTR_CHL_COUNT );
		if ( ret == Success && IsOpNtfNow(opFlag) ){
			m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);

		}

		return ret;
	}

}
inline
ErrorCode BDaqCntrImpl::PropAccessChipOutSignal(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
	if ( IsOpGet( opFlag ) )
	{
		uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.ChipOutSignal;

		return DefCopyT( ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32*)buffer, bufLen );
	} else {
		CHK_PRVLG( m_kstubPtr->getMode() != ModeRead );
		CHK_USER_BUF_LEN( buffer, bufLen >= sizeof(uint32) * CNTR_CHL_COUNT );
	
		ErrorCode  ret = CheckPropSpted(
			(uint32 *)buffer, CNTR_CHL_COUNT,
			(uint32 *)s_cntrChipOutSignals, ARRAY_SIZE(s_cntrChipOutSignals));
	
		CHK_EXPR( ret == Success, ret );

		ret = ConfigCounter( id, 0, CNTR_CHL_COUNT, buffer, sizeof(uint32) * CNTR_CHL_COUNT );
		if ( ret == Success && IsOpNtfNow(opFlag) ){
			m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);

		}

		return ret;
	}

}
inline
ErrorCode BDaqCntrImpl::PropAccessChipHoldValue(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
	if (IsOpGet(opFlag)) {
	   uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.ChipHoldValue;
	
	   return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
	} else {
	   CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	   CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32) * CNTR_CHL_COUNT);
	
	   // Note: don't modify the user buffer.
	   uint32 val[ CNTR_CHL_COUNT ];
	   ErrorCode warning = AdjustHoldValue((uint32 *)buffer, CNTR_CHL_COUNT, val);
	
	   ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	   if (ret == Success && IsOpNtfNow(opFlag)) {
		  m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	   }
	
	   return ret != Success ? ret : warning; // the error code is top-priority
	}

}

inline
ErrorCode BDaqCntrImpl::PropAccessChipLoadValue(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.ChipLoadValue;

	 return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32) * CNTR_CHL_COUNT);

	 // Note: don't modify the user buffer.
	 uint32 val[ CNTR_CHL_COUNT ];
	 ErrorCode warning = AdjustLoadValue((uint32 *)buffer, CNTR_CHL_COUNT, val);

	 ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret != Success ? ret : warning; // the error code is top-priority
  }
}

inline
ErrorCode BDaqCntrImpl::PropAccessEcGatePolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  uint32 i;
  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.EcGatePolarity;

	 return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32));
	 //CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);

	 uint32 val[CNTR_CHL_COUNT] = {0};
	 for(i = 0; i < CNTR_CHL_COUNT; i++) {
		val[i] = (uint32)(*((uint32 *)buffer + i));
		if(val[i] < 0 || val[i] > 1) 
                       return ErrorPropValueNotSpted;
	 }

	 ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret;
  }
	 return Success;

}

inline
ErrorCode BDaqCntrImpl::PropAccessEcClockPolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  uint32 i;
  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.EcClockPolarity;

	 return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32));
	 //CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);

	 uint32 val[CNTR_CHL_COUNT] = {0};
	 for(i = 0; i < CNTR_CHL_COUNT; i++) {
		val[i] = (uint32)(*((uint32 *)buffer + i));
		if(val[i] < 0 || val[i] > 1) 
                       return ErrorPropValueNotSpted;
	 }

	 ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret;
  }
	 return Success;

}

  inline
ErrorCode BDaqCntrImpl::PropAccessEcGateEnabled(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  uint32 i;
  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.EcGateEnable;

	 return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32));
	 //CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);

	 // convert the property value from double to uint32
	 uint32 val[CNTR_CHL_COUNT] = {0};
	 for(i = 0; i < CNTR_CHL_COUNT; i++) {
		val[i] = (uint32)(*((uint32 *)buffer + i));
		if(val[i] < 0 || val[i] > 1){
                       return ErrorPropValueNotSpted;
                }
	 }

	 ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret;
  }
	 return Success;
}

inline
ErrorCode BDaqCntrImpl::PropAccessOsDelayCount(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.OstDelayCount;

	 return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32) * CNTR_CHL_COUNT);

	 // Note: don't modify the user buffer.
	 uint32 val[ CNTR_CHL_COUNT ];
	 ErrorCode warning = AdjustOsDelayCount((uint32 *)buffer, CNTR_CHL_COUNT, val);

	 ErrorCode ret     = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret != Success ? ret : warning; // the error code is top-priority
  }
}

inline
ErrorCode BDaqCntrImpl::PropAccessOsClkSource(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{

  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.OstClkSource;

	 return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32));
	 //CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);

	 ErrorCode  ret = Success;
	 
	 ret = CheckPropSpted( (uint32 *)buffer, 1, 
		 (uint32 *)s_cntrOsClkSrcOfCntr0, ARRAY_SIZE(s_cntrOsClkSrcOfCntr0));
	 CHK_EXPR( ret == Success, ret );
	 
	 ret = CheckPropSpted( (uint32 *)buffer+1, 1, 
		 (uint32 *)s_cntrOsClkSrcOfCntr1, ARRAY_SIZE(s_cntrOsClkSrcOfCntr1));
	 CHK_EXPR( ret == Success, ret );
	 

	 ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, buffer, sizeof(uint32)*CNTR_CHL_COUNT);
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret;
  }
	 return Success;
}

inline
ErrorCode BDaqCntrImpl::PropAccessOsGateSource(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{

  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.OstGateSource;

	 return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32));
	 //CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);

	 ErrorCode  ret = Success;
	 
	 ret = CheckPropSpted( (uint32 *)buffer, 1, 
		 (uint32*)s_cntrOsGateSrcOfCntr0, ARRAY_SIZE(s_cntrOsGateSrcOfCntr0));
	 CHK_EXPR( ret == Success, ret );
	 
	 ret = CheckPropSpted( (uint32 *)buffer+1, 1, 
		 (uint32 *)s_cntrOsGateSrcOfCntr1, ARRAY_SIZE(s_cntrOsGateSrcOfCntr1));
	 CHK_EXPR( ret == Success, ret );

	 uint32 val[ CNTR_CHL_COUNT ];
	 ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, buffer, sizeof(val));
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret;
  }
	 return Success;
}

inline
ErrorCode BDaqCntrImpl::PropAccessOsGatePolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  uint32 i;
  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.OstGatePolarity;

	 return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32));
	 //CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);

	 uint32 val[CNTR_CHL_COUNT] = {0};
	 for(i = 0; i < CNTR_CHL_COUNT; i++) {
		val[i] = (uint32)(*((uint32 *)buffer + i));
		if(val[i] < 0 || val[i] > 1) 
                       return ErrorPropValueNotSpted;
	 }

	 ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret;
  }
	 return Success;
}

inline
ErrorCode BDaqCntrImpl::PropAccessOsOutSignal(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  uint32 i;
  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.OstOutSignal;

	 return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32));
	 //CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);

	 uint32 val[CNTR_CHL_COUNT] = {0};
	 for(i = 0; i < CNTR_CHL_COUNT; i++) {
		val[i] = (uint32)(*((uint32 *)buffer + i));
		if(val[i] > 6 || val[i] < 3) 
      return ErrorPropValueNotSpted;
	 }

	 ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret;
  }
	 return Success;
}

inline
ErrorCode BDaqCntrImpl::PropAccessOsClkPolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
	if ( IsOpGet( opFlag ) )
	{
		uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.OsClkPolarity;

		return DefCopyT( ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen );
	} else {
		CHK_PRVLG( m_kstubPtr->getMode() != ModeRead );
		CHK_USER_BUF_LEN( buffer, bufLen >= sizeof(uint32) * CNTR_CHL_COUNT );
	
		ErrorCode  ret = CheckPropSpted(
			(uint32 *)buffer, CNTR_CHL_COUNT, 
			(uint32 *)s_cntrOsClkPolarities, ARRAY_SIZE(s_cntrOsClkPolarities));
	
		CHK_EXPR( ret == Success, ret );

		ret = ConfigCounter( id, 0, CNTR_CHL_COUNT, buffer, sizeof(uint32) * CNTR_CHL_COUNT );
		if ( ret == Success && IsOpNtfNow(opFlag) )
		{
			m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
		}
		return ret;
	}

}

inline
ErrorCode BDaqCntrImpl::PropAccessTmrFrequency(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  CNTR_CONFIG const & cntrCfg = m_kstubPtr->getShared()->CntrConfig;
  if (IsOpGet(opFlag)) {
	 // convert the counter value to frequency
	 uint32 const * divisor = cntrCfg.TmrFreqDiv;
	 double freq[CNTR_CHL_COUNT];
	 for (int i = 0; i < CNTR_CHL_COUNT; ++i) {
		freq[i] = divisor[i] ? CNTR_CLK_BASE / (double)divisor[i] : 0;
	 }

	 return DefCopyT(freq, sizeof(freq), (double*)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);

	 // convert the frequency to counter value
	 uint32    divisor[CNTR_CHL_COUNT];
	 double    *userFreq = (double*)buffer;
	 ErrorCode warning   = Success;
	 for (int i = 0; i < CNTR_CHL_COUNT; ++i) {
		//To avoid divider by zero error
		if (userFreq[i] < s_cntrTmrFreqRange.Min) {
		   userFreq[i] = s_cntrTmrFreqRange.Min;
		  SET_WARN(warning, WarningParamOutOfRange);
		} else if (userFreq[i] > s_cntrTmrFreqRange.Max){
		  userFreq[i] = s_cntrTmrFreqRange.Max;
		  SET_WARN(warning, WarningParamOutOfRange);
		}
		divisor[i] = (uint32)(CNTR_CLK_BASE / __max(userFreq[i], 0.0001));
	 }

	 ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, divisor, sizeof(divisor));
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret != Success ? ret : warning; // the error code is top-priority
  }
}

	inline
  ErrorCode BDaqCntrImpl::PropAccessTmrGateEnabled(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
  {
	uint32 i;
	if (IsOpGet(opFlag)) {
	   uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.TmGateEnable;
  
	   return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
	} else {
	   CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	   CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32));
	   //CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);
  
	   // convert the property value from double to uint32
	   uint32 val[CNTR_CHL_COUNT] = {0};
	   for(i = 0; i < CNTR_CHL_COUNT; i++) {
		  val[i] = (uint32)(*((uint32 *)buffer + i));
		  if(val[i] < 0 || val[i] > 1) 
                       return ErrorPropValueNotSpted;
	   }
  
	   ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	   if (ret == Success && IsOpNtfNow(opFlag)) {
		  m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	   }
  
	   return ret;
	}
	   return Success;
  }
  
  ErrorCode BDaqCntrImpl::PropAccessTmrGatePolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
  {
	uint32 i;
	if (IsOpGet(opFlag)) {
	   uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.TmGatePolarity;
  
	   return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
	} else {
	   CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	   CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32));
	   //CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);
  
	   uint32 val[CNTR_CHL_COUNT] = {0};
	   for(i = 0; i < CNTR_CHL_COUNT; i++) {
		  val[i] = (uint32)(*((uint32 *)buffer + i));
		  if(val[i] < 0 || val[i] > 1) 
                       return ErrorPropValueNotSpted;
	   }
  
	   ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	   if (ret == Success && IsOpNtfNow(opFlag)) {
		  m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	   }
  
	   return ret;
	}
	   return Success;
  }
  
  ErrorCode BDaqCntrImpl::PropAccessTmrOutSignal(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
  {
	uint32 i;
	if (IsOpGet(opFlag)) {
	   uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.TmOutSignal;
  
	   return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
	} else {
	   CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	   CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32));
	   //CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);
  
	   uint32 val[CNTR_CHL_COUNT] = {0};
	   for(i = 0; i < CNTR_CHL_COUNT; i++) {
		  val[i] = (uint32)(*((uint32 *)buffer + i));
		  if(val[i] < 3 || val[i] > 6) 
                       return ErrorPropValueNotSpted;
	   }
  
	   ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	   if (ret == Success && IsOpNtfNow(opFlag)) {
		  m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	   }
  
	   return ret;
	}
	   return Success;
  }

inline
ErrorCode BDaqCntrImpl::PropAccessFmMethod(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
	if ( IsOpGet( opFlag ) )
	{
		uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.FmMethod;

		return DefCopyT( ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32*)buffer, bufLen );
	} else {
		CHK_PRVLG( m_kstubPtr->getMode() != ModeRead );
		CHK_USER_BUF_LEN( buffer, bufLen >= sizeof(uint32) * CNTR_CHL_COUNT );
	
		ErrorCode  ret = CheckPropSpted(
			(uint32 *)buffer, CNTR_CHL_COUNT, 
			(uint32 *)s_cntrFmMethods, ARRAY_SIZE(s_cntrFmMethods));
	
		CHK_EXPR( ret == Success, ret );

		ret = ConfigCounter( id, 0, CNTR_CHL_COUNT, buffer, sizeof(uint32) * CNTR_CHL_COUNT );
		if ( ret == Success && IsOpNtfNow(opFlag) )
		{
			m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
		}

		return ret;
	}

}


  inline
ErrorCode BDaqCntrImpl::PropAccessFmCollPeriodOfCntrs(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.FmPeriod;
    double fmPeriod[CNTR_CHL_COUNT];
    
    for ( uint32 i = 0; i < CNTR_CHL_COUNT; i++ )
    {
      fmPeriod[i] = (ptr[i] > 0) ? (double)ptr[i] : 0.5;
    }

	 return DefCopyT(fmPeriod, sizeof(double) * CNTR_CHL_COUNT, (double*)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);

	 // convert the property value from double to uint32
	 uint32 val[CNTR_CHL_COUNT];
        
         double *fmPeriod = (double*)buffer; 
         
	 for(uint32 i = 0; i < CNTR_CHL_COUNT; i++) {
              if(fmPeriod[i]<5)  
                     val[i] = 0;
              else if(fmPeriod[i]<50)   
                     val[i] = 5;
              else if(fmPeriod[i]<500)  
                     val[i] = 50;
              else
                    val[i] = 500;
         }

	 ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret;
  }
}

  inline
ErrorCode BDaqCntrImpl::PropAccessPoHiPeriod(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  uint32 i;
  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.PoHighDiv;
	
         double period[CNTR_CHL_COUNT];
          
	 for(i = 0; i < CNTR_CHL_COUNT; i++) {
		period[i] = (double)((*((uint32 *)ptr + i ) + 1)/(double) CNTR_CLK_BASE);
	 }

         return DefCopyT(period, sizeof(period), (double*)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);

	 double    *PoPeriod  = (double*)buffer;
         
	 ErrorCode warning   = Success;
	 // convert the property value from double to uint32
	 uint32 val[CNTR_CHL_COUNT];
	 for(i = 0; i < CNTR_CHL_COUNT; i++) {
                
		if (PoPeriod[i] < s_cntrPoHiLoPeriodRange.Min) {
		    PoPeriod[i] = s_cntrPoHiLoPeriodRange.Min;
		    SET_WARN(warning, WarningParamOutOfRange);
		} else if (PoPeriod[i] > s_cntrPoHiLoPeriodRange.Max){
		  PoPeriod[i] = s_cntrPoHiLoPeriodRange.Max;
		  SET_WARN(warning, WarningParamOutOfRange);
		}

		val[i] = (uint32)(PoPeriod[i] * CNTR_CLK_BASE) - 1;
	 }

	 ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret;
  }
}

  inline
ErrorCode BDaqCntrImpl::PropAccessPoLoPeriod(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  uint32 i;
  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.PoLowDiv;

         double period[CNTR_CHL_COUNT];
	 for(i = 0; i < CNTR_CHL_COUNT; i++) {
		period[i] = (double)((*((uint32 *)ptr + i ) + 1)/(double) CNTR_CLK_BASE);
	 }

	 return DefCopyT(period, sizeof(period), (double*)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);

	 ErrorCode warning   = Success;
	 double    *PoPeriod  = (double*)buffer;
	 // convert the property value from double to uint32
	 uint32 val[CNTR_CHL_COUNT];
	 for(i = 0; i < CNTR_CHL_COUNT; i++) {

		if (PoPeriod[i] < s_cntrPoHiLoPeriodRange.Min) {
		    PoPeriod[i] = s_cntrPoHiLoPeriodRange.Min;
		    SET_WARN(warning, WarningParamOutOfRange);
		} else if (PoPeriod[i] > s_cntrPoHiLoPeriodRange.Max){
		  PoPeriod[i] = s_cntrPoHiLoPeriodRange.Max;
		  SET_WARN(warning, WarningParamOutOfRange);
		}
		val[i] = (uint32)(PoPeriod[i] * CNTR_CLK_BASE) - 1;
	 }

	 ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret;
  }
}

  inline
ErrorCode BDaqCntrImpl::PropAccessPoGateEnabled(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  uint32 i;
  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.PoGateEn;

	 return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32));
	 //CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);

	 // convert the property value from double to uint32
	 uint32 val[CNTR_CHL_COUNT] = {0};
	 for(i = 0; i < CNTR_CHL_COUNT; i++) {
		val[i] = (uint32)(*((uint32 *)buffer + i));
		if(val[i] < 0 || val[i] > 1) 
                       return ErrorPropValueNotSpted;
	 }

	 ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret;
  }
	 return Success;
}

inline
ErrorCode BDaqCntrImpl::PropAccessPoGatePolarity(uint32 id, uint32 &bufLen, void *buffer, uint32 opFlag)
{
  uint32 i;
  if (IsOpGet(opFlag)) {
	 uint32 const * ptr = m_kstubPtr->getShared()->CntrConfig.PoGatePolarity;

	 return DefCopyT(ptr, sizeof(uint32) * CNTR_CHL_COUNT, (uint32 *)buffer, bufLen);
  } else {
	 CHK_PRVLG(m_kstubPtr->getMode() != ModeRead);
	 CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(uint32));
	 //CHK_USER_BUF_LEN(buffer, bufLen >= sizeof(double) * CNTR_CHL_COUNT);

	 uint32 val[CNTR_CHL_COUNT] = {0};
	 for(i = 0; i < CNTR_CHL_COUNT; i++) {
		val[i] = (uint32)(*((uint32 *)buffer + i));
		if(val[i] < 0 || val[i] > 1) 
                       return ErrorPropValueNotSpted;
	 }

	 ErrorCode ret = ConfigCounter(id, 0, CNTR_CHL_COUNT, val, sizeof(val));
	 if (ret == Success && IsOpNtfNow(opFlag)) {
		m_kstubPtr->Ioctl(IOCTL_DEVICE_NOTIFY_PROP_CHGED, NULL);
	 }

	 return ret;
  }
	 return Success;

}

  inline
ErrorCode BDaqCntrImpl::ConfigCounter(uint32 id, uint32 start, uint32 count, void *val, uint32 len)
{
  CNTR_SET_CFG cfg = {id, start, count, val};
  m_kstubPtr->Ioctl(IOCTL_CNTR_SET_CFG, &cfg);
  return Success;
}

  inline
ErrorCode BDaqCntrImpl::StartCounter(uint32 start, uint32 count, uint32 operation, void *param, uint32 paramLen)
{
  assert(start < CNTR_CHL_COUNT && count <= CNTR_CHL_COUNT);
  assert(operation >= Primary && operation <= UpDownCount);

  if (count == 0) {
	 return Success;
  }

  CNTR_START cntrs = {operation, start, count, param};
  if (m_kstubPtr->Ioctl(IOCTL_CNTR_START, &cntrs)){
	 return ErrorFuncBusy;
  }

  return Success;
}

  inline
ErrorCode BDaqCntrImpl::ReadCounter(uint32 start, uint32 count, void *buffer, uint32 bufLen)
{
  assert(start < CNTR_CHL_COUNT && count <= CNTR_CHL_COUNT);
  if (count == 0) {
	 return Success;
  }

  CNTR_READ  cntrs = { start, count, 0, buffer };
  if (m_kstubPtr->Ioctl(IOCTL_CNTR_READ, &cntrs)){
	 return ErrorDeviceIoTimeOut;
  }
  return Success;
}

  inline 
int GetPropValueIndex(uint32 propVal, uint32 * propList, uint32 propCountInList)
  {
	  for ( unsigned i = 0; i < propCountInList; i++ )
	  {
		  if ( propVal == propList[i] )
		  {
			  return i;
		  }
	  }
  
	  return -1;
  }
inline  
ErrorCode BDaqCntrImpl::CheckPropSpted(uint32 * Prop, uint32 PropCount, uint32* FeatureList, uint32 FeatureItemCount)
  {
	  for ( unsigned i = 0; i < PropCount; i++ )
	  {
		  if ( -1 == GetPropValueIndex(Prop[i], FeatureList, FeatureItemCount))
		  {
			  return ErrorPropValueNotSpted;
		  }
	  }
	  return Success;
  }



  inline
ErrorCode BDaqCntrImpl::CheckClkSource(uint32 *clkSrcs, uint32 count)
{
  for (unsigned i = 0; i < count; ++i){
   if(i == 0)
   {
   	 if (ARRAY_INDEX_OF(s_sptedClkSrc0, clkSrcs[i]) == -1){
   		return ErrorPropValueNotSpted;
   	 }
   }else if(i == 1){
       if (ARRAY_INDEX_OF(s_sptedClkSrc1, clkSrcs[i]) == -1){
   		return ErrorPropValueNotSpted;
   	 }
   }
  }

  return Success;
}

  inline
ErrorCode BDaqCntrImpl::CheckChipMode(uint32 *modes, uint32 count)
{
  ErrorCode ret = ErrorPropValueNotSpted;

  for (unsigned i = 0; i < count; ++i){
       for(unsigned j = 0;j < ARRAY_SIZE(s_sptedChipOpModes);j++){
	   if(modes[i] ==  s_sptedChipOpModes[j]){

                ret = Success;
                break;
	    }
        }
  }

  return ret;
}

  inline
ErrorCode BDaqCntrImpl::AdjustLoadValue(uint32 *loadVals, uint32 count, uint32 *actualVals)
{
  if (actualVals){
	 memcpy(actualVals, loadVals, count * sizeof(uint32));
  }

  ErrorCode ret = Success;
  for (unsigned i = 0; i < count; ++i){
	 if (loadVals[i] < CNTR_MIN_VAL){
		ret = WarningPropValueOutOfRange;
		if (actualVals){
		  actualVals[i] = CNTR_MIN_VAL;
		}
	 } else if (loadVals[i] > CNTR_MAX_VAL){
		ret = WarningPropValueOutOfRange;
		if (actualVals){
		  actualVals[i] = CNTR_MAX_VAL;
		}
	 }
  }

  return ret;
}

	inline
  ErrorCode BDaqCntrImpl::AdjustHoldValue(uint32 *holdVals, uint32 count, uint32 *actualVals)
  {
	if (actualVals){
	   memcpy(actualVals, holdVals, count * sizeof(uint32));
	}
  
	ErrorCode ret = Success;
	for (unsigned i = 0; i < count; ++i){
	   if (holdVals[i] < CNTR_MIN_VAL){
		  ret = WarningPropValueOutOfRange;
		  if (actualVals){
			actualVals[i] = CNTR_MIN_VAL;
		  }
	   } else if (holdVals[i] > CNTR_MAX_VAL){
		  ret = WarningPropValueOutOfRange;
		  if (actualVals){
			actualVals[i] = CNTR_MAX_VAL;
		  }
	   }
	}
  
	return ret;
  }


  inline
ErrorCode BDaqCntrImpl::AdjustOsDelayCount(uint32 *delays, uint32 count, uint32 *actualVals)
{
  
  if (actualVals){
	 memcpy(actualVals, delays, count * sizeof(uint32));
  }

  ErrorCode ret = Success;
  for (unsigned i = 0; i < count; ++i){
	 if (delays[i] < s_cntrOsDelayCountRange.Min){
		ret = WarningPropValueOutOfRange;
		if (actualVals){
		  actualVals[i] =s_cntrOsDelayCountRange.Min;
		}
	 } else if (delays[i] > s_cntrOsDelayCountRange.Max){
		ret = WarningPropValueOutOfRange;
		if (actualVals){
		  actualVals[i] = s_cntrOsDelayCountRange.Max;
		}
	 }
  }

  return ret;
}

#endif /* _BDAQ_COUNTER_IMPL_H_ */
