/*
 * biodllcmndef.h
 *
 *  Created on: Nov 25, 2011
 *      Author: rocky
 */

#ifndef _BIONIC_DAQ_PROPERTY_ATTRIBUTE_DEF_H
#define _BIONIC_DAQ_PROPERTY_ATTRIBUTE_DEF_H

/*-------------------------------------------------------------------
* Bionic driver property attributes
*-------------------------------------------------------------------*/
// Note:
//     Only Modal/Nature bit is defined here. The R/W bit is determined
// by the macro used to define a item in the property table when the
// property table is constructed
/*-----------------------------------------------------------------
* common property
*-----------------------------------------------------------------*/
#define CFG_Number_Attr                      ( Modal )
#define CFG_ComponentType_Attr               ( Modal )
#define CFG_Description_Attr                 ( Nature )
#define CFG_Parent_Attr                      ( Modal )
#define CFG_ChildList_Attr                   ( Modal )

/*-----------------------------------------------------------------
* component specified Property IDs -- group
*-----------------------------------------------------------------*/
#define CFG_DevicesNumber_Attr               ( Modal )
#define CFG_DevicesHandle_Attr               ( Modal )

/*-----------------------------------------------------------------
* component specified Property IDs -- device
*-----------------------------------------------------------------*/
#define CFG_DeviceGroupNumber_Attr              ( Modal )
#define CFG_DeviceProductID_Attr                ( Modal )
#define CFG_DeviceBoardID_Attr                  ( Modal )
#define CFG_DeviceBoardVersion_Attr             ( Modal )
#define CFG_DeviceDriverVersion_Attr            ( Modal )
#define CFG_DeviceDllVersion_Attr               ( Modal )
#define CFG_DeviceLocation_Attr                 ( Modal )
#define CFG_DeviceBaseAddress_Attr              ( Modal )
#define CFG_DeviceBaseAddresses_Attr            ( Modal )
#define CFG_DeviceInterrupts_Attr               ( Modal )
#define CFG_DeviceSupportedTerminalBoardTypes_Attr ( Modal )
#define CFG_DeviceTerminalBoardType_Attr        ( Modal )
#define CFG_DeviceSupportedEvents_Attr          ( Modal )
#define CFG_DeviceHotResetPreventable_Attr      ( Modal )
#define CFG_DeviceLoadingTimeInit_Attr          ( Modal )
#define CFG_DeviceWaitingForReconnect_Attr      ( Nature )
#define CFG_DeviceWaitingForSleep_Attr          ( Nature )
#define CFG_DevicePrivateRegionLength_Attr      ( Modal  )

/*-----------------------------------------------------------------
* component specified Property IDs -- group
*-----------------------------------------------------------------*/
#define CFG_GroupAmsiSettingtable_Attr          ( Modal )

/*-----------------------------------------------------------------
* component specified Property IDs -- AI, AO...
*-----------------------------------------------------------------*/
#define CFG_FeatureResolutionInBit_Attr         ( Nature )
#define CFG_FeatureDataSize_Attr                ( Nature )
#define CFG_FeatureDataMask_Attr                ( Nature )
#define CFG_FeatureSynchronousOut_Attr          ( Modal )
#define CFG_FeatureChannelNumberMax_Attr        ( Nature )
#define CFG_FeatureChannelConnectionType_Attr   ( Modal )
#define CFG_FeatureBurnDetectedReturnTypes_Attr ( Modal )
#define CFG_FeatureBurnoutDetectionChannels_Attr   ( Modal )
#define CFG_FeatureOverallVrgType_Attr          ( Modal )
#define CFG_FeatureVrgTypes_Attr                ( Modal )
#define CFG_FeatureExtRefRange_Attr             ( Nature )
#define CFG_FeatureExtRefAntiPolar_Attr         ( Modal )
#define CFG_FeatureCjcChannels_Attr             ( Modal )
#define CFG_FeatureChannelScanMethod_Attr       ( Modal )
#define CFG_FeatureScanChannelStartBase_Attr    ( Modal )
#define CFG_FeatureScanChannelCountBase_Attr    ( Modal )

