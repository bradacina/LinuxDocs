
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#define DRIVER_AUTHOR "Bogdan Radacina"
#define DRIVER_DESC "Demo Driver for a Char device driver" 
#define DRIVER_NAME "eudyptula"

static struct miscdevice myDevice;

static ssize_t do_read(struct file *file, char __user *buf, size_t count,
			   loff_t *ppos)
{
	return 0;
}

static ssize_t do_write(struct file *file, const char __user *buf,
			    size_t count, loff_t *ppos)
{
	return 0;
}

static const struct file_operations myDevice_fops = {
	.owner = THIS_MODULE,
	.read = &do_read,
	.write = &do_write
};


static __init int hello_init(void)
{
	int error;
	myDevice.minor = MISC_DYNAMIC_MINOR; 
	myDevice.name = DRIVER_NAME;
	myDevice.fops = &myDevice_fops; 

	error = misc_register(&myDevice);
	if(error) {
		pr_err("eudyptula_dev: failed to register misc device\n");
		return error;
	}

	pr_info("eudyptula_dev: Registered with Minor: %i\n", myDevice.minor);
	return 0;
}

static __exit void hello_exit(void)
{
	misc_deregister(&myDevice);
	pr_info("eudyptula_dev: HelloWorld exiting.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
