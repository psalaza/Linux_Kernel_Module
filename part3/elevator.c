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

struct task_struct * proc_entry;
struct list_head passenger_list;

struct { int animal; int total_size; int destination; int type; int total_weight; struct list_head list; } elevator;
typedef struct passengers {  int destination; int animal; int weight;  struct list_head list; } Passengers;
typedef struct floors { int start; int destination; int animal; int weight;  struct list_head list; } Floors;

int elevator_on(void * data);
int add_passenger(Floors *b , struct list_head * position, int onfloor);
int delete_passenger(struct list_head* position, Passengers *a, int ev);
int request(Floors * myF, int rStart, int rDest, int rAnimal, int rWeight);


int elevator_on(void * data) {
	int currentfloor = 1;
	Passengers *a;
	Floors *b;

	INIT_LIST_HEAD(&elevator.list);
	INIT_LIST_HEAD(&passenger_list);
	struct list_head *position;
	struct list_head *dummy;
	printk(KERN_INFO "bob");

	// Declaration for adding a request

	/*
	Floors* people1 = kmalloc(sizeof(Floors) * 1, __GFP_RECLAIM);
	if (people1 == NULL)
		return -ENOMEM;
	people1->start = 3;
	people1->destination = 5;
	people1->animal = 1;
	people1->weight = 1;
	list_add_tail(&people1->list, &passenger_list);
	 people1 = kmalloc(sizeof(Floors) * 1, __GFP_RECLAIM);
	 people1->start = 3;
	 people1->destination = 7;
	 people1->animal = 1;
	 people1->weight = 1;
	 list_add_tail(&people1->list, &passenger_list);
	 people1 = kmalloc(sizeof(Floors) * 1, __GFP_RECLAIM);
	 people1->start = 8;
	 people1->destination = 2;
	 people1->animal = 1;
	 people1->weight = 1;
	 list_add_tail(&people1->list, &passenger_list);
*/


	 Floors* people_one = kmalloc(sizeof(Floors) * 1, __GFP_RECLAIM);
	 request(people_one, 3, 5, 1, 1);
	 people_one = kmalloc(sizeof(Floors) * 1, __GFP_RECLAIM);
	 request(people_one, 3, 7, 1, 1);
	 people_one = kmalloc(sizeof(Floors) * 1, __GFP_RECLAIM);
	 request(people_one, 8, 2, 1, 1);


	while (!kthread_should_stop()) {
		ssleep(1);
		printk("me");
		a = list_first_entry_or_null(&elevator.list, Passengers, list);
		b = list_first_entry_or_null(&passenger_list, Floors, list);
		if (elevator.total_size != 0 && elevator.destination == 0 && a != NULL) {
			elevator.destination = a->destination;
			printk(KERN_INFO " elvator people %d", elevator.destination);

		}
		else if (elevator.destination == 0 && elevator.total_size == 0 && b != NULL) {
			elevator.destination = b->start;
			printk(KERN_INFO " floor people %d", b->start);
			//kfree(b);
		}


		// Place Lock
		printk(KERN_INFO " %d", elevator.total_size);
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
	else if (((elevator.total_weight + b->weight )> MAX_Weight) || (elevator.type != b->animal&& elevator.animal != 0) || (onfloor < elevator.destination && b->start > b->destination) || (onfloor > elevator.destination && b->start < b->destination)) {
		printk("me2");
		return 0;
	}
	
	people = kmalloc(sizeof(Passengers) * 1, __GFP_RECLAIM);
	printk("me3");
	if (people == NULL)
		return -ENOMEM;
	people->destination = b->destination;
	people->weight = b->weight;
	printk("me4");
	list_add_tail(&people->list, &elevator.list);
	elevator.animal += b->animal;
	elevator.total_weight += b->weight;
	elevator.type = b->animal;
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
	proc_entry2 = proc_create("elevate", 0666, NULL, &myops);
	 proc_entry  = kthread_run(elevator_on, NULL, "elevator");;

	if(proc_entry == NULL)
		return -ENOMEM;

	return 0;
}


int request(Floors * myF, int rStart, int rDest, int rAnimal, int rWeight) {

	if (myF == NULL)
		return -ENOMEM;
	if (rStart > 10 || rStart < 1 || rDest > 10 || rDest < 1)
		return -EOVERFLOW;
	myF->start = rStart;
	myF->destination = rDest;
	myF->animal = rAnimal;
	myF->weight = rWeight;
	list_add_tail(&myF->list, &passenger_list);

	return 1;
}


static void __exit hello_end(void){

	kthread_stop(proc_entry);
	remove_proc_entry("elevate", NULL);
	printk(KERN_INFO "Cleaning up module.\n");
	return;
}

module_init(elevatorProtacal);
module_exit(hello_end);
