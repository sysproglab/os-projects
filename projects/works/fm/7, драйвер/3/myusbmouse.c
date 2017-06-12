#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb/input.h>
#include <linux/hid.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define DRIVER_AUTHOR "Fomina Masha"
#define DRIVER_LICENSE "GPL"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_LICENSE(DRIVER_LICENSE);

static int btn_right_counter = 0;
static int btn_left_counter = 0;

struct usb_mouse {
	char name[128];
	char phys[64];
	struct usb_device *usbdev;
	struct input_dev *dev;
	struct urb *irq;
	signed char *data;
	dma_addr_t data_dma;
};

/*функция обработки прерывания*/
static void usb_mouse_irq(struct urb *urb)
{
	struct usb_mouse *mouse = urb->context;
	signed char *data = mouse->data;
	struct input_dev *dev = mouse->dev;
	int status;

	switch (urb->status) {
		case 0:			// success
			break;
		case -ECONNRESET:	// unlink
		case -ENOENT:
		case -ESHUTDOWN:
			return;
		// -EPIPE:  should clear the halt
		default:		// error
			goto resubmit;
	}

	/*
	input_report_key(dev, BTN_LEFT,   data[1] & 0x01);
	input_report_key(dev, BTN_RIGHT,  data[1] & 0x02);
	input_report_key(dev, BTN_MIDDLE, data[1] & 0x04);
	input_report_key(dev, BTN_SIDE,   data[1] & 0x08);
	input_report_key(dev, BTN_EXTRA,  data[1] & 0x10);

	input_report_rel(dev, REL_X,     data[2]); // In example data[1]
	input_report_rel(dev, REL_Y,     data[3]); // In example data[2]
	input_report_rel(dev, REL_WHEEL, data[0]); // In example data[3]

	//There is problem with y and wheel
	input_sync(dev);
	*/

	status = usb_submit_urb (urb, GFP_ATOMIC);
	if(!(data[1] & 0x01) && !(data[1] & 0x02)) { // In example were data[0], in real data[1]
		pr_info("Other action!\n");
	}
	else if(data[1] & 0x01) {
		pr_info("Left mouse button clicked!\n");
		btn_left_counter++;
	}
	else if(data[1] & 0x02) {
		pr_info("Right mouse button clicked!\n");
		btn_right_counter++;
	}
	return;

resubmit:
	status = usb_submit_urb (urb, GFP_ATOMIC);
	if (status)
		printk(KERN_ALERT "can't resubmit intr, %s-%s/input0, status %d", mouse->usbdev->bus->bus_name, mouse->usbdev->devpath, status);
}

static int usb_mouse_open(struct input_dev *dev)
{
	struct usb_mouse *mouse = input_get_drvdata(dev);
	mouse->irq->dev = mouse->usbdev;
	/*
	заполнение структуры urb. 
	Эта структура используется для передачи или приема данных от USB в асинхронном режиме.
	*/
	if (usb_submit_urb(mouse->irq, GFP_KERNEL))
		return -EIO;
	return 0;
}

static void usb_mouse_close(struct input_dev *dev)
{
	struct usb_mouse *mouse = input_get_drvdata(dev);
	printk(KERN_INFO "Driver my usb mouse: usb_mouse_close(). btn_right_counter = %d, btn_left_counter = %d\n", btn_right_counter, btn_left_counter );
	/* освобождение структуры urb. */
	usb_kill_urb(mouse->irq);	
}

