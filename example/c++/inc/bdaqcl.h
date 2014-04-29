#ifndef _ADVANTECH_BIONIC_DAQ_CLASS_LIBRARY
#define _ADVANTECH_BIONIC_DAQ_CLASS_LIBRARY

#include <dlfcn.h>

typedef char WCHAR;
typedef void* HMODULE;
typedef void* HANDLE;
typedef bool   BOOL;
typedef char   BYTE;
typedef int    int32;
typedef short  int16;
#define  IN
#define  OUT
#define  OPTIONAL

#define  M4_FIRM_WARE   0
#define  AI_FIRM_WARE   1

typedef void (*PROGRESS_CALLBACK)(int); //firmware download call back function

typedef struct _IO_MODULE_INFO  //IO module infor struct
{
   char  module_id;
   char  module_name[10];
}IO_MODULE_INFO;


#ifdef __cplusplus
#include <map>
#endif

#if !defined(_BDAQ_NO_NAMESPACE) && defined(__cplusplus)
namespace Automation { 
   namespace BDaq {  
#endif

// **********************************************************
// Bionic DAQ types
// **********************************************************
#ifndef _BDAQ_TYPES_DEFINED
#define _BDAQ_TYPES_DEFINED


typedef enum tagModuleType {
   DaqAny   = -1,
   DaqGroup = 1,
   DaqDevice,
   DaqAi,
   DaqAo,
   DaqDio,
} ModuleType;

typedef enum tagAccessMode {
   ModeRead = 0,
   ModeWrite,
   ModeWriteWithReset,
} AccessMode;

typedef struct tagMathInterval {
   long   Type; 
   double Min;
   double Max;
} MathInterval, * PMathInterval;

typedef enum tagAiChannelType {
   AllSingleEnded = 0,
   AllDifferential,
   AllSeDiffAdj,
   MixedSeDiffAdj,
} AiChannelType;

typedef enum tagAiCalibrationType{
   SaveCaliValToFlash = 0x01,
   WriteUserCaliValToFac,
   WriteFacCaliValToUser,
   ZeroCalibration = 0x55,
   SpanCalibration = 0xaa,
}AiCalibrationType;

typedef enum tagAiSignalType {
   SingleEnded = 0,
   Differential,
} AiSignalType;

typedef enum tagDioPortType {
   PortDi = 0,        // the port number references to a DI port
   PortDo,            // the port number references to a DO port
   PortDio,           // the port number references to a DI port and a DO port
   Port8255A,         // the port number references to a PPI port A mode DIO port.
   Port8255C,         // the port number references to a PPI port C mode DIO port.
   PortIndvdlDio,     // the port number references to a port whose each channel can be configured as in or out.
} DioPortType;

typedef enum tagDioPortDir {
   Input   = 0x00,
   LoutHin = 0x0F,
   LinHout = 0xF0,
   Output  = 0xFF,
} DioPortDir;

typedef enum tagValueRange {
   V_OMIT = -1,            /* Unknown when get, ignored when set */
   V_Neg15To15 = 0,        /* +/- 15 V  */
   V_Neg10To10,            /* +/- 10 V  */
   V_Neg5To5,              /* +/- 5 V */
   V_Neg2pt5To2pt5,        /* +/- 2.5 V */
   V_Neg1pt25To1pt25,      /* +/- 1.25 V */
   V_Neg1To1,              /* +/- 1 V */

   V_0To15,                /* 0~15 V  */
   V_0To10,                /* 0~10 V  */
   V_0To5,                 /* 0~5 V */
   V_0To2pt5,              /* 0~2.5 V */
   V_0To1pt25,             /* 0~1.25 V */
   V_0To1,                 /* 0~1 V */

   mV_Neg625To625,         /* +/- 625mV */
   mV_Neg500To500,         /* +/- 500 mV */
   mV_Neg312pt5To312pt5,   /* +/- 312.5 mV */
   mV_Neg200To200,         /* +/- 200 mV */
   mV_Neg150To150,         /* +/- 150 mV */
   mV_Neg100To100,         /* +/- 100 mV */
   mV_Neg50To50,           /* +/- 50 mV */
   mV_Neg30To30,           /* +/- 30 mV */
   mV_Neg20To20,           /* +/- 20 mV */
   mV_Neg15To15,           /* +/- 15 mV  */
   mV_Neg10To10,           /* +/- 10 mV */
   mV_Neg5To5,             /* +/- 5 mV */

   mV_0To625,              /* 0 ~ 625 mV */
   mV_0To500,              /* 0 ~ 500 mV */
   mV_0To150,              /* 0 ~ 150 mV */
   mV_0To100,              /* 0 ~ 100 mV */
   mV_0To50,               /* 0 ~ 50 mV */
   mV_0To20,               /* 0 ~ 20 mV */
   mV_0To15,               /* 0 ~ 15 mV */
   mV_0To10,               /* 0 ~ 10 mV */

   mA_Neg20To20,           /* +/- 20mA */
   mA_0To20,               /* 0 ~ 20 mA */
   mA_4To20,               /* 4 ~ 20 mA */
   mA_0To24,               /* 0 ~ 24 mA */

   /* For USB4702_4704 */
   V_Neg2To2,              /* +/- 2 V */
   V_Neg4To4,              /* +/- 4 V */
   V_Neg20To20,            /* +/- 20 V */

   Jtype_0To760C = 0x8000, /* T/C J type 0~760 'C */
   Ktype_0To1370C,		   /* T/C K type 0~1370 'C */
   Ttype_Neg100To400C,     /* T/C T type -100~400 'C */
   Etype_0To1000C,		   /* T/C E type 0~1000 'C */
   Rtype_500To1750C,	      /* T/C R type 500~1750 'C */
   Stype_500To1750C,	      /* T/C S type 500~1750 'C */
   Btype_500To1800C,	      /* T/C B type 500~1800 'C */

   Pt392_Neg50To150,	      /* Pt392 -50~150 'C  */
   Pt385_Neg200To200,	   /* Pt385 -200~200 'C */
   Pt385_0To400,		      /* Pt385 0~400 'C */
   Pt385_Neg50To150,	      /* Pt385 -50~150 'C */
   Pt385_Neg100To100,      /* Pt385 -100~100 'C */
   Pt385_0To100,		      /* Pt385 0~100 'C  */
   Pt385_0To200,		      /* Pt385 0~200 'C */
   Pt385_0To600,		      /* Pt385 0~600 'C */
   Pt392_Neg100To100,      /* Pt392 -100~100 'C  */
   Pt392_0To100,           /* Pt392 0~100 'C */
   Pt392_0To200,           /* Pt392 0~200 'C */
   Pt392_0To600,           /* Pt392 0~600 'C */
   Pt392_0To400,           /* Pt392 0~400 'C */
   Pt392_Neg200To200,      /* Pt392 -200~200 'C  */
   Pt1000_Neg40To160,      /* Pt1000 -40~160 'C  */

   Balcon500_Neg30To120,   /* Balcon500 -30~120 'C  */

   Ni518_Neg80To100,       /* Ni518 -80~100 'C */
   Ni518_0To100,           /* Ni518 0~100 'C */
   Ni508_0To100,           /* Ni508 0~100 'C */
   Ni508_Neg50To200,       /* Ni508 -50~200 'C */

   Thermistor_3K_0To100,   /* Thermistor 3K 0~100 'C */
   Thermistor_10K_0To100,  /* Thermistor 10K 0~100 'C */

   Jtype_Neg210To1200C,    /* T/C J type -210~1200 'C */
   Ktype_Neg270To1372C,    /* T/C K type -270~1372 'C */
   Ttype_Neg270To400C,     /* T/C T type -270~400 'C */
   Etype_Neg270To1000C,		/* T/C E type -270~1000 'C */
   Rtype_Neg50To1768C,	   /* T/C R type -50~1768 'C */
   Stype_Neg50To1768C,	   /* T/C S type -50~1768 'C */
   Btype_40To1820C,	      /* T/C B type 40~1820 'C */

   Jtype_Neg210To870C,     /* T/C J type -210~870 'C */
   Rtype_0To1768C,	      /* T/C R type 0~1768 'C */
   Stype_0To1768C,	      /* T/C S type 0~1768 'C */

   /* 0xC000 ~ 0xF000 : user customized value range type */
   UserCustomizedVrgStart = 0xC000,
   UserCustomizedVrgEnd = 0xF000,

   /* AO external reference type */
   V_ExternalRefBipolar = 0xF001, /* External reference voltage unipolar */
   V_ExternalRefUnipolar = 0xF002, /* External reference voltage bipolar */
} ValueRange;

typedef enum tagSignalDrop {
   SignalNone = 0,      /* No connection */

   /*Internal signal connector*/
   SigInternalClock,        /* Device built-in clock, If the device has several built-in clock, this represent the highest freq one. */
   SigInternal1KHz,         /* Device built-in clock, 1KHz */
   SigInternal10KHz,        /* Device built-in clock, 10KHz */
   SigInternal100KHz,       /* Device built-in clock, 100KHz */
   SigInternal1MHz,         /* Device built-in clock, 1MHz */
   SigInternal10MHz,        /* Device built-in clock, 10MHz */
   SigInternal20MHz,        /* Device built-in clock, 20MHz */
   SigInternal30MHz,        /* Device built-in clock, 30MHz */
   SigInternal40MHz,        /* Device built-in clock, 40MHz */
   SigInternal50MHz,        /* Device built-in clock, 50MHz */
   SigInternal60MHz,        /* Device built-in clock, 60MHz */

   SigDiPatternMatch,       /* When DI pattern match occurred */
   SigDiStatusChange,       /* When DI status change occurred */

   /*Function pin on connector*/
   SigExtAnaClock,          /* Analog clock pin of connector */
   SigExtAnaScanClock,      /* scan clock pin of connector */
   SigExtAnaTrigger,        /* external analog trigger pin of connector */
   SigExtDigClock,          /* digital clock pin of connector */
   SigExtDigTrigger0,       /* external digital trigger 0 pin( or DI start trigger pin) of connector */
   SigExtDigTrigger1,       /* external digital trigger 1 pin( or DI stop trigger pin) of connector  */
   SigExtDigTrigger2,       /* external digital trigger 2 pin( or DO start trigger pin) of connector */
   SigExtDigTrigger3,       /* external digital trigger 3 pin( or DO stop trigger pin) of connector  */
   SigCHFrzDo,              /* Channel freeze DO ports pin */

   /*Signal source or target on the connector*/
   /*AI channel pins*/
   SigAi0,  SigAi1,  SigAi2,  SigAi3,  SigAi4,  SigAi5,  SigAi6,  SigAi7, 
   SigAi8,  SigAi9,  SigAi10, SigAi11, SigAi12, SigAi13, SigAi14, SigAi15,
   SigAi16, SigAi17, SigAi18, SigAi19, SigAi20, SigAi21, SigAi22, SigAi23,
   SigAi24, SigAi25, SigAi26, SigAi27, SigAi28, SigAi29, SigAi30, SigAi31, 
   SigAi32, SigAi33, SigAi34, SigAi35, SigAi36, SigAi37, SigAi38, SigAi39,
   SigAi40, SigAi41, SigAi42, SigAi43, SigAi44, SigAi45, SigAi46, SigAi47,
   SigAi48, SigAi49, SigAi50, SigAi51, SigAi52, SigAi53, SigAi54, SigAi55, 
   SigAi56, SigAi57, SigAi58, SigAi59, SigAi60, SigAi61, SigAi62, SigAi63,

   /*AO channel pins*/
   SigAo0,  SigAo1,  SigAo2,  SigAo3,  SigAo4,  SigAo5,  SigAo6,  SigAo7,
   SigAo8,  SigAo9,  SigAo10, SigAo11, SigAo12, SigAo13, SigAo14, SigAo15,
   SigAo16, SigAo17, SigAo18, SigAo19, SigAo20, SigAo21, SigAo22, SigAo23,
   SigAo24, SigAo25, SigAo26, SigAo27, SigAo28, SigAo29, SigAo30, SigAo31,

   /*DI pins*/
   SigDi0,   SigDi1,   SigDi2,   SigDi3,   SigDi4,   SigDi5,   SigDi6,   SigDi7,
   SigDi8,   SigDi9,   SigDi10,  SigDi11,  SigDi12,  SigDi13,  SigDi14,  SigDi15,
   SigDi16,  SigDi17,  SigDi18,  SigDi19,  SigDi20,  SigDi21,  SigDi22,  SigDi23,
   SigDi24,  SigDi25,  SigDi26,  SigDi27,  SigDi28,  SigDi29,  SigDi30,  SigDi31,
   SigDi32,  SigDi33,  SigDi34,  SigDi35,  SigDi36,  SigDi37,  SigDi38,  SigDi39,
   SigDi40,  SigDi41,  SigDi42,  SigDi43,  SigDi44,  SigDi45,  SigDi46,  SigDi47,
   SigDi48,  SigDi49,  SigDi50,  SigDi51,  SigDi52,  SigDi53,  SigDi54,  SigDi55,
   SigDi56,  SigDi57,  SigDi58,  SigDi59,  SigDi60,  SigDi61,  SigDi62,  SigDi63,
   SigDi64,  SigDi65,  SigDi66,  SigDi67,  SigDi68,  SigDi69,  SigDi70,  SigDi71,
   SigDi72,  SigDi73,  SigDi74,  SigDi75,  SigDi76,  SigDi77,  SigDi78,  SigDi79,
   SigDi80,  SigDi81,  SigDi82,  SigDi83,  SigDi84,  SigDi85,  SigDi86,  SigDi87,
   SigDi88,  SigDi89,  SigDi90,  SigDi91,  SigDi92,  SigDi93,  SigDi94,  SigDi95,
   SigDi96,  SigDi97,  SigDi98,  SigDi99,  SigDi100, SigDi101, SigDi102, SigDi103,
   SigDi104, SigDi105, SigDi106, SigDi107, SigDi108, SigDi109, SigDi110, SigDi111,
   SigDi112, SigDi113, SigDi114, SigDi115, SigDi116, SigDi117, SigDi118, SigDi119,
   SigDi120, SigDi121, SigDi122, SigDi123, SigDi124, SigDi125, SigDi126, SigDi127,
   SigDi128, SigDi129, SigDi130, SigDi131, SigDi132, SigDi133, SigDi134, SigDi135,
   SigDi136, SigDi137, SigDi138, SigDi139, SigDi140, SigDi141, SigDi142, SigDi143,
   SigDi144, SigDi145, SigDi146, SigDi147, SigDi148, SigDi149, SigDi150, SigDi151,
   SigDi152, SigDi153, SigDi154, SigDi155, SigDi156, SigDi157, SigDi158, SigDi159,
   SigDi160, SigDi161, SigDi162, SigDi163, SigDi164, SigDi165, SigDi166, SigDi167,
   SigDi168, SigDi169, SigDi170, SigDi171, SigDi172, SigDi173, SigDi174, SigDi175,
   SigDi176, SigDi177, SigDi178, SigDi179, SigDi180, SigDi181, SigDi182, SigDi183,
   SigDi184, SigDi185, SigDi186, SigDi187, SigDi188, SigDi189, SigDi190, SigDi191,
   SigDi192, SigDi193, SigDi194, SigDi195, SigDi196, SigDi197, SigDi198, SigDi199,
   SigDi200, SigDi201, SigDi202, SigDi203, SigDi204, SigDi205, SigDi206, SigDi207,
   SigDi208, SigDi209, SigDi210, SigDi211, SigDi212, SigDi213, SigDi214, SigDi215,
   SigDi216, SigDi217, SigDi218, SigDi219, SigDi220, SigDi221, SigDi222, SigDi223,
   SigDi224, SigDi225, SigDi226, SigDi227, SigDi228, SigDi229, SigDi230, SigDi231,
   SigDi232, SigDi233, SigDi234, SigDi235, SigDi236, SigDi237, SigDi238, SigDi239,
   SigDi240, SigDi241, SigDi242, SigDi243, SigDi244, SigDi245, SigDi246, SigDi247,
   SigDi248, SigDi249, SigDi250, SigDi251, SigDi252, SigDi253, SigDi254, SigDi255,

   /*DIO pins*/
   SigDio0,   SigDio1,   SigDio2,   SigDio3,   SigDio4,   SigDio5,   SigDio6,   SigDio7,
   SigDio8,   SigDio9,   SigDio10,  SigDio11,  SigDio12,  SigDio13,  SigDio14,  SigDio15,
   SigDio16,  SigDio17,  SigDio18,  SigDio19,  SigDio20,  SigDio21,  SigDio22,  SigDio23,
   SigDio24,  SigDio25,  SigDio26,  SigDio27,  SigDio28,  SigDio29,  SigDio30,  SigDio31,
   SigDio32,  SigDio33,  SigDio34,  SigDio35,  SigDio36,  SigDio37,  SigDio38,  SigDio39,
   SigDio40,  SigDio41,  SigDio42,  SigDio43,  SigDio44,  SigDio45,  SigDio46,  SigDio47,
   SigDio48,  SigDio49,  SigDio50,  SigDio51,  SigDio52,  SigDio53,  SigDio54,  SigDio55,
   SigDio56,  SigDio57,  SigDio58,  SigDio59,  SigDio60,  SigDio61,  SigDio62,  SigDio63,
   SigDio64,  SigDio65,  SigDio66,  SigDio67,  SigDio68,  SigDio69,  SigDio70,  SigDio71,
   SigDio72,  SigDio73,  SigDio74,  SigDio75,  SigDio76,  SigDio77,  SigDio78,  SigDio79,
   SigDio80,  SigDio81,  SigDio82,  SigDio83,  SigDio84,  SigDio85,  SigDio86,  SigDio87,
   SigDio88,  SigDio89,  SigDio90,  SigDio91,  SigDio92,  SigDio93,  SigDio94,  SigDio95,
   SigDio96,  SigDio97,  SigDio98,  SigDio99,  SigDio100, SigDio101, SigDio102, SigDio103,
   SigDio104, SigDio105, SigDio106, SigDio107, SigDio108, SigDio109, SigDio110, SigDio111,
   SigDio112, SigDio113, SigDio114, SigDio115, SigDio116, SigDio117, SigDio118, SigDio119,
   SigDio120, SigDio121, SigDio122, SigDio123, SigDio124, SigDio125, SigDio126, SigDio127,
   SigDio128, SigDio129, SigDio130, SigDio131, SigDio132, SigDio133, SigDio134, SigDio135,
   SigDio136, SigDio137, SigDio138, SigDio139, SigDio140, SigDio141, SigDio142, SigDio143,
   SigDio144, SigDio145, SigDio146, SigDio147, SigDio148, SigDio149, SigDio150, SigDio151,
   SigDio152, SigDio153, SigDio154, SigDio155, SigDio156, SigDio157, SigDio158, SigDio159,
   SigDio160, SigDio161, SigDio162, SigDio163, SigDio164, SigDio165, SigDio166, SigDio167,
   SigDio168, SigDio169, SigDio170, SigDio171, SigDio172, SigDio173, SigDio174, SigDio175,
   SigDio176, SigDio177, SigDio178, SigDio179, SigDio180, SigDio181, SigDio182, SigDio183,
   SigDio184, SigDio185, SigDio186, SigDio187, SigDio188, SigDio189, SigDio190, SigDio191,
   SigDio192, SigDio193, SigDio194, SigDio195, SigDio196, SigDio197, SigDio198, SigDio199,
   SigDio200, SigDio201, SigDio202, SigDio203, SigDio204, SigDio205, SigDio206, SigDio207,
   SigDio208, SigDio209, SigDio210, SigDio211, SigDio212, SigDio213, SigDio214, SigDio215,
   SigDio216, SigDio217, SigDio218, SigDio219, SigDio220, SigDio221, SigDio222, SigDio223,
   SigDio224, SigDio225, SigDio226, SigDio227, SigDio228, SigDio229, SigDio230, SigDio231,
   SigDio232, SigDio233, SigDio234, SigDio235, SigDio236, SigDio237, SigDio238, SigDio239,
   SigDio240, SigDio241, SigDio242, SigDio243, SigDio244, SigDio245, SigDio246, SigDio247,
   SigDio248, SigDio249, SigDio250, SigDio251, SigDio252, SigDio253, SigDio254, SigDio255,

   /*new clocks*/
   SigInternal2Hz,         /* Device built-in clock, 2Hz */
   SigInternal20Hz,        /* Device built-in clock, 20Hz */
   SigInternal200Hz,       /* Device built-in clock, 200KHz */
   SigInternal2KHz,        /* Device built-in clock, 2KHz */
   SigInternal20KHz,       /* Device built-in clock, 20KHz */
   SigInternal200KHz,      /* Device built-in clock, 200KHz */
   SigInternal2MHz,        /* Device built-in clock, 2MHz */
} SignalDrop;

typedef enum tagEventId {
   EvtDeviceRemoved = 0,  /* The device was removed from system */
   EvtDeviceReconnected,  /* The device is reconnected */
   EvtPropertyChanged,    /* Some properties of the device were changed */
   /*-----------------------------------------------------------------
   * AI events
   *-----------------------------------------------------------------*/
   EvtBufferedAiDataReady,
   EvtBufferedAiOverrun,
   EvtBufferedAiCacheOverflow,
   EvtBufferedAiStopped,

   /*-----------------------------------------------------------------
   * AO event IDs
   *-----------------------------------------------------------------*/
   EvtBufferedAoDataTransmitted,
   EvtBufferedAoUnderrun,
   EvtBufferedAoCacheEmptied,
   EvtBufferedAoTransStopped,
   EvtBufferedAoStopped,

   /*-----------------------------------------------------------------
   * DIO event IDs
   *-----------------------------------------------------------------*/
   EvtDiintChannel000, EvtDiintChannel001, EvtDiintChannel002, EvtDiintChannel003,
   EvtDiintChannel004, EvtDiintChannel005, EvtDiintChannel006, EvtDiintChannel007,
   EvtDiintChannel008, EvtDiintChannel009, EvtDiintChannel010, EvtDiintChannel011,
   EvtDiintChannel012, EvtDiintChannel013, EvtDiintChannel014, EvtDiintChannel015,
   EvtDiintChannel016, EvtDiintChannel017, EvtDiintChannel018, EvtDiintChannel019,
   EvtDiintChannel020, EvtDiintChannel021, EvtDiintChannel022, EvtDiintChannel023,
   EvtDiintChannel024, EvtDiintChannel025, EvtDiintChannel026, EvtDiintChannel027,
   EvtDiintChannel028, EvtDiintChannel029, EvtDiintChannel030, EvtDiintChannel031,
   EvtDiintChannel032, EvtDiintChannel033, EvtDiintChannel034, EvtDiintChannel035,
   EvtDiintChannel036, EvtDiintChannel037, EvtDiintChannel038, EvtDiintChannel039,
   EvtDiintChannel040, EvtDiintChannel041, EvtDiintChannel042, EvtDiintChannel043,
   EvtDiintChannel044, EvtDiintChannel045, EvtDiintChannel046, EvtDiintChannel047,
   EvtDiintChannel048, EvtDiintChannel049, EvtDiintChannel050, EvtDiintChannel051,
   EvtDiintChannel052, EvtDiintChannel053, EvtDiintChannel054, EvtDiintChannel055,
   EvtDiintChannel056, EvtDiintChannel057, EvtDiintChannel058, EvtDiintChannel059,
   EvtDiintChannel060, EvtDiintChannel061, EvtDiintChannel062, EvtDiintChannel063,
   EvtDiintChannel064, EvtDiintChannel065, EvtDiintChannel066, EvtDiintChannel067,
   EvtDiintChannel068, EvtDiintChannel069, EvtDiintChannel070, EvtDiintChannel071,
   EvtDiintChannel072, EvtDiintChannel073, EvtDiintChannel074, EvtDiintChannel075,
   EvtDiintChannel076, EvtDiintChannel077, EvtDiintChannel078, EvtDiintChannel079,
   EvtDiintChannel080, EvtDiintChannel081, EvtDiintChannel082, EvtDiintChannel083,
   EvtDiintChannel084, EvtDiintChannel085, EvtDiintChannel086, EvtDiintChannel087,
   EvtDiintChannel088, EvtDiintChannel089, EvtDiintChannel090, EvtDiintChannel091,
   EvtDiintChannel092, EvtDiintChannel093, EvtDiintChannel094, EvtDiintChannel095,
   EvtDiintChannel096, EvtDiintChannel097, EvtDiintChannel098, EvtDiintChannel099,
   EvtDiintChannel100, EvtDiintChannel101, EvtDiintChannel102, EvtDiintChannel103,
   EvtDiintChannel104, EvtDiintChannel105, EvtDiintChannel106, EvtDiintChannel107,
   EvtDiintChannel108, EvtDiintChannel109, EvtDiintChannel110, EvtDiintChannel111,
   EvtDiintChannel112, EvtDiintChannel113, EvtDiintChannel114, EvtDiintChannel115,
   EvtDiintChannel116, EvtDiintChannel117, EvtDiintChannel118, EvtDiintChannel119,
   EvtDiintChannel120, EvtDiintChannel121, EvtDiintChannel122, EvtDiintChannel123,
   EvtDiintChannel124, EvtDiintChannel125, EvtDiintChannel126, EvtDiintChannel127,
   EvtDiintChannel128, EvtDiintChannel129, EvtDiintChannel130, EvtDiintChannel131,
   EvtDiintChannel132, EvtDiintChannel133, EvtDiintChannel134, EvtDiintChannel135,
   EvtDiintChannel136, EvtDiintChannel137, EvtDiintChannel138, EvtDiintChannel139,
   EvtDiintChannel140, EvtDiintChannel141, EvtDiintChannel142, EvtDiintChannel143,
   EvtDiintChannel144, EvtDiintChannel145, EvtDiintChannel146, EvtDiintChannel147,
   EvtDiintChannel148, EvtDiintChannel149, EvtDiintChannel150, EvtDiintChannel151,
   EvtDiintChannel152, EvtDiintChannel153, EvtDiintChannel154, EvtDiintChannel155,
   EvtDiintChannel156, EvtDiintChannel157, EvtDiintChannel158, EvtDiintChannel159,
   EvtDiintChannel160, EvtDiintChannel161, EvtDiintChannel162, EvtDiintChannel163,
   EvtDiintChannel164, EvtDiintChannel165, EvtDiintChannel166, EvtDiintChannel167,
   EvtDiintChannel168, EvtDiintChannel169, EvtDiintChannel170, EvtDiintChannel171,
   EvtDiintChannel172, EvtDiintChannel173, EvtDiintChannel174, EvtDiintChannel175,
   EvtDiintChannel176, EvtDiintChannel177, EvtDiintChannel178, EvtDiintChannel179,
   EvtDiintChannel180, EvtDiintChannel181, EvtDiintChannel182, EvtDiintChannel183,
   EvtDiintChannel184, EvtDiintChannel185, EvtDiintChannel186, EvtDiintChannel187,
   EvtDiintChannel188, EvtDiintChannel189, EvtDiintChannel190, EvtDiintChannel191,
   EvtDiintChannel192, EvtDiintChannel193, EvtDiintChannel194, EvtDiintChannel195,
   EvtDiintChannel196, EvtDiintChannel197, EvtDiintChannel198, EvtDiintChannel199,
   EvtDiintChannel200, EvtDiintChannel201, EvtDiintChannel202, EvtDiintChannel203,
   EvtDiintChannel204, EvtDiintChannel205, EvtDiintChannel206, EvtDiintChannel207,
   EvtDiintChannel208, EvtDiintChannel209, EvtDiintChannel210, EvtDiintChannel211,
   EvtDiintChannel212, EvtDiintChannel213, EvtDiintChannel214, EvtDiintChannel215,
   EvtDiintChannel216, EvtDiintChannel217, EvtDiintChannel218, EvtDiintChannel219,
   EvtDiintChannel220, EvtDiintChannel221, EvtDiintChannel222, EvtDiintChannel223,
   EvtDiintChannel224, EvtDiintChannel225, EvtDiintChannel226, EvtDiintChannel227,
   EvtDiintChannel228, EvtDiintChannel229, EvtDiintChannel230, EvtDiintChannel231,
   EvtDiintChannel232, EvtDiintChannel233, EvtDiintChannel234, EvtDiintChannel235,
   EvtDiintChannel236, EvtDiintChannel237, EvtDiintChannel238, EvtDiintChannel239,
   EvtDiintChannel240, EvtDiintChannel241, EvtDiintChannel242, EvtDiintChannel243,
   EvtDiintChannel244, EvtDiintChannel245, EvtDiintChannel246, EvtDiintChannel247,
   EvtDiintChannel248, EvtDiintChannel249, EvtDiintChannel250, EvtDiintChannel251,
   EvtDiintChannel252, EvtDiintChannel253, EvtDiintChannel254, EvtDiintChannel255,

   EvtDiCosintPort000, EvtDiCosintPort001, EvtDiCosintPort002, EvtDiCosintPort003,
   EvtDiCosintPort004, EvtDiCosintPort005, EvtDiCosintPort006, EvtDiCosintPort007,
   EvtDiCosintPort008, EvtDiCosintPort009, EvtDiCosintPort010, EvtDiCosintPort011,
   EvtDiCosintPort012, EvtDiCosintPort013, EvtDiCosintPort014, EvtDiCosintPort015,
   EvtDiCosintPort016, EvtDiCosintPort017, EvtDiCosintPort018, EvtDiCosintPort019,
   EvtDiCosintPort020, EvtDiCosintPort021, EvtDiCosintPort022, EvtDiCosintPort023,
   EvtDiCosintPort024, EvtDiCosintPort025, EvtDiCosintPort026, EvtDiCosintPort027,
   EvtDiCosintPort028, EvtDiCosintPort029, EvtDiCosintPort030, EvtDiCosintPort031,

   EvtDiPmintPort000,  EvtDiPmintPort001,  EvtDiPmintPort002,  EvtDiPmintPort003,
   EvtDiPmintPort004,  EvtDiPmintPort005,  EvtDiPmintPort006,  EvtDiPmintPort007,
   EvtDiPmintPort008,  EvtDiPmintPort009,  EvtDiPmintPort010,  EvtDiPmintPort011,
   EvtDiPmintPort012,  EvtDiPmintPort013,  EvtDiPmintPort014,  EvtDiPmintPort015,
   EvtDiPmintPort016,  EvtDiPmintPort017,  EvtDiPmintPort018,  EvtDiPmintPort019,
   EvtDiPmintPort020,  EvtDiPmintPort021,  EvtDiPmintPort022,  EvtDiPmintPort023,
   EvtDiPmintPort024,  EvtDiPmintPort025,  EvtDiPmintPort026,  EvtDiPmintPort027,
   EvtDiPmintPort028,  EvtDiPmintPort029,  EvtDiPmintPort030,  EvtDiPmintPort031,

   EvtBufferedDiDataReady,
   EvtBufferedDiOverrun,
   EvtBufferedDiCacheOverflow,
   EvtBufferedDiStopped,

   EvtBufferedDoDataTransmitted,
   EvtBufferedDoUnderrun,
   EvtBufferedDoCacheEmptied,
   EvtBufferedDoTransStopped,
   EvtBufferedDoStopped,

   EvtReflectWdtOccured,

} EventId ;

typedef enum tagPropertyAttribute {
   ReadOnly = 0,
   Writable = 1,
   Modal = 0,
   Nature = 2,
} PropertyAttribute;

typedef enum tagPropertyId {
   /*-----------------------------------------------------------------
   * common property
   *-----------------------------------------------------------------*/
   CFG_Number,
   CFG_ComponentType,
   CFG_Description,
   CFG_Parent,
   CFG_ChildList,

   /*-----------------------------------------------------------------
   * component specified Property IDs -- group
   *-----------------------------------------------------------------*/
   CFG_DevicesNumber,
   CFG_DevicesHandle,

   /*-----------------------------------------------------------------
   * component specified Property IDs -- device
   *-----------------------------------------------------------------*/
   CFG_DeviceGroupNumber,
   CFG_DeviceProductID,
   CFG_DeviceBoardID,
   CFG_DeviceBoardVersion,
   CFG_DeviceDriverVersion,
   CFG_DeviceDllVersion,
   CFG_DeviceLocation,                       /* Reserved for later using */
   CFG_DeviceBaseAddresses,                  /* Reserved for later using */
   CFG_DeviceInterrupts,                     /* Reserved for later using */
   CFG_DeviceSupportedTerminalBoardTypes,    /* Reserved for later using */
   CFG_DeviceTerminalBoardType,              /* Reserved for later using */
   CFG_DeviceSupportedEvents,
   CFG_DeviceHotResetPreventable,            /* Reserved for later using */
   CFG_DeviceLoadingTimeInit,                /* Reserved for later using */
   CFG_DeviceWaitingForReconnect,
   CFG_DeviceWaitingForSleep,

   /*-----------------------------------------------------------------
   * component specified Property IDs -- AI, AO...
   *-----------------------------------------------------------------*/
   CFG_FeatureResolutionInBit,
   CFG_FeatureDataSize,
   CFG_FeatureDataMask,
   CFG_FeatureChannelNumberMax,
   CFG_FeatureChannelConnectionType,
   CFG_FeatureBurnDetectedReturnTypes,
   CFG_FeatureBurnoutDetectionChannels,
   CFG_FeatureOverallVrgType,
   CFG_FeatureVrgTypes,
   CFG_FeatureExtRefRange,
   CFG_FeatureExtRefAntiPolar,
   CFG_FeatureCjcChannels,
   CFG_FeatureChannelScanMethod,
   CFG_FeatureScanChannelStartBase,
   CFG_FeatureScanChannelCountBase,
   CFG_FeatureConvertClockSources,
   CFG_FeatureConvertClockRateRange,       /* Reserved for later using */
   CFG_FeatureScanClockSources,
   CFG_FeatureScanClockRateRange,         /* Reserved for later using */
   CFG_FeatureScanCountMax,               /* Reserved for later using */
   CFG_FeatureTriggersCount,
   CFG_FeatureTriggerSources,
   CFG_FeatureTriggerActions,
   CFG_FeatureTriggerDelayCountRange,
   CFG_FeatureTriggerSources1,            /* Reserved for later using */
   CFG_FeatureTriggerActions1,            /* Reserved for later using */
   CFG_FeatureTriggerDelayCountRange1,    /* Reserved for later using */

   CFG_ChannelCount,
   CFG_ConnectionTypeOfChannels,
   CFG_VrgTypeOfChannels,
   CFG_BurnDetectedReturnTypeOfChannels,
   CFG_BurnoutReturnValueOfChannels,
   CFG_ExtRefValueForUnipolar,         /* Reserved for later using */
   CFG_ExtRefValueForBipolar,          /* Reserved for later using */

   CFG_CjcChannel,
   CFG_CjcUpdateFrequency,             /* Reserved for later using */
   CFG_CjcValue,

   CFG_SectionDataCount,
   CFG_ConvertClockSource,
   CFG_ConvertClockRatePerChannel,
   CFG_ScanChannelStart,
   CFG_ScanChannelCount,
   CFG_ScanClockSource,                /* Reserved for later using */
   CFG_ScanClockRate,                  /* Reserved for later using */
   CFG_ScanCount,                      /* Reserved for later using */
   CFG_TriggerSource,
   CFG_TriggerSourceEdge,
   CFG_TriggerSourceLevel,
   CFG_TriggerDelayCount,
   CFG_TriggerAction,
   CFG_TriggerSource1,                 /* Reserved for later using */
   CFG_TriggerSourceEdge1,             /* Reserved for later using */
   CFG_TriggerSourceLevel1,            /* Reserved for later using */
   CFG_TriggerDelayCount1,             /* Reserved for later using */
   CFG_TriggerAction1,                 /* Reserved for later using */
   CFG_ParentSignalConnectionChannel,
   CFG_ParentCjcConnectionChannel,
   CFG_ParentControlPort,

   /*-----------------------------------------------------------------
   * component specified Property IDs -- DIO
   *-----------------------------------------------------------------*/
   CFG_FeaturePortsCount,
   CFG_FeaturePortsType,
   CFG_FeatureDiNoiseFilterOfChannels,
   CFG_FeatureDiNoiseFilterBlockTimeRange,     /* Reserved for later using */
   CFG_FeatureDiintTriggerEdges,
   CFG_FeatureDiintOfChannels,
   CFG_FeatureDiintGateOfChannels,
   CFG_FeatureDiCosintOfChannels,
   CFG_FeatureDiPmintOfChannels,
   CFG_FeatureDiSnapEventSources,
   CFG_FeatureDoFreezeSignalSources,            /* Reserved for later using */
   CFG_FeatureDoReflectWdtFeedIntervalRange,    /* Reserved for later using */

   CFG_FeatureDiPortScanMethod,                 /* Reserved for later using */
   CFG_FeatureDiConvertClockSources,            /* Reserved for later using */
   CFG_FeatureDiConvertClockRateRange,          /* Reserved for later using */
   CFG_FeatureDiScanClockSources,
   CFG_FeatureDiScanClockRateRange,             /* Reserved for later using */
   CFG_FeatureDiScanCountMax,
   CFG_FeatureDiTriggersCount,
   CFG_FeatureDiTriggerSources,
   CFG_FeatureDiTriggerActions,
   CFG_FeatureDiTriggerDelayCountRange,
   CFG_FeatureDiTriggerSources1,
   CFG_FeatureDiTriggerActions1,
   CFG_FeatureDiTriggerDelayCountRange1,

   CFG_FeatureDoPortScanMethod,                 /* Reserved for later using */
   CFG_FeatureDoConvertClockSources,            /* Reserved for later using */
   CFG_FeatureDoConvertClockRateRange,          /* Reserved for later using */
   CFG_FeatureDoScanClockSources,
   CFG_FeatureDoScanClockRateRange,             /* Reserved for later using */
   CFG_FeatureDoScanCountMax,
   CFG_FeatureDoTriggersCount,
   CFG_FeatureDoTriggerSources,
   CFG_FeatureDoTriggerActions,
   CFG_FeatureDoTriggerDelayCountRange,
   CFG_FeatureDoTriggerSources1,
   CFG_FeatureDoTriggerActions1,
   CFG_FeatureDoTriggerDelayCountRange1,

   CFG_DirectionOfPorts,
   CFG_DiDataMaskOfPorts,
   CFG_DoDataMaskOfPorts,

   CFG_DiNoiseFilterOverallBlockTime,              /* Reserved for later using */
   CFG_DiNoiseFilterEnabledChannels,
   CFG_DiintTriggerEdgeOfChannels,
   CFG_DiintGateEnabledChannels,
   CFG_DiCosintEnabledChannels,
   CFG_DiPmintEnabledChannels,
   CFG_DiPmintValueOfPorts,
   CFG_DoInitialStateOfPorts,                   /* Reserved for later using */
   CFG_DoFreezeEnabled,                         /* Reserved for later using */
   CFG_DoFreezeSignalState,                     /* Reserved for later using */
   CFG_DoReflectWdtFeedInterval,                /* Reserved for later using */
   CFG_DoReflectWdtLockValue,                   /* Reserved for later using */
   CFG_DiSectionDataCount,
   CFG_DiConvertClockSource,
   CFG_DiConvertClockRatePerPort,
   CFG_DiScanPortStart,
   CFG_DiScanPortCount,
   CFG_DiScanClockSource,
   CFG_DiScanClockRate,
   CFG_DiScanCount,
   CFG_DiTriggerAction,
   CFG_DiTriggerSource,
   CFG_DiTriggerSourceEdge,
   CFG_DiTriggerSourceLevel,                    /* Reserved for later using */
   CFG_DiTriggerDelayCount,
   CFG_DiTriggerAction1,
   CFG_DiTriggerSource1,
   CFG_DiTriggerSourceEdge1,
   CFG_DiTriggerSourceLevel1,                   /* Reserved for later using */
   CFG_DiTriggerDelayCount1,

   CFG_DoSectionDataCount,
   CFG_DoConvertClockSource,
   CFG_DoConvertClockRatePerPort,
   CFG_DoScanPortStart,
   CFG_DoScanPortCount,
   CFG_DoScanClockSource,
   CFG_DoScanClockRate,
   CFG_DoScanCount,
   CFG_DoTriggerAction,
   CFG_DoTriggerSource,
   CFG_DoTriggerSourceEdge,
   CFG_DoTriggerSourceLevel,                    /* Reserved for later using */
   CFG_DoTriggerDelayCount,
   CFG_DoTriggerAction1,
   CFG_DoTriggerSource1,
   CFG_DoTriggerSourceEdge1,
   CFG_DoTriggerSourceLevel1,                   /* Reserved for later using */
   CFG_DoTriggerDelayCount1,
} PropertyId;

#define BioFailed(c)       ( (unsigned)(c) >= (unsigned)0xE0000000 )   
typedef enum tagErrorCode {
   /// <summary>
   /// The operation is completed successfully. 
   /// </summary>
   Success = 0, 

   ///************************************************************************
   /// warning                                                              
   ///************************************************************************
   /// <summary>
   /// The interrupt resource is not available. 
   /// </summary>
   WarningIntrNotAvailable = 0xA0000000,

   /// <summary>
   /// The parameter is out of the range. 
   /// </summary>
   WarningParamOutOfRange = 0xA0000001,

   /// <summary>
   /// The property value is out of range. 
   /// </summary>
   WarningPropValueOutOfRange = 0xA0000002,

   /// <summary>
   /// The property value is not supported. 
   /// </summary>
   WarningPropValueNotSpted = 0xA0000003,

   /// <summary>
   /// The property value conflicts with the current state.
   /// </summary>
   WarningPropValueConflict = 0xA0000004,

   ///***********************************************************************
   /// error                                                                
   ///***********************************************************************
   /// <summary>
   /// The handle is NULL or its type doesn't match the required operation. 
   /// </summary>
   ErrorHandleNotValid = 0xE0000000,

   /// <summary>
   /// The parameter value is out of range.
   /// </summary>
   ErrorParamOutOfRange = 0xE0000001,

   /// <summary>
   /// The parameter value is not supported.
   /// </summary>
   ErrorParamNotSpted = 0xE0000002,

   /// <summary>
   /// The parameter value format is not the expected. 
   /// </summary>
   ErrorParamFmtUnexpted = 0xE0000003,

   /// <summary>
   /// Not enough memory is available to complete the operation. 
   /// </summary>
   ErrorMemoryNotEnough = 0xE0000004,

   /// <summary>
   /// The data buffer is null. 
   /// </summary>
   ErrorBufferIsNull = 0xE0000005,

   /// <summary>
   /// The data buffer is too small for the operation. 
   /// </summary>
   ErrorBufferTooSmall = 0xE0000006,

   /// <summary>
   /// The data length exceeded the limitation. 
   /// </summary>
   ErrorDataLenExceedLimit = 0xE0000007,

   /// <summary>
   /// The required function is not supported. 
   /// </summary>
   ErrorFuncNotSpted = 0xE0000008,

   /// <summary>
   /// The required event is not supported. 
   /// </summary>
   ErrorEventNotSpted = 0xE0000009,

   /// <summary>
   /// The required property is not supported. 
   /// </summary>
   ErrorPropNotSpted = 0xE000000A, 

   /// <summary>
   /// The required property is read-only. 
   /// </summary>
   ErrorPropReadOnly = 0xE000000B,

   /// <summary>
   /// The specified property value conflicts with the current state.
   /// </summary>
   ErrorPropValueConflict = 0xE000000C,

   /// <summary>
   /// The specified property value is out of range.
   /// </summary>
   ErrorPropValueOutOfRange = 0xE000000D,

   /// <summary>
   /// The specified property value is not supported. 
   /// </summary>
   ErrorPropValueNotSpted = 0xE000000E,

   /// <summary>
   /// The handle hasn't own the privilege of the operation the user wanted. 
   /// </summary>
   ErrorPrivilegeNotHeld = 0xE000000F,

   /// <summary>
   /// The required privilege is not available because someone else had own it. 
   /// </summary>
   ErrorPrivilegeNotAvailable = 0xE0000010,

   /// <summary>
   /// The driver of specified device was not found. 
   /// </summary>
   ErrorDriverNotFound = 0xE0000011,

   /// <summary>
   /// The driver version of the specified device mismatched. 
   /// </summary>
   ErrorDriverVerMismatch = 0xE0000012,

   /// <summary>
   /// The loaded driver count exceeded the limitation. 
   /// </summary>
   ErrorDriverCountExceedLimit = 0xE0000013,

   /// <summary>
   /// The device is not opened. 
   /// </summary>
   ErrorDeviceNotOpened = 0xE0000014,      

   /// <summary>
   /// The required device does not exist. 
   /// </summary>
   ErrorDeviceNotExist = 0xE0000015,

   /// <summary>
   /// The required device is unrecognized by driver. 
   /// </summary>
   ErrorDeviceUnrecognized = 0xE0000016,

   /// <summary>
   /// The configuration data of the specified device is lost or unavailable. 
   /// </summary>
   ErrorConfigDataLost = 0xE0000017,

   /// <summary>
   /// The function is not initialized and can't be started. 
   /// </summary>
   ErrorFuncNotInited = 0xE0000018,

   /// <summary>
   /// The function is busy. 
   /// </summary>
   ErrorFuncBusy = 0xE0000019,

   /// <summary>
   /// The interrupt resource is not available. 
   /// </summary>
   ErrorIntrNotAvailable = 0xE000001A,

   /// <summary>
   /// The DMA channel is not available. 
   /// </summary>
   ErrorDmaNotAvailable = 0xE000001B,

   /// <summary>
   /// Time out when reading/writing the device. 
   /// </summary>
   ErrorDeviceIoTimeOut = 0xE000001C,

   /// <summary>
   /// The given signature does not match with the device current one.
   /// </summary>
   ErrorSignatureNotMatch = 0xE000001D,

   /// <summary>
   /// Undefined error 
   /// </summary>
   ErrorUndefined = 0xE000FFFF,
} ErrorCode;

// Advantech CardType ID 
typedef enum tagProductId {
   BD_DEMO   = 0x00,		// demo board
} ProductId;

#endif // _BDAQ_TYPES_DEFINED

// **********************************************************
// Bionic DAQ APIs
// **********************************************************
#define BDAQ_DEVICE_DESC_MAX_LEN          64
#define BDAQ_VALUE_RANGE_DESC_MAX_LEN     256
#define BDAQ_SIGNAL_DROP_DESC_MAX_LEN     256

struct BDaqLib
{
   HMODULE   instHandle;
   // global APIs
   ErrorCode ( *pfnGetValueRangeInformation)(long,long,WCHAR*,MathInterval*,long*);
   ErrorCode ( *pfnGetSignalConnectionInformation)(long,long,WCHAR*,long*);
   double    ( *pfnTranslateTemperatureScale)(long,double);
   ErrorCode ( *pfnDeviceGetLinkageInfo)(long,long,long*,WCHAR*,long*);
   // event APIs
   ErrorCode ( *pfnEventGetHandle)(HANDLE,long,HANDLE*);
   ErrorCode ( *pfnEventGetLastStatus)(HANDLE,long,long*,long*);
   ErrorCode ( *pfnEventClearFlag)(HANDLE,long,long,long);
   // property APIs
   ErrorCode ( *pfnPropertyRead)(HANDLE,long,long,void*,long*,long*);
   ErrorCode ( *pfnPropertyWrite)(HANDLE,long,long,void*,long);
   // device APIs
   ErrorCode ( *pfnDeviceOpen)(long,long,HANDLE*);
   ErrorCode ( *pfnDeviceClose)(HANDLE);
   ErrorCode ( *pfnDeviceReset)(HANDLE,long);
   ErrorCode ( *pfnDeviceGetModuleHandle)(HANDLE,long,long,HANDLE*);
   ErrorCode ( *pfnDeviceRefreshProperties)(HANDLE);
   ErrorCode ( *pfnDeviceFirmwareUpdate)(HANDLE, long, FILE*, long, PROGRESS_CALLBACK);
   ErrorCode ( *pfnDeviceIoModuleSearch)(HANDLE, IO_MODULE_INFO*, long, long*);
   // Analog input APIs
   ErrorCode ( *pfnAiReadSamples)(HANDLE,long,long,long,void*,double*);
   ErrorCode ( *pfnAiAccessValueRange)(HANDLE,long,long,long,void*,long);
   ErrorCode ( *pfnAiCalibrate)(HANDLE,long,long);
   ErrorCode ( *pfnBufferedAiPrepare)(HANDLE,long,void**);
   ErrorCode ( *pfnBufferedAiRunOnce)(HANDLE,BOOL);
   ErrorCode ( *pfnBufferedAiRun)(HANDLE);
   ErrorCode ( *pfnBufferedAiStop)(HANDLE);
   ErrorCode ( *pfnBufferedAiScaleData)(HANDLE,void*,double*,long,long*);
   ErrorCode ( *pfnBufferedAiRelease)(HANDLE);
   // Analog output APIs
   ErrorCode ( *pfnAoWriteSamples)(HANDLE,long,long,long,void*,double*);
   ErrorCode ( *pfnAoAccessValueRange)(HANDLE,long,long,long,void*,long);
   ErrorCode ( *pfnBufferedAoPrepare)(HANDLE,long,void**);
   ErrorCode ( *pfnBufferedAoRunOnce)(HANDLE,BOOL);
   ErrorCode ( *pfnBufferedAoRun)(HANDLE);
   ErrorCode ( *pfnBufferedAoStop)(HANDLE,long);
   ErrorCode ( *pfnBufferedAoScaleData)(HANDLE,double*,void*,long,long*);
   ErrorCode ( *pfnBufferedAoRelease)(HANDLE);
   // Digital input/output APIs
   ErrorCode ( *pfnDiReadPorts)(HANDLE,long,long,long,BYTE*);
   ErrorCode ( *pfnDoWritePorts)(HANDLE,long,long,long,BYTE*);
   ErrorCode ( *pfnDoReadBackPorts)(HANDLE,long,long,long,BYTE*);
   ErrorCode ( *pfnDiSnapStart)(HANDLE,long,long,long,BYTE**);
   ErrorCode ( *pfnDiSnapStop)(HANDLE,long);
   ErrorCode ( *pfnBufferedDiPrepare)(HANDLE,long,void**);
   ErrorCode ( *pfnBufferedDiRunOnce)(HANDLE,BOOL);
   ErrorCode ( *pfnBufferedDiRun)(HANDLE);
   ErrorCode ( *pfnBufferedDiStop)(HANDLE);
   ErrorCode ( *pfnBufferedDiRelease)(HANDLE);
   ErrorCode ( *pfnBufferedDoPrepare)(HANDLE,long,void**);
   ErrorCode ( *pfnBufferedDoRunOnce)(HANDLE,BOOL);
   ErrorCode ( *pfnBufferedDoRun)(HANDLE);
   ErrorCode ( *pfnBufferedDoStop)(HANDLE,long);
   ErrorCode ( *pfnBufferedDoRelease)(HANDLE);
};

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
__declspec(noinline)
#endif
__inline BOOL InitializeBioDaqLibray(BDaqLib *bdaqLib)
   {
      if (bdaqLib->instHandle == NULL)
      {
         if ((bdaqLib->instHandle = dlopen("libbio3660.so", RTLD_LAZY)) != NULL) 
         {
            //global apis
				//( *(void **)&bdaqLib->pfnGetValueRangeInformation       = dlsym(bdaqLib->instHandle, "AdxGetValueRangeInformation"));
				//( *(void **)&bdaqLib->pfnGetSignalConnectionInformation = dlsym(bdaqLib->instHandle, "AdxGetSignalConnectionInformation"));
				//( *(void **)&bdaqLib->pfnTranslateTemperatureScale      = dlsym(bdaqLib->instHandle, "AdxTranslateTemperatureScale"));
				//( *(void **)&bdaqLib->pfnDeviceGetLinkageInfo           = dlsym(bdaqLib->instHandle, "AdxDeviceGetLinkageInfo"));
            //event apis
				//( *(void **)&bdaqLib->pfnEventGetHandle                 = dlsym(bdaqLib->instHandle, "AdxEventGetHandle"));
				//( *(void **)&bdaqLib->pfnEventGetLastStatus             = dlsym(bdaqLib->instHandle, "AdxEventGetLastStatus"));
				//( *(void **)&bdaqLib->pfnEventClearFlag                 = dlsym(bdaqLib->instHandle, "AdxEventClearFlag"));
            //property apis
				( *(void **)&bdaqLib->pfnPropertyRead                   = dlsym(bdaqLib->instHandle, "AdxPropertyRead"));
				( *(void **)&bdaqLib->pfnPropertyWrite                  = dlsym(bdaqLib->instHandle, "AdxPropertyWrite"));
            //device apis
				( *(void **)&bdaqLib->pfnDeviceOpen                     = dlsym(bdaqLib->instHandle, "AdxDeviceOpen"));
				( *(void **)&bdaqLib->pfnDeviceClose                    = dlsym(bdaqLib->instHandle, "AdxDeviceClose"));
				( *(void **)&bdaqLib->pfnDeviceReset                    = dlsym(bdaqLib->instHandle, "AdxDeviceReset"));
				( *(void **)&bdaqLib->pfnDeviceGetModuleHandle          = dlsym(bdaqLib->instHandle, "AdxDeviceGetModuleHandle"));
				( *(void **)&bdaqLib->pfnDeviceRefreshProperties        = dlsym(bdaqLib->instHandle, "AdxDeviceRefreshProperties"));
            ( *(void **)&bdaqLib->pfnDeviceFirmwareUpdate           = dlsym(bdaqLib->instHandle, "AdxDeviceFirmwareUpdate"));
            ( *(void **)&bdaqLib->pfnDeviceIoModuleSearch           = dlsym(bdaqLib->instHandle, "AdxDeviceIoModuleSearch"));
            
				//( bdaqLib->pfnDeviceShowConfigDialogBox      = dlsym(bdaqLib->instHandle, L"AdxDeviceShowConfigDialogBox"));
				//AI apis
				( *(void **)&bdaqLib->pfnAiReadSamples                  = dlsym(bdaqLib->instHandle, "AdxAiReadSamples"));
            ( *(void **)&bdaqLib->pfnAiAccessValueRange             = dlsym(bdaqLib->instHandle, "AdxAiAccessValueRange")); 
            ( *(void **)&bdaqLib->pfnAiCalibrate                    = dlsym(bdaqLib->instHandle, "AdxAiCalibrate"));
				//( *(void **)&bdaqLib->pfnBufferedAiPrepare              = dlsym(bdaqLib->instHandle, "AdxBufferedAiPrepare"));
				//( *(void **)&bdaqLib->pfnBufferedAiRunOnce              = dlsym(bdaqLib->instHandle, "AdxBufferedAiRunOnce"));
				//( *(void **)&bdaqLib->pfnBufferedAiRun                  = dlsym(bdaqLib->instHandle, "AdxBufferedAiRun"));
				//( *(void **)&bdaqLib->pfnBufferedAiStop                 = dlsym(bdaqLib->instHandle, "AdxBufferedAiStop"));
				//( *(void **)&bdaqLib->pfnBufferedAiScaleData            = dlsym(bdaqLib->instHandle, "AdxBufferedAiScaleData"));
				//( *(void **)&bdaqLib->pfnBufferedAiRelease              = dlsym(bdaqLib->instHandle, "AdxBufferedAiRelease"));
            //AO apis
				( *(void **)&bdaqLib->pfnAoWriteSamples                 = dlsym(bdaqLib->instHandle, "AdxAoWriteSamples"));
            ( *(void **)&bdaqLib->pfnAoAccessValueRange             = dlsym(bdaqLib->instHandle, "AdxAoAccessValueRange"));
            //DIO apis
				( *(void **)&bdaqLib->pfnDiReadPorts                    = dlsym(bdaqLib->instHandle, "AdxDiReadPorts"));
				( *(void **)&bdaqLib->pfnDoWritePorts                   = dlsym(bdaqLib->instHandle, "AdxDoWritePorts"));
				( *(void **)&bdaqLib->pfnDoReadBackPorts                = dlsym(bdaqLib->instHandle, "AdxDoReadBackPorts"));
								
         }
      }
      return bdaqLib->instHandle != NULL;
   }
__inline BDaqLib * GetBDaqLib()
{
	static BDaqLib bdaqLib;
	return &bdaqLib;
}

//-------------------------------------------------------
// event APIs
//-------------------------------------------------------
__inline ErrorCode   AdxEventGetHandle(
   IN HANDLE   handle,
   IN EventId  id,
   OUT HANDLE* eventHandle)
{
   return GetBDaqLib()->pfnEventGetHandle(handle, id, eventHandle);
}
__inline ErrorCode   AdxEventGetLastStatus(
   IN HANDLE           handle, 
   IN EventId          id, 
   OUT OPTIONAL long* statusLParam,
   OUT OPTIONAL long* statusRParam)
{
   return GetBDaqLib()->pfnEventGetLastStatus(handle, id, statusLParam, statusRParam);
}
__inline ErrorCode   AdxEventClearFlag(
   IN HANDLE  handle, 
   IN EventId id, 
   IN long    flagLParam,
   IN long    flagRParam)
{
   return GetBDaqLib()->pfnEventClearFlag(handle, id, flagLParam, flagRParam);
}

//-------------------------------------------------------
// property APIs
//-------------------------------------------------------
__inline ErrorCode   AdxPropertyRead(
   IN HANDLE          handle, 
   IN PropertyId      id, 
   IN long            bufferSize,
   OUT OPTIONAL void* buffer, 
   OUT OPTIONAL long* dataLength,
   OUT OPTIONAL long* attribute)
{
   return GetBDaqLib()->pfnPropertyRead(handle, id, bufferSize, buffer, dataLength, attribute);
}
__inline ErrorCode   AdxPropertyWrite(
   IN HANDLE     handle, 
   IN PropertyId id, 
   IN long       dataLength,
   IN void*      buffer,
   IN long       notifyNow)
{
   return GetBDaqLib()->pfnPropertyWrite(handle, id, dataLength, buffer, notifyNow);
}

//-------------------------------------------------------
// device APIs
//-------------------------------------------------------
__inline ErrorCode   AdxDeviceOpen(
   IN long       deviceNumber,
   IN AccessMode accessMode,
   OUT HANDLE*   deviceHandle)
{
   if (InitializeBioDaqLibray(GetBDaqLib()))
   {
      return GetBDaqLib()->pfnDeviceOpen(deviceNumber, accessMode, deviceHandle);
   }
   return ErrorDriverNotFound;
}
__inline ErrorCode   AdxDeviceClose(IN HANDLE deviceHandle)
{
   return GetBDaqLib()->pfnDeviceClose(deviceHandle);
}
__inline ErrorCode   AdxDeviceReset(
   IN HANDLE deviceHandle, 
   IN long   state)
{
   return GetBDaqLib()->pfnDeviceReset(deviceHandle, state);
}
__inline ErrorCode   AdxDeviceGetModuleHandle(
   IN HANDLE     deviceHandle,
   IN ModuleType type,
   IN long       index,
   OUT HANDLE*   moduleHandle )
{
   return GetBDaqLib()->pfnDeviceGetModuleHandle(deviceHandle, type, index, moduleHandle);
}
__inline ErrorCode   AdxDeviceRefreshProperties(IN HANDLE deviceHandle)
{
   return GetBDaqLib()->pfnDeviceRefreshProperties(deviceHandle);
}

__inline ErrorCode   AdxDeviceFirmwareUpdate(
   IN HANDLE     deviceHandle,
   IN long       mdlNumber,
   IN FILE*      fp,
   IN long       type,
   IN PROGRESS_CALLBACK progress)
{
   return GetBDaqLib()->pfnDeviceFirmwareUpdate(deviceHandle, mdlNumber, fp, type, progress);
}

__inline ErrorCode   AdxDeviceIoModuleSearch(
   IN HANDLE            deviceHandle,
   IN IO_MODULE_INFO*   modulesInfo,
   IN long              count,
   IN long*             actualCnt)
{
   return GetBDaqLib()->pfnDeviceIoModuleSearch(deviceHandle, modulesInfo, count, actualCnt);
}

//-------------------------------------------------------
// analog input APIs
//-------------------------------------------------------
__inline ErrorCode   AdxAiReadSamples(
   IN HANDLE            aiHandle,
   IN long              mdlNumber,
   IN long              channelStart, 
   IN long              channelCount,
   OUT OPTIONAL void*   rawData,
   OUT OPTIONAL double* scaledData)
{
   return GetBDaqLib()->pfnAiReadSamples(aiHandle, mdlNumber, channelStart, channelCount, rawData, scaledData);
}

__inline ErrorCode AdxAiCalibrate(
   IN HANDLE            aiHandle,
   IN long              mdlNumber,
   IN long              caliType)
{
   return GetBDaqLib()->pfnAiCalibrate(aiHandle, mdlNumber, caliType);
}
__inline ErrorCode   AdxAiSetValueRange(
   IN HANDLE            aiHandle,
   IN long              mdlNumber,
   IN long              channelStart,
   IN long              channelCount,
   OUT OPTIONAL void*   valueRange)
{
   return GetBDaqLib()->pfnAiAccessValueRange(aiHandle, mdlNumber, channelStart, channelCount, valueRange, 1);
}
__inline ErrorCode   AdxAiGetValueRange(
   IN HANDLE            aiHandle,
   IN long              mdlNumber,
   IN long              channelStart,
   IN long              channelCount,
   OUT OPTIONAL void*   valueRange)
{
   return GetBDaqLib()->pfnAiAccessValueRange(aiHandle, mdlNumber, channelStart, channelCount, valueRange, 0);
}

//-------------------------------------------------------
// analog output APIs
//-------------------------------------------------------
__inline ErrorCode   AdxAoWriteSamples(
   IN HANDLE           aoHandle,
   IN long             mdlNumber,
   IN long             channelStart,
   IN long             channelCount,
   IN OPTIONAL void*   rawData,
   IN OPTIONAL double* scaledData)
{
   return GetBDaqLib()->pfnAoWriteSamples(aoHandle, mdlNumber, channelStart, channelCount, rawData, scaledData);
}
__inline ErrorCode   AdxAoSetValueRange(
   IN HANDLE            aoHandle,
   IN long              mdlNumber,
   IN long              channelStart,
   IN long              channelCount,
   OUT OPTIONAL void*   valueRange)
{
   return GetBDaqLib()->pfnAiAccessValueRange(aoHandle, mdlNumber, channelStart, channelCount, valueRange, 1);
}
__inline ErrorCode   AdxAoGetValueRange(
   IN HANDLE            aoHandle,
   IN long              mdlNumber,
   IN long              channelStart,
   IN long              channelCount,
   OUT OPTIONAL void*   valueRange)
{
   return GetBDaqLib()->pfnAiAccessValueRange(aoHandle, mdlNumber, channelStart, channelCount, valueRange, 0);
}

__inline ErrorCode   AdxBufferedAoPrepare(
   IN HANDLE  aoHandle,
   IN long    dataCount,
   OUT void** dataBuffer)
{
   return GetBDaqLib()->pfnBufferedAoPrepare(aoHandle, dataCount, dataBuffer);
}
__inline ErrorCode   AdxBufferedAoRunOnce(
   IN HANDLE aoHandle, 
   IN BOOL   asynchronous)
{
   return GetBDaqLib()->pfnBufferedAoRunOnce(aoHandle, asynchronous);
}
__inline ErrorCode   AdxBufferedAoRun(IN HANDLE aoHandle)
{
   return GetBDaqLib()->pfnBufferedAoRun(aoHandle);
}
__inline ErrorCode   AdxBufferedAoStop(
   IN HANDLE aoHandle, 
   IN long   action)
{
   return GetBDaqLib()->pfnBufferedAoStop(aoHandle, action);
}
__inline ErrorCode   AdxBufferedAoRelease(IN HANDLE aoHandle)
{
   return GetBDaqLib()->pfnBufferedAoRelease(aoHandle);
}
__inline ErrorCode   AdxBufferedAoScaleData(
   IN HANDLE             aoHandle,
   IN double*            scaledData,
   OUT void*             rawData,
   IN long               dataCount,
   IN OUT OPTIONAL long* channelOffset)
{
   return GetBDaqLib()->pfnBufferedAoScaleData(aoHandle, scaledData, rawData, dataCount, channelOffset);
}

//-------------------------------------------------------
// digital input/output APIs
//-------------------------------------------------------
__inline ErrorCode   AdxDiReadPorts(
   IN HANDLE dioHandle,
   IN long   mdlNumber,
   IN long   portStart,
   IN long   portCount,
   OUT BYTE* dataBuffer)
{
   return GetBDaqLib()->pfnDiReadPorts(dioHandle, mdlNumber, portStart, portCount, dataBuffer);
}
__inline ErrorCode   AdxDoWritePorts(
   IN HANDLE dioHandle,
   IN long   mdlNumber,
   IN long   portStart,
   IN long	 portCount,
   IN BYTE*  dataBuffer)
{
   return GetBDaqLib()->pfnDoWritePorts(dioHandle, mdlNumber, portStart, portCount, dataBuffer);
}
__inline ErrorCode   AdxDoReadBackPorts(
   IN HANDLE dioHandle,
   IN long   mdlNumber,
   IN long   portStart,
   IN long   portCount,
   OUT BYTE* dataBuffer)
{
   return GetBDaqLib()->pfnDoReadBackPorts(dioHandle, mdlNumber, portStart, portCount, dataBuffer);
}


// **********************************************************
// Bionic DAQ classes, for c++ only
// **********************************************************
#ifdef __cplusplus

// **********************************************************
// Forward declare
// **********************************************************
class BDaqModule;
class BDaqAi;
class BDaqAo;
class BDaqDio;
class BDaqDevice;
class BDaqModuleManager;

// **********************************************************
// classes definition
// **********************************************************
class BDaqEvent
{
public:
   ErrorCode GetHandle(EventId id, HANDLE & eventHandle);
   long GetStatus(EventId id);
   long GetStatus(EventId id, long & statusRParam);
   void ClearFlag(EventId id, long flagLParam, long flagRParam);

protected: 
   BDaqEvent( ) 
   {
      m_parent = NULL;
   }
   void SetParent(BDaqModule * parent)
   {
      m_parent = parent;
   }

protected:
   BDaqModule * m_parent;
   friend class BDaqModule;
};
class BDaqProperty
{
public:
   long NotifyPropertyChanged; // whether or not notify the others the property was changed.

