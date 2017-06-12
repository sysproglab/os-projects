#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/fs.h>
#include <asm/uaccess.h> 

// ���� �� ����� ���������� � ������, ������� ����� ����� ������� � ������� Modinfo
MODULE_AUTHOR( "Player1" );
MODULE_DESCRIPTION( "Lab3 module" );
MODULE_SUPPORTED_DEVICE( "lab3" ); /* /dev/testdevice */

#define DEVICE_NAME "lab3"

// �������������� ����� ����������� ��������
static int device_open( struct inode *, struct file * );
static int device_release( struct inode *, struct file * );
static ssize_t device_read( struct file *, char *, size_t, loff_t * );
static ssize_t device_write( struct file *, const char *, size_t, loff_t * );

// ���������� ����������, ��������� ��� static, ����������� ���������� ����.
static int major_number; /* ������� ����� ���������� ������ �������� */
static int is_device_open = 0; /* ������������ �� ������ ? */
static char text[ 5 ] = "test\n"; /* �����, ������� �� ����� �������� ��� ��������� � ������ ���������� */
static char* text_ptr = text; /* ��������� �� ������� ������� � ������ */

// ����������� ����������� �������� �� �����������
static struct file_operations fops =
 {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release
 };

// ������� �������� ������. ������� �����.
static int __init test_init( void )
{
 printk( KERN_ALERT "TEST driver loaded!\n" );

 // ������������ ��������� � �������� ������� ����� ����������
 major_number = register_chrdev( 0, DEVICE_NAME, &fops );

 if ( major_number < 0 )
 {
  printk( "Registering the character device failed with %d\n", major_number );
  return major_number;
 }

 // �������� ����������� ��� ������� ����� ����������
 printk( "Test module is loaded!\n" );

 return 0;
}

// ������� �������� ������
static void __exit test_exit( void )
{
 // ����������� ����������
 unregister_chrdev( major_number, DEVICE_NAME );

 printk( KERN_ALERT "Test module is unloaded!\n" );
}

// ��������� ���� ������� �������� � ��������
module_init( test_init );
module_exit( test_exit );

static int device_open( struct inode *inode, struct file *file )
{
 text_ptr = text;

 if ( is_device_open )
  return -EBUSY;

 is_device_open++;

 return 0;
}

static int device_release( struct inode *inode, struct file *file )
{
 is_device_open--;
 return 0;
}

device_write( struct file *filp, const char *buff, size_t len, loff_t * off )
{
 printk( "Write in device\n" );
 return -EINVAL;
}

static ssize_t device_read( struct file *filp, /* include/linux/fs.h */
       char *buffer, /* buffer */
       size_t length, /* buffer length */
       loff_t * offset )
{
 int byte_read = 0;

 if ( *text_ptr == 0 )
  return 0;

 while ( length && *text_ptr )
 {
  put_user( *( text_ptr++ ), buffer++ );
  length--;
  byte_read++;
 }

 return byte_read;
}
