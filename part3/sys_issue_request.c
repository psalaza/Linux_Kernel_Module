#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
/* System call stub */
long(*STUB_issue_request)(int,int,int,int) = NULL;
EXPORT_SYMBOL(STUB_issue_request);/* System call wrapper */
SYSCALL_DEFINE4(test_call, int, num_pets, int , pet_type, int, start_floor, int, destination_floor ) {
	printk(KERN_NOTICE "Inside SYSCALL_DEFINE1 block. %s: Your int is %d\n", __FUNCTION__, test_int);
	if (STUB_issue_request != NULL)
		return STUB_issue_request(num_pets,pet_type,start_floor,destination_floor); else
		return -ENOSYS;
}
