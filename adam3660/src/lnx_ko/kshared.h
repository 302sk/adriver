/*
 *kshared.h
 *
 * Created on: 2014-1-15
 * Author: shenkun
 */

#ifndef KERNEL_MODULE_SHARED_H_
#define KERNEL_MODULE_SHARED_H_

#include <linux/types.h>


#define ADVANTECH_VID	0x13fe
#define DRIVER_NAME	"bio3660"
#define DEVICE_NAME 	"adam3660-io"
#define DEVICE_ID	0x3660

#define MODULE_MAX_COUNT   5
#define MODULE_MAX_FUNC    4
#define MODULE_MAX_CHANNEL 8

#define MODULE_DATA_MAX 16

#define AI_CHL_COUNT	8
#define AI_CHL_MASK	(AI_CHL_COUNT - 1)
#define AI_DATA_SIZE	sizeof(uint32)
#define AI_DATA_MASK	0xffff0000

#define AI_RESOLUTION_IN_BIT             16


#define AI_GAIN_V_Neg10T010	0
#define AI_GAIN_V_Neg5To5	1


#define DEF_AI_CHTYPE           1 // differential
#define DEF_AI_GAIN             0 //AI_GAIN_V_Neg10To10


//Ao default values
#define VOLTAGE 	0
#define CURRENT		1

#define AO_CHL_COUNT	2
#define AO_CHL_MASK	(AO_CHL_COUNT - 1)
#define AO_DATA_SIZE	sizeof(__u32)
#define AO_DATA_MASK	0xfff0

#define AO_RES_IN_BIT         12

#define DEF_AO_GAIN             V_0To5
#define DEF_AO_MODE             VOLTAGE
#define DEF_AO_DATA             0x0

#define DEF_AO_INIT_STATE       0


//dio default values
#define DIO_PORT_COUNT	2
#define DIO_CHL_COUNT	(DIO_PORT_COUNT * 8)

#define DEF_DIO_DIR             0x00
#define DEF_DO_STATE            0x00


enum KRNL_EVENT_IDX{
	KdxDevPropChged = 0,
	KdxAiDataReady,
	KrnlSptedEventCount,
};

static inline __u32 GetEventKIndex(__u32 eventType)
{
	__u32 kdx;
	switch( eventType )
	{
		case EvtPropertyChanged:	kdx = KdxDevPropChged;	break;
		case EvtBufferedAiDataReady:	kdx = KdxAiDataReady;	break;
		
		default: kdx = -1; break;
	}
	return kdx;
}

enum MODULE_FUNC_TYPE  //function types in a module
{
   module_func_ai = 0,
   module_func_ao,
   module_func_di,
   module_func_do,
};

enum MODULE_DTA_TAYPE
{
   module_data_gain = 0,
   module_data_aio,
   module_data_dio,
};

typedef struct _FUNC_INFO
{
   int32  funcType;
   int32  chlCount;
   __u8   gainCode[MODULE_MAX_CHANNEL];
}FUNC_INFO;

typedef struct _MODULE_FUNC_INFO
{
   int32     mdlNumber;
   __u8        mdlName[16];
   FUNC_INFO   funcInfo[MODULE_MAX_FUNC + 1];  
}MODULE_FUNC_INFO;

typedef struct _DEVICE_SHARED
{
	__u32	size;   //size of the structure
	__u32	product_id;	//Device Type
	__u32   device_number;	//zero-based device number
	__u32	BoardId;
/*
	__u8	AiChanType[AI_CHL_COUNT];
	__u8	AiChanGain[AI_CHL_COUNT];
	__u32	AiLogChanCount;
	__u32	AoChanGain[AO_CHL_COUNT];
	__u8	AoChanState[AO_CHL_COUNT];
	double	AoExtRefUnipolar;
	__u8	DioPortDir[DIO_PORT_COUNT];
	__u8	DoPortState[DIO_PORT_COUNT];
*/
   MODULE_FUNC_INFO mdlFuncInfo[MODULE_MAX_COUNT];
   
	__u32	IsEvtSignaled[KrnlSptedEventCount];

}DEVICE_SHARED;


