#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

static int irq = 1, my_dev_id, irq_counter = 0, esc_counter = 0;

/* Функция-обработчик прерывания*/
static irqreturn_t my_interrupt( int irq, void *dev_id ) {
	irq_counter++;
	printk( KERN_INFO "In the ISR: counter = %d\n", irq_counter );
	static unsigned char scancode;
	/* Чтение скан кода нажатой клавиши */
	scancode = inb (0x60);
	/*Логирование в  сиcтемный журнал */
	if (scancode == 0x01) /*скан-код нажатия клавиши Esc*/
	{
		esc_counter++;
		printk ("Press Esc!\n");
	}
	if (scancode == 0x81) /*скан-код отпускания клавиши Esc*/
		printk ("Release Esc!\n");
	return IRQ_NONE; /* we return IRQ_NONE because we are just observing */
}
/* Инициализация модуля и регистрация обработчика прерывания*/
static int __init my_init( void ) {
	if (request_irq(irq, my_interrupt, IRQF_SHARED, "my_interrupt", &my_dev_id ) )
		return -1;
	printk( KERN_INFO "Successfully loading ISR handler on IRQ %d\n", irq );
	return 0;
}
/* Удаление обработчика прерывания */
static void __exit my_exit( void ) {
	synchronize_irq( irq );
	free_irq( irq, &my_dev_id );
	printk( KERN_INFO "Successfully unloading, irq_counter = %d, esc_counter = %d\n", irq_counter, esc_counter );
}
module_init( my_init );
module_exit( my_exit );
MODULE_AUTHOR( "Masha Fomina" );
MODULE_DESCRIPTION( "lab 7" );
MODULE_LICENSE( "GPL v2" );
