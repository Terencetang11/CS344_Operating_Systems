# Assignment 4: Multi-threaded Producer Consumer Pipeline
 	By: Terence Tang
 	tangte@oregonstate.edu
 	CS 344 Operating Systems
 	5/16/2021


    Included files:
    - main.c 
    - thread_input.c
    - thread_line_separator.c
    - thread_plus_signs.c
    - thread_output.c
    - multithreaded.h


    Simple multithreaded line processor app for reformating text input written in C.  Features
	multithreaded implementation and highlights how to handle multi threaded processes:
	- Communication via shared resources across producer and consumer piplines
	- Mutual exclusivity access of shared resources
	- Conditional variables for process signaling


## Installation
    Instructions on how to compile Assignment #3 Smallsh:
	- Terminal Command for compiling -
	gcc --std=c99 -o line_processor main.c thread_input.c thread_line_separator.c thread_plus_signs.c thread_output.c

    - Terminal Command for running resulting executable -
    ./line_processor