/////////////////spi protocal///////////////////

#define MESSAGE_VERSION    0x01

enum HEADER_COMMAND_CODE{
   header_com_common = 0x0,
   header_com_search,
   header_com_download,
};

typedef struct _PROTOCOL_HEADER_INFO
{
   __u8     header_str[4];    //always are "ADAM"
   __u16    module_count;     //how many modules info this frame contain
   __u16    message_id;       //sequence number
   __u8     message_ver;      //spi protocol version
   __u8     command;          //download firmware or search device
   __u16    data_len;         //data lenght of whole frame
   __u16    check_sum;
}HEADER_INFO;

typedef struct _PROTOCOL_MODULE_INFO
{
   __u8     module_id;
   __u8     data_len;
}MODULE_INFO;

//----------module data command type--------------//
//bit7   status   0:successful 1:error
//bit6   req/res  0:request 1:response
//bit5   w/r      0:write command 1:read command
//bit4:0 reserved

enum COMMAND_TYPE_STATUS{
   comm_st_success = 0,
   comm_st_error = (1<<7),
};
enum COMMAND_TYPE_DIR{
   comm_dir_req = 0,
   comm_dir_res = (1<<6),
   
};
enum COMMAND_TYPE_MODE{
   comm_mode_write = 0,
   comm_mode_read = (1<<5),
};

enum MODULE_COMMAND_CODE{
   comm_search_dev = 0x01,
   comm_download_fw,
   comm_upload_data,
   comm_reset_fc,
   comm_set_module,

   //DO command
   comm_do_rngcode = 0x10,
   comm_do_data,
   comm_do_pwm,

   //DI command
   comm_di_rngcode = 0x20,
   comm_di_data,
   comm_di_filter,

   //AO command
   comm_ao_rngcode = 0x30,
   comm_ao_data,
   comm_ao_cal,
   comm_ao_caltofac,
   comm_ao_unit,

   //AI command
   comm_ai_rngcode = 0x40,
   comm_ai_data,
   comm_ai_cal,
   comm_ai_caltofac,
   comm_ai_unit,
   comm_ai_intgtime,
   comm_ai_calcmp,
   comm_ai_burnchk,
   comm_ai_alarm, 
};

typedef union _CHANNEL_RANGE
{
   __u8 value;
   struct{
      __u8 stop_chl  : 4;
      __u8 start_chl : 4;
   };
}CHANNEL_RANGE;

//----------
#pragma pack(1)
typedef struct __PROTOCOL_MODULE_DATA
{
   __u8           command_type;
   __u8           command;
   CHANNEL_RANGE  channel_rng;  //bit7:4 start channel bit3:0 end channel
}MODULE_DATA;
#pragma pack()


typedef struct __PROTOCOL_SPI_PACKAGE
{
   __u16    len;
   __u16    offset;
   __u8*    data;
}SPI_PACKAGE;

#define PKG_ERROR -1
#define PKG_DIR_SND  0
#define PKG_DIR_RCV  1

HEADER_INFO* add_header_info(SPI_PACKAGE *package, __u16 module_count, __u8 command, __u16 len);
MODULE_INFO* add_module_info(SPI_PACKAGE *package, __u8 module_id, __u8 len);
int add_module_data(SPI_PACKAGE *package, __u8 type, __u8 cmd, CHANNEL_RANGE rng, __u8 data[]);
//int add_module_chksum(SPI_PACKAGE *package, __u16 chk_sum);
int add_module_chksum(SPI_PACKAGE *package, MODULE_INFO *info);
int set_header_len(HEADER_INFO *header, __u16 len);


int calc_data_len(int pkg_dir, int cmd_type, __u8 cmd, CHANNEL_RANGE chl_rng);


int find_header(SPI_PACKAGE *package);
int get_header_info(SPI_PACKAGE *package, HEADER_INFO *header);
int get_module_info(SPI_PACKAGE *package, MODULE_INFO *module);
int get_module_data(SPI_PACKAGE *package, MODULE_DATA *module_data);
__u16 get_module_chksum(SPI_PACKAGE *package);
int get_data(SPI_PACKAGE *package, void *data, int len);



#endif

