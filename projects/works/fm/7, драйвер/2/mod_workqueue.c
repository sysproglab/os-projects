#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/interrupt.h>
#include <linux/timex.h>
#include<linux/slab.h>
MODULE_LICENSE("GPL");
static struct workqueue_struct *my_wq;
typedef struct {
	struct work_struct my_work;
	int id;
	u32 j;
	cycles_t cycles;
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
	kfree( (void *)wrk );
	return;
}
int init_module( void ) {
	my_work_t *work1, *work2;
	int ret;
	my_wq = create_workqueue( "my_queue" );
	if( my_wq ) {
		/* Queue some work (item 1) */
		work1 = (my_work_t*)kmalloc( sizeof(my_work_t), GFP_KERNEL );
		if( work1 ) {
			INIT_WORK( (struct work_struct *)work1, my_wq_function );
			work1->id = 1;
			work1->j = jiffies;
			work1->cycles = get_cycles();
			ret = queue_work( my_wq, (struct work_struct *)work1 );
		}
		/* Queue some additional work (item 2) */
		work2 = (my_work_t*)kmalloc( sizeof(my_work_t), GFP_KERNEL );
		if( work2 ) {
			INIT_WORK( (struct work_struct *)work2, my_wq_function );
			work2->id = 2;work2->j = jiffies;
			work2->cycles = get_cycles();
			ret = queue_work( my_wq, (struct work_struct *)work2 );
                }
	}
	return 0;
}
void cleanup_module( void ) {
	flush_workqueue( my_wq );
	destroy_workqueue( my_wq );
	return;
}
