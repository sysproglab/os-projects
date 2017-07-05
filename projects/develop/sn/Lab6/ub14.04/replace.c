#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h>
#include <linux/kallsyms.h>
#include <linux/utsname.h>

#define GPF_DISABLE write_cr0(read_cr0() & (~ 0x10000))
#define GPF_ENABLE write_cr0(read_cr0() | 0x10000)

// sys_call_table address in System.map
void **sys_call_table = (void*)0xffffffff818001c0;
long unsigned int addr = 0xffffffff818001c0;

asmlinkage int (*original_call) (const char*, int, int);
asmlinkage int (*original_setpriority) (int, int, int);
asmlinkage int (*original_getpriority) (int, int);
asmlinkage int (*original_uname) (struct old_utsname __user *);

asmlinkage int my_uname(struct old_utsname __user *name)
{
	printk("System call uname was catched!\n");
	return original_uname(name);
}

asmlinkage long my_setpriority(int which, int who, int niceval)
{
	printk("System call setpriority was catched: which = %d, who = %d, niceval = %d\n", which, who, niceval);
	return original_setpriority(which, who, niceval);
}
asmlinkage long my_getpriority(int which, int who)
{
	printk("System call getpriority was catched: which = %d, who = %d\n", which, who);
	return original_getpriority(which, who);
}

int init_module()
{
	original_uname = sys_call_table[__NR_uname];
	original_setpriority = sys_call_table[__NR_setpriority];
	original_getpriority = sys_call_table[__NR_getpriority];

	GPF_DISABLE;
	sys_call_table[__NR_uname] = my_uname;
	sys_call_table[__NR_setpriority] = my_setpriority;
	sys_call_table[__NR_getpriority] = my_getpriority;
	GPF_ENABLE;
	return 0;
}

void cleanup_module()
{
	// Restore the original calls
	GPF_DISABLE;
	sys_call_table[__NR_uname] = original_uname;
	sys_call_table[__NR_setpriority] = original_setpriority;
	sys_call_table[__NR_getpriority] = original_getpriority;
	GPF_ENABLE;
}
