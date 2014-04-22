#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x628db709, "module_layout" },
	{ 0x50c25f7a, "cdev_del" },
	{ 0x5cc9a78d, "cdev_init" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x5487b746, "mem_map" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0x3c4f3e84, "dev_set_drvdata" },
	{ 0xc03b3986, "hrtimer_forward" },
	{ 0x9574fa8c, "daq_class_get" },
	{ 0xc5ae0182, "malloc_sizes" },
	{ 0x96091af1, "hrtimer_cancel" },
	{ 0x1958c905, "__rt_mutex_init" },
	{ 0x4d6859a3, "device_destroy" },
	{ 0x353e3fa5, "__get_user_4" },
	{ 0x5b02ec09, "filp_close" },
	{ 0x91715312, "sprintf" },
	{ 0xc727934c, "kthread_create_on_node" },
	{ 0x6ccf7bd7, "__pv_phys_offset" },
	{ 0x7d11c268, "jiffies" },
	{ 0xa6c07160, "rt_spin_lock" },
	{ 0x48c97839, "rt_spin_unlock" },
	{ 0xa8cf9b38, "daq_event_wait" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x37befc70, "jiffies_to_msecs" },
	{ 0x27e1a049, "printk" },
	{ 0xa2d57b86, "kthread_stop" },
	{ 0x56319944, "daq_event_close" },
	{ 0x7885f1e8, "daq_devid_free" },
	{ 0x80a75d8c, "daq_event_set" },
	{ 0x4f909ed5, "device_create" },
	{ 0x391cca48, "daq_event_create" },
	{ 0x481bcf13, "__rt_spin_lock_init" },
	{ 0x1de1b77, "device_create_file" },
	{ 0xdd350459, "cdev_add" },
	{ 0x8836fca7, "kmem_cache_alloc" },
	{ 0xbc10dd97, "__put_user_4" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x4b63b01f, "daq_event_reset" },
	{ 0x984ff93a, "hrtimer_start" },
	{ 0xa94dfcb8, "__raw_spin_lock_init" },
	{ 0x1934faed, "wake_up_process" },
	{ 0xd2965f6f, "kthread_should_stop" },
	{ 0x37a0cba, "kfree" },
	{ 0xe1c7b3df, "remap_pfn_range" },
	{ 0x9d669763, "memcpy" },
	{ 0x58470613, "hrtimer_init" },
	{ 0xfc3c44b3, "daq_devid_alloc" },
	{ 0x3e1a0666, "dev_get_drvdata" },
	{ 0x295f29a0, "filp_open" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=biokernbase";


MODULE_INFO(srcversion, "D4AC22711FFDBFF36D3BF0E");
