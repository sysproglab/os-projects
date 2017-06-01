//
// Created by kivi on 24.09.16.
//

#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/stat.h>
#include <asm/io.h>
#include <asm/irq_vectors.h>

#define SHARED_IRQ 17

static int irq = SHARED_IRQ, my_dev_id, irq_counter = 0;
module_param( irq, int, S_IRUGO );

static irqreturn_t my_interrupt( int irq, void *dev_id ) {
    static unsigned char scancode;

    irq_counter++;

    scancode = inb(0x60);

    printk( KERN_INFO "In the ISR: counter = %d\n", irq_counter );
    printk( KERN_INFO "Scancode: = %d\n", scancode );
    return IRQ_NONE;  /* we return IRQ_NONE because we are just observing */
}

static int __init my_init( void ) {
    if ( request_irq( irq, my_interrupt, IRQF_SHARED, "my_interrupt", &my_dev_id ) )
        return -1;
    printk( KERN_INFO "Successfully loading ISR handler on IRQ %d\n", irq );
    return 0;
}

static void __exit my_exit( void ) {
    synchronize_irq( irq );
    free_irq( irq, &my_dev_id );
    printk( KERN_INFO "Successfully unloading, irq_counter = %d\n", irq_counter );
}

module_init( my_init );
module_exit( my_exit );
MODULE_LICENSE( "GPL v2" );
