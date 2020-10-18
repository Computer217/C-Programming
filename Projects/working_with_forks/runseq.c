#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_NUM_PROGRAMS 10
#define OPEN_MODE   0600
#define FD_STDIN    0 
#define FD_STDOUT   1
#define OUTPUT_FN   "seqOutput"

void die(char *s)
{
    fprintf(stderr, "Error: %s\n", s);
    if (errno)
        perror("errno");
    exit(EXIT_FAILURE);
} 

void check_pointer(void *p)
{
    if (p == NULL)
       die("Pointer is NULL.");
}

// structure for storing program arguments
typedef struct program_tag {
    char**   argv;      // array of pointers to arguments
    int      argc;      // number of arguments
} Program;


/* This function runs a program, waits for it to complete.
 *
 * stdout of the program is redicted into the file specified by outputfn. If
 * the outputfn exists, it is truncated (to length 0). If it does not exist, 
 * the file is created. 
 *
 * After fork(), the parent waits for child process to exit.
 *
 * The child process needs to do the following:
 *      open the file
 *      set up stdout for the program
 *      exec
 *
 *  Close unused FDs in both processes. 
 *  Properly handle errors. For example, what if exec fails.
 *
 * Parameters:
 *
 *      prog:   points to the Program structure that has all the information 
 *              needed to run the program.
 *
 *      outputfn:  specifies the output file where stdout is redirected to.
 *
 * The process is 
 *
 * Return value:
 *
 *  0-255: the exit value of the child process (after being processed by WEXITSTATUS)
 *
 *  Child process should not return from this function.
 */
int run_program(Program *prog, char * outputfn) 
{
    pid_t cpid;      // child process ID
    int exitStatus;  // exitStatus of the child process
    int fd; 	// initialize variable that saves file descriptor
    
    
    cpid = fork(); // create another process
    
    //run the executable and pass the amount of arguments into the file being ran
    
    /*check if the creation was succesful or not */
    if (cpid < 0){
    	die("fork failed");
    }
    
    /*the work you want the child to do*/
    if (cpid == 0){
    	fd = open(outputfn, O_WRONLY | O_CREAT | O_APPEND, OPEN_MODE); //returns a file descriptor if succesful else it returns -1
    	/*check if there is an error opening the file*/
    	if (fd<0){
    		die("fork failed");
    	}
    	/*redirect by reading from standard output and writing into outputfn*/
    	if (dup2(fd,1)<0){
    		die("dup2 failed");
    	}
    	/*close the file not being used after redirecting the output*/
    	if (close(fd)){
    		die("close failed");
    	}
    
    execvp(prog->argv[0],prog->argv); // need to create the pipe first then run the program so it runs through the pipe 
    // execvp replaces the program so for example instead of being runseq it becomes echo 
	}
	
	/*wait for child process to complete and return*/
	waitpid(cpid, &exitStatus,0);
    // TODO
    return WEXITSTATUS(exitStatus);
}

/*********************************************************/
/* Do not change code below                              */
/*********************************************************/
/* initialize a Program structure that allows argc arguments */
void init_program(Program *prog, int argc)
{
    // allocate memory for array of arguments
    prog->argv = malloc((argc + 1) * sizeof(char *));
    check_pointer(prog->argv);
    prog->argc= 0;
    prog->argv[0] = NULL;
}

/* free the memory used by prog */
void free_programs(Program *programs, int num_programs)
{
    for (int i = 0; i <  num_programs; i ++) {
        free(programs[i].argv);
        programs[i].argv = NULL;
    }
}

// parse the command line arguments
// split them into programs
// return the number of programs found
int parse_command_line(Program *progs, int max_num_progs, int argc, char **argv)
{
    int     cur_arg = 1;
    int     prog_count = 0;

    while (cur_arg < argc) {
        // get arguments for a new program

        if (prog_count == max_num_progs) 
            die("Too many programs.");

        // allocate space for argv for progs[prog_count]
        // Could have scanned the arguments and find out how many arguments
        // the current program has. 
        init_program(&progs[prog_count], argc);

        int     ia = 0;  // index of arguments for progs[prog_count]
        while (cur_arg < argc && strcmp(argv[cur_arg], "--")) {
            progs[prog_count].argv[ia++] = argv[cur_arg++];
        }
        progs[prog_count].argv[ia] = NULL;
        progs[prog_count].argc = ia;
        prog_count ++;
        if (ia == 0) 
            die("Empty program.");

        cur_arg ++; // skip "--"
        if (cur_arg == argc) 
            die("Last program is empty."); 
    }

    return prog_count;
}

int main(int argc, char **argv)
{
    Program programs[MAX_NUM_PROGRAMS];
    int     num_programs;

    if (argc <= 1) 
        die ("Specify at least one program to run. Multiple programs are separated by --");

    // Prepare programs and their arguments
    num_programs = parse_command_line(programs, MAX_NUM_PROGRAMS, argc, argv); 

    // remove the output file 
    // not checking the return value because the file may not exist
    unlink(OUTPUT_FN);

    // run each program sequentially
    for (int i = 0; i < num_programs; i ++) {
        fprintf(stderr, "Parent: to run program %d: %s\n", i, programs[i].argv[0]);
        // output filename only appears here
        int rv = run_program(&programs[i], OUTPUT_FN);
        fprintf(stderr, "Parent: run_program() returned %d.\n", rv);
    }

    // free memory
    free_programs(programs, num_programs);

    // You should see the message only once
    fprintf(stderr, "Parent: Everything is good.\n");
    return 0;
}
