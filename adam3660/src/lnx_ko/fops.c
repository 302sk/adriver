/* file operations */
#include "kdriver.h"

static
daq_file_ctx_t * daq_file_alloc_context( daq_device_t *daq_dev )
{
	daq_file_ctx_t *ctx = NULL;
	unsigned long flags;
	int i;
	
	if(likely(daq_dev->file_ctx_pool_size)){
		spin_lock_irqsave(&daq_dev->dev_lock, flags);
		for(i = 0; i < daq_dev->file_ctx_pool_size; ++i){
			if( !daq_dev->file_ctx_pool[i].busy){
				ctx = &daq_dev->file_ctx_pool[i];
				ctx->busy = 1;
				break;
			}
		}
		spin_unlock_irqrestore(&daq_dev->dev_lock, flags);
	}

	if (ctx == NULL){
		ctx = (daq_file_ctx_t*)kzalloc(sizeof(daq_file_ctx_t), GFP_KERNEL);
	}
	if (ctx){
		INIT_LIST_HEAD(&ctx->ctx_list);
		ctx->daq_dev = daq_dev;
	}
	
	return ctx;
}

static
void daq_file_free_context( daq_file_ctx_t *ctx)
{
	daq_device_t	*daq_dev = ctx->daq_dev;
	unsigned long	flags;
	
	if (likely( daq_dev->file_ctx_pool_size &&
			daq_dev->file_ctx_pool <= ctx &&
			ctx < daq_dev->file_ctx_pool + daq_dev->file_ctx_pool_size)){
		spin_lock_irqsave(&daq_dev->dev_lock, flags);
		ctx->busy = 0;
		spin_unlock_irqrestore(&daq_dev->dev_lock, flags);
	}else{
		kfree(ctx);
	}
}


int daq_file_open(struct inode *in, struct file *fp)
{
	daq_device_t	*daq_dev = container_of( in->i_cdev, daq_device_t, cdev );
	daq_file_ctx_t	*ctx;
	unsigned long	flags;
	int		first_user, fmode_chk_ret = 0;
   printk(KERN_ALERT"adam3660 open device\n");
	if(unlikely(daq_dev->remove_pending)){
		return -ENODEV;
	}

	ctx = daq_file_alloc_context(daq_dev);
	if(unlikely(ctx == NULL)){
		return -ENOMEM;
	}
	ctx->write_access = fp->f_mode & FMODE_WRITE;

	spin_lock_irqsave(&daq_dev->dev_lock, flags);

	first_user = list_empty(&daq_dev->file_ctx_list);
/*	if(ctx->write_access){
		daq_file_ctx_t *curr;
		list_for_each_entry(curr, &daq_dev->file_ctx_list, ctx_list){
			if (curr->write_access){
				fmode_chk_ret = -EPERM;
				break;
			}
		}
	}
*/
	if (!fmode_chk_ret){
		list_add( &ctx->ctx_list, &daq_dev->file_ctx_list );
		fp->private_data = ctx;
	}

	spin_unlock_irqrestore( &daq_dev->dev_lock, flags );

	if (fmode_chk_ret){
		daq_file_free_context(ctx);
	}

	return fmode_chk_ret;
}

