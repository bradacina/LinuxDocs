
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
static const struct file_operations  foo_fops = {
	.owner = THIS_MODULE,
	.read = foo_do_read,
	.write = foo_do_write
};

static const struct file_operations jiffies_fops = {
	.owner = THIS_MODULE,
	.read = jiffies_do_read,
	.write = jiffies_do_write
};

static const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.read = id_do_read,
	.write = id_do_write
};

static struct dentry *dirEntry;
static struct dentry *idEntry;
static struct dentry *jiffiesEntry;
static struct dentry *fooEntry;

static char jiffiesBuf[25] = {0};
static char fooBuf[PAGE_SIZE] = {0};
static size_t fooBufEndOffset;
DECLARE_RWSEM(foo_rw_semaphore);

struct kobject kobj_var;

memset(&kobj_var, 0, sizeof(kobj_var));
kobject_init(&kobj_var);

int result = sysfs_create_dir(kobj_variable)
result >= 0 for success

sysfs_create_file(kobj_var, struct attribute_var)


sysfs_remove_file(kobj_var, struct attribute_var)
sysfs_remove_dir(kobj_variable)

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

	jiffiesEntry = debugfs_create_file(JIFFIES_FILE_NAME, 0444,
		dirEntry, NULL, &jiffies_fops);
	if (IS_ERR_OR_NULL(jiffiesEntry)) {
		cleanup();
		return PTR_ERR(jiffiesEntry);
	}

	fooEntry = debugfs_create_file(FOO_FILE_NAME, 0644,
		dirEntry, NULL, &foo_fops);
	if (IS_ERR_OR_NULL(fooEntry)) {
		cleanup();
		return PTR_ERR(fooEntry);
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
