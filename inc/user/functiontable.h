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

/****************************************************************************
*                                                                           *
* FunctionTable.h -- Bionic driver function and function table structure    *
*                    definition                                             *
*                                                                           *
****************************************************************************/

#ifndef _BIONIC_FUNCTION_TABLE_DEFINITION
#define _BIONIC_FUNCTION_TABLE_DEFINITION

//##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// V1.0, API prototype(initial version)
//##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// component common routines
typedef LONG (APIENTRY *FPEventGetHandle)(HANDLE, ULONG, HANDLE*);
typedef LONG (APIENTRY *FPEventGetLastStatus)(HANDLE, ULONG, ULONG*, ULONG*);
typedef LONG (APIENTRY *FPEventClearFlag)(HANDLE, ULONG, ULONG, ULONG);
typedef LONG (APIENTRY *FPPropertyRead)(HANDLE, ULONG, ULONG, PVOID, ULONG*, ULONG*);
typedef LONG (APIENTRY *FPPropertyReadFromRegistry)(HANDLE, ULONG, ULONG, PVOID, ULONG*);
typedef LONG (APIENTRY *FPPropertyWrite)(HANDLE, ULONG, ULONG, PVOID, ULONG);
typedef LONG (APIENTRY *FPPropertyWriteToRegistry)(HANDLE, ULONG, ULONG, PVOID);
typedef LONG (APIENTRY *FPGetLastError)(HANDLE, PWCHAR, ULONG, ULONG*);

// device routines
typedef LONG (APIENTRY *FPDeviceOpen)(LONG, ULONG, HANDLE*);
typedef LONG (APIENTRY *FPDeviceClose)(HANDLE);
typedef LONG (APIENTRY *FPDeviceReset)(HANDLE, ULONG);
typedef LONG (APIENTRY *FPDeviceGetFuncHandle)(HANDLE, ULONG, ULONG, HANDLE*);
typedef LONG (APIENTRY *FPDeviceRefresh)(HANDLE);
typedef LONG (APIENTRY *FPDeviceReadPorts)(HANDLE, ULONG, ULONG, PVOID);
typedef LONG (APIENTRY *FPDeviceWritePorts)(HANDLE, ULONG, ULONG, PVOID);
typedef LONG (APIENTRY *FPDeviceShowCfgDlgBox)(HANDLE, HWND, LPRECT, ULONG, ULONG, HWND*);

// AI routines
typedef LONG (APIENTRY *FPAiGetChanMapping)(HANDLE, ULONG, ULONG*);
typedef LONG (APIENTRY *FPAiReadSamples)(HANDLE, ULONG, ULONG, PVOID, double*);

typedef LONG (APIENTRY *FPBufferedAiPrepare)(HANDLE, ULONG, PVOID *);
typedef LONG (APIENTRY *FPBufferedAiRunOnce)(HANDLE, ULONG);
typedef LONG (APIENTRY *FPBufferedAiRun)(HANDLE);
typedef LONG (APIENTRY *FPBufferedAiStop)(HANDLE);
typedef LONG (APIENTRY *FPBufferedAiRelease)(HANDLE);
typedef LONG (APIENTRY *FPBufferedAiScaleData)(HANDLE, PVOID, double*, ULONG, ULONG*);

typedef LONG (APIENTRY *FPAiGetScaleTable)(HANDLE, ULONG, ULONG*, PBioMapFuncPiece);
typedef LONG (APIENTRY *FPAiSetScaleTable)(HANDLE, ULONG, ULONG, PBioMapFuncPiece);
typedef LONG (APIENTRY *FPAiGenerateNewVrg)(HANDLE, ULONG, ULONG, PBioMapFuncPiece, ULONG*);
typedef LONG (APIENTRY *FPAiGetCjcScaleTable)(HANDLE, ULONG*, PBioMapFuncPiece);
typedef LONG (APIENTRY *FPAiSetCjcScaleTable)(HANDLE, ULONG, PBioMapFuncPiece);

