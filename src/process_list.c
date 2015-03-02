#include "process_list.h"

int add_process(pid_t pid, char* cmd) {
	/* Création du nouveau processus à ajouter à la liste */
	struct process_list* new_p = malloc(sizeof(struct process_list));
	new_p->no = 1;
	new_p->pid = pid;
	new_p->cmd = malloc((strlen(cmd) + 1) * sizeof(char));
	strcpy(new_p->cmd, cmd);
	new_p->prev = NULL;
	new_p->next = NULL;
	
	/* Ajout de ce processus en fin de liste */
	if(plist == NULL) plist = new_p;
	else {
		(new_p->no)++;
		/* On se place sur le dernier processus de la liste */
		struct process_list* cur = plist;
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

void remove_process(int sig) {
	/* Récupération du PID d'un fils ayant terminé */
	int status;
	pid_t pid = waitpid(-1, &status, WNOHANG);
	
	/* On recherche le processus à supprimer */
	struct process_list* cur = plist;
	while(cur != NULL && cur->pid != pid) {
		cur = cur->next;
	}

	/* Si le processus trouvé correspond au processus terminé on le supprime de la liste */
	if(cur != NULL && cur->pid == pid) {
		if(cur->prev == NULL) {
			plist = cur->next;
			if(cur->next != NULL) cur->next->prev = NULL;
		} else {
			cur->prev->next = cur->next;
			if(cur->next != NULL) cur->next->prev = cur->prev;
		}
		int no = cur->no;
		free(cur);

		/* Affichage du processus qui vient de se terminer */
		printf("\n[%d]+  %s\t\t%d\n", no, (status)?"Error":"Done", pid);
	}
}
