#ifndef __PROCESS_LIST_H
#define __PROCESS_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern struct process_list* plist;

/* La liste des processus est doublement chainée */
struct process_list {
	int no;
	pid_t pid;
	char* cmd;
	struct process_list* prev;
	struct process_list* next;
};

int add_process(pid_t pid, char* cmd);
void remove_process(int sig);

#endif
