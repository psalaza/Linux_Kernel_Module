obj-m := my_timer.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

#TO RUN CODE:
#(compile)	make
#(CREATE proc)	sudo insmod my_timer.ko
#(test)		cat /proc/timed
#		.
#		.
#		.
#		. keep calling cat /proc/timed
#		. can also use sleep 1....x (x > 1) to show wait results
#		.
#(DEL PROC)	sudo rmmod my_timer.ko
