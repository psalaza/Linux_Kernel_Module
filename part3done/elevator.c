#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#define ENTRY_SIZE 800

MODULE_LICENSE("Dual BSD/GPL");

#define MAX_Weight 15
int count = 0;
static char *message;
static int read;
int value = 0;
static struct proc_dir_entry* proc_entry2;
int waiting = 0;
int serviced = 0;
int currentfloor;
struct task_struct * proc_entry;
struct list_head passenger_list;
struct mutex mutex;
int stop=-1;

struct { int animal; int total_size; int destination; int type; int total_weight; struct list_head list; } elevator;
typedef struct passengers {int total;  int destination; int animal; int weight;  struct list_head list; } Passengers;
typedef struct floors { int start; int destination; int animal; int total; int weight;  struct list_head list; } Floors;
int elevator_on(void * data);
int add_passenger(Floors *b , struct list_head * position, int onfloor);
int delete_passenger(struct list_head* position, Passengers *a, int ev);
int request(int rStart, int rDest, int rAnimal, int rWeight);
extern long (*STUB_start_elevator) (void);
long start_elevator(void) {
	currentfloor = 1;
        stop =0;
	return 0;
}

extern long (*STUB_stop_elevator) (void);
long  stop_elevator(void) {
	stop = -1;
	return 0;
}

extern long (*STUB_issue_request) (int, int , int,int);
long issue_request(int num_pets, int pet_type, int  start_floor, int destination_floor) {
	// I DO NOT THINK THIS IS CORRECT!!!!!!!!!!!!!!!!!
	// not sure what to do with the animal variable here
	
	request(start_floor, destination_floor, pet_type, num_pets);
}


int elevator_on(void * data) {

	INIT_LIST_HEAD(&elevator.list);
	INIT_LIST_HEAD(&passenger_list);

	currentfloor = 1;
	Passengers *a;
	Floors *b;

	struct list_head *position;
	struct list_head *dummy;
	int check=0;

	// request(3, 5, 1, 1);
	// request(3, 7, 1, 1);
	// request(8, 2, 2, 1);
	// request(5, 2, 1, 1);
	// request(8, 2, 1, 2);


	while (!kthread_should_stop()) {
		ssleep(1);
		printk("me");

		mutex_lock_interruptible(&mutex);
		a = list_first_entry_or_null(&elevator.list, Passengers, list);
		b = list_first_entry_or_null(&passenger_list, Floors, list);
		if (elevator.total_size != 0 && elevator.destination == 0 && a != NULL) {
			elevator.destination = a->destination;

		}
		else if (elevator.destination == 0 && elevator.total_size == 0 && b != NULL && stop !=-1) {
			elevator.destination = b->start;
			//kfree(b);
		}
		mutex_unlock(&mutex);


		printk(KERN_INFO " %d" ,elevator.destination);
		printk(KERN_INFO " %d", currentfloor);
		if (elevator.destination != 0) {
			if(elevator.total_size != 0){
				mutex_lock_interruptible(&mutex);
				list_for_each_safe(position,dummy ,&elevator.list) {
					a = list_entry(position, Passengers, list);
					if (a->destination == currentfloor) {
						delete_passenger(position, a, 0);
						value = 1;
						printk("me");
					}
				}
			mutex_unlock(&mutex);
		}

		
		if(stop !=-1){
			mutex_lock_interruptible(&mutex);
			list_for_each_safe(position,dummy ,&passenger_list) {
				b = list_entry(position, Floors, list);
				if (b->start == currentfloor) {
					check = add_passenger(b,position,currentfloor);
                                       if(check == 1){
					value=1;	
					}
				}
				printk("me8");
			}
			mutex_unlock(&mutex);
		}
			printk("me9");
		}
		printk("me10");
		if (value == 1) {


			ssleep(1);
		}
		value = 0;
		check=0;
		printk("me12");
	//	if (a != NULL) {
	//		kfree(a);
	//	}
	//	if (b != NULL) {
	//		kfree(b);
	//	}
	if (elevator.destination == 0){
		}
	else if (currentfloor > elevator.destination) {
		ssleep(2);
		currentfloor--;
	}
	else if (currentfloor < elevator.destination) {
		ssleep(2);
		currentfloor++;
	}
	else if (elevator.destination == currentfloor) {
		elevator.destination = 0;
		//break;
	}


}

	return 0;
}

int add_passenger(Floors* b, struct list_head * position,int onfloor) {
	Passengers *people;
	if(elevator.total_size ==0){}
	else  if ((elevator.total_weight + b->weight) > MAX_Weight || ((elevator.type != b->animal && elevator.animal != 0)|| (b->animal==0)) || (onfloor < elevator.destination && b->start > b->destination) || (onfloor > elevator.destination && b->start < b->destination)) {
		return 0;
	}
	printk("me2");
	people = kmalloc(sizeof(Passengers) * 1, __GFP_RECLAIM);
	printk("me3");
	if (people == NULL)
		return -ENOMEM;
	people->destination = b->destination;
	people->weight = b->weight;
	people->animal = b->animal;
	people->total = b->total;	
	printk("me4");
	list_add_tail(&people->list, &elevator.list);
	elevator.animal += (b->weight-3);
	elevator.total_weight += b->weight;
	if (elevator.type == 0) {
		elevator.type = b->animal;
	}
	elevator.total_size+= b->total;

	printk("me5");
	waiting-=b->total;
	list_del(position);
	kfree(b);

	return 1;
}

int delete_passenger(struct list_head* position, Passengers *a,int ev) {
	if (ev == 0) {
		elevator.animal -= a->animal;
		elevator.total_weight -= a->weight;
		elevator.total_size-= a->total;
		if (elevator.total_size == 0) {
			elevator.type = 0;
		}
		
	}
	serviced+= a->total;
	list_del(position);
	kfree(a);
	return 0;

}