/* подключение устройства к драйверу */
static int usb_mouse_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	struct usb_device *dev = interface_to_usbdev(intf);
	struct usb_host_interface *interface;
	struct usb_endpoint_descriptor *endpoint;
	struct usb_mouse *mouse;
	struct input_dev *input_dev;
	int pipe, maxp;
	
	interface = intf->cur_altsetting;

	if (interface->desc.bNumEndpoints != 1)
		return -ENODEV;
	endpoint = &interface->endpoint[0].desc;
	if (!usb_endpoint_is_int_in(endpoint))
		return -ENODEV;

	/* usb_rcvintpipe - определяет IN endpoint прерывания для USB устройства с заданным endpoint */
	pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress);
	maxp = usb_maxpacket(dev, pipe, usb_pipeout(pipe));

	mouse = kzalloc(sizeof(struct usb_mouse), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!mouse || !input_dev)
		goto fail1;

	/* allocate dma-consistent buffer for URB_NO_xxx_DMA_MAP  */
	mouse->data = usb_alloc_coherent(dev, 8, GFP_ATOMIC, &mouse->data_dma);
	if (!mouse->data)
		goto fail1;

	/* creates a new urb (USB Request Block) for a USB driver to use */
	mouse->irq = usb_alloc_urb(0, GFP_KERNEL);
	if (!mouse->irq)
		goto fail2;

	mouse->usbdev = dev;
	mouse->dev = input_dev;
	input_dev->name = mouse->name;
	input_dev->phys = mouse->phys;
	usb_to_input_id(dev, &input_dev->id);
	input_dev->dev.parent = &intf->dev;

	/* 
	EV_KEY type represents key press and release events,
	EV_REL type represents relative axis events (such as mouse movements)
	REL_WHEEL, REL_HWHEEL: - These codes are used for vertical and horizontal scroll wheels, respectively.
	*/
	input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
	input_dev->keybit[BIT_WORD(BTN_MOUSE)] = BIT_MASK(BTN_LEFT) |
		BIT_MASK(BTN_RIGHT) | BIT_MASK(BTN_MIDDLE);
	input_dev->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y);
	input_dev->keybit[BIT_WORD(BTN_MOUSE)] |= BIT_MASK(BTN_SIDE) |
		BIT_MASK(BTN_EXTRA);
	input_dev->relbit[0] |= BIT_MASK(REL_WHEEL);

	input_set_drvdata(input_dev, mouse);

	input_dev->open = usb_mouse_open;
	input_dev->close = usb_mouse_close;

	/* Установка функции-обработчика прерывания */
	usb_fill_int_urb(mouse->irq, dev, pipe, mouse->data,
			 (maxp > 8 ? 8 : maxp),
			 usb_mouse_irq, mouse, endpoint->bInterval);
	mouse->irq->transfer_dma = mouse->data_dma;
	/* URB_NO_TRANSFER_DMA_MAP - this bit is used for control urbs that have a DMA buffer already set up */
	mouse->irq->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

	if (input_register_device(mouse->dev))
		goto fail3;

	/* save our data pointer in this interface device */
	usb_set_intfdata(intf, mouse);
	return 0;

fail3:	
	usb_free_urb(mouse->irq);
fail2:
	usb_free_coherent(dev, 8, mouse->data, mouse->data_dma);
fail1:	
	input_free_device(input_dev);
	kfree(mouse);
	return -1;
}

/*отключение устройства от драйвера*/
static void usb_mouse_disconnect(struct usb_interface *intf)
{
	struct usb_mouse *mouse = usb_get_intfdata (intf);
	usb_set_intfdata(intf, NULL);
	if (mouse) {
		usb_kill_urb(mouse->irq);
		input_unregister_device(mouse->dev);
		usb_free_urb(mouse->irq);
		usb_free_coherent(interface_to_usbdev(intf), 8, mouse->data, mouse->data_dma);
		kfree(mouse);
	}
}

static struct usb_device_id usb_mouse_id_table [] = {
	 /* USB HID interface subclass and protocol codes. */ 
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
		USB_INTERFACE_PROTOCOL_MOUSE) },
	{ }	
};

MODULE_DEVICE_TABLE (usb, usb_mouse_id_table);



/* 	имя драйвера должно быть установлено перед вызовом функции регитрации драйвера;
	поле id содержит идентификатор шины  (PCI, USB, ...), vendor ID и device ID устройства;
	probe  и  disconnect  -  это callback-функции, вызываемые системой при
   	подключении  и  отключении  USB-устройства.  probe  будет  вызыван для
   	каждого  устройства,  если  список  id_table  пуст, или только для тех
   	устройств, которые соответствуют параметрам, перечисленным в списке.
*/
static struct usb_driver usb_mouse_driver = {
	.name		= "myusbmouse",
	.probe		= usb_mouse_probe,
	.disconnect	= usb_mouse_disconnect,
	.id_table	= usb_mouse_id_table,
};

static int __init usb_mouse_init(void)
{
	int retval = usb_register(&usb_mouse_driver);
	if (retval == 0)
		printk(KERN_INFO "myusbmouse init\n");
	return retval;
}

static void __exit usb_mouse_exit(void)
{
	usb_deregister(&usb_mouse_driver);
	printk(KERN_INFO "myusbmouse exit. btn_right_counter = %d, btn_left_counter = %d\n", btn_right_counter, btn_left_counter);
}

module_init(usb_mouse_init);
module_exit(usb_mouse_exit);
