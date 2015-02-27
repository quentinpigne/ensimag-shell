#include "process_list.h"

int add_process(struct process_list** plist, pid_t pid, char* cmd) {
	int pos = 1;
	struct process_list* new_process = malloc(sizeof(struct process_list));
	new_process->pid = pid;
	new_process->cmd = malloc((strlen(cmd) + 1) * sizeof(char));
	strcpy(new_process->cmd, cmd);
	new_process->next = NULL;
	if(*plist == NULL) *plist = new_process;
	else {
		struct process_list* courant = *plist;
		while(courant->next) {
			courant = courant->next;
			pos++;
		}
		courant->next = new_process;
		pos++;
	}
	return pos;
}

int remove_process(struct process_list** plist, pid_t pid) {
	int pos = 1;
	struct process_list* prev = NULL;
	struct process_list* cur = *plist;
	while(cur->pid != pid && cur != NULL) {
		pos++;
		prev = cur;
		cur = cur->next;
	}
	if(prev != NULL) {
		prev->next = cur->next;
	} else {
		*plist = NULL;
	}
	free(cur);
	return pos;
}

void check_finish(struct process_list** plist) {
	int status;
	pid_t finished_pid = waitpid(-1, &status, WNOHANG);
	while(finished_pid > 0) {
		printf("[%d] %s \t %d\n", remove_process(plist, finished_pid), (status)?"Error":"Done", finished_pid);
		finished_pid = waitpid(-1, &status, WNOHANG);
	}
}
