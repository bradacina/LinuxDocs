
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/debugfs.h>

#define DRIVER_AUTHOR "Bogdan Radacina"
#define DRIVER_DESC "Demo Usage of Debugfs"
#define DIR_NAME "eudyptula"
#define ID_FILE_NAME "id"
#define MY_ID "25e6eec82542"
#define MY_ID_CHAR_COUNT 12

/* forward declarations */
static ssize_t id_do_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t id_do_write(struct file *, const char __user *,
							size_t, loff_t *);

static const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.read = id_do_read,
	.write = id_do_write
};

static struct dentry *dirEntry;
static struct dentry *idEntry;

static ssize_t id_do_read(struct file *file, char __user *buf, size_t count,
			   loff_t *ppos)
{
	return simple_read_from_buffer(buf,
			count, ppos, MY_ID, MY_ID_CHAR_COUNT);
}

static ssize_t id_do_write(struct file *file, const char __user *buf,
			    size_t count, loff_t *ppos)
{
	size_t numWritten = 0;
	char myBuf[MY_ID_CHAR_COUNT] = {0};

	numWritten = simple_write_to_buffer(myBuf,
		MY_ID_CHAR_COUNT, ppos, buf, count);
	if (numWritten < 0 || count > MY_ID_CHAR_COUNT+1) {
		numWritten = -EINVAL;
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

static void cleanup(void)
{
	debugfs_remove_recursive(dirEntry);
}

static __init int hello_init(void)
{
	dirEntry = debugfs_create_dir(DIR_NAME, NULL);
	if (IS_ERR_OR_NULL(dirEntry))
		return PTR_ERR(dirEntry);

	idEntry = debugfs_create_file(ID_FILE_NAME, 0666,
						dirEntry, NULL, &id_fops);
	if (IS_ERR_OR_NULL(idEntry)) {
		cleanup();
		return PTR_ERR(idEntry);
	}

	return 0;
}

static __exit void hello_exit(void)
{
	cleanup();
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