#define CFG_FeatureConvertClockSources_Attr     ( Modal )
#define CFG_FeatureConvertClockRateRange_Attr   ( Nature )
#define CFG_FeatureScanClockSources_Attr        ( Modal )
#define CFG_FeatureScanClockRateRange_Attr      ( Nature )
#define CFG_FeatureScanCountMax_Attr            ( Nature )
#define CFG_FeatureFifoSize_Attr                ( Nature )
#define CFG_FeatureTriggersCount_Attr           ( Modal )
#define CFG_FeatureTriggerSources_Attr          ( Modal )
#define CFG_FeatureTriggerActions_Attr          ( Modal )
#define CFG_FeatureTriggerDelayCountRange_Attr  ( Nature )
#define CFG_FeatureTriggerSources1_Attr         ( Modal )
#define CFG_FeatureTriggerActions1_Attr         ( Modal )
#define CFG_FeatureTriggerDelayCountRange1_Attr ( Nature )

#define CFG_ChannelCount_Attr                      ( Nature )
#define CFG_ConnectionTypeOfChannels_Attr          ( Modal )
#define CFG_VrgTypeOfChannels_Attr                 ( Modal )
#define CFG_BurnDetectedReturnTypeOfChannels_Attr  ( Modal )
#define CFG_BurnoutReturnValueOfChannels_Attr      ( Nature )
#define CFG_SynchronousOutEnabled_Attr             ( Modal )
#define CFG_ExtRefValueForUnipolar_Attr            ( Nature )
#define CFG_ExtRefValueForBipolar_Attr             ( Nature )

#define CFG_CjcChannel_Attr                     ( Modal )
#define CFG_CjcUpdateFrequency_Attr             ( Nature )
#define CFG_CjcValue_Attr                       ( Nature )

#define CFG_SectionDataCount_Attr               ( Nature )
#define CFG_ConvertClockSource_Attr             ( Modal )
#define CFG_ConvertClockRatePerChannel_Attr     ( Nature )
#define CFG_ScanChannelStart_Attr               ( Nature )
#define CFG_ScanChannelCount_Attr               ( Nature )
#define CFG_ScanClockSource_Attr                ( Modal )
#define CFG_ScanClockRate_Attr                  ( Nature )
#define CFG_ScanCount_Attr                      ( Nature )
#define CFG_TriggerSource_Attr                  ( Modal )
#define CFG_TriggerSourceEdge_Attr              ( Modal )
#define CFG_TriggerSourceLevel_Attr             ( Nature )
#define CFG_TriggerDelayCount_Attr              ( Nature )
#define CFG_TriggerAction_Attr                  ( Modal )
#define CFG_TriggerSource1_Attr                 ( Modal )
#define CFG_TriggerSourceEdge1_Attr             ( Modal )
#define CFG_TriggerSourceLevel1_Attr            ( Nature )
#define CFG_TriggerDelayCount1_Attr             ( Nature )
#define CFG_TriggerAction1_Attr                 ( Modal )
#define CFG_ParentSignalConnectionChannel_Attr  ( Modal )
#define CFG_ParentCjcConnectionChannel_Attr     ( Modal )
#define CFG_ParentControlPort_Attr              ( Modal )

#define CFG_SaiAutoConvertClockRate_Attr        ( Nature )
#define CFG_SaiAutoConvertChannelStart_Attr     ( Nature )
#define CFG_SaiAutoConvertChannelCount_Attr     ( Nature )
#define CFG_ExtPauseSignalEnabled_Attr          ( Nature )
#define CFG_ExtPauseSignalPolarity_Attr         ( Nature )
#define CFG_OrderOfChannels_Attr                ( Modal )

#define CFG_InitialStateOfChannels_Attr         ( Nature )

#define CFG_FeatureFilterTypes_Attr             ( Modal )
#define CFG_FeatureFilterCutoffFreqRange_Attr   ( Modal )
#define CFG_FeatureFilterCutoffFreq1Range_Attr  ( Modal )
#define CFG_FilterTypeOfChannels_Attr           ( Modal )
#define CFG_FilterCutoffFreqOfChannels_Attr     ( Nature )
#define CFG_FilterCutoffFreq1OfChannels_Attr    ( Nature )