int daq_file_close( struct inode *inode, struct file *filp )
{
	daq_file_ctx_t	*ctx	= filp->private_data;
	daq_device_t	*daq_dev= ctx->daq_dev;
	unsigned long 	flags;
	int 		i, last_user;

	spin_lock_irqsave(&daq_dev->dev_lock, flags);
	list_del(&ctx->ctx_list);
	last_user = list_empty( &daq_dev->file_ctx_list );
	spin_unlock_irqrestore( &daq_dev->dev_lock, flags );

	for ( i = 0; i < KrnlSptedEventCount; ++i){
		if( ctx->events[i] ){
			daq_event_close(ctx->events[i]);
			ctx->events[i] = NULL;
		}
	}

	daq_file_free_context(ctx);

	if( last_user ){
		if(daq_dev->remove_pending){
//			daq_device_cleanup(daq_dev);
		}

	}
	return 0;
}
/*
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23))
struct page * daq_vma_nopage(struct vm_area_struct *vma, unsigned long address, int *type)
{
   daq_device_t *daq_dev = vma->vm_private_data;
   struct page     *page = NOPAGE_SIGBUS;
   unsigned long   pgoff = (address - vma->vm_start) >> PAGE_SHIFT;

   if (pgoff < daq_dev->fai_buffer.nr_pages) {
      page = daq_dev->fai_buffer.pages[pgoff];
      get_page(page);
   }

   return page;
}

static struct vm_operations_struct daq_vm_ops = {
   .nopage = daq_vma_nopage,
};
#else
static int daq_vma_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
   daq_device_t *daq_dev = vma->vm_private_data;
   unsigned long   pgoff = vmf->pgoff - vma->vm_pgoff;

   if (pgoff < daq_dev->fai_buffer.nr_pages) {
      vmf->page = daq_dev->fai_buffer.pages[pgoff];
      get_page(vmf->page);
      return 0;
   }

   return VM_FAULT_SIGBUS;
}

static struct vm_operations_struct daq_vm_ops = {
   .fault = daq_vma_fault,
};
#endif
*/
int daq_file_mmap(struct file *filp, struct vm_area_struct *vma)
{
   daq_device_t * daq_dev = ((daq_file_ctx_t *)filp->private_data)->daq_dev;

   if (vma->vm_pgoff == 0){
      if (vma->vm_end - vma->vm_start > PAGE_SIZE){
         return -EIO;
      }

      return remap_pfn_range(vma, vma->vm_start,
               virt_to_phys((void *)daq_dev) >> PAGE_SHIFT,
               vma->vm_end - vma->vm_start, vma->vm_page_prot);
   } /*else {
      if (!daq_dev->fai_buffer.nr_pages) {
         return -EIO;
      }
      vma->vm_ops    = &daq_vm_ops;
      vma->vm_flags |= VM_RESERVED;
      vma->vm_private_data = daq_dev;

      return 0;
   }*/
   return 0;
}


static
int daq_ioctl_dev_get_desc(daq_device_t *daq_dev, unsigned long arg)
{
	return 0;
}

static
int daq_ioctl_dev_get_location(daq_device_t *daq_dev, unsigned long arg)
{
	return 0;
}

static
int daq_ioctl_dev_reg_event(daq_device_t *daq_dev, daq_file_ctx_t *ctx, unsigned long arg)
{
	unsigned long	flags;
	unsigned	kdx;
	uint32		event_id;
	HANDLE		event;

	if(unlikely(get_user(event_id, &((USER_EVENT_INFO*)arg)->EventId))){
		return -EFAULT;
	}

	kdx = GetEventKIndex(event_id);
	if (likely(kdx < KrnlSptedEventCount)){
		event = ctx->events[kdx];
		if(event == NULL){
			event = daq_event_create();
			
			spin_lock_irqsave(&daq_dev->dev_lock, flags);
			if(ctx->events[kdx] == NULL){
				ctx->events[kdx] = event;
			}else{
				daq_event_close(event);
				event = ctx->events[kdx];
			}
			spin_unlock_irqrestore(&daq_dev->dev_lock, flags);
		}
		
		put_user(event, &((USER_EVENT_INFO*)arg)->Handle);
		return event != NULL ? 0 : -ENOMEM;
	}
	
	return -EINVAL;
}

static
int daq_ioctl_dev_unreg_event(daq_device_t *daq_dev, daq_file_ctx_t *ctx, unsigned long arg)
{
	unsigned long	flags;
	unsigned	kdx = GetEventKIndex(arg);

	if(likely(kdx < KrnlSptedEventCount)){
		spin_lock_irqsave(&daq_dev->dev_lock, flags);
		if(ctx->events[kdx] != NULL){
			daq_event_close(ctx->events[kdx]);
			ctx->events[kdx] = NULL;
		}
		spin_unlock_irqrestore(&daq_dev->dev_lock, flags);
	}
	
	return 0;
}

int daq_device_signal_event(daq_device_t *daq_dev, unsigned index)
{
	daq_file_ctx_t *curr;
	unsigned long	flags;

	if (index < KrnlSptedEventCount){
		spin_lock_irqsave(&daq_dev->dev_lock, flags);
		list_for_each_entry(curr, &daq_dev->file_ctx_list, ctx_list){
			if(curr->events[index] != NULL){
				daq_event_set(curr->events[index]);
			}
		}
		spin_unlock_irqrestore(&daq_dev->dev_lock, flags);
		return 0;
	}

	return -EINVAL;
}

