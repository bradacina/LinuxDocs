
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <uapi/linux/errno.h>
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

static struct miscdevice myDevice;
static const struct file_operations myDevice_fops = {
	.owner = THIS_MODULE,
	.read = do_read,
	.write = do_write
};

static ssize_t do_read(struct file *file, char __user *buf, size_t count,
			   loff_t *ppos)
{
	ssize_t toRead = 0;

	if ((*ppos) >= MY_ID_CHAR_COUNT) {
		goto out;
	}

	if (count < MY_ID_CHAR_COUNT) {
		toRead = count;
	}

	if (count >= MY_ID_CHAR_COUNT) {
		toRead = MY_ID_CHAR_COUNT;
	}

	if (copy_to_user(buf, MY_ID, toRead)) {
		toRead = -EFAULT;
		goto out;
	}
	*ppos += toRead;
out:
	return toRead;
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
	int error;
	myDevice.minor = MISC_DYNAMIC_MINOR;
	myDevice.name = DRIVER_NAME;
	myDevice.fops = &myDevice_fops;

	error = misc_register(&myDevice);
	if (error) {
		pr_err("eudyptula_dev: failed to register misc device\n");
		return error;
	}

	pr_info("eudyptula_dev: Hello World Registered with Minor: %i\n",
		myDevice.minor);
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