/*-----------------------------------------------------------------
* component specified Property IDs -- DIO,...
*-----------------------------------------------------------------*/
//-------------------new feature for di open state 2013-5-6
#define CFG_FeatureDiOpenStatePorts_Attr                 ( Nature )
#define CFG_FeatureDiOpenStates_Attr                     ( Nature )
//-------------------
#define CFG_FeaturePortsCount_Attr                       ( Nature )
#define CFG_FeaturePortsType_Attr                        ( Modal )
#define CFG_FeatureNoiseFilterOfChannels_Attr            ( Modal )
#define CFG_FeatureNoiseFilterBlockTimeRange_Attr        ( Nature )
#define CFG_FeatureDiintTriggerEdges_Attr                ( Modal )
#define CFG_FeatureDiintOfChannels_Attr                  ( Modal )
#define CFG_FeatureDiintGateOfChannels_Attr              ( Modal )
#define CFG_FeatureDiCosintOfChannels_Attr               ( Modal )
#define CFG_FeatureDiPmintOfChannels_Attr                ( Modal )
#define CFG_FeatureSnapEventSources_Attr               ( Modal )
#define CFG_FeatureDiSnapEventSources_Attr               ( Modal )
#define CFG_FeatureDoFreezeSignalSources_Attr            ( Modal )
#define CFG_FeatureDoReflectWdtFeedIntervalRange_Attr    ( Nature )

#define CFG_FeatureDiPortScanMethod_Attr                 ( Modal )
#define CFG_FeatureDiConvertClockSources_Attr            ( Modal )
#define CFG_FeatureDiConvertClockRateRange_Attr          ( Nature )
#define CFG_FeatureDiScanClockSources_Attr               ( Modal )
#define CFG_FeatureDiScanClockRateRange_Attr             ( Nature )
#define CFG_FeatureDiScanCountMax_Attr                   ( Nature )
#define CFG_FeatureDiTriggersCount_Attr                  ( Modal )
#define CFG_FeatureDiTriggerActions_Attr                 ( Modal )
#define CFG_FeatureDiTriggerSources_Attr                 ( Modal )
#define CFG_FeatureDiTriggerDelayCountRange_Attr         ( Nature )
#define CFG_FeatureDiTriggerActions1_Attr                ( Modal )
#define CFG_FeatureDiTriggerSources1_Attr                ( Modal )
#define CFG_FeatureDiTriggerDelayCountRange1_Attr        ( Nature )

#define CFG_FeatureDoPortScanMethod_Attr                 ( Modal )
#define CFG_FeatureDoConvertClockSources_Attr            ( Modal )
#define CFG_FeatureDoConvertClockRateRange_Attr          ( Nature )
#define CFG_FeatureDoScanClockSources_Attr               ( Modal )
#define CFG_FeatureDoScanClockRateRange_Attr             ( Nature )
#define CFG_FeatureDoScanCountMax_Attr                   ( Nature )
#define CFG_FeatureDoTriggersCount_Attr                  ( Modal )
#define CFG_FeatureDoTriggerActions_Attr                 ( Modal )
#define CFG_FeatureDoTriggerSources_Attr                 ( Modal )
#define CFG_FeatureDoTriggerDelayCountRange_Attr         ( Nature )
#define CFG_FeatureDoTriggerActions1_Attr                ( Modal )
#define CFG_FeatureDoTriggerSources1_Attr                ( Modal )
#define CFG_FeatureDoTriggerDelayCountRange1_Attr        ( Nature )

// new: for di open state  2013-5-6
#define CFG_DiOpenStatesOfPorts_Attr                     ( Modal )

#define CFG_DirectionOfPorts_Attr                        ( Modal )
#define CFG_DiDataMaskOfPorts_Attr                       ( Modal )
#define CFG_DoDataMaskOfPorts_Attr                       ( Modal )

#define CFG_NoiseFilterOverallBlockTime_Attr             ( Nature )
#define CFG_NoiseFilterEnabledChannels_Attr              ( Modal )
#define CFG_DiintTriggerEdgeOfChannels_Attr              ( Modal )
#define CFG_DiintGateEnabledChannels_Attr                ( Modal )
#define CFG_DiCosintEnabledChannels_Attr                 ( Modal )
#define CFG_DiPmintEnabledChannels_Attr                  ( Modal )
#define CFG_DiPmintValueOfPorts_Attr                     ( Nature )
#define CFG_DoInitialStateOfPorts_Attr                   ( Modal )
#define CFG_DoFreezeEnabled_Attr                         ( Modal )
#define CFG_DoFreezeSignalState_Attr                     ( Modal )
#define CFG_DoReflectWdtFeedInterval_Attr                ( Nature )
#define CFG_DoReflectWdtLockValue_Attr                   ( Modal )

