/************************************************************
 *                                                          *
 *  CSCI 480            Assignment 1          Fall   2019   *
 *                                                          *
 *  Programmer:  Dominykas Karalius - Z1809478              *
 *                                                          *
 *  Date Due:    11:59 PM on Friday, 09/06/2019             *
 *                                                          *
 *  Assign1.cc                                              *
 *                                                          *
 *  This program utilizes the fork() function in order to   *
 *  create 3 processes. For each process, it prinds out     *
 *  the PID of itself and it's parent. The program          *
 *  then waits for the grandchild process to terminate,     *
 *  before printing out the PIDs again. The parent process  *
 *  calls the 'ps' system command to show current processes *
 *  running before terminating itself.                      *
 *                                                          *
 ***********************************************************/

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int main()
{
   cerr << "This is the original process and my PID is: " << getpid() << endl;
   cerr << "My parent's PID is: " << getppid() << endl << endl;
   cerr << "Calling first fork()..." << endl << endl;

   int child = fork();

   //If fork() fails, exit program.
   if(child < 0)
   {
      cerr << "The first fork failed!";
      return -1;
   }
   //If the fork() succeeds, print out the PID of child and PID of parent.
   //Then fork() again.
   if(child == 0)
   {
      cerr << "This is the first child and my PID is: " <<getpid() << endl;
      cerr << "My parent's PID is: " << getppid() << endl << endl;
      cerr << "Calling second fork()..." << endl << endl;

      int grandChild = fork();

      //If fork() fails, exit program.
      if(grandChild < 0)
      {
         cerr << "The second fork failed!";
         return -1;
      }
      //If the fork() succeeds, print out the PID of grandchild and PID of parent.
      //Then fork() again.
      if(grandChild == 0)
      {
         cerr << "This is the grandchild and my PID is: " <<getpid() << endl;
         cerr << "My parent's PID is: " << getppid() << endl <<endl;
         cerr << "The grandchild is about to exit..." << endl << endl;
         exit(0);
      }
      //Execute this block after 2nd fork().
      else
      {
         cerr << "This is the first child and my PID is: " <<getpid() << endl;
         cerr << "My parent's PID is: " << getppid() << endl << endl;
         wait(0);
         cerr << "The child is about to exit..." << endl << endl;
         exit(0);
      }
   }
   //Parent process executes this block after the 1st fork() call.
   else
   {
      cerr << "This is the parent process and my PID is: " <<getpid() << endl;
      cerr << "My parent's PID is: " << getppid() << endl << endl;
      sleep(2);
      cerr << "This is the parent process, I am about to call 'ps'." << endl <<endl;
      system("ps");
      cerr << endl;
      wait(0);
      cerr << "The parent is about to exit..." << endl << endl;
      exit(0);

   }
   return 0;
}
