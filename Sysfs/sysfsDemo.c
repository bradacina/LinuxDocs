#include <linux/fs.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <linux/rwsem.h>
#include <linux/uaccess.h>

#define DRIVER_AUTHOR "Bogdan Radacina"
#define DRIVER_DESC "Demo Usage of sysfs"
#define DIR_NAME "eudyptula"
#define ID_FILE_NAME id
#define JIFFIES_FILE_NAME jiffies
#define FOO_FILE_NAME foo
#define MY_ID "25e6eec82542"
#define MY_ID_CHAR_COUNT 12

/* forward declarations */
static ssize_t show_id(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf);
static ssize_t store_id(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count);

static ssize_t show_jiffies(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf);
static ssize_t store_jiffies(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count);

static ssize_t show_foo(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf);
static ssize_t store_foo(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count);

/* end forward declarations */

static struct kobj_attribute idAttr = __ATTR(ID_FILE_NAME, 0666, show_id,
							store_id);

static struct kobj_attribute jiffiesAttr = __ATTR(JIFFIES_FILE_NAME, 0444,
					&show_jiffies, &store_jiffies);

static struct kobj_attribute fooAttr = __ATTR(FOO_FILE_NAME, 0644,
						&show_foo, &store_foo);

static struct attribute *attributes[] = {
	&idAttr.attr,
	&jiffiesAttr.attr,
	&fooAttr.attr,
	NULL
};

static char fooBuf[PAGE_SIZE] = {0};
static size_t fooBufEndOffset;
DECLARE_RWSEM(foo_rw_semaphore);



static ssize_t show(struct kobject *kobj, struct attribute *attr,
							char *buf)
{
	struct kobj_attribute *kattr;
	ssize_t ret = -EIO;

	kattr = container_of(attr, struct kobj_attribute, attr);
	if (kattr->show)
		ret = kattr->show(kobj, kattr, buf);
	return ret;
}

static ssize_t store(struct kobject *kobj, struct attribute *attr,
			       const char *buf, size_t count)
{
	struct kobj_attribute *kattr;
	ssize_t ret = -EIO;

	kattr = container_of(attr, struct kobj_attribute, attr);
	if (kattr->store)
		ret = kattr->store(kobj, kattr, buf, count);
	return ret;
}

static const struct sysfs_ops my_sysfs_ops = {
	.show = show,
	.store = store
};

static struct kobj_type dirKtype = {
	.sysfs_ops = &my_sysfs_ops,
	.default_attrs = attributes
};

static struct kobject dirKobj = {0};

static ssize_t show_id(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf) {

	return sprintf(buf, "%s", MY_ID);
}

static ssize_t store_id(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count) {

	char readBuf[MY_ID_CHAR_COUNT+1] = {0};

	if (count != MY_ID_CHAR_COUNT + 1)
		return -EINVAL;

	memcpy(readBuf, buf, count);
	pr_debug("%s\n", readBuf);

	if (strncmp(readBuf, MY_ID, MY_ID_CHAR_COUNT) == 0)
		return count;

	return -EINVAL;
}

static ssize_t show_jiffies(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf) {

	int n = sprintf(buf, "%lud", jiffies);

	buf[n] = 0;
	return n;
}

static ssize_t store_jiffies(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count) {
	return -EINVAL;
}

static ssize_t show_foo(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf) {
	int n = 0;

	down_read(&foo_rw_semaphore);
	n = snprintf(buf, fooBufEndOffset, "%s", fooBuf);
	up_read(&foo_rw_semaphore);
	return n;
}

static ssize_t store_foo(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count) {

	fooBufEndOffset = count > PAGE_SIZE ? PAGE_SIZE-1 : count;
	down_write(&foo_rw_semaphore);
	memset(fooBuf, 0, sizeof(fooBuf));
	memcpy(fooBuf, buf, fooBufEndOffset);
	fooBuf[fooBufEndOffset] = 0;
	up_write(&foo_rw_semaphore);
	return fooBufEndOffset;
}

static void cleanup(void)
{
	kobject_put(&dirKobj);
}

static __init int hello_init(void)
{
	int error = kobject_init_and_add(&dirKobj, &dirKtype, kernel_kobj,
			"%s", DIR_NAME);
	if (error < 0)
		return error;

	return error;
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
