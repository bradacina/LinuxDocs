
#include <linux/module.h>
#include <linux/usb.h>
#include <uapi/linux/hid.h>

#define DRIVER_AUTHOR "Bogdan Radacina"
#define DRIVER_DESC "Demo Driver for USB Keyboard interface class"

static __init int hello_init(void)
{
	pr_debug("HelloWorld loaded.\n");
	return 0;
}

static __exit void hello_exit(void)
{
	pr_debug("HelloWorld exiting.\n");
}

static const struct usb_device_id usbkbd_ids[] = {
	/* USB Keyboard class */
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
				USB_INTERFACE_SUBCLASS_BOOT,
				USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{ }	/* Terminating entry */
};

MODULE_DEVICE_TABLE(usb, usbkbd_ids);

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
