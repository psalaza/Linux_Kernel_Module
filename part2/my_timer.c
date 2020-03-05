#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/time.h>


MODULE_LICENSE("Dual BSD/GPL");


int oldSec,oldNSec;
static struct proc_dir_entry* proc_entry;
int countt = 0;


static ssize_t procfile_read(struct file *file, char *ubuf, size_t count, loff_t *ppos){
	// finished variable to run proc print only once
	static int finished = 0;
	if(finished){
		finished = 0;
		return 0;
	}
	finished = 1;

	struct timespec t = current_kernel_time();
	// capture the difference between the new secnds and the old seconds
	int secDifference = (int)t.tv_sec - oldSec;
	int nsec_Difference;
	//make sure we subtract the larger number first to avoid negatives
	if(oldNSec > (int)t.tv_nsec)
		nsec_Difference = oldNSec - (int)t.tv_nsec;
	else
		nsec_Difference = (int)t.tv_nsec - oldNSec;
	//counter because first print is different then rest.
	if (countt == 0) {
		sprintf(ubuf, "current time: %10i.%09i \n", (int)t.tv_sec, (int)t.tv_nsec);
	}
	else
		sprintf(ubuf, "current time: %10i.%09i \nelapsed time: %d.%09d \n",(int)t.tv_sec,(int)t.tv_nsec,secDifference,nsec_Difference);
	//setting up a temp variable to equal the values that are going to be needed as the old seconds/nseconds
	oldSec = (int)t.tv_sec;
	oldNSec = (int)t.tv_nsec;

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
