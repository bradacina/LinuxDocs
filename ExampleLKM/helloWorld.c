
#include <linux/module.h>
#include <linux/usb.h>

#define DRIVER_AUTHOR "Bogdan Radacina"
#define DRIVER_DESC "Demo Driver for USB Keyboard interface class"

#define USB_INTERFACE_CLASS_AND_PROTOCOL(cl, pr) \
	.match_flags = USB_DEVICE_ID_MATCH_INT_CLASS | USB_DEVICE_ID_MATCH_INT_PROTOCOL , \
	.bInterfaceClass = (cl), \
	.bInterfaceProtocol = (pr)


static __init int hello_init(void) 
{
	printk(KERN_DEBUG "HelloWorld loaded.");
	return 0;
}

static __exit void hello_exit(void)
{
	printk(KERN_DEBUG "HelloWorld exiting.");
}

static const struct usb_device_id usbkbd_ids[] = {
	{ USB_INTERFACE_CLASS_AND_PROTOCOL(0x03, 0x01) }, /* USB Keyboard class */
	{ }						/* Terminating entry */
};

MODULE_DEVICE_TABLE(usb, usbkbd_ids);

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
