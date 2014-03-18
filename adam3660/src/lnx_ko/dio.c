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
   return 0;
}

int daq_ioctl_do_write_port( daq_device_t *daq_dev, unsigned long arg )
{
   printk(KERN_ALERT"kernel message: daq_ioctl_do_write_sample\n");
	return 0;
}

int daq_ioctl_do_read_port( daq_device_t *daq_dev, unsigned long arg )
{
   printk(KERN_ALERT"kernel message: daq_ioctl_do_read_sample\n");
	return 0;
}

