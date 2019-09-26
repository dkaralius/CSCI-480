/************************************************************
 *                                                          *
 *  CSCI 480            Assignment 2          Fall   2019   *
 *                                                          *
 *  Programmer:  Dominykas Karalius - Z1809478              *
 *                                                          *
 *  Date Due:    11:59 PM on Tueday, 09/17/2019             *
 *                                                          *
 *  Assign2.cc                                              *
 *                                                          *
 *  This program utilizes the fork() function in order to   *
 *  create 3 processes. It then starts off with a default   *
 *  value of 1, which is then passed along to the child     *
 *  process, which then the work() function is called,      *
 *  where a calculation is made. This number is then passed *
 *  along to the grandchild via pipe, which then another    *
 *  calculation is made to alter the number further. This   *
 *  number is again passed back into the parent via pipe    *
 *  where another calculation is made and repeats the cycle.*
 *  This is done until the value reaches a 99999999999.     *
 *  Once this is reached, the processes terminate and the   *
 *  pipes are closed.                                       *
 *                                                          *
 ***********************************************************/
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

//Pipe declaractions
int pipeA[2];
int pipeB[2];
int pipeC[2];

//Used to distinguish between parent,child and grandchild
char process;

//Prototypes of functions
void PWork();
void CWork();
void GWork();
void work(int rID, int wID, string& Buffer, string& Value, long int& M, char& process);


/*
The PWork() function returns nothing and takes nothing as an argument.

The purpose of this function is to let the user know the parent process
is ready to process a command, then assigning the read and write
file descriptors to the respective pipes. It starts by assigning the
default value to 1 and sends it to the child via pipeA and also assings
the process value to 'p', to be used in the work() function. It calls the
work() function when the parent process gets a value from the
grandchild process from pipeC. The parent will wait() until the child
dies, before exitting.
*/
void PWork()
{
	cerr << "The parent process is ready to proceed." << endl;

	string Buffer;
	string Value;
	long int M = 1;

	process = 'p';
	int readID = pipeC[0];
	int writeID = pipeA[1];

	Buffer = to_string(M);
	Buffer += '@';

	write(writeID, Buffer.c_str(), Buffer.length());
	cerr <<"Parent:       Value = " << M << endl;

	work(readID, writeID, Buffer, Value, M, process);

	wait(NULL);
	exit(0);
}
/*
The CWork() function returns nothing and takes nothing as an argument.

The purpose of this function is to let the user know the child process
is ready to process a command, then assigning the read and write
file descriptors to the respective pipes. It then assigns the process
value to 'c' to be used in the work() function. It reads the value from
pipeA and calls the work() function, before writing it into pipeB which
will be used in the grandchild process. The child will wait() until
the granchild dies, before exitting.
*/
void CWork()
{
	cerr << "The child process is ready to proceed." << endl;

	string Buffer;
	string Value;
	long int M = 1;

	process = 'c';
	int readID = pipeA[0];
	int writeID = pipeB[1];

	work(readID, writeID, Buffer, Value, M, process);

	wait(NULL);
	exit(0);
}
/*
The GWork() function returns nothing and takes nothing as an argument.

The purpose of this function is to let the user know the grandchild
process is ready to process a command, then assigning the read and write
file descriptors to the respective pipes. It then assigns the process
value to 'g' to be used in the work() function. It reads the value from
pipeB and calls the work() function, before writing it into pipeC which
will be used in the parent process.
*/
void GWork()
{
	sleep(1);
	cerr << "The grandchild process is ready to proceed." << endl;

	string Buffer;
	string Value;
	long int M = 1;

	process = 'g';
	int readID = pipeB[0];
	int writeID = pipeC[1];

	work(readID, writeID, Buffer, Value, M, process);

	exit(0);
}
/*
The work() function returns nothing and takes 6 arguments.

int rID - integer to represent the pipe that it will read from
int wID - integer to represent the pipe that it will write to
string& Buffer - reference to a string that is used to hold M after
                 calculations
string& Value  - reference to a string that reads the value provided
		 from the child/grandchild processes
long int& M    - reference to a long integer that is used in the math
	         calculations
char& process  - reference to a character that is used to distinguish
                 which process called the work() function
		 (parent/child/grandchild)

The purpose of this function is to do the heavy lifting of the program.
This function reads the pipe for the given process that called it and
reads a value until it reaches a delimiter of "@". It stores this value
in... value... and then looks to see what process called it. There is then
a switch case that has different math calculations and print messages
depending on which process called it. It then writes the new value into
the pipe of the process that called it. This continues along the 3
processes, until the M value is greater than 99999999999. After which,
it closes the read and write pipes.
*/
void work(int rID, int wID, string& Buffer, string& Value, long int& M, char& process)
{
	char temp = 'A';
	do
	{
		Value = "";
		Buffer = "";
		temp = 'A';
		while(temp != '@')
		{
			read(rID, &temp, 1);
			if(temp != '@')
			{
				Value += temp;
			}
		}

		switch(process)
		{
			case 'p':
				M = stol(Value);
				M = 3 * M + 7;
				Buffer = to_string(M);
				cerr <<"Parent:       Value = " << Buffer << endl;
				break;
			case 'c':
				M = stol(Value);
                                M = 2 * M + 5;
                                Buffer = to_string(M);
                                cerr <<"Child:        Value = " << Buffer << endl;
				break;
			case 'g':
				M = stol(Value);
                                M = 5 * M + 1;
                                Buffer = to_string(M);
                                cerr <<"Grandchild:   Value = " << Buffer << endl;
				break;
			default:
				break;
		}
		Buffer += "@";

		write(wID, Buffer.c_str(), Buffer.length());

	}while(M < 99999999999);

	close(rID);
	close(wID);
}

int main()
{
	//Check to see if all pipes are working
	if(pipe(pipeA) == -1)
	{
		cerr << "Pipe #1 error!";
		exit(-5);
	}
	if(pipe(pipeB) == -1)
	{
		cerr << "Pipe #2 error!";
		exit(-5);
	}
	if(pipe(pipeC) == -1)
	{
		cerr << "Pipe #3 error!";
		exit(-5);
	}

	//If they are, fork to create a child
	pid_t pid = fork();

	//Check to see if fork() was successful
	if(pid == -1)
	{
		cerr << "Fork #1 error!";
		exit(-5);
	}
	//If it was, fork again, to create grandchild
	else if(pid == 0)
	{
		pid = fork();

		//Check to see if 2nd fork() was successful
		if(pid == -1)
		{
			cerr << "Fork #2 error!";
			exit(-5);
		}
		//If it was, continue with program
		else if(pid == 0)
		{
			//We close all pipes we won't be using in GWork();
			close(pipeA[0]);
			close(pipeA[1]);
			close(pipeB[1]);
			close(pipeC[0]);

			GWork();
		}
		else
		{
			//We close all pipes we won't be using in CWork();
			close(pipeA[1]);
			close(pipeB[0]);
			close(pipeC[0]);
			close(pipeC[1]);

			CWork();
		}
	}
	else
	{
		//We close all pipes we won't be using in PWork();
		close(pipeA[0]);
		close(pipeB[0]);
		close(pipeB[1]);
		close(pipeC[1]);

		PWork();
	}
	return 0;
}
