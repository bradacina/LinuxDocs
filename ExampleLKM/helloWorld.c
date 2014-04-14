#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static int mod_init(void)
{
	pr_debug("HelloWorld!\n");
	return 0;
}

static void mod_exit(void)
{
	pr_debug("HelloWorld unloaded\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*ic03isc*ip01in*");