/*
int elevator_open(void){

	read = 1;
	message = kmalloc(sizeof(char) * ENTRY_SIZE, __GFP_RECLAIM | __GFP_IO | __GFP_FS);

	strcpy(message,"testing\n");

	return 0;
}
*/

ssize_t procfile_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset){

	message = kmalloc(sizeof(char) * 2000, __GFP_RECLAIM | __GFP_IO | __GFP_FS);
	char temp[100];

	int len = strlen(message);

	strcpy(message,"Elevators info\n");
	mutex_lock_interruptible(&mutex);

	strcat(message,"Elevator state: ");
	printk("1\n");	
	 if (stop == -1 &&elevator.total_size ==0) {
		strcat(message,"OFFLINE\n");
	}

	else if (elevator.destination == 0 && elevator.total_size == 0 && waiting == 0){
		strcat(message,"IDLE\n");
		}
	
	else if (currentfloor > elevator.destination && elevator.destination != 0) {
		strcat(message,"DOWN\n");

	}
	else if (currentfloor < elevator.destination && elevator.destination !=0) {
		strcat(message,"UP\n");
	}
	else {
		strcat(message,"LOADING\n");
	}

	printk("2\n");	
	
	strcat(message,"Elevator animals: ");
	if(elevator.type == 2)
		strcat(message,"dog\n");
	else if(elevator.type == 1)
		strcat(message,"cat\n");
	else if(elevator.type == 0){
		strcat(message,"nothing\n");
		}
	strcat(message,"Current floor: ");
	sprintf(temp,"%d\n",currentfloor);
	strcat(message,temp);
	strcat(message,"Number of passengers: ");
	sprintf(temp,"%d \n",elevator.total_size);
	strcat(message,temp);
	printk("3\n");	

	strcat(message,"Current weight: ");
	sprintf(temp,"%d \n",elevator.total_weight);
	strcat(message,temp);

	strcat(message,"Number of passengers waiting: ");
	sprintf(temp,"%d \n",waiting);
	strcat(message,temp);

	strcat(message,"Number of passengers serviced: ");
	sprintf(temp,"%d \n",serviced);
	strcat(message,temp);
	printk("4\n");	

	int i;
        int dog;
	char t;
	Floors *cat;
	struct list_head *dummy;
	struct list_head *position;
	int arr = 0;
	char temp2[200];
	strcpy(temp2,"");

	printk("5\n");	

	for(i = 10; i > 0;i--){
		strcpy(temp2,"");
		if(currentfloor == i)
			strcat(message,"[*] Floor ");
		else
			strcat(message,"[ ] Floor ");
		printk("6\n");	

		list_for_each_safe(position, dummy, &passenger_list) {
			cat = list_entry(position, Floors, list);
			if(cat != NULL && cat->start == i){
				printk("7\n");	
				strcat(temp2,"| ");
				arr += cat->total;
                                for(dog =0;dog < cat->total-1;dog++){
					if(cat->animal ==2){
						strcat(temp2,"o ");
					}
					else{
						strcat(temp2,"x ");
					}
				}
			printk("8\n");	

			}
			
		//	printk("in elevator %d" , cat->destination);

		}
		printk("9\n");	

		sprintf(temp,"%d",i);
		strcat(message,temp);
		if(i == 10)
			strcat(message,": ");
		else
			strcat(message,":  ");

		sprintf(temp,"%d ",arr);
		strcat(message,temp);
		strcat(message,temp2);
		strcat(message,"\n");
		arr=0;
	}

	mutex_unlock(&mutex);
//	printk(message);
	static int finished = 0;

	if(finished){
		finished = 0;
		return 0;
	}
	finished++;
//	read = !read;
//	if(read)
//		return 0;
	
//	sprintf(buf,"testing...\n12123123123123\n");
	copy_to_user(buf,message,len);
	sprintf(buf,message);
	printk("10\n");	

//	kfree(message);
//	kfree(cat);

	return strlen(buf);
}

static struct file_operations myops = {
	.owner = THIS_MODULE,
	.read = procfile_read,
	//.open = elevator_open,
	//.close = elevator_close
};

static int __init elevatorProtacal(void){

	mutex_init(&mutex);
	STUB_start_elevator = start_elevator;
	STUB_stop_elevator = stop_elevator;
	STUB_issue_request = issue_request;
	mutex_init(&mutex);
	proc_entry2 = proc_create("elevator", 0666, NULL, &myops);
	proc_entry  = kthread_run(elevator_on, NULL, "elevator");

	if(proc_entry == NULL)
		return -ENOMEM;

	return 0;
}


int request(int rStart, int rDest, int rAnimal, int rWeight) {
	Floors * myF = kmalloc(sizeof(Floors) * 1, __GFP_RECLAIM);
	if (myF == NULL)
		return -ENOMEM;
	if (rStart > 10 || rStart < 1 || rDest > 10 || rDest < 1)
		return -EOVERFLOW;
	myF->total = rWeight + 1;
	myF->start = rStart;
	myF->destination = rDest;
	myF->animal = rAnimal;
	myF->weight = (rWeight*rAnimal)+3;
	list_add_tail(&myF->list, &passenger_list);
	waiting+= myF->total;
	return 1;
}


static void __exit hello_end(void){
	mutex_destroy(&mutex);
	STUB_start_elevator = NULL;
	STUB_stop_elevator = NULL;
	STUB_issue_request = NULL;
	kthread_stop(proc_entry);
	remove_proc_entry("elevator", NULL);
	printk(KERN_INFO "Cleaning up module.\n");
	return;
}

module_init(elevatorProtacal);
module_exit(hello_end);