#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/mm.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/kthread.h>

#include "kdriver.h"
#include "kshared.h"
#include "package.h"
#include <biosysfshelper.h>

MODULE_AUTHOR("Advantech");
MODULE_DESCRIPTION("Adam3660 IO Driver base on SPI bus.");

#ifndef SLEEP_MILLI_SEC
#define SLEEP_MILLI_SEC(nMilliSec) \
   do{ \
      long timeout = (nMilliSec)*HZ/1000; \
      while(timeout>0) \
      { \
         timeout = schedule_timeout(timeout); \
      } \
      }while(0);
#endif

#define next_transaction(cur) (cur==0 ? 1 : 0)

static daq_device_t *g_daq_dev;
 
#define SPI_SND "/dev/spidev1.0"
#define SPI_RCV "/dev/spidev2.0"

static struct file_operations daq_fops = {
	.owner	= THIS_MODULE,
	.open	= daq_file_open,
	.release= daq_file_close,
	.mmap     = daq_file_mmap,
	.unlocked_ioctl	= daq_file_ioctl,
};
#if 1
static int snd_thread(void *arg)
{
   daq_device_t *daq_dev = (daq_device_t *)arg;
   daq_spi_transaction_t *cur;
   SPI_PACKAGE pkg;
   __u8 data[256];
   __u8 real_data[16];
   __u16 real_data_len = 0;
   __u16 module_len = 0;
   int i = 0;
   CHANNEL_RANGE chl_rng;
   pkg.len = 256;
   pkg.offset = 0;
   pkg.data = data;
   
   while(!kthread_should_stop())
   {
      memset((__u8*)pkg.data, 0, 256);
      pkg.len = 256;
      pkg.offset = 0;
      
      cur = &(daq_dev->spi_transaction[daq_dev->curr_trsc]);
      //task list isn't empty, fire
      if( cur->task_count > 0) 
      {
         __u8 proc_mask = 0;
//         printk(KERN_ALERT"++++task_count = %d current transcation: %d\n", cur->task_count, daq_dev->curr_trsc);
         for(i=0; i < cur->task_count; i++)
         {
//            daq_trace((KERN_ALERT"++++module_id = %d, type = %x, cmd = %x, chl_rng = %x\n",cur->task_list[i].module_id, \
//                       cur->task_list[i].module_data.command_type, cur->task_list[i].module_data.command, cur->task_list[i].module_data.channel_rng.value));
         }
         spin_lock(&daq_dev->trsc_lock);  
         daq_dev->request = cur;
         cur->recv_count = 0;
         cur->msg_id += 1;
         if( cur->msg_id > 0xfffe)
         {
            cur->msg_id = 0;
         }
         daq_dev->curr_trsc = next_transaction(daq_dev->curr_trsc);  //change current transaction to collect task
         spin_unlock(&daq_dev->trsc_lock);
         //make package use current->task_list
         //////////////////////////////////////////////////////
         int module_count = 0;
         int total_len = 0;
         HEADER_INFO *head_info = add_header_info(&pkg, 1, header_com_common, 0);
//         daq_trace((KERN_ALERT"++++1. add_header_info\n"));
         ///////////////////////////preprocess task list////////////////////////////

         for(i = 0; i < cur->task_count; i++)
         {
            if( cur->task_list[i].header_cmd == header_com_search)
            {
               head_info->command = header_com_search;
               break;
            }else if( cur->task_list[i].header_cmd == header_com_download ){
               head_info->command = header_com_download;
            }
         }

         if(head_info->command != header_com_search)
         {
            /////////////////////////////////////////////////////////
            for(i = 0; i < cur->task_count; i++)           
            {
               int j = 0;
               int module_len = 0;
               if((head_info->command == header_com_download) && (cur->task_list[i].header_cmd != header_com_download)) //if there is a task with "download", skip those which is without "download"
                  continue;
               if(cur->task_list[i].module_id == 0)  //invalid id
                  continue;
               if(((1 << cur->task_list[i].module_id) & proc_mask)) //had been processed
                  continue;
               MODULE_INFO *module_info = add_module_info(&pkg, cur->task_list[i].module_id, 0);
               proc_mask |= (1 << cur->task_list[i].module_id); //record processed id
               module_count ++;
               daq_trace((KERN_ALERT"++++2. add_module_info: id = %d, proc_mask = 0x%x, count = %d, <<%x\n ", module_info->module_id, proc_mask, module_count,\
                  (1<<cur->task_list[i].module_id)&proc_mask));
               for(j = i; j < cur->task_count; j++)
               {
                  if(cur->task_list[j].module_id == module_info->module_id)
                  {
                     MODULE_DATA *module_data = &cur->task_list[j].module_data;
                     module_len += add_module_data(&pkg, module_data, cur->task_list[j].data, cur->task_list[j].len);
                     daq_trace((KERN_ALERT"++++3. add_module_data: module_len = %d, cmd = 0x%x, len = %d\n", module_len, module_data->command, cur->task_list[j].len));
                  }
               }
               module_info->data_len += module_len;  //add module len to module info
               add_module_chksum(&pkg, module_info);
               daq_trace((KERN_ALERT"++++4. add_module_chksum: moduleInfo_len = %d\n", module_info->data_len));
               total_len += module_info->data_len;  //total len in header info
            }
         }
         head_info->module_count = module_count;
         head_info->message_id = cur->msg_id;  //set message id
 //        head_info->command = header_com_download;//header_com_common;
         set_header_len(head_info, total_len + sizeof(HEADER_INFO));
         pkg.len = pkg.offset;
         
         daq_trace((KERN_ALERT"++++5. head_info->len = %d pkg offset = %d\n", head_info->data_len, pkg.offset));
#if 1
         printk(KERN_ALERT"++++package data: len = %d\n", pkg.offset);
         for(i=0; i<head_info->data_len; i++)
         {
            printk(KERN_ALERT"++++pkg.data[%d] = %x", i, pkg.data[i]);
         }
         printk(KERN_ALERT"++++package data end\n");
#endif
         /////////////////////////////////////////////////////
         //clear task list
         spin_lock(&daq_dev->trsc_lock);
         cur->task_count = 0;
         memset(cur->rcv_data, 0, 256);
         spin_unlock(&daq_dev->trsc_lock);

         uint32 pre = jiffies;

         spi_send(daq_dev->spi_snd, &pkg); 
         int ret = daq_event_wait(1, &cur->rsp_event, 1, 5000);  //wait data return from spi receive port or time out
         daq_trace((KERN_ALERT"++++wait %d ms, %d jiffies\n", jiffies_to_msecs(jiffies-pre), jiffies-pre));
  //       daq_trace((KERN_ALERT"++++daq_event_wait ret = %d\n", ret));
         if(ret == 0)  //not time out
         {
            daq_event_set(cur->cmd_event);  //make those processes waiting for this event go on
         }
      }

      SLEEP_MILLI_SEC(10);
//         struct timespec sleep;
//         sleep.tv_sec = 0;
//         sleep.tv_nsec = 500000;
//         nanosleep(CLOCK_REALTIME, 0, &sleep, NULL);

   }
}

