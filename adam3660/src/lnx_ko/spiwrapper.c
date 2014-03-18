/* Use user mode interface to operate spi device */
#include "kdriver.h"

struct file* spi_open( const char *name )
{
	struct file *fp = filp_open( name, O_RDWR, 0666);
	if( IS_ERR(fp) ){
		daq_trace((KERN_ALERT"filp_open file %s failed.\n", name));
		fp = NULL;
	}
	return fp;
}

int spi_close( struct file *fp )
{
	return filp_close( fp, NULL );
}

int spi_ioctl( struct file *filp, u32 cmd, unsigned long arg )
{
	int error = -ENOTTY;
   mm_segment_t old_fs;

   
	if( !filp->f_op )
		goto out;

	if( filp->f_op->unlocked_ioctl ){
      old_fs = get_fs();
      set_fs( KERNEL_DS );
		error = filp->f_op->unlocked_ioctl( filp, cmd, arg );
      set_fs( old_fs );
		goto out;	
	}

	out:
		return error;
}

int spi_set_mode(struct file *filp, unsigned long mode)
{
      int ret = 0;
      char spi_mode;
      char spi_bits;
      unsigned int speed_max;
      if(mode == SPI_SND_PORT)
      {
         //init mode bit speed for sending
         spi_mode = SPI_SND_MODE;
         spi_bits = SPI_SND_BITS;
         speed_max = SPI_SND_SPEED;
      }else if(mode == SPI_RCV_PORT){
         //init mode bit speed for receiving
         spi_mode = SPI_RCV_MODE;
         spi_bits = SPI_RCV_BITS;
         speed_max = SPI_RCV_SPEED;
      }

      ret = spi_ioctl(filp, SPI_IOC_WR_MODE, &spi_mode);
      if(ret == -1)
         printk(KERN_ALERT"can't set spi mode\n");
      ret = spi_ioctl(filp, SPI_IOC_WR_BITS_PER_WORD, &spi_bits);
      if(ret == -1)
         printk(KERN_ALERT"can't set spi bit\n");
      ret = spi_ioctl(filp, SPI_IOC_WR_MAX_SPEED_HZ, &speed_max);
      if(ret == -1)
         printk(KERN_ALERT"can't set spi speed\n");
      return ret;
      
}

int spi_send(struct file *filp, unsigned long arg)
{
      int ret = 0;
      SPI_PACKAGE *pkg = (SPI_PACKAGE *)arg;

      struct spi_ioc_transfer tr={
         .tx_buf = (unsigned long)pkg->data,
         .rx_buf = 0,
         .len = pkg->len,
         .delay_usecs = SPI_SND_DELAY,
         .speed_hz = SPI_SND_SPEED,
         .bits_per_word = SPI_SND_BITS,
      };
      
      ret = spi_ioctl(filp, SPI_IOC_MESSAGE(1), &tr);
      if(ret < 1)
         printk(KERN_ALERT"can't send spi message\n");
      
      return ret;
}

int spi_receive(struct file *filp, unsigned long arg)
{
      int ret = 0;
      SPI_PACKAGE *pkg = (SPI_PACKAGE *)arg;

      struct spi_ioc_transfer tr={
         .tx_buf = 0,
         .rx_buf = (unsigned long)pkg->data,
         .len = pkg->len,
         .delay_usecs = SPI_RCV_DELAY,
         .speed_hz = SPI_RCV_SPEED,
         .bits_per_word = SPI_RCV_BITS,
      };
      
      ret = spi_ioctl(filp, SPI_IOC_MESSAGE(1), &tr);
      if(ret < 1)
         printk(KERN_ALERT"can't receive spi message\n");
      
      return ret;
}

