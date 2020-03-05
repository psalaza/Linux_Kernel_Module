#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#define __NR_part1_CALL1 323			//system call number
#define __NR_part1_CALL2 324			//system call number
#define __NR_part1_CALL3  325			//system call number
#define __NR_part1_CALL4  326			//system call number
#define __NR_part1_CALL5  327           //system call number
int main() {
	syscall(__NR_part1_CALL1, 1);       //calling specific call at number
	syscall(__NR_part1_CALL2, 2);		//calling specific call at number
	syscall(__NR_part1_CALL3, 4);		//calling specific call at number
	syscall(__NR_part1_CALL4, 5);		//calling specific call at number
	syscall(__NR_part1_CALL5, 6);		//calling specific call at number


}