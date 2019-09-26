/************************************************************
 *                                                          *
 *  CSCI 480            Assignment 3          Fall   2019   *
 *                                                          *
 *  Programmer:  Dominykas Karalius - Z1809478              *
 *                                                          *
 *  Date Due:    11:59 PM on Saturday, 09/28/2019           *
 *                                                          *
 *  Assign3.cc                                              *
 *                                                          *
 *  This program acts as a microshell on our linux machgines*
 *  The program starts out by printing a header and waits   *
 *  for the user to enter an input. The microshell then     *
 *  executes the command. The user also has the option to   *
 *  pipe their input using "||". We utiize a textfile named *
 *  ourfile for this program.                               *
 ***********************************************************/
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

int main(void)
{
	char buf[1024];
	int pid;
	int status;
	printf("480shell>");

	while (fgets(buf,1024,stdin) != NULL)
	{
		buf[strlen(buf) -1] = 0;  //remove the last character. Important!

		char* tokens[30];
		tokens[0] = strtok(buf, " ");

		for(int i = 1; i < 30; i ++)//Assuming will be 30 arguments, at max
		{
			tokens[i] = strtok(NULL, " ");
			if(tokens[i] == NULL)
			{
				tokens[i] = NULL; //The last element of the array should be NULL.
				break;
			}
		}
		if(strcmp(tokens[0], "q") == 0 || strcmp(tokens[0], "quit") == 0)//Check if input is "q" or "quit"
 		{
			exit(1);
		}

		//Check if user wishes to pipe their input
		int index = -1;
		for(int i = 1; i < 30; i ++)
		{
			if(tokens[i] != NULL)
			{
				if(strcmp(tokens[i], "||") == 0)
				{
					index = i;
					tokens[i] = NULL;
				}
			}
			else
			{
				break;
			}
		}

		// Pipes to be piped
		int pipeA[2];
		int pipeB[2];

		if(index > 0) //If user wants to execute a pipe(input included "||"
		{
			pipe(pipeA);
			pipe(pipeB);
			if ((pid = fork()) < 0)
			{
				printf("fork error");
			}
			else if (pid == 0)
			{
				/*Child*/
				close(1);
				close(pipeB[0]);
				close(pipeA[0]);
				close(pipeA[1]);
				//Closed unused pipes

				dup2(pipeB[1],1);//Duplicate write pipe FD
				execvp(tokens[0],tokens);

				printf("couldn't execute: %s", tokens[0]);
				exit(127);
			}
			if ((pid = fork()) < 0)
                                printf("fork error");
                        else if (pid == 0)
			{
				close(1);
				close(0);
                                close(pipeA[0]);
                                close(pipeB[1]);
				//Closed unused pipes

				dup2(pipeB[0],0);//Duplicate read pipe FD
				dup2(pipeA[1],1);//Duplicate write pipe FD

				execvp(tokens[0],&tokens[index+1]);

				printf("couldn't execute: %s", tokens[0]);
				exit(127);
			}
			close(pipeB[0]);
			close(pipeB[1]);
			close(pipeA[1]);
			//Close pipes that are no longer needed
		}
		else //Else, user did not enter "||"
		{
			pipe(pipeA);

			if ((pid = fork()) < 0)
			{
				printf("fork error");
			}
			else if (pid == 0)
			{
				/*Child*/
				close(1);
				close(pipeA[0]);
				//Close unused pipes

				dup2(pipeA[1],1);//Duplicate write pipe FD

				execvp(tokens[0],tokens);
                                printf("couldn't execute: %s", tokens[0]);
                                exit(127);

			}
			close(pipeA[1]);
			//Close pipes that are no longer needed
		}
		/*Parent*/
		if ( (pid = waitpid(pid, &status, 0)) < 0)
		{
			printf("waitpid error");
		}
		char buff[2];
		buff[1] = '\0';
		while(read(pipeA[0], buff, 1) != 0)
		{
			cerr << buff;
		}
		printf("\n480shell>");
	}
	exit(0);
}
