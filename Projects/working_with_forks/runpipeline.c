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

// Search TODO to find the functions you need to work on

#define MAX_NUM_PROGRAMS 10
#define FD_STDIN    0
#define FD_STDOUT   1
#define PIPEFD_READ     0
#define PIPEFD_WRITE    1

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
    int	     pid;	    // process ID of the program
    int      fd_in;     // FD for stdin
    int      fd_out;    // FD for stdout
} Program;

/* start a program. This function does not wait for the child process to 
 * finish.
 *
 * This function needs to fork, redirect stdin/stdout, and then upgrade
 * the child process into the program specified.
 *
 * If pipes are not created earlier, create pipes.
 *
 * Close file descriptors that are not needed, in both parent and child processes.
 *
 * If a child process is created successfully, save its pid in the
 * corresponding Program structure. 
 *
 * If any system call fails, call die() to exit.  
 * For example, die("fork() failed."); 
 * It will be messy if some programs are already started.
 *
 * Parameters:
 *      programs: an array of Program structures.
 *      num_programs: number of programs in the array.
 *      cur: the index of the program that needs to be started in this call.
 *
 * Return value: 
 *      This function does not return a value.  
 *
 * The child process never returns from this function.
 */
void start_program(Program *programs, int num_programs, int cur) 
{
    // TODO
    pid_t id1;
    if ((id1 = fork()) == -1){
    	die("fork() failed");
    }
  
    programs[cur].pid = id1;
    
    if (id1 == 0){
    	
    	// close all the unused pipes 
    	for (int i = 0 ; i < num_programs; i++){
    		//close pipe for first program
    		if (i == 0 && i != cur){
    			close(programs[i].fd_out);
    			continue;
    		}
    		// close pipes for last program
    		if (i == num_programs - 1 && i != cur){
    			close(programs[i].fd_in);
    			continue;
    		}
    		//if current dont close
    		if (i == cur){
    			continue;
    		}
    		// close rest of pipes
    		close(programs[i].fd_in);
    		close(programs[i].fd_out);
    	}
    	
    	//office hours
    	dup2(programs[cur].fd_in, 0); //
    	dup2(programs[cur].fd_out, 1);// youre redirecting the fd_out of program[curr] to the write end of the pipe 
    	
    	//close used pipes
    	if (cur != 0 && cur != num_programs - 1){
    		close(programs[cur].fd_in);
    		close(programs[cur].fd_out);
    		execvp(programs[cur].argv[0], programs[cur].argv);
    	}
    	else if(cur == 0){
    		close(programs[cur].fd_out);
    		execvp(programs[cur].argv[0], programs[cur].argv);
    	}
    	else if (cur == num_programs - 1){
    		close(programs[cur].fd_in);
    		execvp(programs[cur].argv[0], programs[cur].argv);
    	}
    	
    	
    	/* if the fd.in or std.out is > 0 itll be used for standard input
    	if ((programs[cur].fd_out > 0) || (programs[cur].fd_in > 0)){
    		redirect in childs new memory space
    		//redirect the output of current program (child process) to the output of the next program
    		//which is pointing to the write end of the next pipe 
    		dup2(programs[cur + 1].fd_out, programs[cur].fd_out); //output of second program reading
    		close(programs[cur].fd_out);
    		close(programs[cur + 1].fd_in); */
    		
    	die("L");
    }
    else{
    	//save its pid in the corresponding Program structure.
    	//close pipes that youre not using since child process is done for current process just close the in and out of the current process. 
    	/*if (cur == 0){
    		close(programs[cur].fd_out);
    	}
    	else if ( cur == num_programs - 1){
    		close(programs[cur].fd_out);
    	}
    	else{
	    	close(programs[cur].fd_out);
	    	close(programs[cur].fd_in);
    	}*/
    	close(programs[cur].fd_out);
	    close(programs[cur].fd_in);
    }
   
}

/* Wait on a program. 
 *
 * Parameter:
 *  prog:   A pointer to a Program, which has the pid of the program,
 *          which is set when the child process is created.
 *
 * Return value:
 *  -1:     prog->pid is less than 0, or waitpid() returns error.
 *  0-255:  exit value of the program (after WEXITSTATUS()).
 *
 * */
int wait_on_program(Program *prog)
{
    int exitStatus;

    if (prog->pid < 0)
        return -1;

    // TODO
    waitpid(prog->pid, &exitStatus, 0); // store exitstatus of program id into where exitStatus is defined in memory(where it lives &exitStatus)
    return WEXITSTATUS(exitStatus);
}

/* This function creates pipes to be used for connecting two pipeline stages.
 * You can create all pipes here,
 * or you can create pipes when they are needed in start_program().
 * If you decide to create pipes in start_program(),
 * leave this function empty.
 *
 * Paramters:
 *  programs: an array of Program structures.
 *  num_programs: number of programs in the array.
 *
 * Return value:
 *  This function does not return a value. If pipe() fails, 
 *  call the following function to exit from the program.
 *       die("pipe() failed.");
 */
void prepare_pipes(Program *programs, int num_programs)
{
    // TODO
    //create n-1 pipes
    int r;
    int fd[2]; // pipe with read end fd[0] znd write end fd[1]
    for (int i = 1; i < num_programs; i++){
    	r = pipe(fd); // create the pipe this populates fd with value
    	//check if pipe was created sucessfully 
    	if (r == -1){
    		die("pipe() failed");
    	}
    	
    	programs[i - 1].fd_out = fd[PIPEFD_WRITE]; //make the first programs standard output the write end of the pipe 
    	programs[i].fd_in = fd[PIPEFD_READ];  //make standard input of the second program the read end of the pipe 
    	/* why isnt it 
    	fd[PIPEFD_WRITE] = programs[i].fd_out 
    	fd[PIPEFD_READ] = programs[i + 1].fd_in
    	*/

    	//programs[i + 1].fd_in = fd[PIPEFD_READ];  //make standard input of the second program the read end of the pipe 
	
    	//programs[i].fd_out = fd[PIPEFD_WRITE]; //make the first programs standard output the write end of the pipe 
    }
    
    //asign these values for dup2
    // programs[0].fd_in = -1; 
    // programs[num_programs - 1].fd_out = -1;
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
    prog->pid = prog->fd_in = prog->fd_out = -1;
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

    // Prepare pipes
    prepare_pipes(programs, num_programs);

    // spawn children
    for (int i = 0; i < num_programs; i ++) {
        fprintf(stderr, "Starting program %d:%s\n", i, programs[i].argv[0]);
        start_program(programs, num_programs, i);
    }

    // wait for children
    for (int i = 0; i < num_programs; i ++) {
        fprintf(stderr, "Waiting for program %d:%s\n", i, programs[i].argv[0]);
        int status = wait_on_program(&programs[i]);
        fprintf(stderr, "Program %d:%s exited with %d\n", i, programs[i].argv[0], status);
    }

    // free memory
    free_programs(programs, num_programs);

    // You should see the message only once
    fprintf(stderr, "Parent: Everything is good.\n");
    return 0;
}
