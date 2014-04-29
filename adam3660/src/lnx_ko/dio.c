#include "kdriver.h"

void daq_dio_initialize_hw( daq_device_t *daq_dev )
{

}

int daq_ioctl_dio_set_port_dir( daq_device_t *daq_dev, unsigned long arg )
{
	return 0;
}

int daq_ioctl_di_read_port( daq_device_t *daq_dev, unsigned long arg )
{
   printk(KERN_ALERT"kernel message: daq_ioctl_di_read_sample\n");
   DEVICE_SHARED *shared = &daq_dev->shared;
   daq_spi_transaction_t * cur;
   daq_task_info_t task;
   DIO_RW_PORTS xbuf;
   CHANNEL_RANGE chl_rng;
   int i = 0;
   MODULE_INFO module_info;
   int evt_ret = 0;

   if(unlikely(copy_from_user(&xbuf, (void*)arg, sizeof(xbuf)))){
      return -EFAULT;
   }

   chl_rng.start_chl = xbuf.PortStart;
   chl_rng.stop_chl = xbuf.PortStart + xbuf.PortCount - 1;

   task.module_id = xbuf.mdlNumber;
   task.module_data.command = comm_di_data;
   task.module_data.command_type = comm_mode_read;
   task.module_data.channel_rng = chl_rng;
   task.header_cmd = header_com_common;
   task.len = 0; // calculate by add_module_data

   spin_lock(&daq_dev->trsc_lock);
   cur = &daq_dev->spi_transaction[daq_dev->curr_trsc];
   add_task(cur, &task);
   spin_unlock(&daq_dev->trsc_lock);
   

   uint32 pre = jiffies;
   evt_ret = daq_event_wait(1, &cur->cmd_event, 1, 1100);
   daq_trace((KERN_ALERT"****wait %d ms\n", jiffies_to_msecs(jiffies-pre)));
   daq_trace((KERN_ALERT"****cur = %x cur->recv_count = %d\n", cur, cur->recv_count));

   if( evt_ret == 0)
   {
      daq_trace((KERN_ALERT"****pid = %x,  %d ticks/ms go on\n", current->pid, msecs_to_jiffies(1)));
   }else{      
      daq_trace((KERN_ALERT"****pid = wait until time out\n", current->pid));
      return -EBUSY;
   }
   for(i = 0; i<cur->recv_count; i++)
   {

      if(cur->task_list_rcv[i].module_id == xbuf.mdlNumber 
         && cur->task_list_rcv[i].module_data.command == comm_di_data
         && cur->task_list_rcv[i].module_data.channel_rng.start_chl == chl_rng.start_chl
         && cur->task_list_rcv[i].module_data.channel_rng.stop_chl == chl_rng.stop_chl)
      {
         daq_trace((KERN_ALERT"****Response data are found out!\n"));
       //  daq_trace((KERN_ALERT"****command type = %x, cmd = %x, len = %d, data[0]=%x, data[7]=%x\n", cur->task_list_rcv[i].module_data.command_type, \
       //                    cur->task_list_rcv[i].module_data.command, cur->task_list_rcv[i].len, cur->task_list_rcv[i].data[0], cur->task_list_rcv[i].data[7]));
         if(unlikely(copy_to_user((void *)xbuf.Data, cur->task_list_rcv[i].data, cur->task_list_rcv[i].len)))
         {
            return -EFAULT;
         }else{
            return 0;
         }
      }
   }
   

	return 0;
}

