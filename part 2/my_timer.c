#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/time.h>


MODULE_LICENSE("Dual BSD/GPL");


int z,z1;
static struct proc_dir_entry* proc_entry;
int countt = 0;
static char msg[100];
static int procfs_buf_len;


static ssize_t procfile_read(struct file *file, char *ubuf, size_t count, loff_t *ppos){

	static int finished = 0;
	if(finished){
		finished = 0;
		return 0;
	}
	finished = 1;
	struct timespec temp;
	struct timespec t = current_kernel_time();

	int r = (int)t.tv_sec - z;
	int r1;

	if(z1 > (int)t.tv_nsec)
		r1 = z1 - (int)t.tv_nsec;
	else
		r1 = (int)t.tv_nsec - z1;

	if(countt == 0)
		sprintf(ubuf, "current time: %10i.%09i \n",(int)t.tv_sec,(int)t.tv_nsec);
	else
		sprintf(ubuf, "current time: %10i.%09i \nelapsed time: %d.%09d \n",(int)t.tv_sec,(int)t.tv_nsec,r,r1);

	z = (int)t.tv_sec;
	z1 = (int)t.tv_nsec;
	countt = 1;
	return strlen(ubuf);
		

}


static struct file_operations myops = {
	.owner = THIS_MODULE,
	.read = procfile_read,
};

static int __init hello_start(void){

	proc_entry = proc_create("timed",0666,NULL,&myops);

	if(proc_entry == NULL)
		return -ENOMEM;

	return 0;
}

static void __exit hello_end(void){

	remove_proc_entry("timed",NULL);
	return;
}

module_init(hello_start);
module_exit(hello_end);
