#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

using namespace std;

void setup_for_rgen(){
    int pipe_myrgen[2];
    close(pipe_myrgen[0]);
    dup2(pipe_myrgen[1], STDOUT_FILENO);
    close(pipe_myrgen[1]);
}

void setup_for_a1(){
    int pipe_myrgen[2];
    int pipe_my_a1[2];

    // set the input of the rgen pipe
    dup2(pipe_myrgen[0], STDIN_FILENO);
    close(pipe_myrgen[0]);
    close(pipe_my_a1[0]);

    //set the output of the assignment 1 pipe
    dup2(pipe_my_a1[1], STDOUT_FILENO);
    close(pipe_my_a1[1]);
}


int main(int argc, char** argv) {
    pid_t child_myrgen;
    int pipe_myrgen[2];

    // this is the pipe intended to be used between rgen and assignment 1
    pipe(pipe_myrgen);

    // this is to create the child process for rgen
    child_myrgen = fork();

    // if this is the child process for myrgen
        if (child_myrgen == 0) {
            setup_for_rgen();

            // the rgen file may be in any location but the variables depend on stdin, thus argv
            execvp("./rgen", argv);

        } else if (child_myrgen == -1) {

            exit(1);

        }

    close(pipe_myrgen[1]);

 // the parent finishes the  random input generator fork

 // the parent starts the assignment 1 fork

    pid_t child_a1;
    int pipe_my_a1[2];

    pipe(pipe_my_a1);

    child_a1 = fork();

    if(child_a1 == 0) {
        setup_for_a1();

    // there are multiple variables in the python file so we use execl
        execl("/run/bin/sh", "sh", "-c", "python ./ece650-a1.py",(char *) NULL);

    } else if (child_a1 == -1){

        exit(1);

    }

    close(pipe_myrgen[0]);
    close(pipe_my_a1[1]); // Because no output should be displayed from assignment 1 

	// the parent finishes the assignment 1 fork

    FILE* writeTo_a2;
    writeTo_a2 = popen("./ece650-a2", "w");

    pid_t child_a2;

    child_a2 = fork();

    if (child_a2 == 0) {
        FILE* readFrom_a1 = fdopen(pipe_my_a1[0],"r");

        char* line = NULL;

        while (fgets(line, 500, readFrom_a1)){

            fputs(line, writeTo_a2);
            fflush(writeTo_a2);

        }

        fclose(readFrom_a1);

        fclose(writeTo_a2);

        exit(0);
    
    } else if (child_a2 == -1) {

        exit(1);
    
    }
    
    close(pipe_my_a1[0]); // stop reading assignment 1

    char* s_line = NULL;

    while(fgets(s_line, 500, stdin))
    {
        cout << s_line <<endl;
        fputs(s_line, writeTo_a2);
        fflush(writeTo_a2);
    }

    pclose(writeTo_a2); // close the Assignment 2

    kill(child_myrgen, SIGTERM);;

    return 0;
}
