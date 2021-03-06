#include "kdriver.h"




void daq_ai_initialize_hw( daq_device_t *daq_dev )
{

}

int daq_ioctl_ai_calibrate( daq_device_t *daq_dev, unsigned long arg)
{
   DEVICE_SHARED *shared = &daq_dev->shared;
   daq_spi_transaction_t * cur;
   daq_task_info_t task;
   AI_CALI_CMD xbuf;
   CHANNEL_RANGE chl_rng;
   int i = 0;
   MODULE_INFO module_info;
   int evt_ret = 0;
   unsigned char expected_ret = 0;

   if(unlikely(copy_from_user(&xbuf, (void*)arg, sizeof(xbuf)))){
      return -EFAULT;
   }
 
   task.module_id = xbuf.mdlNumber;
   if(xbuf.caliType == 0x55 || xbuf.caliType == 0xaa){
      task.module_data.command = comm_ai_cal;
   }else{
      task.module_data.command = comm_ai_caltofac;
   }
   
   if(xbuf.getResult){   
      task.module_data.command_type = comm_mode_read;
      expected_ret = xbuf.caliType;
      task.len = 0;
   }else{
      task.module_data.command_type = comm_mode_write;
      task.len = 1; 
   }
   task.header_cmd = header_com_common;
   task.module_data.channel_rng.value = 0;
   task.data = &xbuf.caliType;

   spin_lock(&daq_dev->trsc_lock);
   cur = &daq_dev->spi_transaction[daq_dev->curr_trsc];
   add_task(cur, &task);
   spin_unlock(&daq_dev->trsc_lock);
   

   uint32 pre = jiffies;
   evt_ret = daq_event_wait(1, &cur->cmd_event, 1, 11000);
   daq_trace((KERN_ALERT"****calibration wait %d ms\n", jiffies_to_msecs(jiffies-pre)));

   if( evt_ret != 0)
   {
      daq_trace((KERN_ALERT"****pid = calibration: wait until time out\n", current->pid));
      return -EBUSY;
   }
   for(i = 0; i<cur->recv_count; i++)
   {

      if(cur->task_list_rcv[i].module_id == xbuf.mdlNumber 
         && ((cur->task_list_rcv[i].module_data.command == comm_ai_cal) || (cur->task_list_rcv[i].module_data.command == comm_ai_caltofac))
         && cur->task_list_rcv[i].module_data.command_type == (comm_mode_read | comm_dir_res))
      {
         daq_trace((KERN_ALERT"****Response data are found out!\n"));
         printk(KERN_ALERT"****cur->task_list_rcv[%d].data[0] = %x\n", i, cur->task_list_rcv[i].data[0]);
         if(cur->task_list_rcv[i].data[0] == expected_ret){
            return 0;
         }else{
            return -EBUSY;
         }
         /*
         if(unlikely(copy_to_user((void *)xbuf.Data, cur->task_list_rcv[i].data, xbuf.LogChanCount * 2))){
            return -EFAULT;
         }else{
            return 0;
         }
         */
      }
   }
   
   return 0;
}

int daq_ioctl_ai_read_sample( daq_device_t *daq_dev, unsigned long arg )
{
   DEVICE_SHARED *shared = &daq_dev->shared;
   daq_spi_transaction_t * cur;
   daq_task_info_t task;
   AI_READ_SAMPLES xbuf;
   CHANNEL_RANGE chl_rng;
   int i = 0;
   MODULE_INFO module_info;
   int evt_ret = 0;

   if(unlikely(copy_from_user(&xbuf, (void*)arg, sizeof(xbuf)))){
      return -EFAULT;
   }

   chl_rng.start_chl = xbuf.PhyChanStart;
   chl_rng.stop_chl = xbuf.PhyChanStart + xbuf.LogChanCount - 1;

   task.module_id = xbuf.mdlNumber;
   task.module_data.command = comm_ai_data;
   task.module_data.command_type = comm_mode_read;
   task.module_data.channel_rng = chl_rng;
   task.header_cmd = header_com_common;
   task.len = 0; // calculate by add_module_data

   spin_lock(&daq_dev->trsc_lock);
   cur = &daq_dev->spi_transaction[daq_dev->curr_trsc];
   daq_trace((KERN_ALERT"****CUR = %x\n", cur));
   add_task(cur, &task);
   spin_unlock(&daq_dev->trsc_lock);
   

   uint32 pre = jiffies;
   evt_ret = daq_event_wait(1, &cur->cmd_event, 1, 3000);
   daq_trace((KERN_ALERT"****wait %d ms\n", jiffies_to_msecs(jiffies-pre)));
   daq_trace((KERN_ALERT"****cur = %x cur->recv_count = %d\n", cur, cur->recv_count));

   if( evt_ret != 0)
   {
      daq_trace((KERN_ALERT"****pid = wait until time out\n", current->pid));
      return -EBUSY;
   }
   for(i = 0; i<cur->recv_count; i++)
   {

      if(cur->task_list_rcv[i].module_id == xbuf.mdlNumber 
         && cur->task_list_rcv[i].module_data.command == comm_ai_data
         && cur->task_list_rcv[i].module_data.channel_rng.start_chl == chl_rng.start_chl
         && cur->task_list_rcv[i].module_data.channel_rng.stop_chl == chl_rng.stop_chl)
      {
         daq_trace((KERN_ALERT"****Response data are found out!\n"));
         if(unlikely(copy_to_user((void *)xbuf.Data, cur->task_list_rcv[i].data, xbuf.LogChanCount * 2))){
            return -EFAULT;
         }else{
            return 0;
         }
      }
   }
   

	return 0;
}

