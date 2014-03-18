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

static int snd_thread(void *arg)
{
   daq_device_t *daq_dev = (daq_device_t *)arg;
   while(!kthread_should_stop())
   {
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

      memset(data, 0, sizeof(data));
      memset(real_data, 0, sizeof(real_data));
      
      HEADER_INFO *header = add_header_info(&pkg, 1, header_com_common, 0);
      MODULE_INFO *info = add_module_info(&pkg, 1, 0);
      chl_rng.start_chl = 0;
      chl_rng.stop_chl = 7;
      module_len += add_module_data(&pkg, comm_mode_read, comm_ai_rngcode, chl_rng,  real_data);  //add module data
      ////// add real data for testing protocol parse function /////////
      real_data_len = (chl_rng.stop_chl - chl_rng.start_chl + 1);    ///range code : 1byte per channel
      memcpy((__u8*)(pkg.data + pkg.offset), real_data, real_data_len);
      pkg.offset += real_data_len;                //+offset
      module_len += real_data_len;
      /////////////////////////////////////////////////////////////////
      module_len += add_module_data(&pkg, comm_mode_read | comm_st_error, comm_ai_data, chl_rng, real_data);  //add module data
      info->data_len += module_len;
      add_module_chksum(&pkg, info);
      set_header_len(header, info->data_len + sizeof(HEADER_INFO));
      pkg.len = pkg.offset;

      printk(KERN_ALERT"package data: len = %d\n", pkg.offset);
      for(i=0; i<info->data_len + sizeof(HEADER_INFO); i++)
      {
         printk(KERN_ALERT"pkg.data[%d] = %x", i, pkg.data[i]);
      }
      printk(KERN_ALERT"package data end\n");
      spi_send(daq_dev->spi_snd, &pkg);
      SLEEP_MILLI_SEC(1000);
   }
   return 0;
}

static int rcv_thread(void *arg)
{
   __u8 data[256];
   SPI_PACKAGE pkg_rcv;
   HEADER_INFO header_info_rcv;
   MODULE_INFO module_info_rcv;
   MODULE_DATA module_data_rcv;
   daq_device_t *daq_dev = (daq_device_t *)arg;
   int i = 0;
   pkg_rcv.len = 256;
   pkg_rcv.offset = 0;
   pkg_rcv.data = data;
   while(!kthread_should_stop())
   {
         memset((__u8*)pkg_rcv.data, 0, 256);
         pkg_rcv.len = 256;
         pkg_rcv.offset = 0;
         spi_receive(daq_dev->spi_rcv, &pkg_rcv);  //get data from spi
        // printk(KERN_ALERT"receive data[2] = %x, data[3] = %x, data[18] = %x, data[27] = %x\n", pkg_rcv.data[2], pkg_rcv.data[3], pkg_rcv.data[18], pkg_rcv.data[27]);
        for(i=0;i<32;i++)
        {
         printk(KERN_ALERT"-------------rcv_data[%d] = %x\n", i, pkg_rcv.data[i]);
        }


         if(PKG_ERROR != find_header(&pkg_rcv))
         {
            printk(KERN_ALERT"++++1.find header\n");
            if(PKG_ERROR != get_header_info(&pkg_rcv, &header_info_rcv))
            {
               int count = 0;
               printk(KERN_ALERT"++++2. header len = %d\n", header_info_rcv.data_len);
               for(count = 0; count < header_info_rcv.data_len - sizeof(HEADER_INFO);)
               {
                  int module_cnt = 0;
                  int chk_sum = 0;
                  if(PKG_ERROR == get_module_info(&pkg_rcv, &module_info_rcv))  //check sum is not correct
                  {
                     count += module_info_rcv.data_len;
                     continue;
                  }
                  printk(KERN_ALERT"++++3. module info id = %d, len = %d\n", module_info_rcv.module_id, module_info_rcv.data_len);
                  count += module_info_rcv.data_len;       //move pointer to next module info
                  for(module_cnt; module_cnt < module_info_rcv.data_len - sizeof(MODULE_INFO) - 2;)
                  {
                     __u16 real_data_len = 0;
                     __u8 real_data[16] = {0};
                     get_module_data(&pkg_rcv, &module_data_rcv);
                     printk(KERN_ALERT"++++4. module data type = %x, cmd = %x, chl_rng = %x\n", module_data_rcv.command_type, module_data_rcv.command, module_data_rcv.channel_rng);
                     
                     real_data_len = calc_data_len(PKG_DIR_RCV, module_data_rcv.command_type, module_data_rcv.command, module_data_rcv.channel_rng);
                     get_data(&pkg_rcv, real_data, real_data_len);
                     printk(KERN_ALERT"++++5. real data len = %d\n", real_data_len);
                     module_cnt += sizeof(MODULE_DATA) + real_data_len;
                     printk(KERN_ALERT"++++6. module count = %d\n", module_cnt);
                     //for()
                  }
                  
               }
            }
         }

//      SLEEP_MILLI_SEC(1000);
   }
   return 0;
}