static int rcv_thread(void *arg)
{
   daq_device_t *daq_dev = (daq_device_t *)arg;
   __u8 data[256];
   SPI_PACKAGE pkg_rcv;
   HEADER_INFO header_info_rcv;
   MODULE_INFO module_info_rcv;
   MODULE_DATA module_data_rcv;
   int i = 0;
   pkg_rcv.len = 256;
   pkg_rcv.offset = 0;
   pkg_rcv.data = data;
   
   while(!kthread_should_stop())
   {
      
      memset((__u8*)pkg_rcv.data, 0, 256);
      pkg_rcv.len = 256;
      pkg_rcv.offset = 0;
      spi_receive(daq_dev->spi_rcv, &pkg_rcv); //receive data
 //     memcpy(daq_dev->request->rcv_data, pkg_rcv.data, 256);     //cpy
      int rcv_data_offset = 0;

  //    printk(KERN_ALERT"----package data: len = %d\n", header_info_rcv.data_len);
//         for(i=0; i< 50; i++)
//         {
//            printk(KERN_ALERT"----pkg_rcv.data[%d] = %x", i, pkg_rcv.data[i]);
//         }
//      printk(KERN_ALERT"----package data end\n");

      if(PKG_ERROR == find_header(&pkg_rcv))   //there is no header in this package
      {
         continue;
      }
      if(PKG_ERROR == get_header_info(&pkg_rcv, &header_info_rcv))  //failed to get header info, check sum may be incorrect
      {
         continue;
      }
#if 1
      printk(KERN_ALERT"----package data: len = %d\n", header_info_rcv.data_len);
         for(i=0; i< header_info_rcv.data_len; i++)
         {
            printk(KERN_ALERT"----pkg_rcv.data[%d] = %x", i, pkg_rcv.data[i]);
         }
      printk(KERN_ALERT"----package data end\n");
#endif
       
      if(header_info_rcv.message_id == daq_dev->request->msg_id)
      {
         //response
         int count = 0;
 //        printk(KERN_ALERT"----2. header len = %d\n", header_info_rcv.data_len);
         for(count = 0; count < header_info_rcv.data_len - sizeof(HEADER_INFO);)
         {
            int module_cnt = 0;
            int chk_sum = 0;
            if(PKG_ERROR == get_module_info(&pkg_rcv, &module_info_rcv))  //check sum is not correct
            {
               count += module_info_rcv.data_len;
               continue;
            }
//            printk(KERN_ALERT"----3. module info id = %d, len = %d\n", module_info_rcv.module_id, module_info_rcv.data_len);
            count += module_info_rcv.data_len;       //move pointer to next module info
            for(module_cnt; module_cnt < module_info_rcv.data_len - sizeof(MODULE_INFO) - 2;)
            {
               __u16 real_data_len = 0;
               __u8 real_data[16] = {0};
               get_module_data(&pkg_rcv, &module_data_rcv);
//               printk(KERN_ALERT"----4. module data type = %x, cmd = %x, chl_rng = %x\n", module_data_rcv.command_type, module_data_rcv.command, module_data_rcv.channel_rng);
               
               real_data_len = calc_data_len(PKG_DIR_RCV, &module_data_rcv);
               get_data(&pkg_rcv, real_data, real_data_len);
               //////////////////push data to task list fro receive//////////////////////
               daq_dev->request->task_list_rcv[daq_dev->request->recv_count].module_id = module_info_rcv.module_id;
               daq_dev->request->task_list_rcv[daq_dev->request->recv_count].module_data = module_data_rcv;
               //memcpy(daq_dev->request->task_list[daq_dev->request->recv_count].data, real_data, real_data_len);
               memcpy(daq_dev->request->rcv_data + rcv_data_offset, real_data, real_data_len);
               daq_dev->request->task_list_rcv[daq_dev->request->recv_count].data = daq_dev->request->rcv_data + rcv_data_offset;
               daq_dev->request->task_list_rcv[daq_dev->request->recv_count].len = real_data_len;
               rcv_data_offset += real_data_len;
               daq_dev->request->recv_count ++;
               ////////////////////////////////
//               printk(KERN_ALERT"----5. real data len = %d, reques = %x,recv_count = %d\n", real_data_len, daq_dev->request, daq_dev->request->recv_count);
               module_cnt += (sizeof(MODULE_DATA) + real_data_len);
//               printk(KERN_ALERT"----6. module count = %d\n", module_cnt);
               //for()
            }
            
         }
         
         daq_event_set(daq_dev->request->rsp_event);
      }else{
         //upload
      }
   }
}

