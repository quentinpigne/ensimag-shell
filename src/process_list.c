#include "process_list.h"

struct process_list* add_process(struct process_list* list, int* pos, pid_t pid, char* cmd) {
	*pos = 1;
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
		(*pos)++;
	}
	printf("[DEBUG] add_process exiting - list value : %p / process added : %d - %s\n", list, new_process->pid, new_process->cmd);
	return list;
}

struct process_list* remove_process(struct process_list* list, int* pos, pid_t pid) {
	*pos = 1;
	struct process_list* prev = NULL;
	struct process_list* cur = list;
	while(cur->pid != pid && cur != NULL) {
		(*pos)++;
		prev = cur;
		cur = cur->next;
	}
	if(prev != NULL) {
		prev->next = cur->next;
	} else {
		list = NULL;
	}
	free(cur);
	return list;
}

struct process_list* check_finish(struct process_list* plist) {
	int status;
	int pos;
	pid_t finished_pid = waitpid(-1, &status, WNOHANG);
	while(finished_pid > 0) {
		plist = remove_process(plist, &pos, finished_pid);
		printf("[%d] %s \t %d\n", pos, (status)?"Error":"Done", finished_pid);
		finished_pid = waitpid(-1, &status, WNOHANG);
	}
	return plist;
}
