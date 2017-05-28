//
// Created by kivi on 24.09.16.
//
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/termios.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <linux/file.h>
#include <linux/fs.h>

#define  DEVICE_NAME "sniffer"
#define  CLASS_NAME  "sniff"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Azat Abdullin");
MODULE_DESCRIPTION("A simple Linux char driver sniffer");
MODULE_VERSION("0.1");
 
static int    majorNumber;
static int    numberOpens = 0;
static struct class*  snifferClass  = NULL;
static struct device* snifferDevice = NULL;

static struct file* fd;
static mm_segment_t old_fs;
static char* real_device = "/dev/ttyUSB0";
 
// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};


////////////////////////////////////////////////////////////////////////////////////
/// File I/O functions
////////////////////////////////////////////////////////////////////////////////////
struct file* file_open(const char* path, int flags, int rights) {
    struct file* filp = NULL;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(get_ds());
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if(IS_ERR(filp)) {
        err = PTR_ERR(filp);
        return NULL;
    }
    return filp;
}

void file_close(struct file* file) {
    filp_close(file, NULL);
}

int file_read(struct file* file, unsigned long long offset, unsigned char* data, unsigned int size) {
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_read(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}

int file_write(struct file* file, unsigned long long offset, const unsigned char* data, unsigned int size) {
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}

////////////////////////////////////////////////////////////////////////////////////
/// Fake device driver functions
//////////////////////////////////////////////////////////////////////////////////// 
static int __init sniffer_init(void) {
   printk(KERN_INFO "sniffer: Initializing the sniffer LKM\n");
 
   // Try to dynamically allocate a major number for the device -- more difficult but worth it
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0) {
      printk(KERN_ALERT "sniffer failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "sniffer: registered correctly with major number %d\n", majorNumber);
 
   // Register the device class
   snifferClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(snifferClass)) {
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(snifferClass);
   }
   printk(KERN_INFO "sniffer: device class registered correctly\n");
 
   // Register the device driver
   snifferDevice = device_create(snifferClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(snifferDevice)) {
      class_destroy(snifferClass);
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(snifferDevice);
   }
   printk(KERN_INFO "sniffer: device class created correctly\n");
   return 0;
}

static void __exit sniffer_exit(void) {
   device_destroy(snifferClass, MKDEV(majorNumber, 0));     // remove the device
   class_unregister(snifferClass);                          // unregister the device class
   class_destroy(snifferClass);                             // remove the device class
   unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
   printk(KERN_INFO "sniffer: Goodbye from the LKM!\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
   numberOpens++;
   printk(KERN_INFO "sniffer: Device has been opened %d time(s)\n", numberOpens);

   /// Connecting to real device   
   fd = file_open (real_device, O_RDWR | O_NOCTTY | O_SYNC, 0);
   if (fd == NULL) {
      printk(KERN_INFO "sniffer: cannot open device %s\n", real_device);
     return 1;
   }

   // Assume that device is already configured
   return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
   int error_count = 0;
   int i = 0;
   error_count = file_read(fd, *offset, buffer, len);
   if (error_count > 0) {
      printk(KERN_INFO "sniffer: readed %d bytes\n", error_count);
      for (i = 0; i < error_count; ++i) {
         printk(KERN_INFO "sniffer: %d", (int)buffer[i]);
      }
      printk(KERN_INFO "sniffer: message end\n");
   }
   return error_count;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
   file_write(fd, *offset, buffer, len);
   printk(KERN_INFO "sniffer: Message to device: %s\n", buffer);
   return len;
}

static int dev_release(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "sniffer: Device successfully closed\n");
   file_close(fd);
   return 0;
}

module_init(sniffer_init);
module_exit(sniffer_exit);