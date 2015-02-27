#ifndef __PROCESS_LIST_H
#define __PROCESS_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct process_list {
	pid_t pid;
	char* cmd;
	struct process_list* next;
};

int add_process(struct process_list** plist, pid_t pid, char* cmd);
int remove_process(struct process_list** plist, pid_t pid);

void check_finish(struct process_list** plist);

#endif
