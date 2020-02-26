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
MODULE_LICENSE("Dual BSD/GPL");



#define MAX_Weight 15
int floor;
int count = 0;

int value = 0;
static struct proc_dir_entry* proc_entry2;

struct task_struct * proc_entry;
struct list_head passenger_list;

struct { int animal; int total_size; int destination; int total_weight; struct list_head list; } elevator;
typedef struct passengers {  int destination; int animal; int weight;  struct list_head list; } Passengers;
typedef struct floors { int start; int destination; int animal; int weight;  struct list_head list; } Floors;
int elevator_on(void * data);
int add_passenger(int animal, int floor, int weight);
int delete_passenger(struct list_head* position, Passengers *a, int ev);
int elevator_on(void * data) {
	int currentfloor = 1;
	Passengers *a;
	Floors *b;
	
	INIT_LIST_HEAD(&elevator.list);
	INIT_LIST_HEAD(&passenger_list);
	struct list_head *position;
	struct list_head *dummy;
	printk(KERN_INFO "bob");
	Floors* people1 = kmalloc(sizeof(Floors) * 1, __GFP_RECLAIM);
	if (people1 == NULL)
		return -ENOMEM;
	people1->start = 3;
	people1->destination = 5;
	people1->animal =1;
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
		
		//if (a != NULL) {
		//	kfree(a);
		//}
		//if (b != NULL) {
		//	kfree(b);
		//}
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
			list_for_each_safe(position,dummy ,&passenger_list) {
				b = list_entry(position, Floors, list);
				if (b->start == currentfloor) {
					add_passenger(b->animal, b->destination, b->weight);
					list_del(position);
					printk("me6");
					kfree(b);
					printk("me7");
					value = 1;
				}
				printk("me8");
			}
			printk("me9");
		}
		printk("me10");
		if (value == 1) {
			value = 0;
			printk("me11");
			//ssleep(1);
		}
		
		
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
int add_passenger(int animal,int floor,int weight) {
	Passengers *people;
	if (elevator.total_weight + weight > MAX_Weight)
		return 0;
	printk("me2");
	people = kmalloc(sizeof(Passengers) * 1, __GFP_RECLAIM);
	printk("me3");
	if (people == NULL)
		return -ENOMEM;
	people->destination = floor;
	people->animal = animal;
	people->weight = weight;
	printk("me4");
	list_add_tail(&people->list, &elevator.list);
	elevator.animal += animal;
	elevator.total_weight += weight;
	elevator.total_size++;
	printk("me5");
	return 1;
}
int delete_passenger(struct list_head* position, Passengers *a,int ev) {
	if (ev == 0) {
		elevator.animal -= a->animal;
		elevator.total_weight -= a->weight;
		elevator.total_size--;
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



static void __exit hello_end(void){
	
	kthread_stop(proc_entry);
	remove_proc_entry("elevate", NULL);
	return;
}

module_init(elevatorProtacal);
module_exit(hello_end);
