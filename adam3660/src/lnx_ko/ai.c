#include "kdriver.h"




void daq_ai_initialize_hw( daq_device_t *daq_dev )
{

}

int daq_ioctl_ai_read_sample( daq_device_t *daq_dev, unsigned long arg )
{
   DEVICE_SHARED *shared = &daq_dev->shared;
   daq_spi_transaction_t * cur;
   daq_task_info_t task;
   AI_READ_SAMPLES xbuf;
   __u32 module_id;
   CHANNEL_RANGE chl_rng;
   int i = 0;
   MODULE_INFO module_info;
   int evt_ret = 0;

   
   

   if(unlikely(copy_from_user(&xbuf, (void*)arg, sizeof(xbuf)))){
      return -EFAULT;
   }

   module_id = xbuf.mdlNumber;
   chl_rng.start_chl = xbuf.PhyChanStart;
   chl_rng.stop_chl = xbuf.PhyChanStart + xbuf.LogChanCount - 1;

   task.module_id = module_id;
   task.module_data.command = comm_ai_data;
   task.module_data.command_type = comm_mode_read;
   task.module_data.channel_rng = chl_rng;
   //memcpy(task.data, xbuf.Data, xbuf.LogChanCount * 2); ///2 byte per ai value

//   printk(KERN_ALERT"kernel message: daq_ioctl_ai_read_sample. id = %d chStart = %d, chStop = %d\n", module_id, chl_rng.start_chl, chl_rng.stop_chl);
   
   spin_lock(&daq_dev->trsc_lock);
   cur = &daq_dev->spi_transaction[daq_dev->curr_trsc];

   task.module_id = 1;
   task.module_data.command = comm_ai_rngcode;
   task.module_data.command_type = comm_mode_read;
   task.header_cmd = header_com_search;
   chl_rng.start_chl = 0;
   chl_rng.stop_chl = 7;
   task.module_data.channel_rng = chl_rng;
   task.len = 0; // calculate by add_module_data
   add_task(cur, &task);
   spin_unlock(&daq_dev->trsc_lock);

   uint32 pre = jiffies;
   evt_ret = daq_event_wait(1, &cur->cmd_event, 1, 5000);
   daq_trace((KERN_ALERT"****wait %d ms\n", jiffies_to_msecs(jiffies-pre)));
   daq_trace((KERN_ALERT"****cur = %x cur->recv_count = %d\n", cur, cur->recv_count));

   if( evt_ret == 0)
   {
      daq_trace((KERN_ALERT"****pid = %x,  %d ticks/ms go on\n", current->pid, msecs_to_jiffies(1)));
   }else{      
      daq_trace((KERN_ALERT"****pid = wait until time out\n", current->pid));
//      return -EBUSY;
   }
   for(i = 0; i<cur->recv_count; i++)
   {
//      daq_trace((KERN_ALERT"****module id = %d, cmd = %x\n", cur->task_list[i].module_id, cur->task_list[i].module_data.command));
      if(//cur->task_list_rcv[i].module_id == xbuf.mdlNumber 
         cur->task_list_rcv[i].module_data.command == header_com_search)
 //        && cur->task_list_rcv[i].module_data.channel_rng.start_chl == chl_rng.start_chl
 //        && cur->task_list_rcv[i].module_data.channel_rng.stop_chl == chl_rng.stop_chl)
      {
         daq_trace((KERN_ALERT"****Response data are found out!\n"));
         daq_trace((KERN_ALERT"****command type = %x, cmd = %x, len = %d, data[0]=%x, data[7]=%x\n", cur->task_list_rcv[i].module_data.command_type, \
                           cur->task_list_rcv[i].module_data.command, cur->task_list_rcv[i].len, cur->task_list_rcv[i].data[0], cur->task_list_rcv[i].data[7]));
//         if(unlikely(copy_to_user((void *)xbuf.Data, cur->task_list[i].data, xbuf.LogChanCount * 2)))
//         {
//            return -EFAULT;
//         }
      }
   }
   

	return 0;
}

int daq_ioctl_ai_set_channel( daq_device_t *daq_dev, unsigned long arg )
{
	return 0;
}
