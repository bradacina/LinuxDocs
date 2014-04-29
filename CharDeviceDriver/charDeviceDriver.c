
#include <linux/module.h>
#include <linux/fs.h>

#define DRIVER_AUTHOR "Bogdan Radacina"
#define DRIVER_DESC "Demo Driver for a Char device driver" 
#define DRIVER_NAME "eudyptula"
#define MAX_DEVICES 1

static dev_t myDevice;

static __init int hello_init(void)
{
	int allocResult = alloc_chrdev_region(&myDevice, 0, MAX_DEVICES, DRIVER_NAME);
	if (allocResult)
	{
		pr_error("eudyptula_dev : alloc_chrdev_region failed.\n");
		goto error; 
	}

	pr_info("HelloWorld loaded.\n");
	return 0;

error:
	return allocResult;
}

static __exit void hello_exit(void)
{
	unregister_chrdev_region(myDevice, MAX_DEVICE);	
	pr_info("HelloWorld exiting.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