// AO routines
typedef LONG (APIENTRY *FPAoWriteSamples)(HANDLE, ULONG, ULONG, PVOID, double*);

typedef LONG (APIENTRY *FPBufferedAoPrepare)(HANDLE, ULONG, PVOID*);
typedef LONG (APIENTRY *FPBufferedAoRunOnce)(HANDLE, ULONG);
typedef LONG (APIENTRY *FPBufferedAoRun)(HANDLE);
typedef LONG (APIENTRY *FPBufferedAoStop)(HANDLE, ULONG);
typedef LONG (APIENTRY *FPBufferedAoRelease)(HANDLE);
typedef LONG (APIENTRY *FPBufferedAoScaleData)(HANDLE, double*, PVOID, ULONG, ULONG*);

typedef LONG (APIENTRY *FPAoGetScaleTable)(HANDLE, ULONG, ULONG*, PBioMapFuncPiece);
typedef LONG (APIENTRY *FPAoSetScaleTable)(HANDLE, ULONG, ULONG, PBioMapFuncPiece);
typedef LONG (APIENTRY *FPAoGenerateNewVrg)(HANDLE, ULONG, ULONG, PBioMapFuncPiece, ULONG*);

// DIO routines
typedef LONG (APIENTRY *FPDiReadPorts)(HANDLE, ULONG, ULONG, PBYTE);
typedef LONG (APIENTRY *FPDoWritePorts)(HANDLE, ULONG, ULONG, PBYTE);
typedef LONG (APIENTRY *FPDoReadBackPorts)(HANDLE, ULONG, ULONG, PBYTE);

typedef LONG (APIENTRY *FPBufferedDiPrepare)(HANDLE, ULONG, PVOID*);
typedef LONG (APIENTRY *FPBufferedDiRunOnce)(HANDLE, ULONG);
typedef LONG (APIENTRY *FPBufferedDiRun)(HANDLE);
typedef LONG (APIENTRY *FPBufferedDiStop)(HANDLE);
typedef LONG (APIENTRY *FPBufferedDiRelease)(HANDLE);

typedef LONG (APIENTRY *FPBufferedDoPrepare)(HANDLE, ULONG, PVOID*);
typedef LONG (APIENTRY *FPBufferedDoRunOnce)(HANDLE, ULONG);
typedef LONG (APIENTRY *FPBufferedDoRun)(HANDLE);
typedef LONG (APIENTRY *FPBufferedDoStop)(HANDLE, ULONG);
typedef LONG (APIENTRY *FPBufferedDoRelease)(HANDLE);

typedef LONG (APIENTRY *FPDiSnapStart)(HANDLE, ULONG, ULONG, ULONG, PBYTE*);
typedef LONG (APIENTRY *FPDiSnapStop)(HANDLE, ULONG);
typedef LONG (APIENTRY *FPDoReflectWdtLockStart)(HANDLE);
typedef LONG (APIENTRY *FPDoReflectWdtFeed)(HANDLE);
typedef LONG (APIENTRY *FPDoReflectWdtLockStop)(HANDLE);

// Counter routines
typedef LONG (APIENTRY *FPCounterReset)(HANDLE, ULONG, ULONG);
typedef LONG (APIENTRY *FPCounterRead)(HANDLE, ULONG, ULONG, ULONG*,ULONG*);
typedef LONG (APIENTRY *FPCounterStart)(HANDLE, ULONG, ULONG);
typedef LONG (APIENTRY *FPEventCountStart)(HANDLE, ULONG, ULONG);
typedef LONG (APIENTRY *FPEventCountRead)(HANDLE, ULONG, ULONG, ULONG*);
typedef LONG (APIENTRY *FPOneShotStart)(HANDLE, ULONG, ULONG);
typedef LONG (APIENTRY *FPTimerPulseStart)(HANDLE, ULONG, ULONG);
typedef LONG (APIENTRY *FPFreqMeasureStart)(HANDLE, ULONG, ULONG);
typedef LONG (APIENTRY *FPFreqMeasureRead)(HANDLE, ULONG, ULONG, double*);