   // helper method to get the attribute of a property
   long GetLength(PropertyId id);
   PropertyAttribute GetAttribute(PropertyId id);

   // get single-value property
   ErrorCode Get(PropertyId id, int& data);
   ErrorCode Get(PropertyId id, long& data);
   ErrorCode Get(PropertyId id, double& data);
   ErrorCode Get(PropertyId id, MathInterval& data);

   // get array type property
   ErrorCode Get(PropertyId id, long count, BYTE data[]);
   ErrorCode Get(PropertyId id, long count, long data[]);
   ErrorCode Get(PropertyId id, long count, double data[]);
//   ErrorCode Get(PropertyId id, long count, WCHAR data[]);

   // set single-value property
   ErrorCode Set(PropertyId id, int data); // we need an int overload to disambiguate for the immediate number case
   ErrorCode Set(PropertyId id, long data);
   ErrorCode Set(PropertyId id, double data);
   ErrorCode Set(PropertyId id, MathInterval data);

   // set array type property
   ErrorCode Set(PropertyId id, long count, BYTE data[]);
   ErrorCode Set(PropertyId id, long count, long data[]);
   ErrorCode Set(PropertyId id, long count, double data[]);
//   ErrorCode Set(PropertyId id, long count, WCHAR data[]);

protected:
   BDaqProperty()
   {
      NotifyPropertyChanged = 0; // default is not sending property change event to other listeners.
      m_parent = NULL;
   }
   void SetParent(BDaqModule * parent)
   {
      m_parent = parent;
   }
protected:
   BDaqModule * m_parent;
   friend class BDaqModule;
};

class BDaqModule
{
public: // Methods
#if defined(__BORLANDC__) || defined(_MSC_VER)
   __declspec(property(get = get_Handle)) HANDLE Handle;
#endif
   HANDLE get_Handle()
   {
      return m_moduleHandle;
   }

#if defined(__BORLANDC__) || defined(_MSC_VER)
   __declspec(property(get = get_Property)) BDaqProperty &Property;
#endif
   BDaqProperty & get_Property( )
   {
      return m_property;
   }

#if defined(__BORLANDC__) || defined(_MSC_VER)
   __declspec(property(get = get_Event)) BDaqEvent &Event;
#endif
   BDaqEvent & get_Event()
   {
      return m_event;
   }

protected:
   BDaqModule(HANDLE moduleHandle)
   {
      m_moduleHandle = moduleHandle;
      m_property.SetParent(this);
      m_event.SetParent(this);
   }
   virtual ~BDaqModule()
   {
   }
   virtual void Release()
   {
      delete this;
   }
protected:
   friend class BDaqModuleManager;
   HANDLE       m_moduleHandle;
   BDaqProperty m_property;
   BDaqEvent    m_event;
};

class BDaqModuleManager
{
public:
   BDaqModuleManager()
   {
   }
   virtual ~BDaqModuleManager()
   {
      ModuleCache::iterator itor;
      BDaqModule* module;
      while( !m_cachedModules.empty() )
      {
         itor   = m_cachedModules.begin();
         module = itor->second;
         m_cachedModules.erase(itor);
         if (module)
         {
            module->Release();
         }
      }
   }
   BDaqModule* Find(long key)
   {
      ModuleCache::const_iterator itor = m_cachedModules.find(key);
      return itor != m_cachedModules.end() ? itor->second : NULL;
   }
   void Remove(long key)
   {
      m_cachedModules.erase( key );
   }
   BDaqModule* & operator[](long key)
   {
      return m_cachedModules[key];
   }

protected:
   typedef std::map<long, BDaqModule*> ModuleCache;
   ModuleCache m_cachedModules;
};

class BDaqAi : public BDaqModule
{
public:

