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


/*Path, name and arguments of program that you want to execute can be provided by writing it as arguments of cmd.*/


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
		switch (argc) {
			case 1:
				printf ("You didn't write any program name\n");
			case 2:
				execl(argv[1], argv[1], NULL);
			case 3:
				execl(argv[1], argv[2], NULL);
			case 4:
				execl(argv[1], argv[2], argv[3], NULL);
			case 5: execl(argv[1], argv[2], argv[3], argv[4], NULL);
			default:
				printf("There are too many arguments!\n");
		}
	}
	return 0;
}