//##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// V1.1, API prototype, read/write the H/W private region read/write
//##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef LONG (APIENTRY *FPDevicePrivateRegionRead)(HANDLE, ULONG, ULONG, PBYTE);
typedef LONG (APIENTRY *FPDevicePrivateRegionWrite)(HANDLE, ULONG, ULONG, PBYTE);

//##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// V1.2, API prototype, Pulse width measure and Pulse width modulation
//##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef LONG (APIENTRY *FPPwmInStart)(HANDLE, ULONG, ULONG);
typedef LONG (APIENTRY *FPPwmInRead)(HANDLE, ULONG, ULONG, double*, double*);
typedef LONG (APIENTRY *FPPwmOutStart)(HANDLE, ULONG, ULONG);

//##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// V1.3, API prototype, Up-down count
//##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef LONG (APIENTRY *FPUpDownCountStart)(HANDLE, ULONG, ULONG, ULONG);
typedef LONG (APIENTRY *FPUpDownCountValueReset)(HANDLE, ULONG, ULONG);
typedef LONG (APIENTRY *FPUpDownCountRead)(HANDLE, ULONG, ULONG, ULONG*);
typedef LONG (APIENTRY *FPCounterSnapStart)(HANDLE, ULONG, ULONG, ULONG, ULONG**, ULONG**);
typedef LONG (APIENTRY *FPCounterSnapStop)(HANDLE, ULONG);
typedef LONG (APIENTRY *FPCounterCompareSetTable)(HANDLE, ULONG, ULONG, ULONG*);
typedef LONG (APIENTRY *FPCounterCompareSetInterval)(HANDLE, ULONG, ULONG, ULONG, ULONG);
typedef LONG (APIENTRY *FPCounterCompareClear)(HANDLE, ULONG);

