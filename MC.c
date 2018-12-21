		#define _GNU_SOURCE
		#include <stdio.h>
		#include <stdlib.h>
		#include <string.h>
		#include <unistd.h>
		#include <sys/types.h>
		#include <sys/wait.h>
		#include <stdarg.h>
		#include <sys/time.h>
		#include <setjmp.h>
		#include <sys/resource.h>
		#include <limits.h>

	//
		 unsigned long int EvalAttmpt;
		 unsigned long int SegFaultGen;
		static sigjmp_buf newbuffer;

		 int print = 0;
		 int m_sig;
		 int finish = 0;
		 int sigstpfault = 0;
		 int pwroften = 10;
		 int alarmflag = 0;
		 int siguser2flag = 0;

		void handlerSIGSEGV(int signal)
		{
			m_sig = signal;
 
			SegFaultGen++;
			if(EvalAttmpt == ULONG_MAX)
			{
				finish = 1;
			}
			else{
				siglongjmp(newbuffer, 1);
			}
	}

		void handlerSIGINT(int signal)
		{
			if(signal == SIGINT){
				finish = 1;
			}
			else{
			 siglongjmp(newbuffer, 1);
			}

		}

		void handlerSIGTSTP(int signal)
		{
			//m_sig = signal;
			sigstpfault = 1;
			siglongjmp(newbuffer, 1);

		}

		void handlerALARM(int signal)
		{
			//m_sig = signal;
			alarmflag = 1;
			siglongjmp(newbuffer, 1);

		}

			void handlerSIGUSR2(int signal)
		{
			//m_sig = signal;
			//alarmflag = 1;
			if(signal == SIGUSR2){
				//SET FLAG
				siguser2flag = 1;
			}
			else{
				siglongjmp(newbuffer, 1);	
			}

		}

		

		int main(int argc, char **argv)
		{
			printf(" Jakub \n\n");
		 float var = 0.0;
		 //int Random = 
		 srand( time( NULL ));
		 int variable = 0;
		 int *randomAddress;
		 // = (int*)malloc(sizeof(int));

		 signal(SIGSEGV, handlerSIGSEGV);
		 signal(SIGINT, handlerSIGINT);
		 signal(SIGTSTP, handlerSIGTSTP);
		 signal(SIGALRM, handlerALARM);
		 signal(SIGUSR2, handlerSIGUSR2);
		// signal(SIGUSR2, handlerSIGUSR2);


		 

		if(argc > 1){
			var = atof(argv[1]);
			if(var >= 0){
		 	 alarm(var);
			}
		}

		 int i;
		 for(i = 0; i < ULONG_MAX; i++){
			sigsetjmp(newbuffer, 1);

			EvalAttmpt++;

		 if(EvalAttmpt == pwroften){
		 	kill(getppid(), SIGUSR1);
		 	pwroften = pwroften* 10;

		 }

			if(finish == 1){
				printf("%d\n", m_sig);
				printf("\nNumber of SegFaults caught %lu\n", SegFaultGen);
		 	 	printf("Number of Attempts is  %lu\n", EvalAttmpt);
		 	 	printf("Percentage of SegFaults  %lf\n", (float)SegFaultGen/(float)EvalAttmpt);
				print = 0;
				exit(0);
			}

			if(sigstpfault == 1){
				printf("\nNumber of SegFaults caught %lu\n", SegFaultGen);
		 	 	printf("Number of Attempts is  %lu\n", EvalAttmpt);
		 	 	printf("Percentage of SegFaults  %lf\n", (float)SegFaultGen/(float)EvalAttmpt);
		 	 	sigstpfault = 0;
			}

			if(alarmflag == 1){
				printf("\nNumber of SegFaults caught %lu\n", SegFaultGen);
		 	 	printf("Number of Attempts is  %lu\n", EvalAttmpt);
		 	 	printf("Percentage of SegFaults  %lf\n", (float)SegFaultGen/(float)EvalAttmpt);
		 	 	exit(0);
			}
			if(siguser2flag == 1){

				printf("\nNumber of SegFaults caught %lu\n", SegFaultGen);
		 	 	printf("Number of Attempts is  %lu\n", EvalAttmpt);
		 	 	printf("Percentage of SegFaults  %lf\n", (float)SegFaultGen/(float)EvalAttmpt);
		 	 	exit(0);
			}
			
			 
			randomAddress = rand();
			variable = *randomAddress;
		 }
		}