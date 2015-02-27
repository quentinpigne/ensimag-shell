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

struct process_list* add_process(struct process_list *list, int* pos, pid_t pid, char* cmd);
struct process_list* remove_process(struct process_list *list, int* pos, pid_t pid);

struct process_list* check_finish(struct process_list* plist);

#endif
