// /security/mycatch/mycatch.c
//---INCLUDES
#include <linux/module.h>
#include <linux/lsm_hooks.h>

//---HOOKS
//setpriority hook
static int mycatch_task_setpriority(int which, int who, int niceval)
{
    	printk("<mycatch> hook for setpriority (which - %d, who - %d, niceval - %d)\n", which, who, niceval);
    	return 0;
}
//getpriority hook
static int mycatch_task_getpriority(int which, int who)
{
    	printk("%s\n","<mycatch> hook for getpriority(which - %d, who - %d)", which, who);
    	return 0;
}
//uname hook
static int mycatch_task_uname(struct old_utsname __user *)
{
    	printk("%s\n","<mycatch> hook for uname");
    	return 0;
}
//---HOOKS REGISTERING
static struct security_hook_list mycatch_hooks[] =
{
    	LSM_HOOK_INIT(task_setpriority, mycatch_task_setpriority),
	LSM_HOOK_INIT(task_getpriority, mycatch_task_getpriority),
	LSM_HOOK_INIT(task_uname, mycatch_task_uname),
};

//---INIT
void __init mycatch_add_hooks(void)
{
    	security_add_hooks(mycatch_hooks, ARRAY_SIZE(mycatch_hooks));
}

