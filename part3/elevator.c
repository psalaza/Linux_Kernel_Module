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


MODULE_LICENSE("Dual BSD/GPL");

#define MAX_Weight 15
int count = 0;

int value = 0;
static struct proc_dir_entry* proc_entry2;

int currentfloor;
struct task_struct * proc_entry;
struct list_head passenger_list;
struct mutex my_mutex;


struct { int animal; int total_size; int destination; int type; int total_weight; struct list_head list; } elevator;
typedef struct passengers {  int destination; int animal; int weight;  struct list_head list; } Passengers;
typedef struct floors { int start; int destination; int animal; int weight;  struct list_head list; } Floors;

int elevator_on(void * data);
int add_passenger(Floors *b , struct list_head * position, int onfloor);
int delete_passenger(struct list_head* position, Passengers *a, int ev);
int request(int rStart, int rDest, int rAnimal, int rWeight);


int elevator_on(void * data) {
	currentfloor = 1;
	Passengers *a;
	Floors *b;

	INIT_LIST_HEAD(&elevator.list);
	INIT_LIST_HEAD(&passenger_list);
	struct list_head *position;
	struct list_head *dummy;
	printk(KERN_INFO "bob");

/*
	 request(people_one, 3, 5, 1, 1);
	 request(people_one, 3, 7, 1, 1);
	 request(people_one, 8, 2, 1, 1);
*/

	while (!kthread_should_stop()) {
		ssleep(1);
		printk("me");
		a = list_first_entry_or_null(&elevator.list, Passengers, list);
		b = list_first_entry_or_null(&passenger_list, Floors, list);
		if (elevator.total_size != 0 && elevator.destination == 0 && a != NULL) {
			elevator.destination = a->destination;
			printk(KERN_INFO " elvator people %d", a->animal);

		}
		else if (elevator.destination == 0 && elevator.total_size == 0 && b != NULL) {
			elevator.destination = b->start;
			printk(KERN_INFO " floor people %d", b->start);
			//kfree(b);
		}


		// Place Lock
		printk(KERN_INFO " %d" ,elevator.destination);
		printk(KERN_INFO " %d", currentfloor);
		if (elevator.destination != 0) {
			list_for_each_safe(position,dummy ,&elevator.list) {
				a = list_entry(position, Passengers, list);
				if (a->destination == currentfloor) {
					delete_passenger(position, a, 0);
					value = 1;
					printk("me");
				}
			}

		// Place Lock
			list_for_each_safe(position,dummy ,&passenger_list) {
				b = list_entry(position, Floors, list);
				if (b->start == currentfloor) {
					value = add_passenger(b,position,currentfloor);
				}
				printk("me8");
			}
			printk("me9");
		}
		printk("me10");
		if (value == 1) {


			//ssleep(1);
		}
		value = 0;

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
		//ssleep(2);
		currentfloor--;
	}
	else if (currentfloor < elevator.destination) {
		//ssleep(2);
		currentfloor++;
	}
	else if (elevator.destination == currentfloor) {
		elevator.destination = 0;
		//break;
	}
	else if (elevator.destination == -1) {
	}
	list_for_each_safe(position, dummy, &elevator.list) {
		a = list_entry(position, Passengers, list);
		printk("in elevator %d" , a->destination);
	}

}

	return 0;
}

int add_passenger(Floors* b, struct list_head * position,int onfloor) {
	Passengers *people;
	if(elevator.total_size ==0 ){}
	else if (elevator.total_weight + b->weight > MAX_Weight || ((elevator.type == b->animal && elevator.animal != 0)|| (b->animal==0)) || (onfloor < elevator.destination && b->start > b->destination) || (onfloor > elevator.destination && b->start < b->destination)) {
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
	printk("me4");
	list_add_tail(&people->list, &elevator.list);
	elevator.animal += (b->weight-3);
	elevator.total_weight += b->weight;
	if (elevator.type == 0) {
		elevator.type = b->animal;
	}
	elevator.total_size++;
	printk("me5");
	list_del(position);
	kfree(b);

	return 1;
}

int delete_passenger(struct list_head* position, Passengers *a,int ev) {
	if (ev == 0) {
		elevator.animal -= a->animal;
		elevator.total_weight -= a->weight;
		elevator.total_size--;
		if (elevator.animal == 0) {
			elevator.type = 0;
		}
	}
	list_del(position);
	kfree(a);
	return 0;

}

static struct file_operations myops = {
	.owner = THIS_MODULE,
	//.read = procfile_read,
};

static int __init elevatorProtacal(void){

	STUB_start_elevator = start_elevator;
	STUB_stop_elevator = stop_elevator;
	STUB_issue_request = issue_request;

	proc_entry2 = proc_create("elevate", 0666, NULL, &myops);
	proc_entry  = kthread_run(elevator_on, NULL, "elevator");;

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
	myF->start = rStart;
	myF->destination = rDest;
	myF->animal = rAnimal;
	myF->weight = (rWeight*rAnimal)+3;
	list_add_tail(&myF->list, &passenger_list);

	return 1;
}

extern long (*STUB_start_elevator) (void);
int start_elevator(void) {
	currentfloor = 1;
	return 0;
}

extern long (*STUB_stop_elevator) (void);
int stop_elevator(void) {
	elvator.destination = -1;
	return 0;
}

extern long (*STUB_issue_request) (int, int , int,int);
int issue_request(int num_pets, int pet_type, int  start_floor, int destination_floor) {
	int i,b;
	get_random_bytes(&i, sizeof(int));
	i = i % 4;
	get_random_bytes(&i, sizeof(int));
	b = 0% 2
	request(start_floor, destination_floor, pet_type, num_pets);
}

static void __exit hello_end(void){

	kthread_stop(proc_entry);
	remove_proc_entry("elevate", NULL);
	printk(KERN_INFO "Cleaning up module.\n");
	return;
}

module_init(elevatorProtacal);
module_exit(hello_end);
