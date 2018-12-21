#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <setjmp.h>
#include <signal.h>

struct rusage usuage;
int m_sig;
static sigjmp_buf newbuffer;
int sigusercounter = 0;
int arguement2 = 0;

void handler(int signal)
		{
			m_sig = signal;

			siglongjmp(newbuffer, 1);
		}

	void handlerSIGUSR1(int signal)
		{
			sigusercounter++;

			if(sigusercounter == arguement2){
				kill(getpid(), SIGUSR2);
			}
			else{
				
				siglongjmp(newbuffer, 1);
			}
			//m_sig = signal;
			//alarmflag = 1;
			//siglongjmp(newbuffer, 1);

		}

int main(int argc, char **argv)
{
	printf(" Jakub\n");

	if(argc > 1){
	arguement2 = atoi(argv[1]);
	}
	else{
		arguement2 = 3;
	}
	
	signal(SIGINT, handler);
	signal(SIGTSTP, handler);
	signal(SIGUSR1,handlerSIGUSR1);

        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        int status;
        int options;

        char** array = (char**)malloc(sizeof(char*) * 100);

        int i = 0;
        for( i = 0; i < 100; i++)
        {
                array[i] = (char*)malloc(sizeof(char) *100);
        }

	        int a = 0;
        while ((read = getline(&line, &len, stdin)) != -1)
        {
        	if(strlen(array[0]) >= 100){
        		 array[0] = (char*)malloc(sizeof(char) *len);
        	}

                if(strcmp(line, "exit\n") == 0)
                {
                        break;
                }
        
            printf("\nThe Printed line is: %s", line);

                //char *token;
                array[0] = strtok(line, " \n");

                while(a <= len)
                {

                //array[a] = token;
                printf("array[%d]: %s\n", a, array[a]);
                a++;
                array[a] = strtok(NULL, " \n");
                

                    if(array[a] == NULL)
                    {
                            break;
                    }
                }

            //printf("be4 FORK: %s\n", array[0]);
            sigusercounter = 0;
		    int PID = fork();
		    
		    if(PID == 0)
		    {

		    	execvp(*array, array);

		    	printf("Not a valid command %s\n\n", array[0]);

		    	exit(-1);

		    }

		    printf("The childs PID is:  %d\n", PID);

		    sigsetjmp(newbuffer, 1);
		    
		    if(PID < 0 ){
		    	printf("Forking...\n");
		    }

		    //int k = getrusage(RUSAGE_THREAD, &usuage);
		   	pid_t wpid = wait4(PID, &status, WUNTRACED, &usuage);
		    //pid_t wpid = wait(&status);

		    if (WIFEXITED(status))
		    {
            printf("Child %d terminated with exit status %d\n",
                   wpid, WEXITSTATUS(status));
        	}
	        else
	        {
	            printf("Child %d terminated abnormally\n", wpid);
	        }

        //rusuage.ru_nsignals
        	printf("Number of page faults experinced %ld", usuage.ru_majflt+ usuage.ru_minflt);
        	printf("\n\n# of signals %ld\n", usuage.ru_nsignals);
        	printf("Total time %ld\n", usuage.ru_utime);
		    a = 0;

        }

        free(line);
}