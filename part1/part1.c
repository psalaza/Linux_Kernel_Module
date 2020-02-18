#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#define __NR_part1_CALL1 323
#define __NR_part1_CALL2 324
#define __NR_part1_CALL3  325
#define __NR_part1_CALL4  326
#define __NR_part1_CALL5  327
int main() {
	syscall(__NR_part1_CALL1, 1);
	syscall(__NR_part1_CALL2, 2);
	syscall(__NR_part1_CALL3, 4);
	syscall(__NR_part1_CALL4, 5);
	syscall(__NR_part1_CALL5, 6);


}