   // instant AI single channel methods
   ErrorCode Read(long mdlNumber, long channel, double& dataScaled);
   ErrorCode Read(long mdlNumber, long channel, short& dataRaw);   // for the device whose raw data is in 16bits format
   ErrorCode Read(long mdlNumber, long channel, long& dataRaw);    // for the device whose raw data is in 32bits format

   // instant AI multi channel methods
   ErrorCode Read(long mdlNumber, long chStart, long chCount, double dataScaled[]);
   ErrorCode Read(long mdlNumber, long chStart, long chCount, short dataRaw[], double dataScaled[]);   // for the device whose raw data is in 16bits format
   ErrorCode Read(long mdlNumber, long chStart, long chCount, long dataRaw[], double dataScaled[]) ;   // for the device whose raw data is in 32bits format

   ErrorCode GetValueRange(long mdlNumber, long chStart, long chCount, long valueRange[]);
   ErrorCode SetValueRange(long mdlNumber, long chStart, long chCount, long valueRange[]);
   ErrorCode Calibrate(long mdlNumber, long caliType);
protected:
   friend class BDaqDevice;
   // internal used only
   BDaqAi(HANDLE aiHandle) : BDaqModule(aiHandle)
   {
   }
   virtual ~BDaqAi()
   {
   }
};

class BDaqAo : public BDaqModule
{
public:
   // instant AO single channel methods
   ErrorCode Write(long mdlNumber, long channel, double dataScaled);
   ErrorCode Write(long mdlNumber, long channel, short dataRaw);   // for the device whose raw data is in 16bits format
   ErrorCode Write(long mdlNumber, long channel, long dataRaw);    // for the device whose raw data is in 32bits format