int daq_ioctl_do_write_port( daq_device_t *daq_dev, unsigned long arg )
{
   printk(KERN_ALERT"kernel message: daq_ioctl_do_write_sample\n");
   DEVICE_SHARED *shared = &daq_dev->shared;
   daq_spi_transaction_t * cur;
   daq_task_info_t task;
   DIO_RW_PORTS xbuf;
   CHANNEL_RANGE chl_rng;
   int i = 0;
   MODULE_INFO module_info;
   int evt_ret = 0;
   __u8 do_data[2];
   int do_data_len = 0;

   if(unlikely(copy_from_user(&xbuf, (void*)arg, sizeof(xbuf)))){
      return -EFAULT;
   }

   chl_rng.start_chl = xbuf.PortStart;
   chl_rng.stop_chl = xbuf.PortStart + xbuf.PortCount - 1;

   task.module_id = xbuf.mdlNumber;
   task.module_data.command = comm_do_data;
   task.module_data.command_type = comm_mode_write;
   task.module_data.channel_rng = chl_rng;
   task.header_cmd = header_com_common;
   task.len = 0; // calculate by add_module_data

   do_data_len = calc_data_len(PKG_DIR_SND, &task.module_data);
   if(unlikely(copy_from_user(do_data, xbuf.Data, do_data_len))){
      return -EFAULT;
   }
   task.data = do_data;
   
   spin_lock(&daq_dev->trsc_lock);
   cur = &daq_dev->spi_transaction[daq_dev->curr_trsc];
   add_task(cur, &task);
   spin_unlock(&daq_dev->trsc_lock);
   

   uint32 pre = jiffies;
   evt_ret = daq_event_wait(1, &cur->cmd_event, 1, 1100);
   daq_trace((KERN_ALERT"****wait %d ms\n", jiffies_to_msecs(jiffies-pre)));
   daq_trace((KERN_ALERT"****cur = %x cur->recv_count = %d\n", cur, cur->recv_count));

   if( evt_ret == 0)
   {
      daq_trace((KERN_ALERT"****pid = %x,  %d ticks/ms go on\n", current->pid, msecs_to_jiffies(1)));
   }else{      
      daq_trace((KERN_ALERT"****pid = wait until time out\n", current->pid));
      return -EBUSY;
   }
   for(i = 0; i<cur->recv_count; i++)
   {

      if(cur->task_list_rcv[i].module_id == xbuf.mdlNumber 
         && cur->task_list_rcv[i].module_data.command == comm_do_data
         && cur->task_list_rcv[i].module_data.channel_rng.start_chl == chl_rng.start_chl
         && cur->task_list_rcv[i].module_data.channel_rng.stop_chl == chl_rng.stop_chl)
      {
         daq_trace((KERN_ALERT"****Response data are found out!\n"));
       //  daq_trace((KERN_ALERT"****command type = %x, cmd = %x, len = %d, data[0]=%x, data[7]=%x\n", cur->task_list_rcv[i].module_data.command_type, \
       //                    cur->task_list_rcv[i].module_data.command, cur->task_list_rcv[i].len, cur->task_list_rcv[i].data[0], cur->task_list_rcv[i].data[7]));
            return 0;
         
      }
   }
   
	return -EFAULT;
}

int daq_ioctl_do_read_port( daq_device_t *daq_dev, unsigned long arg )
{
   printk(KERN_ALERT"kernel message: daq_ioctl_do_read_sample\n");
	DEVICE_SHARED *shared = &daq_dev->shared;
   daq_spi_transaction_t * cur;
   daq_task_info_t task;
   DIO_RW_PORTS xbuf;
   CHANNEL_RANGE chl_rng;
   int i = 0;
   MODULE_INFO module_info;
   int evt_ret = 0;

   if(unlikely(copy_from_user(&xbuf, (void*)arg, sizeof(xbuf)))){
      return -EFAULT;
   }

   chl_rng.start_chl = xbuf.PortStart;
   chl_rng.stop_chl = xbuf.PortStart + xbuf.PortCount - 1;

   task.module_id = xbuf.mdlNumber;
   task.module_data.command = comm_do_data;
   task.module_data.command_type = comm_mode_read;
   task.module_data.channel_rng = chl_rng;
   task.header_cmd = header_com_common;
   task.len = 0; // calculate by add_module_data

   spin_lock(&daq_dev->trsc_lock);
   cur = &daq_dev->spi_transaction[daq_dev->curr_trsc];
   add_task(cur, &task);
   spin_unlock(&daq_dev->trsc_lock);
   

   uint32 pre = jiffies;
   evt_ret = daq_event_wait(1, &cur->cmd_event, 1, 1100);
   daq_trace((KERN_ALERT"****wait %d ms\n", jiffies_to_msecs(jiffies-pre)));
   daq_trace((KERN_ALERT"****cur = %x cur->recv_count = %d\n", cur, cur->recv_count));

   if( evt_ret == 0)
   {
      daq_trace((KERN_ALERT"****pid = %x,  %d ticks/ms go on\n", current->pid, msecs_to_jiffies(1)));
   }else{      
      daq_trace((KERN_ALERT"****pid = wait until time out\n", current->pid));
      return -EBUSY;
   }
   for(i = 0; i<cur->recv_count; i++)
   {

      if(cur->task_list_rcv[i].module_id == xbuf.mdlNumber 
         && cur->task_list_rcv[i].module_data.command == comm_do_data
         && cur->task_list_rcv[i].module_data.channel_rng.start_chl == chl_rng.start_chl
         && cur->task_list_rcv[i].module_data.channel_rng.stop_chl == chl_rng.stop_chl)
      {
         daq_trace((KERN_ALERT"****Response data are found out!\n"));
       //  daq_trace((KERN_ALERT"****command type = %x, cmd = %x, len = %d, data[0]=%x, data[7]=%x\n", cur->task_list_rcv[i].module_data.command_type, \
       //                    cur->task_list_rcv[i].module_data.command, cur->task_list_rcv[i].len, cur->task_list_rcv[i].data[0], cur->task_list_rcv[i].data[7]));
         if(unlikely(copy_to_user((void *)xbuf.Data, cur->task_list_rcv[i].data, cur->task_list_rcv[i].len)))
         {
            return -EFAULT;
         }else{
            return 0;
         }
      }
   }
   

	return 0;
}

