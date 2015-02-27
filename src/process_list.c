#include "process_list.h"

int add_process(struct process_list** plist, pid_t pid, char* cmd) {
	/* Création du nouveau processus à ajouter à la liste */
	struct process_list* new_p = malloc(sizeof(struct process_list));
	new_p->no = 1;
	new_p->pid = pid;
	new_p->cmd = malloc((strlen(cmd) + 1) * sizeof(char));
	strcpy(new_p->cmd, cmd);
	new_p->prev = NULL;
	new_p->next = NULL;
	
	/* Ajout de ce processus en fin de liste */
	if(*plist == NULL) *plist = new_p;
	else {
		(new_p->no)++;
		/* On se place sur le dernier processus de la liste */
		struct process_list* cur = *plist;
		while(cur->next != NULL) {
			cur = cur->next;
			(new_p->no)++;
		}
		/* On ajoute en fin de liste */
		new_p->prev = cur;
		cur->next = new_p;
	}

	return new_p->no;
}

int remove_process(struct process_list** plist, pid_t pid) {
	/* On recherche le processus à supprimer */
	struct process_list* cur = *plist;
	while(cur->pid != pid && cur != NULL) {
		cur = cur->next;
	}

	/* On le supprime de la liste des processus */
	if(cur->prev == NULL) {
		*plist = cur->next;
		if(cur->next != NULL) cur->next->prev = NULL;
	} else {
		cur->prev->next = cur->next;
		if(cur->next != NULL) cur->next->prev = cur->prev;
	}
	int no = cur->no;
	free(cur);

	return no;
}

void check_finish(struct process_list** plist) {
	/* Récupération du PID d'un fils ayant terminé */
	int status;
	pid_t finished_pid = waitpid(-1, &status, WNOHANG);

	/* On affiche tous les processus ayant terminés */
	while(finished_pid > 0) {
		int no = remove_process(plist, finished_pid);
		printf("[%d]+  %s\t\t%d\n", no, (status)?"Error":"Done", finished_pid);
		finished_pid = waitpid(-1, &status, WNOHANG);
	}
}