   // instant AO multi channel methods
   ErrorCode Write(long mdlNumber, long chStart, long chCount, double dataScaled[]);
   ErrorCode Write(long mdlNumber, long chStart, long chCount, short dataRaw[]);   // for the device whose raw data is in 16bits format
   ErrorCode Write(long mdlNumber, long chStart, long chCount, long dataRaw[]);    // for the device whose raw data is in 32bits format

   ErrorCode GetValueRange(long mdlNumber, long chStart, long chCount, long valueRange[]);
   ErrorCode SetValueRange(long mdlNumber, long chStart, long chCount, long valueRange[]);
   // buffered AO methods
   ErrorCode BfdAoPrepare(long dataCount, void* * dataRaw);
   ErrorCode BfdAoRunOnce(bool asynchronous);
   ErrorCode BfdAoRun();
   ErrorCode BfdAoScaleData(double dataScaled[], void* dataRaw, long dataCount, long* chOffset);
   ErrorCode BfdAoStop(long action);
   ErrorCode BfdAoRelease();

protected:
   friend class BDaqDevice;

   // internal used only
   BDaqAo(HANDLE aoHandle) : BDaqModule(aoHandle)
   {
   }
   virtual ~BDaqAo()
   {
   }
};

class BDaqDio : public BDaqModule
{
public:
   // Instant DI/O methods
   ErrorCode DiRead(long mdlNumber, long port, BYTE& data);
   ErrorCode DiRead(long mdlNumber, long portStart, long portCount, BYTE data[]);
   ErrorCode DoWrite(long mdlNumber, long port, BYTE data);
   ErrorCode DoWrite(long mdlNumber, long portStart, long portCount, BYTE data[]);
   ErrorCode DoRead(long mdlNumber, long port, BYTE& data);
   ErrorCode DoRead(long mdlNumber, long portStart, long portCount, BYTE data[]);

protected:
   friend class BDaqDevice;

