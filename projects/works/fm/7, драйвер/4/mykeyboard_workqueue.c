#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/timex.h>
#include <linux/slab.h>
#include "mykeyboard_workqueue.h"

static int irq = 1, my_dev_id, irq_counter = 0, esc_counter = 0;
static struct workqueue_struct *my_wq;
typedef struct {
	struct work_struct my_work;
	int id;
	u32 j;
	cycles_t cycles;
	int irq_cnt;
} my_work_t;

/* Bottom Half Function */
static void my_wq_function( struct work_struct *work ) {
	u32 j = jiffies;
	cycles_t cycles = get_cycles();
	my_work_t *wrk = (my_work_t*) work;
	printk( "#%d : %010lld [%05d] => %010lld [%05d]\n",
    		wrk->id,
    		(long long unsigned)wrk->cycles, wrk->j,
    		(long long unsigned)cycles, j
	);
	printk( KERN_INFO "In the ISR: counter = %d\n", wrk->irq_cnt );
	kfree( (void *)wrk );
	return;
}

/* Функция-обработчик прерывания*/
static irqreturn_t my_interrupt( int irq, void *dev_id ) {
	my_work_t *work1;
	irq_counter++;
	int ret;
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
	if( my_wq ) {
		/* Queue some work (item 1) */
		work1 = (my_work_t*)kmalloc( sizeof(my_work_t), GFP_KERNEL );
		if( work1 ) {
			INIT_WORK( (struct work_struct *)work1, my_wq_function );
			work1->id = 1;
			work1->j = jiffies;
			work1->cycles = get_cycles();
			work1->irq_cnt = irq_counter;
			ret = queue_work( my_wq, (struct work_struct *)work1 );
		}
	}
	return IRQ_NONE; /* we return IRQ_NONE because we are just observing */
}
/* Инициализация модуля и регистрация обработчика прерывания*/
static int __init my_init( void ) {
	my_wq = create_workqueue( "my_queue" );
	if (request_irq(irq, my_interrupt, IRQF_SHARED, "my_interrupt", &my_dev_id ) )
		return -1;
	printk( KERN_INFO "Successfully loading ISR handler on IRQ %d\n", irq );
	return 0;
}
/* Удаление обработчика прерывания */
static void __exit my_exit( void ) {
	flush_workqueue( my_wq );
	destroy_workqueue( my_wq );
	synchronize_irq( irq );
	free_irq( irq, &my_dev_id );
	printk( KERN_INFO "Successfully unloading, irq_counter = %d, esc_counter = %d\n", irq_counter, esc_counter );
}

int getIrqCounter( void )
{
	return irq_counter;
}
EXPORT_SYMBOL( getIrqCounter ); 
int getEscCounter( void )
{
	return esc_counter;
}
EXPORT_SYMBOL( getEscCounter ); 

module_init( my_init );
module_exit( my_exit );
MODULE_AUTHOR( "Masha Fomina" );
MODULE_DESCRIPTION( "lab 7" );
MODULE_LICENSE( "GPL v2" );