#define CFG_DiSectionDataCount_Attr                      ( Nature )
#define CFG_DiConvertClockSource_Attr                    ( Modal )
#define CFG_DiConvertClockRatePerPort_Attr               ( Nature )
#define CFG_DiScanPortStart_Attr                         ( Nature )
#define CFG_DiScanPortCount_Attr                         ( Nature )
#define CFG_DiScanClockSource_Attr                       ( Modal )
#define CFG_DiScanClockRate_Attr                         ( Nature )
#define CFG_DiScanCount_Attr                             ( Nature )
#define CFG_DiTriggerAction_Attr                         ( Modal )
#define CFG_DiTriggerSource_Attr                         ( Modal )
#define CFG_DiTriggerSourceEdge_Attr                     ( Modal )
#define CFG_DiTriggerSourceLevel_Attr                    ( Nature )
#define CFG_DiTriggerDelayCount_Attr                     ( Nature )
#define CFG_DiTriggerAction1_Attr                        ( Modal )
#define CFG_DiTriggerSource1_Attr                        ( Modal )
#define CFG_DiTriggerSourceEdge1_Attr                    ( Modal )
#define CFG_DiTriggerSourceLevel1_Attr                   ( Nature )
#define CFG_DiTriggerDelayCount1_Attr                    ( Nature )

#define CFG_DoSectionDataCount_Attr                      ( Nature )
#define CFG_DoConvertClockSource_Attr                    ( Modal )
#define CFG_DoConvertClockRatePerPort_Attr               ( Nature )
#define CFG_DoScanPortStart_Attr                        ( Nature )
#define CFG_DoScanPortCount_Attr                        ( Nature )
#define CFG_DoScanClockSource_Attr                       ( Modal )
#define CFG_DoScanClockRate_Attr                         ( Nature )
#define CFG_DoScanCount_Attr                             ( Nature )
#define CFG_DoTriggerAction_Attr                         ( Modal )
#define CFG_DoTriggerSource_Attr                         ( Modal )
#define CFG_DoTriggerSourceEdge_Attr                     ( Modal )
#define CFG_DoTriggerSourceLevel_Attr                    ( Nature )
#define CFG_DoTriggerDelayCount_Attr                     ( Nature )
#define CFG_DoTriggerAction1_Attr                        ( Modal )
#define CFG_DoTriggerSource1_Attr                        ( Modal )
#define CFG_DoTriggerSourceEdge1_Attr                    ( Modal )
#define CFG_DoTriggerSourceLevel1_Attr                   ( Nature )
#define CFG_DoTriggerDelayCount1_Attr                    ( Nature )

/*-----------------------------------------------------------------
* component specified Property IDs -- Counter/Timer
*-----------------------------------------------------------------*/
/*common*/
#define CFG_FeatureClkPolarities_Attr            ( Modal )
#define CFG_FeatureGatePolarities_Attr           ( Modal )
#define CFG_FeatureOutSignalTypes_Attr           ( Modal )
#define CFG_FeatureCapabilitiesOfCounter0_Attr   ( Modal )
#define CFG_FeatureCapabilitiesOfCounter1_Attr   ( Modal )
#define CFG_FeatureCapabilitiesOfCounter2_Attr   ( Modal )
#define CFG_FeatureCapabilitiesOfCounter3_Attr   ( Modal )
#define CFG_FeatureCapabilitiesOfCounter4_Attr   ( Modal )
#define CFG_FeatureCapabilitiesOfCounter5_Attr   ( Modal )
#define CFG_FeatureCapabilitiesOfCounter6_Attr   ( Modal )
#define CFG_FeatureCapabilitiesOfCounter7_Attr   ( Modal )
#define CFG_ClkPolarityOfCounters_Attr           ( Modal )
#define CFG_GatePolarityOfCounters_Attr          ( Modal )
#define CFG_OutSignalTypeOfCounters_Attr         ( Modal )

