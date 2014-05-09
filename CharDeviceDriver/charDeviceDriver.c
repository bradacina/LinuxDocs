
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define DRIVER_AUTHOR "Bogdan Radacina"
#define DRIVER_DESC "Demo Driver for a Char device driver"
#define DRIVER_NAME "eudyptula"
#define MY_ID "25e6eec82542"
#define MY_ID_CHAR_COUNT 12

/* forward declarations */
static ssize_t do_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t do_write(struct file *, const char __user *, size_t, loff_t *);

static const struct file_operations myDevice_fops = {
	.owner = THIS_MODULE,
	.read = do_read,
	.write = do_write
};

static struct miscdevice myDevice = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DRIVER_NAME,
	.fops = &myDevice_fops

};

static ssize_t do_read(struct file *file, char __user *buf, size_t count,
			   loff_t *ppos)
{
	int retval = min((int)count, MY_ID_CHAR_COUNT);

	if (retval > 0 && copy_to_user(buf, MY_ID, retval)
				retval = -EFAULT;
	
	return retval;	
}

static ssize_t do_write(struct file *file, const char __user *buf,
			    size_t count, loff_t *ppos)
{
	size_t numWritten = 0;
	char myBuf[MY_ID_CHAR_COUNT] = {0};
	if (count != MY_ID_CHAR_COUNT+1) {
		numWritten = -EINVAL;
		goto out;
	}
	if (copy_from_user(myBuf, buf, MY_ID_CHAR_COUNT)) {
		numWritten = -EFAULT;
		goto out;
	}
	if (strncmp(myBuf, MY_ID, MY_ID_CHAR_COUNT)) {
		numWritten = -EINVAL;
		goto out;
	}
	numWritten = count;
out:
	return numWritten;
}

static __init int hello_init(void)
{
	return misc_register(&myDevice);
}

static __exit void hello_exit(void)
{
	misc_deregister(&myDevice);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
