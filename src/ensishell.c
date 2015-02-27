/*****************************************************
 * Copyright Grégory Mounié 2008-2013                *
 *           Simon Nieuviarts 2002-2009              *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "variante.h"
#include "readcmd.h"
#include "process_list.h"

#ifndef VARIANTE
#error "Variante non défini !!"
#endif

int main() {
	printf("Variante %d: %s\n", VARIANTE, VARIANTE_STRING);

	struct process_list* plist = NULL;

	while (1) {
		struct cmdline *l;
		int i;
		char *prompt = "ensishell>";

		l = readcmd(prompt);

		/* If input stream closed, normal termination */
		if (!l) {
			printf("exit\n");
			exit(0);
		}

		if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		if (l->in) printf("in: %s\n", l->in);
		if (l->out) printf("out: %s\n", l->out);

		if (l->seq[0] == NULL) {
			//Affichage des processus terminés et retrait de la liste
			plist = check_finish(plist);

			continue;
		}

		if (strcmp(l->seq[0][0], "jobs") == 0) {
			//Affichage des processus terminés et retrait de la liste
			plist = check_finish(plist);
			printf("[DEBUG] print_jobs entering - plist value : %p\n", plist);
			int i  = 1;
			struct process_list* courant = plist;
			while(courant != NULL) {
				printf("[%d] ", i);
				printf(" Running");
				printf("\t%s\n", courant->cmd);
				courant = courant->next;
				i++;
			}
			printf("[DEBUG] print_jobs exiting\n");
			continue;
		}

		for (i = 0; l->seq[i] != 0; i++) {
			//Affichage des processus terminés et retrait de la liste
			plist = check_finish(plist);

			char **cmd = l->seq[i];
			// Duplication du shell
			pid_t pid = fork();
			// Dans le processus fils, pid = 0. On exécute donc la commande
			if(pid == 0) execvp(cmd[0], cmd);
			else {
				if(!l->bg) {
					// Le père attends la terminaison de son fils
					int status;
					wait(&status);
				} else {
					printf("[DEBUG] Before adding process : plist value : %p\n", plist);
					int pos;
					plist = add_process(plist, &pos, pid, cmd[0]);
					printf("[%d] %d", pos, pid);
					printf("[DEBUG] After adding process : plist value : %p\n", plist);
				}
			}
		}
	}
}
