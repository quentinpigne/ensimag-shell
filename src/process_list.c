#include "process_list.h"

struct process_list* add_process(struct process_list* list, pid_t pid, char* cmd) {
	printf("[DEBUG] add_process entering - list value %p\n", list);
	struct process_list* new_process = malloc(sizeof(struct process_list));
	new_process->pid = pid;
	new_process->cmd = malloc((strlen(cmd) + 1) * sizeof(char));
	strcpy(new_process->cmd, cmd);
	new_process->next = NULL;
	if(list == NULL) list = new_process;
	else {
		struct process_list* courant = list;
		while(courant->next) courant = courant->next;
		courant->next = new_process;
	}
	printf("[DEBUG] add_process exiting - list value : %p / process added : %d - %s\n", list, new_process->pid, new_process->cmd);
	return list;
}

struct process_list* remove_process(struct process_list* list, pid_t pid) {
	struct process_list* prev = NULL;
	struct process_list* cur = list;
	while(cur->pid != pid && cur != NULL) {
		prev = cur;
		cur = cur->next;
	}
	prev->next = cur->next;
	free(cur);
	return list;
}
