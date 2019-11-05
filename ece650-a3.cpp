//#include <iostream>
//#include <stdio.h>
//#include <unistd.h>
//#include <stdlib.h>
//#include <string.h>
//#include <signal.h>
//
//using namespace std;
//
//void setup_for_rgen(){
//    int pipe_myrgen[2];
//    close(pipe_myrgen[0]);
//    dup2(pipe_myrgen[1], STDOUT_FILENO);
//    close(pipe_myrgen[1]);
//}
//
//void setup_for_a1(){
//    int pipe_myrgen[2];
//    int pipe_my_a1[2];
//
//    // set the input of the rgen pipe
//    dup2(pipe_myrgen[0], STDIN_FILENO);
//    close(pipe_myrgen[0]);
//    close(pipe_my_a1[0]);
//
//    //set the output of the assignment 1 pipe
//    dup2(pipe_my_a1[1], STDOUT_FILENO);
//    close(pipe_my_a1[1]);
//}
//
//
//int main(int argc, char** argv) {
//    pid_t child_myrgen;
//    int pipe_myrgen[2];
//
//    // this is the pipe intended to be used between rgen and assignment 1
//    pipe(pipe_myrgen);
//
//    // this is to create the child process for rgen
//    child_myrgen = fork();
//
//    // if this is the child process for myrgen
//        if (child_myrgen == 0) {
//            setup_for_rgen();
//
//            // the rgen file may be in any location but the variables depend on stdin, thus argv
//            execvp("./rgen", argv);
//
//        } else if (child_myrgen == -1) {
//
//            exit(1);
//
//        }
//
//    close(pipe_myrgen[1]);
//
// // the parent finishes the  random input generator fork
//
// // the parent starts the assignment 1 fork
//
//    pid_t child_a1;
//    int pipe_my_a1[2];
//
//    pipe(pipe_my_a1);
//
//    child_a1 = fork();
//
//    if(child_a1 == 0) {
//        setup_for_a1();
//
//    // there are multiple variables in the python file so we use execl
//        execl("/run/bin/sh", "sh", "-c", "python ./ece650-a1.py",(char *) NULL);
//
//    } else if (child_a1 == -1){
//
//        exit(1);
//
//    }
//
//    close(pipe_myrgen[0]);
//    close(pipe_my_a1[1]); // Because no output should be displayed from assignment 1
//
//	// the parent finishes the assignment 1 fork
//
//    FILE* writeTo_a2;
//    writeTo_a2 = popen("./ece650-a2", "w");
//
//    pid_t child_a2;
//
//    child_a2 = fork();
//
//    if (child_a2 == 0) {
//        FILE* readFrom_a1 = fdopen(pipe_my_a1[0],"r");
//
//        char* line = NULL;
//
//        while (fgets(line, 500, readFrom_a1)){
//
//            fputs(line, writeTo_a2);
//            fflush(writeTo_a2);
//
//        }
//
//        fclose(readFrom_a1);
//
//        fclose(writeTo_a2);
//
//        exit(0);
//
//    } else if (child_a2 == -1) {
//
//        exit(1);
//
//    }
//
//    close(pipe_my_a1[0]); // stop reading assignment 1
//
//    char* s_line = NULL;
//
//    while(fgets(s_line, 500, stdin))
//    {
//        cout << s_line <<endl;
//        fputs(s_line, writeTo_a2);
//        fflush(writeTo_a2);
//    }
//
//    pclose(writeTo_a2); // close the Assignment 2
//
//    kill(child_myrgen, SIGTERM);;
//
//    return 0;
//}

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

using namespace std;

int main(int argc, char **argv)
{
    int pid_rgen, pipe_r_rgen[2];
    pipe(pipe_r_rgen);
    switch(pid_rgen = fork())
    {
        case -1:
            exit(EXIT_FAILURE);
        case 0://child process - rgen
            close(pipe_r_rgen[0]);
            dup2(pipe_r_rgen[1], STDOUT_FILENO);
            close(pipe_r_rgen[1]);
            execvp("./rgen", argv);
            //execl("/bin/sh", "sh", "-c", , NULL);
    }

    close(pipe_r_rgen[1]);
    /*parent finish rgen fork*/

    /*parent start fork a1*/
    int pid_a1, pipe_r_a1[2];
    pipe(pipe_r_a1);
    switch(pid_a1 = fork())
    {
        case -1:
            exit(EXIT_FAILURE);
        case 0: //pid_a1
            dup2(pipe_r_rgen[0], STDIN_FILENO);
            close(pipe_r_rgen[0]);
            close(pipe_r_a1[0]);
            dup2(pipe_r_a1[1], STDOUT_FILENO);
            close(pipe_r_a1[1]);
            execl("/bin/sh", "sh", "-c", "python ./ece650-a1.py",(char *) NULL);
    }
    close(pipe_r_rgen[0]);
    close(pipe_r_a1[1]);
    /*parent finish a1 fork*/

    /*parent fork a2 using popen*/
    FILE* write_to_a2 = popen("./ece650-a2", "w");//a2-process

    /*parent start fork a1 reader*/
    int pid_a1_reader;
    switch(pid_a1_reader = fork())
    {
        case -1:
            exit(EXIT_FAILURE);
        case 0://pid_a1_reader
        {
            FILE* read_from_a1 = fdopen(pipe_r_a1[0],"r");
            char* line = NULL;
            size_t bytes = 0;
            while(getline(&line, &bytes, read_from_a1) != -1)
            {
                cout<<line;
                fputs(line, write_to_a2);
                fflush(write_to_a2);
            }
            fclose(read_from_a1);
            fclose(write_to_a2);
            //kill(getppid(),SIGTERM); //commented
            exit(EXIT_SUCCESS);
        }
    }
    //parent, read from stdin, fputs to a2
    close(pipe_r_a1[0]);//does not read a1 at stdin reader
    char* line = NULL;
    size_t bytes = 0;
    while(getline(&line, &bytes, stdin) != -1)
    {
        fputs(line, write_to_a2);
        fflush(write_to_a2);
    }
    kill(pid_rgen, SIGTERM);
    pclose(write_to_a2); //send EOF to a2
    return EXIT_SUCCESS;
}


