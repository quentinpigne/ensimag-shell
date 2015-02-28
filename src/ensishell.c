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
		char *prompt = "ensishell>";

		l = readcmd(prompt);

		/*
			Gestion des erreurs et des cas particuliers
		*/

		/* If input stream closed, normal termination */
		if (!l) {
			check_finish(&plist);
			printf("exit\n");
			exit(0);
		}

		/* Syntax error, read another command */
		if (l->err) {
			check_finish(&plist);
			printf("error: %s\n", l->err);
			continue;
		}

		/* Gestion de l'absence de commande */
		if (l->seq[0] == NULL) {
			check_finish(&plist);
			continue;
		}

		/*
			Gestion des commandes du Shell
		*/

		/* Affichage des processus en arriere-plan */
		if (strcmp(l->seq[0][0], "jobs") == 0) {
			check_finish(&plist);
			struct process_list* courant = plist;
			while(courant != NULL) {
				printf("[%d]+ ", courant->no);
				printf(" Running");
				printf("\t\t%s\n", courant->cmd);
				courant = courant->next;
			}
			continue;
		}

		/* Sortie du Shell */
		if (strcmp(l->seq[0][0], "exit") == 0) {
			check_finish(&plist);
			printf("exit\n");
			exit(0);
		}

		/*
			Gestion des redirections in et out
		*/
		if (l->in || l->out) {
			check_finish(&plist);
			for (int i = 0; l->seq[i] != 0; i++) {
				/* Gestion des redirections */
			}
			continue;
		}

		/*
			Gestion du pipe
		*/
		if(l->seq[1] != NULL) {
			check_finish(&plist);
			for (int i = 0; l->seq[i] != 0; i++) {
				/* Gestion du pipe */
			}
			continue;
		}

		/*
			Gestion des commandes simples
		*/
		check_finish(&plist);
		char **cmd = l->seq[0];
		/*  Duplication du shell */
		pid_t pid = fork();
		/* Dans le processus fils, pid = 0. On exécute donc la commande */
		if(pid == 0) execvp(cmd[0], cmd);
		else {
			if(!l->bg) {
				/* Le père attends la terminaison de son fils */
				int status;
				wait(&status);
			} else {
				int pos = add_process(&plist, pid, cmd[0]);
				printf("[%d] %d\n", pos, pid);
			}
		}
	}
}