   // internal used only
   BDaqDio(HANDLE dioHandle) : BDaqModule(dioHandle)
   {
   }
   virtual ~BDaqDio()
   {
   }
};

class BDaqDevice : public BDaqModule
{
public:
   // Methods
   static ErrorCode Open(int deviceNumber, AccessMode mode, BDaqDevice* & device);

   ErrorCode GetModule(long index, BDaqAi * & ai);
   ErrorCode GetModule(long index, BDaqAo * & ao);
   ErrorCode GetModule(long index, BDaqDio * & dio);
   ErrorCode GetModule(ModuleType type, long index, BDaqModule * & module);

   ErrorCode RefreshProperties();
   ErrorCode UpdateFirmware(long mdlNumber, FILE *fp, long type, PROGRESS_CALLBACK progress);
   ErrorCode SearchIoModules(IO_MODULE_INFO* modulesInfo, long count, long *actualCnt);
/*   
   ErrorCode ShowModalDialog(HWND parentWnd, long dataSource);
   ErrorCode ShowPopupDialog(HWND parentWnd, long dataSource, HWND* dlgWnd);
   ErrorCode ShowEmbedDialog(HWND parentWnd, RECT* wndRect, long dataSource, HWND* dlgWnd);
*/   
   ErrorCode Reset(long state);
   void Close();

#if defined(__BORLANDC__) || defined(_MSC_VER)
   __declspec(property(get = get_DeviceNumber)) long DeviceNumber;
#endif
   long get_DeviceNumber( )
   {
      return m_deviceNumber;
   }

#if defined(__BORLANDC__) || defined(_MSC_VER)
   __declspec(property(get = get_AccessMode)) AccessMode Mode;
#endif
   AccessMode get_AccessMode()
   {
      return m_deviceMode;
   }

protected:// methods for internal using only