/*Primal counter properties */
#define CFG_FeatureChipOperationModes_Attr        ( Modal )
#define CFG_FeatureChipSignalCountingTypes_Attr   ( Modal )
#define CFG_FeatureChipClkSourceOfCounter0_Attr   ( Modal )
#define CFG_FeatureChipClkSourceOfCounter1_Attr   ( Modal )
#define CFG_FeatureChipClkSourceOfCounter2_Attr   ( Modal )
#define CFG_FeatureChipClkSourceOfCounter3_Attr   ( Modal )
#define CFG_FeatureChipClkSourceOfCounter4_Attr   ( Modal )
#define CFG_FeatureChipClkSourceOfCounter5_Attr   ( Modal )
#define CFG_FeatureChipClkSourceOfCounter6_Attr   ( Modal )
#define CFG_FeatureChipClkSourceOfCounter7_Attr   ( Modal )

#define CFG_FeatureChipGateSourceOfCounter0_Attr   ( Modal )
#define CFG_FeatureChipGateSourceOfCounter1_Attr   ( Modal )
#define CFG_FeatureChipGateSourceOfCounter2_Attr   ( Modal )
#define CFG_FeatureChipGateSourceOfCounter3_Attr   ( Modal )
#define CFG_FeatureChipGateSourceOfCounter4_Attr   ( Modal )
#define CFG_FeatureChipGateSourceOfCounter5_Attr   ( Modal )
#define CFG_FeatureChipGateSourceOfCounter6_Attr   ( Modal )
#define CFG_FeatureChipGateSourceOfCounter7_Attr   ( Modal )

#define CFG_FeatureChipValueRegisters_Attr         ( Modal )

#define CFG_ChipOperationModeOfCounters_Attr      ( Modal )
#define CFG_ChipSignalCountingTypeOfCounters_Attr ( Modal )
#define CFG_ChipLoadValueOfCounters_Attr          ( Nature )
#define CFG_ChipHoldValueOfCounters_Attr          ( Nature )
#define CFG_ChipOverCompareValueOfCounters_Attr   ( Nature )
#define CFG_ChipUnderCompareValueOfCounters_Attr  ( Nature )
#define CFG_ChipOverCompareEnabledCounters_Attr   ( Modal )
#define CFG_ChipUnderCompareEnabledCounters_Attr  ( Modal )
#define CFG_ChipClkSourceOfCounters_Attr          ( Modal )
#define CFG_ChipGateSourceOfCounters_Attr         ( Modal )

#define CFG_FeatureChipClkPolarities_Attr         (Modal)
#define CFG_FeatureChipGatePolarities_Attr        (Modal)
#define CFG_FeatureChipOutSignals_Attr            (Modal)

#define CFG_ChipClkPolarityOfCounters_Attr        (Modal)
#define CFG_ChipGatePolarityOfCounters_Attr       (Modal)
#define CFG_ChipOutSignalOfCounters_Attr          (Modal)

#define CFG_FeaturePoOutSignals_Attr              ( Nature )

/*Event counting*/
#define CFG_FeatureEcSignalCountingTypes_Attr     ( Modal )
#define CFG_FeatureEcOverCompareIntCounters_Attr  ( Modal )
#define CFG_FeatureEcUnderCompareIntCounters_Attr ( Modal )
#define CFG_EcOverCompareValueOfCounters_Attr     ( Nature )
#define CFG_EcUnderCompareValueOfCounters_Attr    ( Nature )
#define CFG_EcSignalCountingTypeOfCounters_Attr  ( Modal )

/*One-shot*/
#define CFG_FeatureOsClkSourceOfCounter0_Attr   ( Modal)
#define CFG_FeatureOsClkSourceOfCounter1_Attr   ( Modal)
#define CFG_FeatureOsClkSourceOfCounter2_Attr   ( Modal)
#define CFG_FeatureOsClkSourceOfCounter3_Attr   ( Modal)
#define CFG_FeatureOsClkSourceOfCounter4_Attr   ( Modal)
#define CFG_FeatureOsClkSourceOfCounter5_Attr   ( Modal)
#define CFG_FeatureOsClkSourceOfCounter6_Attr   ( Modal)
#define CFG_FeatureOsClkSourceOfCounter7_Attr   ( Modal)

