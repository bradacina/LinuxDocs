
#include <linux/module.h>
#include <linux/usb.h>

#define DRIVER_AUTHOR "Bogdan Radacina"
#define DRIVER_DESC "Demo Driver for USB Keyboard interface class"

#define USB_INTERFACE_CLASS_AND_PROTOCOL(cl, scl) \
	.match_flags = USB_DEVICE_ID_MATCH_INT_CLASS | \
			 USB_DEVICE_ID_MATCH_INT_SUBCLASS , \
	.bInterfaceClass = (cl), \
	.bInterfaceSubClass = (scl)


static __init int hello_init(void)
{
	printk(KERN_DEBUG "HelloWorld loaded.\n");
	return 0;
}

static __exit void hello_exit(void)
{
	printk(KERN_DEBUG "HelloWorld exiting.\n");
}

static const struct usb_device_id usbkbd_ids[] = {
	/* USB Keyboard class */
	{ USB_INTERFACE_CLASS_AND_PROTOCOL(0x01, 0x02) },
	{ }						/* Terminating entry */
};

MODULE_DEVICE_TABLE(usb, usbkbd_ids);

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