#else

#endif

//call this function after spi opened
static ErrorCode daq_shared_init(daq_device_t *daq_dev)
{
   //DEVICE_SHARED *shared = daq_dev->shared;

   //
   return Success;
}


static int __init daq_driver_init( void )
{
	dev_t		devid;
	daq_device_t	*daq_dev;
	DEVICE_SHARED	*shared;
	struct device	*sysfs_dev;
	size_t		mem_size;
	int		ret;

#define CHK_RESULT(_ok_, err, err_label) if(!(_ok_)) { ret = err; goto err_label; }
	// allocate private data structure
	mem_size = (sizeof(daq_device_t) + PAGE_SIZE - 1) & PAGE_MASK;
	daq_dev = (daq_device_t*)kzalloc(mem_size, GFP_KERNEL);
	CHK_RESULT(daq_dev, -ENOMEM, err_alloc_dev)

   g_daq_dev = daq_dev;
   daq_dev->spi_rcv_thrd = NULL;
   daq_dev->spi_snd_thrd = NULL;
	//initialize the private data in the device
	shared			= &daq_dev->shared;
	shared->size		= sizeof(*shared);
	shared->device_number	= 0;

	spin_lock_init( &daq_dev->dev_lock );
	
	daq_dev->spi_snd = spi_open(SPI_SND);
	CHK_RESULT(daq_dev->spi_snd != NULL, -ENODEV, err_snd_spi)
	daq_dev->spi_rcv = spi_open(SPI_RCV);	
	CHK_RESULT(daq_dev->spi_rcv != NULL, -ENODEV, err_rcv_spi)

   spi_set_mode(daq_dev->spi_snd, SPI_SND_PORT);
   spi_set_mode(daq_dev->spi_rcv, SPI_RCV_PORT);

   daq_dev->spi_transaction[0].cmd_event = daq_event_create();
   daq_dev->spi_transaction[0].rsp_event = daq_event_create();
   daq_dev->spi_transaction[1].cmd_event = daq_event_create();
   daq_dev->spi_transaction[1].rsp_event = daq_event_create();
   daq_dev->curr_trsc = 0;
   daq_dev->request = &daq_dev->spi_transaction[0];
   spin_lock_init(&daq_dev->trsc_lock);

   

   daq_dev->spi_rcv_thrd = kthread_run(rcv_thread, (void *)daq_dev, "spi_rcv");
   if(daq_dev->spi_rcv_thrd == NULL)
   {
      daq_trace((KERN_ALERT"Create spi receive thread failed!\n"));
   }
   daq_dev->spi_snd_thrd = kthread_run(snd_thread, (void *)daq_dev, "spi_snd");
   if(daq_dev->spi_snd_thrd == NULL)
   {
      daq_trace((KERN_ALERT"Create spi send thread failed\n"));
   }

	INIT_LIST_HEAD( &daq_dev->file_ctx_list);
	daq_dev->file_ctx_pool_size = (mem_size - sizeof(daq_device_t)) / sizeof(daq_file_ctx_t);
	if( daq_dev->file_ctx_pool_size){
		daq_dev->file_ctx_pool = (daq_file_ctx_t *)(daq_dev + 1);
	}else{
		daq_dev->file_ctx_pool = NULL;
	} 

	/*Get dynamic device number*/
	devid = daq_devid_alloc();
	CHK_RESULT(devid > (dev_t)0, -ENOSPC, err_no_devid)

	/*register our device into kernel*/
	cdev_init( &daq_dev->cdev, &daq_fops);
	daq_dev->cdev.owner = THIS_MODULE;
	ret = cdev_add(&daq_dev->cdev, devid, 1);
	CHK_RESULT(ret == 0, ret, err_cdev_add)

	/* register our own device in sysfs, and this will cause udev to create corresponding device node*/
	sysfs_dev = DAQ_SYSFS_INITIALIZE(devid, daq_dev);
	CHK_RESULT(!IS_ERR(sysfs_dev), PTR_ERR(sysfs_dev), err_sysfs_reg)
	
	//initialize the device
	daq_device_search(daq_dev, 0);  //search device

	return 0;	
err_sysfs_reg:
	cdev_del(&daq_dev->cdev);
err_cdev_add:
	daq_devid_free(devid);
err_no_devid:
	spi_close( daq_dev->spi_rcv );
err_rcv_spi:
	spi_close( daq_dev->spi_snd );
err_snd_spi:
	kfree(daq_dev);
err_alloc_dev:
	daq_trace((KERN_ALERT"Add 3660 IO driver failed\n"));
	return ret;
}

