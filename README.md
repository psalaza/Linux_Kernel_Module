# OpSys2
Project 2: Kernel Module Programming

Group: Project2 24
Group Members: Charles Harris, Pedro Salazar, Spencer Lewis

File Listing:<br/>
README.md - this file; includes file descriptions, compilation instructions, division of labor, and known bugs<br/>

Directory - SystemCalls contains:<br/>
  Makefile - Used to compile, declare new system calls for when kernel is compiled<br/>
  sys_start_elevator.c - <br/>
  sys_issue_request.c - <br/>
  sys_stop_elevator.c - <br/>
  
Directory - SyscallModule contains:<br/>
  Makefile - Used to compile kernel module and create proc/<br/>
  elevator.c - Contains majority of project code which includes:<br/>
    
THIS STILL NEEDS TO BE EDITED<br/>
Makefile Description:<br/>
In order to run the implemented shell:<br/>
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
