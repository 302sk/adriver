#include "kdriver.h"




void daq_ai_initialize_hw( daq_device_t *daq_dev )
{

}

int daq_ioctl_ai_read_sample( daq_device_t *daq_dev, unsigned long arg )
{
   DEVICE_SHARED *shared = &daq_dev->shared;
   AI_READ_SAMPLES xbuf;
   __u32 module_id;
   __u16 chStart, chStop;
   SPI_PACKAGE pkg;
   SPI_PACKAGE pkg_rcv;
   CHANNEL_RANGE chl_rng;
   __u8 data[MODULE_DATA_MAX] = {0xaa};
   int i = 0;
   __u8 data_rcv[256] = {0};
   HEADER_INFO header_info_rcv;
   MODULE_INFO module_info_rcv;
   MODULE_DATA module_data_rcv;
   __u16 module_chksum = 0;
   __u16 module_len = 0;
   pkg_rcv.len = 256;
   pkg_rcv.data = data_rcv;
   
   

   if(unlikely(copy_from_user(&xbuf, (void*)arg, sizeof(xbuf)))){
      return -EFAULT;
   }

   module_id = xbuf.mdlNumber;
   chStart = xbuf.PhyChanStart;
   chStop = xbuf.PhyChanStart + xbuf.LogChanCount - 1;

   chl_rng.start_chl = 0;//chStart;
   chl_rng.stop_chl = 7;//chStop;

   printk(KERN_ALERT"kernel message: daq_ioctl_ai_read_sample. id = %d chStart = %d, chStop = %d\n", module_id, chStart, chStop);

   pkg.len = 256;//sizeof(HEADER_INFO) + sizeof(MODULE_INFO) + sizeof(MODULE_DATA) + 2; //+ check sum 2 byte
   printk(KERN_ALERT"header_info = %d, module_info = %d, module_data = %d chRng = %d\n", sizeof(HEADER_INFO), 
      sizeof(MODULE_INFO), sizeof(MODULE_DATA), sizeof(CHANNEL_RANGE));
   pkg.offset = 0;
   pkg.data = (__u8*)kzalloc(pkg.len, GFP_KERNEL);

//   init_snd_package(&pkg);  //alloc pkg->data

   add_header_info(&pkg, 1, header_com_common, 0);
   MODULE_INFO *info = add_module_info(&pkg, 1, 0);
   module_len = add_module_data(&pkg, comm_mode_read, comm_ai_rngcode, chl_rng,  data);
   info->data_len += module_len;
   add_module_chksum(&pkg, info);
   pkg.len = pkg.offset;

//   pkg.data[0] = 0;
//   pkg.data[1] = 10; //m4 delay 100 us

   printk(KERN_ALERT"package data: len = %d\n", pkg.offset);
   for(i=0; i<info->data_len + sizeof(HEADER_INFO); i++)
   {
      printk(KERN_ALERT"%x", pkg.data[i]);
   }
   printk(KERN_ALERT"package data end\n");

//   spi_send(daq_dev->spi_snd, &pkg); //send package

//   spi_receive(daq_dev->spi_rcv, &pkg_rcv); //receive package
   
//   printk(KERN_ALERT"receive data[2] = %x, data[3] = %x, data[33] = %x\n", pkg_rcv.data[2], pkg_rcv.data[3], pkg_rcv.data[33]);
/*
   if(PKG_ERROR != find_header(&pkg_rcv))
   {
      if(PKG_ERROR != get_header_info(&pkg_rcv, &header_info_rcv))
      {
         int count = 0;
         for(count = 0; count < header_info_rcv.data_len - sizeof(HEADER_INFO);)
         {
            int module_cnt = 0;
            int chk_sum = 0;
            chk_sum += get_module_info(&pkg_rcv, &module_info_rcv);
            count += module_info_rcv.data_len;
            for(module_cnt; module_cnt < module_info_rcv.data_len - sizeof(MODULE_INFO);)
            {
               __u16 real_data_len = 0;
               __u8 real_data[16] = {0};
               chk_sum += get_module_data(&pkg_rcv, &module_data_rcv);
               
               real_data_len = calc_data_len(PKG_DIR_RCV, module_data_rcv.command_type, module_data_rcv.command, module_data_rcv.channel_rng);
               chk_sum += get_data(&pkg_rcv, real_data, real_data_len);

               module_cnt += sizeof(MODULE_DATA) + real_data_len;
               //for()
            }
            if(chk_sum != get_module_chksum(&pkg_rcv))
            {
               printk(KERN_ALERT"module data check sum is incorrect!\n");
            }
            
         }
      }
   }
*/
   kfree(pkg.data);
	return 0;
}

int daq_ioctl_ai_set_channel( daq_device_t *daq_dev, unsigned long arg )
{
	return 0;
}
