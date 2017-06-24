#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/init.h>	
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>

#define SUCCESS 0
#define DEVICE_NAME "chardev"	/* Имя устройства, будет отображаться в /proc/devices */
#define BUF_LEN 180		/* Максимальная длина сообщения */

int device_init(void);
void device_exit(void);

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

module_init(device_init);
module_exit(device_exit);

/* объявлены как статические */

static int Major;	/* Старший номер устройства - драйвера 	*/
static int Device_Open = 0;	/* Счетчик открытия устройства  
			* Используется для предотвращения обращения 
				 * из нескольких процессов */
static char Message[BUF_LEN];	/* Текст сообщения */
static char *Message_Ptr;

static struct file_operations fops = {
	.read =  device_read,
	.write = device_write,
	.open =  device_open,
	.release = device_release
};

// Ниже мы задаём информацию о модуле, которую можно будет увидеть с помощью Modinfo
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nikita Shalyapin");

/* Загрузка модуля в ядро */
int device_init(void)
{
	/* Регистрация устройства */
        Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
	  printk(KERN_ALERT "Registering char device failed with %d\n", Major);
	  return Major;
	}

	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");

	return SUCCESS;
}

/* Выгрузка модуля из ядра */
void device_exit(void)
{
	/* Освобождение старшего номера устройства */
	unregister_chrdev(Major, DEVICE_NAME);	

	printk(KERN_ALERT "Removing character device /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	return;
}

/* Открытие файла устройства процессом "cat /dev/chardev" */
static int device_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Try to open character device /dev/%s c %d 0'.\n", DEVICE_NAME, Major);	
	if (Device_Open)
	  return -EBUSY;

	Device_Open++; // счетчик открытия устройства 

	if (strlen(Message) == 0) {
		sprintf(Message, "Please, write and read device!\n");
		Message_Ptr = Message;
	}

	try_module_get(THIS_MODULE);

	return SUCCESS;
}

/* Закрытие файла устройства процессом */
static int device_release(struct inode *inode, struct file *file)
{
 	printk(KERN_ALERT "Try to close character device /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	Device_Open--;	/* Возможно обслуживание другого процесса */

	/* 
	 * Уменьшить счетчик обращений, иначе после успешного
	 * открытия не сможем больше выгрузить модуль.
	 */

  	module_put(THIS_MODULE);

  	return SUCCESS;	
}

/* Открытие файла устройства процессом для чтения */
static ssize_t device_read(struct file *filp, /*указатель на структуру file*/
		char *buffer,	/* буфер, куда надо положить данные */
		size_t length,	/* длина буфера */
		loff_t * offset)
{	
	int bytes_read = 0; /* Количество байт, записанных в буфер */
	printk("Try to read character device /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	if (*Message_Ptr == 0) /* Если достигли конца сообщения, */
	 	return 0;      /*вернуть ноль, как признак конца файла*/

	/* Помещение данных в буфер*/
	while (length && *Message_Ptr) {
     			put_user(*(Message_Ptr++), buffer++);
	  	length--;
	  	bytes_read++;
	}

	printk("Read %d bytes, %d left.\n", bytes_read, length);
	/* Возвращаем число байт, записанных в буфер */
	return bytes_read;
}

/* Открытие файла устройства процессом для записи */
static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t * offset)
{
	static int counter = 0;
	int i;
	printk("Try to write character device /dev/%s (%d).\n", DEVICE_NAME, length);
	memset(Message, 0, BUF_LEN);
	for (i = 0; i < length && i < (BUF_LEN - 40); i++)
		get_user(Message[i], buffer + i);
	counter++;
	sprintf(Message + strlen(Message), "\nI already told you %d times HELLO!\n", counter);
	Message_Ptr = Message;
	return i;
}
