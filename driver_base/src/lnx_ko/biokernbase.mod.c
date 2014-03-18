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
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x5cc9a78d, "cdev_init" },
	{ 0xd5b037e1, "kref_put" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x5487b746, "mem_map" },
	{ 0x3c4f3e84, "dev_set_drvdata" },
	{ 0xc5ae0182, "malloc_sizes" },
	{ 0xf7802486, "__aeabi_uidivmod" },
	{ 0x1958c905, "__rt_mutex_init" },
	{ 0xe3d458d6, "usb_kill_urb" },
	{ 0x4d6859a3, "device_destroy" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x6ccf7bd7, "__pv_phys_offset" },
	{ 0xa6c07160, "rt_spin_lock" },
	{ 0x48c97839, "rt_spin_unlock" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x27e1a049, "printk" },
	{ 0xa539a80c, "vmap" },
	{ 0x4f909ed5, "device_create" },
	{ 0xc2409ee1, "usb_free_coherent" },
	{ 0x481bcf13, "__rt_spin_lock_init" },
	{ 0x1de1b77, "device_create_file" },
	{ 0xdd350459, "cdev_add" },
	{ 0xa0abb319, "usb_submit_urb" },
	{ 0x8836fca7, "kmem_cache_alloc" },
	{ 0xbc10dd97, "__put_user_4" },
	{ 0x93fca811, "__get_free_pages" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0xd62c833f, "schedule_timeout" },
	{ 0xa94dfcb8, "__raw_spin_lock_init" },
	{ 0x1934faed, "wake_up_process" },
	{ 0x4302d0eb, "free_pages" },
	{ 0x83800bfa, "kref_init" },
	{ 0x37a0cba, "kfree" },
	{ 0x94961283, "vunmap" },
	{ 0x9d669763, "memcpy" },
	{ 0x9775cdc, "kref_get" },
	{ 0xc9108392, "put_page" },
	{ 0x7873fed0, "class_destroy" },
	{ 0x1db7dc40, "pgprot_kernel" },
	{ 0x6aa2240e, "usb_alloc_coherent" },
	{ 0x9d05e771, "__class_create" },
	{ 0x802274b8, "usb_free_urb" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x29c6b2a2, "get_user_pages_fast" },
	{ 0xb13682bf, "usb_alloc_urb" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "FD47E79DEE4909976FE4C12");
