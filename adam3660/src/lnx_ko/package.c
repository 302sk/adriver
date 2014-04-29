#include "kdriver.h"

int add_task(daq_spi_transaction_t *cur, daq_task_info_t *task)
{
   int ret = 0;
   if(task == NULL)
      ret = -1;
   if(cur->task_count < 10)
   {
      memcpy((__u8*)&cur->task_list[cur->task_count], (__u8*)task, sizeof(daq_task_info_t));
      cur->task_count ++;
      ret = 0;
   }else{
      ret = -1;
   }
   return ret;
}


static int calc_checksum(__u8 *data, __u16 *sum, __u16 len)
{
   int i = 0;
   if(data == NULL)
      return PKG_ERROR;
   *sum = 0;
   for(i = 0; i<len; i++)
   {
      *sum += data[i];
   }
   return 0;
}

int init_snd_package(SPI_PACKAGE *package)
{
   package->len = 256;
   package->offset = sizeof(HEADER_INFO);
   package->data = (__u8*)kzalloc(256, GFP_KERNEL); 
   return 0;
}

int set_header_len(HEADER_INFO *header, __u16 len)
{
   if(header == NULL)
      return PKG_ERROR;
   header->data_len = len;
   calc_checksum((__u8*)header, &header->check_sum, sizeof(HEADER_INFO)-2);
   return 0;
}

HEADER_INFO* add_header_info(SPI_PACKAGE *package, __u16 module_count, __u8 command, __u16 len)
{
   HEADER_INFO *header;
   int ret = 0;

   if(package->len < sizeof(HEADER_INFO))
      return PKG_ERROR;

   header = (HEADER_INFO*)package->data;
   header->header_str[0] = 'A';
   header->header_str[1] = 'D';
   header->header_str[2] = 'A';
   header->header_str[3] = 'M';

   header->module_count =  module_count;
   header->message_id = 1; //?
   header->message_ver = MESSAGE_VERSION;
   header->command = command;
   header->data_len = len;

   package->offset += sizeof(HEADER_INFO);
   return header;
}

MODULE_INFO* add_module_info(SPI_PACKAGE *package, __u8 module_id, __u8 len)
{
   MODULE_INFO *module_info;
   int ret = 0;
   
   if((package->len - package->offset) < sizeof(MODULE_INFO))
      return PKG_ERROR;
   
   module_info = (MODULE_INFO*)(package->data + package->offset);
   module_info->module_id = module_id;
   module_info->data_len = sizeof(MODULE_INFO);
   package->offset += sizeof(MODULE_INFO);

   return module_info;
}

int add_module_data(SPI_PACKAGE *package, MODULE_DATA *mdl_data, __u8 *data, __u16 len)
{
   MODULE_DATA *module_data;
   __u16 data_len = 0;
   __u8*  data_write_pos = 0;
   int ret = 0;
   
   if(len == 0){
      data_len = calc_data_len(PKG_DIR_SND, mdl_data); //calculate how much data should be sent
      printk(KERN_ALERT"++++data_len = %d\n", data_len);
   }else{
      data_len = len;
   }
   
   if((package->len - package->offset) < sizeof(MODULE_DATA) + data_len)
      return PKG_ERROR;
  
   module_data = (MODULE_DATA*)(package->data + package->offset);
   memcpy(module_data, mdl_data, sizeof(MODULE_DATA));
   package->offset += sizeof(MODULE_DATA);
   data_write_pos = (__u8 *)(package->data + package->offset);
   memcpy(data_write_pos, data, data_len);
   package->offset += data_len;

   return sizeof(MODULE_DATA) + data_len;
}

int add_module_chksum(SPI_PACKAGE *package, MODULE_INFO *info)
{
   __u16 sum = 0;
   __u8 *pos = package->data + package->offset;
   if(info == NULL)
      return -1;
   info->data_len += sizeof(__u16);  //check sum 2 byte
   
   calc_checksum((__u8*)info, &sum, info->data_len - 2);
   
   memcpy(pos, &sum, sizeof(__u16));
   package->offset += sizeof(__u16);
   return sum;
}

