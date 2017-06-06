#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/fs.h>
#include <asm/uaccess.h> 

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Chebotarev Michael" );

#define SUCCESS 0
#define DEVICE_NAME "symbolDevice" 

static int device_open( struct inode *, struct file * );
static int device_release( struct inode *, struct file * );
static ssize_t device_read(struct file *file, char *buf, size_t count, loff_t *off);
static ssize_t device_write(struct file *file, const char *buf, size_t count, loff_t *off);

static int major_number; // senior number of device
static int is_device_open = 0; 

static char text[100]= "It`s symbolDriver\n"; 
static char* text_ptr; 

static struct file_operations fops =
 {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release
 };

static int __init test_init( void )
{
 printk( "Driver \”symbolDriver\” is loaded!\n" );

 major_number = register_chrdev( 0, DEVICE_NAME, &fops );

 if ( major_number < 0 )
 {
  printk( "Registering the character device failed with %d\n", major_number );
  return major_number;
 }
 printk( "Major number: %d 0'.\n", major_number ); 
 return SUCCESS;
}

static void __exit test_exit( void )
{
 unregister_chrdev( major_number, DEVICE_NAME );
 printk( KERN_ALERT " Driver \”symbolDriver\” is unloaded!\n" );
}

module_init( test_init );
module_exit( test_exit );

static int device_open( struct inode *inode, struct file *file )
{
 text_ptr = text;
 if ( is_device_open )
  return -EBUSY;
 is_device_open++;
 return SUCCESS;
}

static int device_release( struct inode *inode, struct file *file )
{
 is_device_open--;
 return SUCCESS;
}

static ssize_t device_write(struct file *file, const char *buf, size_t count, loff_t * off )
{
  
int write_read = 0;
 while ( count && *buf )
 {
  get_user( *( text_ptr++ ), buf++ );
  count--;
  write_read++;
  }
 return write_read;
}

static ssize_t device_read( struct file *file, char *buf, size_t count, loff_t * off )
{
 int byte_read = 0;
 if ( *text_ptr == 0 )
  return 0;
 while ( count && *text_ptr )
 {
  put_user( *( text_ptr++ ), buf++ );
  count--;
  byte_read++;
 }

 return byte_read;
}
