#include "kdriver.h"

void daq_ao_initialize_hw( daq_device_t *daq_dev )
{

}

int daq_ioctl_ao_write_sample( daq_device_t *daq_dev, unsigned long arg )
{
   printk(KERN_ALERT"kernel message: daq_ioctl_ao_write_sample\n");
   return 0;
}

int daq_ioctl_ao_set_channel(daq_device_t *daq_dev, unsigned long arg)
{
   return 0;
}