int daq_ioctl_ai_set_channel( daq_device_t *daq_dev, unsigned long arg )
{

   DEVICE_SHARED *shared = &daq_dev->shared;
   daq_spi_transaction_t * cur;
   __u8 vrg[8];
   daq_task_info_t task;
   AI_SET_CHAN xbuf;
   __u32 module_id;
   CHANNEL_RANGE chl_rng;
   int i = 0;
   MODULE_INFO module_info;
   int evt_ret = 0;

   if(unlikely(copy_from_user(&xbuf, (void*)arg, sizeof(xbuf)))){
      return -EFAULT;
   }
   if(xbuf.SetFlag)
   {
      if(unlikely(copy_from_user(vrg, (void*)xbuf.data, xbuf.PhyChanCount)))
         return -EFAULT;
   }

   chl_rng.start_chl = xbuf.PhyChanStart;
   chl_rng.stop_chl = xbuf.PhyChanStart + xbuf.PhyChanCount - 1;

   task.module_id = xbuf.mdlNumber;
   task.module_data.command = comm_ai_rngcode;
   task.module_data.command_type = xbuf.SetFlag ? comm_mode_write : comm_mode_read;
   task.header_cmd = header_com_common;
   task.module_data.channel_rng = chl_rng;
   task.data = vrg;
   task.len = 0;

   daq_trace((KERN_ALERT"****id = %d, cmd_type=%x, start_chl=%d, stop_chl=%d\n", task.module_id, task.module_data.command_type, \
                  task.module_data.channel_rng.start_chl, task.module_data.channel_rng.stop_chl));

   spin_lock(&daq_dev->trsc_lock);
   cur = &daq_dev->spi_transaction[daq_dev->curr_trsc];
   add_task(cur, &task);
   spin_unlock(&daq_dev->trsc_lock);

   evt_ret = daq_event_wait(1, &cur->cmd_event, 1, 3000);

   if( evt_ret != 0)  //wait event timeout
   {
      daq_trace((KERN_ALERT"****Access value range timeout!\n", current->pid));
      return -EBUSY;
   }

   printk(KERN_ALERT"****cur->recv_count = %d\n", cur->recv_count);
   
   for(i = 0; i<cur->recv_count; i++)
   {
      printk(KERN_ALERT"****id=%d, cmd=%x, start=%x, stop=%x\n", cur->task_list_rcv[i].module_id, \
               cur->task_list_rcv[i].module_data.command,\
               cur->task_list_rcv[i].module_data.channel_rng.start_chl,\
               cur->task_list_rcv[i].module_data.channel_rng.stop_chl);

      if((cur->task_list_rcv[i].module_id == xbuf.mdlNumber) 
         && (cur->task_list_rcv[i].module_data.command == comm_ai_rngcode)
         && (cur->task_list_rcv[i].module_data.channel_rng.start_chl == chl_rng.start_chl)
         && (cur->task_list_rcv[i].module_data.channel_rng.stop_chl == chl_rng.stop_chl))
      {
         printk("****error = %x\n", cur->task_list_rcv[i].module_data.command_type);
         if(cur->task_list_rcv[i].module_data.command_type & comm_st_error)
         {
            printk(KERN_ALERT"****Access module %d value range error %x\n", xbuf.mdlNumber, cur->task_list_rcv[i].data[0]);
            return -EFAULT;
         }
         if(!xbuf.SetFlag)
         {
            printk(KERN_ALERT"****data[%d] = %x\n", i, cur->task_list_rcv[i].data[0]);
            if(unlikely(copy_to_user((void *)xbuf.data, cur->task_list_rcv[i].data, xbuf.PhyChanCount)))  //copy value range to user
            {
               printk(KERN_ALERT"****Access value range error\n");
               return -EFAULT;
            }
            printk(KERN_ALERT"****len = %d, %x, %x, %x\n", cur->task_list_rcv[i].len, cur->task_list_rcv[i].data[0], cur->task_list_rcv[i].data[1], cur->task_list_rcv[i].data[2]);
         }else{
            return 0;
         }
      }
   }
   
	return 0;
}