int daq_device_clear_event(daq_device_t *daq_dev, unsigned kdx)
{
	daq_file_ctx_t	*curr;
	unsigned long	flags;

	if (kdx < KrnlSptedEventCount){
		spin_lock_irqsave(&daq_dev->dev_lock, flags);
		list_for_each_entry(curr, &daq_dev->file_ctx_list, ctx_list){
			if(curr->events[kdx] != NULL){
				daq_event_reset(curr->events[kdx]);
			}
		}
		spin_unlock_irqrestore(&daq_dev->dev_lock, flags);
		return 0;
	}

	return -EINVAL;
}

long daq_file_ioctl( struct file *filp, unsigned int cmd, unsigned long arg)
{
	daq_file_ctx_t	*ctx	= filp->private_data;
	daq_device_t	*daq_dev= ctx->daq_dev;
	int		ret;
   printk(KERN_ALERT"daq file ioctl cmd = %x, IOCTL_AI_READ_SAMPLE = %x\n", cmd, IOCTL_AI_READ_SAMPLES);
	switch(cmd)
	{
		case IOCTL_DEVICE_GET_DESC:
			ret = daq_ioctl_dev_get_desc(daq_dev, arg);
			break;
		case IOCTL_DEVICE_GET_LOCATION_INFO:
			ret = daq_ioctl_dev_get_location(daq_dev, arg);
			break;
		
		case IOCTL_DEVICE_REGISTER_USER_EVENT:
			ret = daq_ioctl_dev_reg_event(daq_dev, ctx, arg);
			break;
		case IOCTL_DEVICE_UNREGISTER_USER_EVENT:
			ret = daq_ioctl_dev_unreg_event(daq_dev, ctx, arg);
			break;
		case IOCTL_DEVICE_NOTIFY_PROP_CHGED:
			ret = daq_device_signal_event(daq_dev, KdxDevPropChged);
			break;
		
	//****************************************************
	// IOCTL for AI operation
	//****************************************************
	   case IOCTL_AI_SET_CHAN_CFG:
         ret = daq_ioctl_ai_set_channel(daq_dev, arg);
         break;
		case IOCTL_AI_READ_SAMPLES:
         printk(KERN_ALERT"ai read samples\n");
			ret = daq_ioctl_ai_read_sample(daq_dev, arg);
			break;
      case IOCTL_AO_SET_CHAN_CFG:
         ret = daq_ioctl_ao_set_channel(daq_dev, arg);
         break;   
		case IOCTL_AO_WRITE_SAMPLES:
			ret = daq_ioctl_ao_write_sample(daq_dev, arg);
			break;
		case IOCTL_DIO_SET_PORT_DIR:
			ret = daq_ioctl_dio_set_port_dir(daq_dev, arg);
			break;
		case IOCTL_DIO_READ_DI_PORTS:
			ret = daq_ioctl_di_read_port(daq_dev, arg);
			break;
		case IOCTL_DIO_WRITE_DO_PORTS:
			ret = daq_ioctl_do_write_port(daq_dev, arg);
			break;
		case IOCTL_DIO_READ_DO_PORTS:
			ret = daq_ioctl_do_read_port(daq_dev, arg);
			break;
		default:
			ret = -ENOTTY;
			break;
	}
	
	return ret;
}
/*
int daq_device_signal_event(daq_device_t *daq_dev, unsigned kdx)
{
	daq_file_ctx_t	*curr;
	unsigned long 	flags;

	if(kdx < KrnlSptedEventCount){
		spin_lock_irqsave( &daq_dev->dev_lock, flags );
		list_for_each_entry(curr, &daq_dev->file_ctx_list, ctx_list){
			if (curr->events[kdx] != NULL){
				daq_event_set(curr->events[kdx]);
			}
		}
		spin_unlock_irqrestore(&daq_dev->dev_lock, flags);
		return 0;
	}
	
	return -EINVAL;
}

int daq_device_clear_event( daq_device_t *daq_dev, unsigned kdx)
{
	daq_file_ctx_t	*curr;
	unsigned long	flags;
	
	if( kdx < KrnlSptedEventCount){
		spin_lock_irqsave(&daq_dev->dev_lock, flags);
		list_for_each_entry(curr, &daq_dev->file_ctx_list, ctx_list){
			if (curr->events[kdx] != NULL){
				daq_event_reset(curr->events[kdx]);
			}
		}
		spin_unlock_irqrestore(&daq_dev->dev_lock, flags);
		return 0;
	}
		
	return -EINVAL;
}
*/









