#include <linux/fs.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <linux/rwsem.h>

#define DRIVER_AUTHOR "Bogdan Radacina"
#define DRIVER_DESC "Demo Usage of sysfs"
#define DIR_NAME "eudyptula"
#define ID_FILE_NAME "id"
#define JIFFIES_FILE_NAME "jiffies"
#define FOO_FILE_NAME "foo"
#define MY_ID "25e6eec82542"
#define MY_ID_CHAR_COUNT 12

/* forward declarations */
static ssize_t id_do_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t id_do_write(struct file *, const char __user *,
							size_t, loff_t *);
static ssize_t jiffies_do_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t jiffies_do_write(struct file *, const char __user *,
							size_t, loff_t *);
static ssize_t foo_do_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t foo_do_write(struct file *, const char __user *,
							size_t, loff_t *);
static char jiffiesBuf[25] = {0};
static char fooBuf[PAGE_SIZE] = {0};
static size_t fooBufEndOffset;
static struct rw_semaphore foo_rw_semaphore;

static struct kobj_attribute idAttr = __ATTR(MY_ID, 0644, &id_do_read, &id_do_write); 

static struct attribute * attributes[] = {
	&idAttr.attr,
	NULL
};

static struct kobj_type dirKtype = {
	.sysfs_ops = &kobj_sysfs_ops,
	.default_attrs = &attributes
};

static struct kobject dirKobj = {0};

static ssize_t foo_do_read(struct file *file, char __user *buf,
					size_t count, loff_t *ppos)
{
	int n = 0;
	down_read(&foo_rw_semaphore);
	n = simple_read_from_buffer(buf,
			count, ppos, fooBuf, fooBufEndOffset);
	up_read(&foo_rw_semaphore);
	return n;
}

static ssize_t foo_do_write(struct file *file, const char __user *buf,
			    size_t count, loff_t *ppos)
{
	int numWritten = 0;
	down_write(&foo_rw_semaphore);

	if (*ppos == 0) {
		memset(fooBuf, 0, sizeof(fooBuf));
		fooBufEndOffset = 0;
	}

	if (*ppos + count > PAGE_SIZE) {
		up_write(&foo_rw_semaphore);
		return -EINVAL;
	}

	numWritten = simple_write_to_buffer(fooBuf,
		sizeof(fooBuf), ppos, buf, count);

	fooBufEndOffset += numWritten;

	up_write(&foo_rw_semaphore);
	return numWritten;
}

static ssize_t jiffies_do_read(struct file *file, char __user *buf,
					size_t count, loff_t *ppos)
{
	int n = 0;
	memset(jiffiesBuf, 0, sizeof(jiffiesBuf));
	n = sprintf(jiffiesBuf, "%lud", jiffies);
	jiffiesBuf[n] = '\0';
	return simple_read_from_buffer(buf,
			count, ppos, jiffiesBuf, n);
}

static ssize_t jiffies_do_write(struct file *file, const char __user *buf,
						size_t count, loff_t *ppos)
{
	return -EINVAL;
}

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
	kobject_put(&dirKobj);
}

static __init int hello_init(void)
{
	int error = kobject_init_and_add(&dirKobj, &dirKtype, NULL,
			"%s", DIR_NAME);
	if ( IS_ERR(error) ) {
		return error;
	}

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
