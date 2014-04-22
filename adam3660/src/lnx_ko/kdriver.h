/*
 * kdriver.h
 *
 * Created on: 2014-1-14
 * Author:shenkun
 */

#ifndef _KERNEL_MODULE_H_
#define _KERNEL_MODULE_H_

#ifndef CONFIG_SPI
# error "This driver needs to have SPI support."
#endif

#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/io.h>
#include <linux/spi/spidev.h>
#include <asm/uaccess.h>
#include <linux/time.h>

#include <bdaqdef.h>
#include <ioctls.h>
#include <biokernbase.h>
#include "kshared.h"

typedef struct daq_file_ctx{
	struct list_head	ctx_list;
	struct daq_device	*daq_dev;
	HANDLE			events[KrnlSptedEventCount];
	int			write_access;
	int			busy;
}daq_file_ctx_t;


typedef struct daq_task_info{
   __u8        module_id;
   __u8        header_cmd;
   MODULE_DATA module_data;
   __u16       len;
   __u8*       data;
}daq_task_info_t;

typedef struct daq_spi_transaction{
   __u16 msg_id;
   daq_event_t* cmd_event;
   daq_event_t* rsp_event;
   daq_task_info_t task_list[10];
   daq_task_info_t task_list_rcv[10];
   __u8 rcv_data[256];
   __u8 task_count;
   __u8 recv_count;
}daq_spi_transaction_t;

typedef struct daq_device
{
	DEVICE_SHARED	shared;
	struct cdev	cdev;

	struct file*	spi_snd;
	struct file*	spi_rcv;

   struct task_struct *spi_rcv_thrd;
   struct task_struct *spi_snd_thrd;  //for debug

   //daq_spi_event_t events[10];
   daq_spi_transaction_t spi_transaction[2];
   int   curr_trsc;
   daq_spi_transaction_t *request;
   spinlock_t  trsc_lock;
   
	spinlock_t	dev_lock;
	int		remove_pending;
	struct list_head	file_ctx_list;
	daq_file_ctx_t	*file_ctx_pool;
	int		file_ctx_pool_size;
}daq_device_t;

//fops.c

int daq_file_open(struct inode *in, struct file *fp);
int daq_file_close(struct inode *inode, struct file *filp);
int daq_file_mmap(struct file *filp, struct vm_area_struct *vma);
long daq_file_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

//ao.c

void daq_ao_initialize_hw( daq_device_t *daq_dev );
int daq_ioctl_ao_write_sample( daq_device_t *daq_dev, unsigned long arg );
int daq_ioctl_ao_set_channel(daq_device_t *daq_dev, unsigned long arg);
//dio.c

void daq_dio_initialize_hw( daq_device_t *daq_dev );
int daq_ioctl_dio_set_port_dir( daq_device_t *daq_dev, unsigned long arg);
int daq_ioctl_di_read_port( daq_device_t *daq_dev, unsigned long arg );
int daq_ioctl_do_write_port( daq_device_t *daq_dev, unsigned long arg );
int daq_ioctl_do_read_port( daq_device_t *daq_dev, unsigned long arg );

//ai.c
void daq_ai_initialize_hw( daq_device_t *daq_dev );
int daq_ioctl_ai_read_sample( daq_device_t *daq_dev, unsigned long arg );
int daq_ioctl_ai_set_channel(daq_device_t *daq_dev, unsigned long arg);
int daq_ioctl_ai_calibrate( daq_device_t *daq_dev, unsigned long arg);


//device.c
int daq_device_fw_download(daq_device_t *daq_dev, unsigned long arg );
int daq_device_search(daq_device_t *daq_dev, unsigned long arg );


//
int add_task(daq_spi_transaction_t *cur, daq_task_info_t *task);


//spiwrapper.c
#define SPI_SND_PORT   0
#define SPI_RCV_PORT   1
#define SPI_SND_SPEED  8000000
#define SPI_RCV_SPEED  8000000
#define SPI_SND_MODE    1
#define SPI_RCV_MODE    1
#define SPI_SND_BITS    8
#define SPI_RCV_BITS    8
#define SPI_SND_DELAY   0
#define SPI_RCV_DELAY   0

struct file* spi_open( const char *name );
int spi_close( struct file *fp );
int spi_ioctl( struct file *filp, u32 cmd, unsigned long arg );
int spi_set_mode(struct file *filp, unsigned long mode);
int spi_send(struct file *filp, unsigned long arg);
int spi_receive(struct file *filp, unsigned long arg);


#endif
