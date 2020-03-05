# OpSys2
Project 2: Kernel Module Programming

Group: Project2 24
Group Members: Charles Harris, Pedro Salazar, Spencer Lewis

File Listing:<br/>
README.md - this file; includes file descriptions, compilation instructions, division of labor, and known bugs<br/>

Directory - SystemCalls contains:<br/>
  Makefile - Used to compile, declare new system calls for when kernel is compiled<br/>
  sys_start_elevator.c, sys_issue_request.c, sys_stop_elevator.c - Each file<br/>
  contains the syscall pointer and wrapper functions used to compile the added<br/>
  system calls
  
Directory - SyscallModule contains:<br/>
  Makefile - Used to compile kernel module and create proc/<br/>
  elevator.c - Contains majority of project code which includes:<br/>
     Kernel module elevator implementation <br/>
     Syscall STUBS and functions they call <br/>
     /Proc printing functions <br/>
    
Makefile Description:<br/>
In order to 
  1) To compile project.c run:
  > make
  2) To begin the program run:
  > shell
  3) When finished run:
  > make clean

Division of Labor:<br/><br/>

  Charles Harris:<br/>
  System Call Tracing (Part 1)<br/>
  Elevator Scheduler (Step 1)<br/>
  Commenting, Testing, and Reviewing other sections of project<br/><br/>
  
  Pedro Salazar:<br/>
  Kernel Module (Part 2)<br/>
  Elevator Scheduler (Step 3)<br/>
  Commenting, Testing, and Reviewing other sections of project<br/><br/>
  
  Spencer Lewis:<br/>
  Elevator Scheduler (Step 2)<br/>
  README.md and Documentation<br/>
  Commenting, Testing, and Reviewing other sections of project<br/><br/>
  
Known Bugs:<br/>
  1) 