   // user can only initializes a new instance by the static method 'Open' 
   BDaqDevice(HANDLE deviceHandle, int deviceNumber, AccessMode mode) : BDaqModule(deviceHandle)
   {
      m_deviceNumber = deviceNumber;
      m_deviceMode = mode;
   }

   // user can only delete an instance by the method 'close' 
   virtual ~BDaqDevice()
   {
   }

   virtual void Release()
   {
      //if (InterlockedDecrement(&m_refCount) == 0)
      m_refCount --;
      {
         GetCachedDevices().Remove(GetDeviceKey(m_deviceNumber, m_deviceMode));

         // close the device handle
         AdxDeviceClose(get_Handle());

         delete this;
      }
   }

   static long GetModuleKey(ModuleType type, long index)
   {
      return (long)type | (index << 16);
   }

   static long GetDeviceKey(int deviceNumber, AccessMode mode)
   {
      if (mode == ModeWriteWithReset)
      {
         mode = ModeWrite;
      }
      return (long)deviceNumber | ((long)mode << 16);
   }

   static BDaqModuleManager& GetCachedDevices()
   {
      // cached devices, used to implement the 'singleton' for the devices 
      // which own the same device number and access mode. 
      static BDaqModuleManager cachedDevices;
      return cachedDevices;
   }
protected:   // fields
   friend class BDaqModuleManager;
   long         m_deviceNumber;
   AccessMode   m_deviceMode;
   long         m_refCount;