//##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//Function table definition.
//Note: 
//   the fields of this structure must not be removed or moved.
//   the new fields must be put at the tail of the structure.
//##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct tagBioDriverFunctionTable
{
   //##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
   // V1.0, initial version
   //##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
   // component common routines
   FPEventGetHandle              pfnEventGetHandle;
   FPEventGetLastStatus          pfnEventGetLastStatus;
   FPEventClearFlag              pfnEventClearFlag;
   FPPropertyRead                pfnPropertyRead;
   FPPropertyReadFromRegistry    pfnPropertyReadFromRegistry;
   FPPropertyWrite               pfnPropertyWrite;
   FPPropertyWriteToRegistry     pfnPropertyWriteToRegistry;
   FPGetLastError                pfnGetLastError;

   // device routines
   FPDeviceOpen                  pfnDeviceOpen;
   FPDeviceClose                 pfnDeviceClose;
   FPDeviceReset                 pfnDeviceReset;
   FPDeviceGetFuncHandle         pfnDeviceGetFuncHandle;
   FPDeviceRefresh               pfnDeviceRefresh;
   FPDeviceReadPorts             pfnDeviceReadPorts;
   FPDeviceWritePorts            pfnDeviceWritePorts;
   FPDeviceShowCfgDlgBox         pfnDeviceShowCfgDlgBox;

   // AI routines
   FPAiGetChanMapping		      pfnAiGetChanMapping;
   FPAiReadSamples					pfnAiReadSamples;
   FPBufferedAiPrepare           pfnBufferedAiPrepare;
   FPBufferedAiRunOnce				pfnBufferedAiRunOnce;
   FPBufferedAiRun               pfnBufferedAiRun;
   FPBufferedAiStop					pfnBufferedAiStop;
   FPBufferedAiRelease           pfnBufferedAiRelease;
   FPBufferedAiScaleData			pfnBufferedAiScaleData;
   FPAiGetScaleTable				   pfnAiGetScaleTable;
   FPAiSetScaleTable				   pfnAiSetScaleTable;
   FPAiGenerateNewVrg				pfnAiGenerateNewVrg;
   FPAiGetCjcScaleTable			   pfnAiGetCjcScaleTable;
   FPAiSetCjcScaleTable			   pfnAiSetCjcScaleTable;

   // AO routines
   FPAoWriteSamples					pfnAoWriteSamples;
   FPBufferedAoPrepare           pfnBufferedAoPrepare;
   FPBufferedAoRunOnce				pfnBufferedAoRunOnce;
   FPBufferedAoRun               pfnBufferedAoRun;
   FPBufferedAoStop					pfnBufferedAoStop;
   FPBufferedAoRelease           pfnBufferedAoRelease;
   FPBufferedAoScaleData			pfnBufferedAoScaleData;
   FPAoGetScaleTable					pfnAoGetScaleTable;
   FPAoSetScaleTable					pfnAoSetScaleTable;
   FPAoGenerateNewVrg				pfnAoGenerateNewVrg;

   // DIO routines
   FPDiReadPorts                 pfnDiReadPorts;
   FPDoWritePorts                pfnDoWritePorts;
   FPDoReadBackPorts             pfnDoReadBackPorts;
   FPBufferedDiPrepare           pfnBufferedDiPrepare;
   FPBufferedDiRunOnce           pfnBufferedDiRunOnce;
   FPBufferedDiRun               pfnBufferedDiRun;
   FPBufferedDiStop              pfnBufferedDiStop;
   FPBufferedDiRelease           pfnBufferedDiRelease;
   FPBufferedDoPrepare           pfnBufferedDoPrepare;
   FPBufferedDoRunOnce           pfnBufferedDoRunOnce;
   FPBufferedDoRun               pfnBufferedDoRun;
   FPBufferedDoStop              pfnBufferedDoStop;
   FPBufferedDoRelease           pfnBufferedDoRelease;
   FPDiSnapStart                 pfnDiSnapStart;
   FPDiSnapStop                  pfnDiSnapStop;
   FPDoReflectWdtLockStart       pfnDoReflectWdtLockStart;
   FPDoReflectWdtFeed            pfnDoReflectWdtFeed;
   FPDoReflectWdtLockStop        pfnDoReflectWdtLockStop;

   // Counter routines
   FPCounterReset                pfnCounterReset;
   FPCounterRead                 pfnCounterRead;
   FPCounterStart                pfnCounterStart;
   FPEventCountStart             pfnEventCountStart;
   FPEventCountRead              pfnEventCountRead;
   FPOneShotStart                pfnOneShotStart;
   FPTimerPulseStart             pfnTimerPulseStart;
   FPFreqMeasureStart            pfnFreqMeasureStart;
   FPFreqMeasureRead             pfnFreqMeasureRead;

   //##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
   // V1.1, read/write the H/W private region read/write
   //##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
   FPDevicePrivateRegionRead     pfnDevicePrivateRegionRead;
   FPDevicePrivateRegionWrite    pfnDevicePrivateRegionWrite;

   //##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
   // V1.2, API prototype, Pulse width measure and Pulse width modulation
   //##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
   FPPwmInStart                  pfnPwmInStart;
   FPPwmInRead                   pfnPwmInRead;
   FPPwmOutStart                 pfnPwmOutStart;

   //##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
   // V1.3, API prototype, Up-down count
   //##xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
   FPUpDownCountStart            pfnUpDownCountStart;
   FPUpDownCountValueReset       pfnUpDownCountValueReset;
   FPUpDownCountRead             pfnUpDownCountRead;
   FPCounterSnapStart            pfnCounterSnapStart;
   FPCounterSnapStop             pfnCounterSnapStop;
   FPCounterCompareSetTable      pfnCounterCompareSetTable;
   FPCounterCompareSetInterval   pfnCounterCompareSetInterval;
   FPCounterCompareClear         pfnCounterCompareClear;

} BioDriverFunctionTable, *PBioDriverFunctionTable;

#endif
