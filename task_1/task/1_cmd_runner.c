/*
  Write a program which allow you to:
- run another programs via command line.
- get exit codes of terminated programs

## TIPS:
1. Use "2_fork_wait_exit.c" and "4_exec_dir.c" from examples. Combine them.
2. Parse input string according to the type of exec* (see "man exec").
   a) if execvp is used, string splitting into "path" and "args" is all you need.
3. Collect exit codes via waitpid/WEXITSTATUS.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	pid_t pid = fork();
	if (pid < 0) {
		printf("fork error\n");
	}
	if (pid > 0) {
		int status;
	       	waitpid(pid, &status, 0);
		printf("Ret code:%d\n", WEXITSTATUS(status));
	} else {
		char *full = calloc(sizeof(char), 100);
		printf("Enter the path, the name and the arguments: ");
		gets(full);
		int arg = 1;
		for (int i = 0; i < 100; i++) {
			if (full[i] == ' ')
			arg++;
		}
		char **parsed = calloc(sizeof(char *), arg);
		int j = 0;
		for (int i = 0; i < 100; i++) {
			if (full[i] == ' ') {
				parsed[j] = full + i + 1;
				j++;
			}
		}
		for (int i = 0; i < 100; i++) {
			if (full[i] == ' ')
			full[i] = '\0';
		}
		parsed[j] = NULL;
		execv(full,parsed);
	  return 0;
    }
}