   // module map, used to manage all the modules belongs to the device
   BDaqModuleManager m_cachedModules;
};

// *********************************************************************
// inline functions of BDaqEvent
// *********************************************************************
inline ErrorCode BDaqEvent::GetHandle(EventId id, HANDLE& eventHandle)
{
   return AdxEventGetHandle(m_parent->get_Handle(), id, &eventHandle);
}
inline long BDaqEvent::GetStatus(EventId id)
{
   long statusLParam = 0;
   AdxEventGetLastStatus(m_parent->get_Handle(), id, &statusLParam, NULL);
   return statusLParam;
}
inline long BDaqEvent::GetStatus(EventId id, long & statusRParam)
{
   long statusLParam = 0;
   statusRParam = 0;
   AdxEventGetLastStatus(m_parent->get_Handle(), id, &statusLParam, &statusRParam);
   return statusLParam;
}
inline void BDaqEvent::ClearFlag(EventId id, long flagLParam, long flagRParam)
{
   AdxEventClearFlag(m_parent->get_Handle(), id, flagLParam, flagRParam);
}

// *********************************************************************
// inline functions of BDaqProperty
// *********************************************************************
inline long BDaqProperty::GetLength(PropertyId id)
{
   long dataLength;
   AdxPropertyRead(m_parent->get_Handle(), id, 0, NULL, &dataLength, NULL);
   return dataLength;
}
inline PropertyAttribute BDaqProperty::GetAttribute(PropertyId id)
{
   long propAttr = 0;
   AdxPropertyRead(m_parent->get_Handle(), id, 0, NULL, NULL, &propAttr);
   return (PropertyAttribute)propAttr;
}

// get single-value property
inline ErrorCode BDaqProperty::Get(PropertyId id, int& data)
{
   return AdxPropertyRead(m_parent->get_Handle(), id, sizeof(int), &data, NULL, NULL);
}

inline ErrorCode BDaqProperty::Get(PropertyId id, long& data)
{
   return AdxPropertyRead(m_parent->get_Handle(), id, sizeof(long), &data, NULL, NULL);
}
inline ErrorCode BDaqProperty::Get(PropertyId id, double& data)
{
   return AdxPropertyRead(m_parent->get_Handle(), id, sizeof(double), &data, NULL, NULL);
}
inline ErrorCode BDaqProperty::Get(PropertyId id, MathInterval& data)
{
   return AdxPropertyRead(m_parent->get_Handle(), id, sizeof(MathInterval), &data, NULL, NULL);
}

// get array type property
inline ErrorCode BDaqProperty::Get(PropertyId id, long count, BYTE data[])
{
   return AdxPropertyRead(m_parent->get_Handle(), id, count * sizeof(BYTE), data, NULL, NULL);
}
inline ErrorCode BDaqProperty::Get(PropertyId id, long count, long data[])
{
   return AdxPropertyRead(m_parent->get_Handle(), id, count * sizeof(long), data, NULL, NULL);
}
inline ErrorCode BDaqProperty::Get(PropertyId id, long count, double data[])
{
   return AdxPropertyRead(m_parent->get_Handle(), id, count * sizeof(double), data, NULL, NULL);
}
/*
inline ErrorCode BDaqProperty::Get(PropertyId id, long count, WCHAR data[])
{
   return AdxPropertyRead(m_parent->get_Handle(), id, count * sizeof(WCHAR), data, NULL, NULL);
}
*/
// set single-value property
inline ErrorCode BDaqProperty::Set(PropertyId id, int data)
{  
   // we need this int overload to disambiguate for the immediate number case
   return AdxPropertyWrite(m_parent->get_Handle(), id, sizeof(int), (void*)&data, NotifyPropertyChanged);
}

inline ErrorCode BDaqProperty::Set(PropertyId id, long data)
{
   return AdxPropertyWrite(m_parent->get_Handle(), id, sizeof(long), (void*)&data, NotifyPropertyChanged);
}
inline ErrorCode BDaqProperty::Set(PropertyId id, double data)
{
   return AdxPropertyWrite(m_parent->get_Handle(), id, sizeof(double), (void*)&data, NotifyPropertyChanged);
}
inline ErrorCode BDaqProperty::Set(PropertyId id, MathInterval data)
{
   return AdxPropertyWrite(m_parent->get_Handle(), id, sizeof(MathInterval), (void*)&data, NotifyPropertyChanged);
}

// set array type property
inline ErrorCode BDaqProperty::Set(PropertyId id, long count, BYTE data[])
{
   return AdxPropertyWrite(m_parent->get_Handle(), id, count * sizeof(BYTE), data, NotifyPropertyChanged);
}
inline ErrorCode BDaqProperty::Set(PropertyId id, long count, long data[])
{
   return AdxPropertyWrite(m_parent->get_Handle(), id, count * sizeof(long), data, NotifyPropertyChanged);
}
inline ErrorCode BDaqProperty::Set(PropertyId id, long count, double data[])
{
   return AdxPropertyWrite(m_parent->get_Handle(), id, count * sizeof(double), data, NotifyPropertyChanged);
}
/*
inline ErrorCode BDaqProperty::Set(PropertyId id, long count, WCHAR data[])
{
   return AdxPropertyWrite(m_parent->get_Handle(), id, count * sizeof(WCHAR), data, NotifyPropertyChanged);
}
*/
// *********************************************************************
// inline functions of BDaqAi
// *********************************************************************
// instant AI single channel methods
inline ErrorCode BDaqAi::Read(long mdlNumber, long channel, double& dataScaled)
{
   return AdxAiReadSamples(get_Handle(), mdlNumber, channel, 1, NULL, &dataScaled);
}
// for the device whose raw data is in 16bits format
inline ErrorCode BDaqAi::Read(long mdlNumber, long channel, short& dataRaw)
{
   return AdxAiReadSamples(get_Handle(), mdlNumber, channel, 1, &dataRaw, NULL);
}
// for the device whose raw data is in 32bits format
inline ErrorCode BDaqAi::Read(long mdlNumber, long channel, long& dataRaw)
{
   return AdxAiReadSamples(get_Handle(), mdlNumber, channel, 1, &dataRaw, NULL);
}

// instant AI multi channel methods
inline ErrorCode BDaqAi::Read(long mdlNumber, long chStart, long chCount, double dataScaled[])
{
   return AdxAiReadSamples(get_Handle(), mdlNumber, chStart, chCount, NULL, dataScaled);
}
// for the device whose raw data is in 16bits format
inline ErrorCode BDaqAi::Read(long mdlNumber, long chStart, long chCount, short dataRaw[], double dataScaled[]) 
{
   return AdxAiReadSamples(get_Handle(), mdlNumber, chStart, chCount, dataRaw, dataScaled);
}
// for the device whose raw data is in 32bits format
inline ErrorCode BDaqAi::Read(long mdlNumber, long chStart, long chCount, long dataRaw[], double dataScaled[]) 
{
   return AdxAiReadSamples(get_Handle(), mdlNumber, chStart, chCount, dataRaw, dataScaled);
}

inline ErrorCode BDaqAi::SetValueRange(long mdlNumber,long chStart,long chCount,long valueRange [])
{
   return AdxAiSetValueRange(get_Handle(), mdlNumber, chStart, chCount, valueRange);
}

inline ErrorCode BDaqAi::GetValueRange(long mdlNumber,long chStart,long chCount,long valueRange [])
{
   return AdxAiGetValueRange(get_Handle(), mdlNumber, chStart, chCount, valueRange);
}

inline ErrorCode BDaqAi::Calibrate(long mdlNumber, long caliType)
{
   return AdxAiCalibrate(get_Handle(), mdlNumber, caliType);
}

// *********************************************************************
// inline functions of BDaqAo
// *********************************************************************
// instant AO single channel methods
inline ErrorCode BDaqAo::Write(long mdlNumber, long channel, double dataScaled)
{
   return AdxAoWriteSamples(get_Handle(), mdlNumber, channel, 1, NULL, (double*)&dataScaled);
}
// for the device whose raw data is in 16bits format
inline ErrorCode BDaqAo::Write(long mdlNumber, long channel, short dataRaw)
{
   return AdxAoWriteSamples(get_Handle(), mdlNumber, channel, 1, (void*)&dataRaw, NULL);
}
// for the device whose raw data is in 32bits format
inline ErrorCode BDaqAo::Write(long mdlNumber, long channel, long dataRaw)
{
   return AdxAoWriteSamples(get_Handle(), mdlNumber, channel, 1, (void*)&dataRaw, NULL);
}

// instant AO multi channel methods
inline ErrorCode BDaqAo::Write(long mdlNumber, long chStart, long chCount, double dataScaled[])
{
   return AdxAoWriteSamples(get_Handle(), mdlNumber, chStart, chCount, NULL, dataScaled);
}
// for the device whose raw data is in 16bits format
inline ErrorCode BDaqAo::Write(long mdlNumber, long chStart, long chCount, short dataRaw[])
{
   return AdxAoWriteSamples(get_Handle(), mdlNumber, chStart, chCount, dataRaw, NULL);
}
// for the device whose raw data is in 32bits format
inline ErrorCode BDaqAo::Write(long mdlNumber, long chStart, long chCount, long dataRaw[])
{
   return AdxAoWriteSamples(get_Handle(), mdlNumber, chStart, chCount, dataRaw, NULL);
}

inline ErrorCode BDaqAo::SetValueRange(long mdlNumber,long chStart,long chCount,long valueRange [])
{
   return AdxAoSetValueRange(get_Handle(), mdlNumber, chStart, chCount, valueRange);
}

inline ErrorCode BDaqAo::GetValueRange(long mdlNumber,long chStart,long chCount,long valueRange [])
{
   return AdxAoGetValueRange(get_Handle(), mdlNumber, chStart, chCount, valueRange);
}


// buffered AO methods
inline ErrorCode BDaqAo::BfdAoPrepare(long dataCount, void* * dataRaw)
{
   return AdxBufferedAoPrepare(get_Handle(), dataCount, dataRaw);
}
inline ErrorCode BDaqAo::BfdAoRunOnce(bool asynchronous)
{
   return AdxBufferedAoRunOnce(get_Handle(), asynchronous ? 1 : 0);
}
inline ErrorCode BDaqAo::BfdAoRun()
{
   return AdxBufferedAoRun(get_Handle());
}
inline ErrorCode BDaqAo::BfdAoScaleData(double dataScaled[], void* dataRaw, long dataCount, long* chOffset)
{
   return AdxBufferedAoScaleData(get_Handle(), dataScaled, dataRaw, dataCount, chOffset);
}
inline ErrorCode BDaqAo::BfdAoStop(long action)
{
   return AdxBufferedAoStop(get_Handle(), action);
}
inline ErrorCode BDaqAo::BfdAoRelease()
{
   return AdxBufferedAoRelease(get_Handle());
}

// *********************************************************************
// inline functions of BDaqDio
// *********************************************************************
// Instant DI/O methods
inline ErrorCode BDaqDio::DiRead(long mdlNumber, long port, BYTE& data)
{
   return AdxDiReadPorts(get_Handle(), mdlNumber, port, 1, &data);
}
inline ErrorCode BDaqDio::DiRead(long mdlNumber, long portStart, long portCount, BYTE data[])
{
   return AdxDiReadPorts(get_Handle(), mdlNumber, portStart, portCount, data);
}

inline ErrorCode BDaqDio::DoWrite(long mdlNumber, long port, BYTE data)
{
   return AdxDoWritePorts(get_Handle(), mdlNumber, port, 1, &data);
}
inline ErrorCode BDaqDio::DoWrite(long mdlNumber, long portStart, long portCount, BYTE data[])
{
   return AdxDoWritePorts(get_Handle(), mdlNumber, portStart, portCount, data);
}

inline ErrorCode BDaqDio::DoRead(long mdlNumber, long port, BYTE& data)
{
   return AdxDoReadBackPorts(get_Handle(), mdlNumber, port, 1, &data);
}
inline ErrorCode BDaqDio::DoRead(long mdlNumber, long portStart, long portCount, BYTE data[])
{
   return AdxDoReadBackPorts(get_Handle(), mdlNumber, portStart, portCount, data);
}


// *********************************************************************
// inline functions of BDaqDevice
// *********************************************************************
inline ErrorCode BDaqDevice::Open(int deviceNumber, AccessMode mode, BDaqDevice* & device)
{
   // 'Singleton': all devices which own same device number and access mode
   //  will reference to the same object.
   ErrorCode ret = Success;
   long key = GetDeviceKey(deviceNumber, mode);
   device = (BDaqDevice*)GetCachedDevices().Find(key);
   if ( device )
   {
      //InterlockedIncrement(&device->m_refCount);
      device->m_refCount ++;
   } else{
      HANDLE deviceHandle = NULL;
      ret = AdxDeviceOpen(deviceNumber, mode, &deviceHandle);
      if (ret == Success)
      {
         device = new BDaqDevice(deviceHandle, deviceNumber, mode);
         if ( device != NULL )
         {
            device->m_refCount = 1;
            GetCachedDevices()[key] = device;
         } else {
            ret = ErrorMemoryNotEnough;
         }
      }
   }
   return ret;
}


inline ErrorCode BDaqDevice::GetModule(long index, BDaqAi * & ai)
{
   return GetModule(DaqAi, index, (BDaqModule*&)ai);
}
inline ErrorCode BDaqDevice::GetModule(long index, BDaqAo * & ao)
{
   return GetModule(DaqAo, index, (BDaqModule*&)ao);
}
inline ErrorCode BDaqDevice::GetModule(long index, BDaqDio * & dio)
{
   return GetModule(DaqDio, index, (BDaqModule*&)dio);
}

inline ErrorCode BDaqDevice::GetModule(ModuleType type, long index, BDaqModule* & module)
{
   module = NULL;
   if (get_Handle() == NULL)
   {
      return ErrorHandleNotValid;
   }

   ErrorCode ret = Success;
   long key = GetModuleKey(type, index);
   module = m_cachedModules[key];
   if (!module)
   {
      HANDLE moduleHandle;
      ret = AdxDeviceGetModuleHandle(get_Handle(), type, index, &moduleHandle);
      if (ret == Success)
      {
         switch (type)
         {
         case DaqAi:
            module = new BDaqAi(moduleHandle);
            break;
         case DaqAo:
            module = new BDaqAo(moduleHandle);
            break;
         case DaqDio:
            module = new BDaqDio(moduleHandle);
            break;
         default:
            module = NULL;
            break;
         }
         if (module != NULL) 
         {
            m_cachedModules[key] = module;
         } else {
            ret = ErrorMemoryNotEnough;
         }
      }
   }
   return ret;
}

inline ErrorCode BDaqDevice::RefreshProperties()
{
   return AdxDeviceRefreshProperties(get_Handle());
}

inline ErrorCode BDaqDevice::UpdateFirmware(long mdlNumber, FILE * fp, long type, PROGRESS_CALLBACK progress)
{
   return AdxDeviceFirmwareUpdate(get_Handle(), mdlNumber, fp, type, progress);
}

inline ErrorCode BDaqDevice::SearchIoModules(IO_MODULE_INFO* modulesInfo, long count, long* actualCnt)
{
   return AdxDeviceIoModuleSearch(get_Handle(), modulesInfo, count, actualCnt);
}

inline ErrorCode BDaqDevice::Reset(long state)
{
   return AdxDeviceReset(get_Handle(), state);
}

inline void BDaqDevice::Close()
{
   Release();
}

#endif // __cplusplus

#if !defined(_BDAQ_NO_NAMESPACE) && defined(__cplusplus)
   } // namespace : BDaq
} // namespace : Automation
#endif

#endif