//call this function after spi opened
static ErrorCode daq_shared_init(daq_device_t *daq_dev)
{
   //DEVICE_SHARED *shared = daq_dev->shared;

   //
   return Success;
}

static ErrorCode daq_shared_init_debug(daq_device_t *daq_dev)
{
   uint32 i;
   DEVICE_SHARED *shared = &daq_dev->shared;
   memset(shared->mdlFuncInfo,0,sizeof(shared->mdlFuncInfo));
   
   shared->mdlFuncInfo[0].mdlNumber = 0;
   strcpy(shared->mdlFuncInfo[0].mdlName, "module0");
   shared->mdlFuncInfo[0].funcInfo[module_func_ai].chlCount = 8;
   shared->mdlFuncInfo[0].funcInfo[module_func_ai].funcType = module_func_ai;
   for(i=0; i<8; i++)
   {
      shared->mdlFuncInfo[0].funcInfo[module_func_ai].gainCode[i] = 0;
   }
   
   shared->mdlFuncInfo[1].mdlNumber = 1;
   strcpy(shared->mdlFuncInfo[1].mdlName, "module1");
   printk(KERN_ALERT"%s\n", shared->mdlFuncInfo[1].mdlName);
   shared->mdlFuncInfo[1].funcInfo[module_func_ao].chlCount = 8;
   shared->mdlFuncInfo[1].funcInfo[module_func_ao].funcType = module_func_ao;
   for(i=0; i<8; i++)
   {
      shared->mdlFuncInfo[1].funcInfo[module_func_ao].gainCode[i] = 1;
   }
   shared->mdlFuncInfo[1].funcInfo[module_func_di].chlCount = 8;
   shared->mdlFuncInfo[1].funcInfo[module_func_di].funcType = module_func_di;

   shared->mdlFuncInfo[2].mdlNumber = 255;  //init with invalid module number
   shared->mdlFuncInfo[3].mdlNumber = 255;
   shared->mdlFuncInfo[4].mdlNumber = 255;

   __u8 data[MODULE_DATA_MAX] = {0};
   AI_READ_SAMPLES xbuf = {1, 0, 8, data};
   __u32 module_id;
   __u16 chStart, chStop;
   SPI_PACKAGE pkg;
   CHANNEL_RANGE chl_rng;
   printk(KERN_ALERT"kernel message: daq_ioctl_ai_read_sample\n");

//   if(unlikely(copy_from_user(&xbuf, (void*)arg, sizeof(xbuf)))){
//      return -EFAULT;
//   }

   module_id = xbuf.mdlNumber;
   chStart = xbuf.PhyChanStart;
   chStop = xbuf.PhyChanStart + xbuf.LogChanCount - 1;

   pkg.len = sizeof(HEADER_INFO) + sizeof(MODULE_INFO) + sizeof(MODULE_DATA);
   pkg.offset = 0;
   pkg.data = (__u8*)kzalloc(pkg.len, GFP_KERNEL);

   add_header_info(&pkg, 1, header_com_common, 0);
   add_module_info(&pkg, module_id, sizeof(MODULE_DATA));
   chl_rng.start_chl = chStart;
   chl_rng.stop_chl = chStop;
   add_module_data(&pkg, comm_mode_read, comm_ai_data, chl_rng,  data);

//   spi_send(daq_dev->spi_snd, &pkg);

//   spi_receive(daq_dev->spi_rcv, &pkg);
   
   kfree(pkg.data);
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

   daq_shared_init_debug(daq_dev); 

   daq_dev->spi_rcv_thrd = kthread_run(rcv_thread, (void *)daq_dev, "spi_rcv");
   if(daq_dev->spi_rcv_thrd == NULL)
   {
      printk(KERN_ALERT"Create spi receive thread failed!\n");
   }
   daq_dev->spi_snd_thrd = kthread_run(snd_thread, (void *)daq_dev, "spi_snd");
   if(daq_dev->spi_snd_thrd == NULL)
   {
      printk(KERN_ALERT"Create spi send thread failed\n");
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
/*
	g_spisnd = spi_open( SPI_SND );
	if(g_spisnd == NULL){
		printk(KERN_ALERT"Open SPI device for send failed\n");
	}else{
		printk(KERN_ALERT"Open sending spi device successfully, g_spisnd = %x\n", g_spisnd);
	}
	g_spircv = spi_open( SPI_RCV );
	if(g_spircv == NULL){
		printk(KERN_ALERT"Open SPI device for recieve failed\n");
	}else{
		printk(KERN_ALERT"Open recieving spi device successfully, g_spircv = %x\n", g_spircv);
	}
*/

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
	printk(KERN_ALERT"good by vfs\n");
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


