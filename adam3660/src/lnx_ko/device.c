#include "kdriver.h"

int daq_device_fw_download(daq_device_t *daq_dev, unsigned long arg )
{
   DEVICE_SHARED *shared = &daq_dev->shared;
   daq_spi_transaction_t * cur;
   DEVICE_FIRMWARE_DOWNLOAD xbuf;
   daq_task_info_t task;
   __u8 data[64];
   int ret = 0;
   
   if(unlikely(copy_from_user(&xbuf, (void *)arg, sizeof(xbuf))))
   {
      return -EFAULT;
   }
   if(unlikely(copy_from_user(data, (void *)xbuf.data, xbuf.len)))
   {
      return -EFAULT;
   }

   task.module_id = xbuf.mdlNumber;
   task.module_data.command = xbuf.cmd;
   task.module_data.command_type = comm_mode_write;
   task.module_data.channel_rng.value = xbuf.index;
   task.header_cmd = header_com_download;
//   memcpy(task.data, data, xbuf.len);
   task.data = data;
   task.len = xbuf.len;

   spin_lock(&daq_dev->trsc_lock);
   cur = &daq_dev->spi_transaction[daq_dev->curr_trsc];
   add_task(cur, &task);
   spin_unlock(&daq_dev->trsc_lock);

   uint32 pre = jiffies;
   ret = daq_event_wait(1, &cur->cmd_event, 1, 200);
//   daq_trace((KERN_ALERT"****wait %d ms\n", jiffies_to_msecs(jiffies-pre)));

   if( ret == 0)
   {
      daq_trace((KERN_ALERT"****pid = %x,  %d ticks/ms go on\n", current->pid, msecs_to_jiffies(1)));
   }else{      
      daq_trace((KERN_ALERT"****pid = wait until time out\n", current->pid));
//      return -EBUSY;
   }


   daq_trace((KERN_ALERT"**** cur = %x, recv_count = %d\n", cur, cur->recv_count));

   int i = 0;
   for(i = 0; i<cur->recv_count; i++)
   {
      daq_trace((KERN_ALERT"****module id = %d, cmd = %x\n", cur->task_list_rcv[i].module_id, cur->task_list_rcv[i].module_data.command));
      if((cur->task_list_rcv[i].module_id == xbuf.mdlNumber) 
         && (cur->task_list_rcv[i].module_data.command == 2))
 //        && cur->task_list_rcv[i].module_data.channel_rng.value == xbuf.index)
      {
         daq_trace((KERN_ALERT"****Response data are found out!\n"));
         daq_trace((KERN_ALERT"****command type = %x, cmd = %x\n", cur->task_list[i].module_data.command_type, \
                           cur->task_list[i].module_data.command));
         if(cur->task_list_rcv[i].module_data.command_type & comm_st_error)
         {
            daq_trace((KERN_ALERT"****Error code: %x \n", *(cur->task_list_rcv[i].data)));
         }
         return 0;
      }
   }
   
   return 0;
}