void daq_device_cleanup()
{
   //Delete device node under /dev
   cdev_del(&g_daq_dev->cdev);
   daq_devid_free(g_daq_dev->cdev.dev);
   device_destroy(daq_class_get(), g_daq_dev->cdev.dev);

   //Free the device information structure
   ClearPageReserved(virt_to_page((unsigned long)g_daq_dev));
}

static void __exit daq_driver_exit( void )
{
   daq_event_close(g_daq_dev->spi_transaction[0].cmd_event);
   daq_event_close(g_daq_dev->spi_transaction[0].rsp_event);
   daq_event_close(g_daq_dev->spi_transaction[1].cmd_event);
   daq_event_close(g_daq_dev->spi_transaction[1].rsp_event);

   if(g_daq_dev->spi_rcv_thrd != NULL)  //stop spi receive thread
   {
      kthread_stop(g_daq_dev->spi_rcv_thrd);
      g_daq_dev->spi_rcv_thrd = NULL;
   }
   if(g_daq_dev->spi_snd_thrd != NULL)
   {
      kthread_stop(g_daq_dev->spi_snd_thrd);
      g_daq_dev->spi_snd_thrd = NULL;
   }
	daq_trace((KERN_ALERT"good by vfs\n"));
	if( g_daq_dev->spi_snd != NULL ){
		spi_close( g_daq_dev->spi_snd );
	}
	if( g_daq_dev->spi_rcv != NULL ){
		spi_close( g_daq_dev->spi_rcv );
	}

   daq_device_cleanup();
}









module_init( daq_driver_init );
module_exit( daq_driver_exit );
MODULE_LICENSE("GPL");