#define CFG_FeatureOsGateSourceOfCounter0_Attr   ( Modal)
#define CFG_FeatureOsGateSourceOfCounter1_Attr   ( Modal)
#define CFG_FeatureOsGateSourceOfCounter2_Attr   ( Modal)
#define CFG_FeatureOsGateSourceOfCounter3_Attr   ( Modal)
#define CFG_FeatureOsGateSourceOfCounter4_Attr   ( Modal)
#define CFG_FeatureOsGateSourceOfCounter5_Attr   ( Modal)
#define CFG_FeatureOsGateSourceOfCounter6_Attr   ( Modal)
#define CFG_FeatureOsGateSourceOfCounter7_Attr   ( Modal)

#define CFG_FeatureOsDelayCountRange_Attr        (Nature)
#define CFG_OsClkSourceOfCounters_Attr           ( Modal )
#define CFG_OsGateSourceOfCounters_Attr          ( Modal )
#define CFG_OsDelayCountOfCounters_Attr          ( Nature )

#define CFG_PoOutSignalOfCounters_Attr           (Modal)

/*timer/pulse*/
#define CFG_FeatureTmrCascadeGroups_Attr          ( Modal )
#define CFG_TmrFrequencyOfCounters_Attr           ( Nature )

/*frequency measurement*/
#define CFG_FeatureFmCascadeGroups_Attr           ( Modal )
#define CFG_FeatureFmMethods_Attr                 ( Modal )
#define CFG_FmMethodOfCounters_Attr               ( Modal )
#define CFG_FmCollectionPeriodOfCounters_Attr     ( Nature )

/*Pulse width measurement features*/
#define CFG_FeaturePiCascadeGroups_Attr           ( Modal )

/*Pulse width modulation properties*/
#define CFG_PoHiPeriodOfCounters_Attr             ( Nature )
#define CFG_PoLoPeriodOfCounters_Attr             ( Nature )

/*Event counting features & properties*/
#define CFG_FeatureEcClkPolarities_Attr            (Modal)
#define CFG_FeatureEcGatePolarities_Attr           (Modal)
#define CFG_FeatureEcGateControlOfCounters_Attr    (Modal)

#define CFG_EcClkPolarityOfCounters_Attr           (Modal)
#define CFG_EcGatePolarityOfCounters_Attr          (Modal)
#define CFG_EcGateEnabledOfCounters_Attr           (Modal)

/*one-shot features & properties*/
#define CFG_FeatureOsClkPolarities_Attr            (Modal)
#define CFG_FeatureOsGatePolarities_Attr           (Modal)
#define CFG_FeatureOsOutSignals_Attr               (Modal)

#define CFG_OsClkPolarityOfCounters_Attr           (Modal)
#define CFG_OsGatePolarityOfCounters_Attr          (Modal)
#define CFG_OsOutSignalOfCounters_Attr             (Modal)

/*timer/pulse features & properties*/
#define CFG_FeatureTmrGateControlOfCounters_Attr   (Modal)
#define CFG_FeatureTmrGatePolarities_Attr          (Modal)
#define CFG_FeatureTmrOutSignals_Attr              (Modal)
#define CFG_FeatureTmrFrequencyRange_Attr          (Nature)

#define CFG_TmrGateEnabledOfCounters_Attr          (Modal)
#define CFG_TmrGatePolarityOfCounters_Attr         (Modal)
#define CFG_TmrOutSignalOfCounters_Attr            (Modal)

/*Pulse width modulation features & properties*/
#define CFG_FeaturePoGateControlOfCounters_Attr    (Modal)
#define CFG_FeaturePoGatePolarities_Attr           (Modal)
#define CFG_FeaturePoHiPeriodRange_Attr            (Nature)
#define CFG_FeaturePoLoPeriodRange_Attr            (Nature)
#define CFG_FeaturePoOutCountRange_Attr            (Nature)

#define CFG_PoGateEnabledOfCounters_Attr           (Modal)
#define CFG_PoGatePolarityOfCounters_Attr          (Modal)
#define CFG_PoOutCountOfCounters_Attr              (Nature)

/*Up-down count features & properties*/
#define CFG_FeatureUdCountingTypes_Attr            (Modal)
#define CFG_FeatureUdInitialValues_Attr            (Nature)
#define CFG_UdCountingTypeOfCounters_Attr          (Modal)
#define CFG_UdInitialValueOfCounters_Attr          (Nature)
#define CFG_UdCountValueResetTimesByIndexs_Attr    (Nature)

#endif /* _BIONIC_DAQ_PROPERTY_ATTRIBUTE_DEF_H */
