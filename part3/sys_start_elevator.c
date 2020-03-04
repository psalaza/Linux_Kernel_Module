#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
/* System call stub */
long(*STUB_start_elevator)(void) = NULL;
EXPORT_SYMBOL(STUB_start_elevator);/* System call wrapper */
SYSCALL_DEFINE0(test_call) {
	printk(KERN_NOTICE "Inside SYSCALL_DEFINE1 block. %s: Your int is %d\n", __FUNCTION__, test_int);
	if (STUB_test_call != NULL)
		return STUB_test_call(); 
	else
		return -ENOSYS;
}