int calc_data_len(int pkg_dir, MODULE_DATA *mdl_data)
{
   __u8 cmd_type = mdl_data->command_type;
   __u8 cmd = mdl_data->command;
   CHANNEL_RANGE chl_rng = mdl_data->channel_rng;
   
   __u16 len = 0;
   if((cmd_type & comm_mode_read) && (pkg_dir == PKG_DIR_SND))
   {
      return 0;
   }
   if(pkg_dir == PKG_DIR_RCV)
   {
      if(cmd_type & comm_st_error) //error , 1byte error code
         return 1;
      if(cmd_type & comm_dir_res)         //with response flag
      {
         if(cmd_type & comm_mode_read)   //read
            goto calc_by_cmd;
         else                            //write
            return 0;
      }else{                              //without response flag
         if(cmd_type & comm_mode_read)    //read
            return 0;
         else
            goto calc_by_cmd;             //write
      }
 
   }
   
calc_by_cmd:
   {
      switch(cmd)
      {
         case header_com_search:
            len = 19;
            break;
         case comm_ai_cal:
         case comm_ai_caltofac:
            len = 1;
            break;
         case comm_ao_cal:
         case comm_ao_caltofac:
            len = 0;    //reset just a command sent to module,so there's no data after that command
            break;
         case comm_ao_unit:
         case comm_ai_unit:
         case comm_ai_intgtime:
         case comm_ai_calcmp:
         case comm_ai_burnchk:
         case comm_ai_alarm:
            len = 0;
            break;
         case comm_do_rngcode:
         case comm_di_rngcode:
         case comm_ao_rngcode:
         case comm_ai_rngcode:
            len = (chl_rng.stop_chl - chl_rng.start_chl + 1); /// 1 byte per channel
            break;
         case comm_do_data:
         case comm_di_data:
            len = (chl_rng.stop_chl - chl_rng.start_chl + 1)/8;           ///1 byte per port(8 channel)
            len = ((chl_rng.stop_chl - chl_rng.start_chl + 1)%8 == 0) ? len : len+1;
            break;
         case comm_ai_data:
         case comm_ao_data:
         case comm_do_pwm:     //?
         case comm_di_filter:  //?
            len = (chl_rng.stop_chl - chl_rng.start_chl + 1) * 2;  ///2 byte per channel
            break;
            
         default:
            return 0;
            break;            
      }
   }
   return len;
}


int find_header(SPI_PACKAGE *package)
{
   int header_pos = 0;
   for(header_pos = 0; header_pos < package->len; header_pos++)
   {
      if(package->data[header_pos] == 'A' &&             //'A'
         package->data[header_pos+1] == 'D' &&           //'D'
         package->data[header_pos+2] == 'A' &&
         package->data[header_pos+3] == 'M')
      {
         package->offset += header_pos; //get header
         return header_pos;
      }
   }
   return PKG_ERROR; //there is no header string
}

int get_header_info(SPI_PACKAGE *package, HEADER_INFO *header)
{
   __u16 chk_sum = 0;
   int ret = 0;
   if(package->len < sizeof(HEADER_INFO))
      return PKG_ERROR;
   memcpy((__u8 *)header, (__u8 *)(package->data + package->offset), sizeof(HEADER_INFO));
   ret = calc_checksum((__u8*)header, &chk_sum, sizeof(HEADER_INFO)-2);
   
   if(chk_sum != header->check_sum)  //check sum is incorrect
      return PKG_ERROR;
   
   package->offset += sizeof(HEADER_INFO);
   return 0;
}

int get_module_info(SPI_PACKAGE *package, MODULE_INFO *module)
{
   __u8 *pos;
   __u16 chk_sum = 0;
   __u16 real_sum = 0;
   
   if((package->len - package->offset) < sizeof(MODULE_INFO))
      return PKG_ERROR;
   pos = (__u8*)(package->data + package->offset);
   memcpy((__u8*)module, pos, sizeof(MODULE_INFO));

   //get check sum
   real_sum = *((__u16*)(pos + module->data_len - 2));  
   calc_checksum((__u8*)pos, &chk_sum, module->data_len - 2);

   package->offset += sizeof(MODULE_INFO);

   if(chk_sum == real_sum)
   {
      return 0;
   }else{
      printk(KERN_ALERT"----check sum is not correct!real_sum = %x chk_sum = %x ++++\n", real_sum, chk_sum);
      return PKG_ERROR;
   }
}

int get_module_data(SPI_PACKAGE *package, MODULE_DATA *module_data)
{
   __u8 *pos;
   __u16 chk_sum = 0;
   if((package->len - package->offset) < sizeof(MODULE_DATA))
      return PKG_ERROR;
   pos = (__u8*)(package->data + package->offset);
   memcpy((__u8*)module_data, pos, sizeof(MODULE_DATA));
   calc_checksum((__u8*)module_data, &chk_sum, sizeof(MODULE_DATA));
     
   package->offset += sizeof(MODULE_DATA);
   return chk_sum;
}

int get_data(SPI_PACKAGE *package, void *data, int len)
{
   __u8 *pos;
   __u16 chk_sum = 0;
   if((package->len - package->offset) < len)
      return PKG_ERROR;
   pos = (__u8*)(package->data + package->offset);
   memcpy((__u8*)data, pos, len);

   calc_checksum((__u8*)data, &chk_sum, len);
   package->offset += len;
   return chk_sum;
}

__u16 get_module_chksum(SPI_PACKAGE *package)
{
   __u8 *pos;
   __u16 chk_sum = 0;
   if((package->len - package->offset) < sizeof(__u16))
      return 0;
   pos = (__u8*)(package->data + package->offset);
   memcpy((__u8*)&chk_sum, pos, sizeof(__u16));
   package->offset += sizeof(__u16);
   return chk_sum;
}


