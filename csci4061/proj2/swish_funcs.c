#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "job_list.h"
#include "string_vector.h"
#include "swish_funcs.h"

#define MAX_ARGS 10

int tokenize(char *s, strvec_t *tokens) {
    if(s==NULL){
        perror("String not found");
        return -1;
    }
    char *delimeter = " ";
    char *token = strtok(s, delimeter);
    if(token == NULL){
        perror("String not found");
        return -1;
    }
    if(strvec_add(tokens, token)==-1){
        perror("Unable to add token");
        strvec_clear(tokens);
        return -1;
    }
    while((token = strtok(NULL, delimeter)) != NULL){
        if(strvec_add(tokens, token) == -1){
            perror("Unable to add token");
            strvec_clear(tokens);
            return -1;
        }
    }
    return 0;
}


int run_command(strvec_t *tokens) {
    // TODO Task 2: Execute the specified program (token 0) with the
    // specified command-line arguments
    // THIS FUNCTION SHOULD BE CALLED FROM A CHILD OF THE MAIN SHELL PROCESS
    // Hint: Build a string array from the 'tokens' vector and pass this into execvp()
    // Another Hint: You have a guarantee of the longest possible needed array, so you
    // won't have to use malloc.

    // TODO Task 3: Extend this function to perform output redirection before exec()'ing
    // Check for '<' (redirect input), '>' (redirect output), '>>' (redirect and append output)
    // entries inside of 'tokens' (the strvec_find() function will do this for you)
    // Open the necessary file for reading (<), writing (>), or appending (>>)
    // Use dup2() to redirect stdin (<), stdout (> or >>)
    // DO NOT pass redirection operators and file names to exec()'d program
    // E.g., "ls -l > out.txt" should be exec()'d with strings "ls", "-l", NULL
    
    //-------------TASK: 3---------------start
    int rdirInputIndex = strvec_find(tokens, "<");  //this should rdir stdin (-1 if not present, index if is)
    int rdirOutputIndex = strvec_find(tokens, ">"); //this should rdir stdout
    int appendOutputIndex = strvec_find(tokens, ">>");  //'' stdout and append

    //-------------TASK: 2---------------start
    char *args[MAX_ARGS];
    int directedIn = 0;
    int directedOut = 0;
    int appendOut = 0;
    char *inFile = NULL;
    char *outFile = NULL;
    int i = 0;
    while (i <= tokens->length) {
        if (i == rdirInputIndex || i == rdirOutputIndex || i == appendOutputIndex) {
            if (i == rdirInputIndex) {
                inFile = strvec_get(tokens, i + 1);
                directedIn = 1;
            }
            else if (i == rdirOutputIndex) {
                outFile = strvec_get(tokens, i + 1);
                directedOut = 1;
            }
            else if (i == appendOutputIndex) {
                outFile = strvec_get(tokens, i + 1);
                appendOut = 1;
            }
            args[i] = NULL;
            i++;
            break;
        }
        args[i] = strvec_get(tokens, i);
        i++;
    }
    while (i <= tokens->length && !directedOut && !appendOut) {
        if (i == rdirOutputIndex || i == appendOutputIndex) {
            outFile = strvec_get(tokens, i + 1);
            directedOut = 1;
            if (i == appendOutputIndex) {
                appendOut = 1;
            }
            i++;
            break;
        }
        i++;
    }
    //args[] should now only be the cmd and any parameters included
    
    int stdin_backup = dup(STDIN_FILENO);       //backup for IN/OUT
    int in_fd;                                  //global var for in/out_fileDescriptor
    int stdout_backup = dup(STDOUT_FILENO);
    int out_fd;
    if (directedIn) {
        in_fd = open(inFile, O_RDONLY);
        if (in_fd == -1) {
            perror("Failed to open input file");
            strvec_clear(tokens);
            return -1;
            
        }
        //change STDIN_FILENO to read from file
        if(dup2(in_fd, STDIN_FILENO)==-1){
            perror("Failed to get redirect input");
            strvec_clear(tokens);
            close(in_fd);
            return -1;
        }
    }
    if (directedOut) {
        if (appendOut) {
            //open file for appending
            out_fd = open(outFile, O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR );
            if (out_fd == -1) {
                out_fd = open(outFile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
                if(out_fd == -1) {
                    perror("Failed to open output file");
                    strvec_clear(tokens);
                    return -1;
                }
            }
            if (dup2(out_fd, STDOUT_FILENO) == -1) {
                perror("Failed to redirect output");
                strvec_clear(tokens);
                close(out_fd);
                return -1;
            }
        }
        else {
            //open file for writing. Truncate/create.
            out_fd = open(outFile, O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);  //file exists
            if (out_fd == -1) {
                out_fd = open(outFile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);           //file doesn't exist
                if(out_fd == -1) {
                    perror("Failed to open output file");
                    strvec_clear(tokens);
                    return -1;
                }
            }
            if (dup2(out_fd, STDOUT_FILENO) == -1) {
                perror("Failed to redirect output for appending");
                strvec_clear(tokens);
                close(out_fd);
                return -1;
            }
        }
    }
    
    // TODO Task 4: You need to do two items of setup before exec()'ing
    // 1. Restore the signal handlers for SIGTTOU and SIGTTIN to their defaults.
    // The code in main() within swish.c sets these handlers to the SIG_IGN value.
    // Adapt this code to use sigaction() to set the handlers to the SIG_DFL value.
    // 2. Change the process group of this process (a child of the main shell).
    // Call getpid() to get its process ID then call setpgid() and use this process
    // ID as the value for the new process group ID
    pid_t pid = getpid();
    if(setpgid(pid, pid)==-1){
        perror("setpgid");
        return -1;
    }

    struct sigaction sac;

    sac.sa_handler = SIG_DFL;
    if(sigfillset(&sac.sa_mask) == -1){
        perror("sigfillset");
        strvec_clear(tokens);
        return -1;
    }
    sac.sa_flags = 0;
    if(sigaction(SIGTTIN, &sac, NULL) == -1 || sigaction(SIGTTOU, &sac, NULL) == -1  ){
        perror("sigaction");
        strvec_clear(tokens);
        return -1;
    }
 
    int result = execvp(args[0], args);
    if (result == -1) {
        perror("exec");
        strvec_clear(tokens);
        return -1;
    }
    
    //clean up after
    if (directedIn) {
        if (dup2(stdin_backup, STDIN_FILENO) == -1) {
            perror("dup2 back to normal");
            return -1;
        }
        if (close(in_fd) == -1) {
            perror("close in");
            return -1;
        }
    }
    if (directedOut) {
        if (dup2(stdout_backup, STDOUT_FILENO) == -1) {
            perror("dup2");
            return -1;
        }
        if (close(out_fd) == -1) {
            perror("close out");
            return -1;
        }
    }


    // Not reachable after a successful exec(), but retain here to keep compiler happy
    return 0;
}

int resume_job(strvec_t *tokens, job_list_t *jobs, int is_foreground) {
    // TODO Task 5: Implement the ability to resume stopped jobs in the foreground
    // 1. Look up the relevant job information (in a job_t) from the jobs list
    //    using the index supplied by the user (in tokens index 1)
    //    Feel free to use sscanf() or atoi() to convert this string to an int
    // 2. Call tcsetpgrp(STDIN_FILENO, <job_pid>) where job_pid is the job's process ID
    // 3. Send the process the SIGCONT signal with the kill() system call
    // 4. Use the same waitpid() logic as in main -- dont' forget WUNTRACED
    // 5. If the job has terminated (not stopped), remove it from the 'jobs' list
    // 6. Call tcsetpgrp(STDIN_FILENO, <shell_pid>). shell_pid is the *current*
    //    process's pid, since we call this function from the main shell process

    // TODO Task 6: Implement the ability to resume stopped jobs in the background.
    // This really just means omitting some of the steps used to resume a job in the foreground:
    // 1. DO NOT call tcsetpgrp() to manipulate foreground/background terminal process group
    // 2. DO NOT call waitpid() to wait on the job
    // 3. Make sure to modify the 'status' field of the relevant job list entry to JOB_BACKGROUND
    //    (as it was JOB_STOPPED before this)

    int jobID = atoi(strvec_get(tokens, 1));
    job_t *job = job_list_get(jobs, jobID);
    if (job == NULL) {
        printf("Job index out of bounds\n");
        return -1;
    }

    if (is_foreground) {
        if(tcsetpgrp(STDIN_FILENO, job->pid)==-1){
            perror("tcsetpgrp: jobID");
            return -1;
        }
    }

    if (is_foreground == 0) {
        job->status = JOB_BACKGROUND;
    }
    
    if (kill(job->pid, SIGCONT) == -1) {    //send the sig to continue
        perror("kill failed");
        return -1;
    }

    if (is_foreground) {
        int status;
        pid_t waiting = waitpid(job->pid, &status, WUNTRACED);
        if(waiting < 0) {
            printf("call to \"waitpid()\" failed.\n");
        }
        if(WIFEXITED(status) || WIFSIGNALED(status)){
            if(job_list_remove(jobs, jobID) < 0){
                perror("Can not remove job");
                return -1;
            }
        }
    }

    if (is_foreground) {
        if(tcsetpgrp(STDIN_FILENO, getpid())==-1){
            perror("tcsetpgrp: getpid in resume job");
            return -1;
        }
    }

    return 0;
}

int await_background_job(strvec_t *tokens, job_list_t *jobs) {
    // TODO Task 6: Wait for a specific job to stop or terminate
    // 1. Look up the relevant job information (in a job_t) from the jobs list
    //    using the index supplied by the user (in tokens index 1)
    // 2. Make sure the job's status is JOB_BACKGROUND (no sense waiting for a stopped job)
    // 3. Use waitpid() to wait for the job to terminate, as you have in resume_job() and main().
    // 4. If the process terminates (is not stopped by a signal) remove it from the jobs list
   
    // 1, looking up relevant job information
    int jobID = atoi(strvec_get(tokens, 1));
    job_t *job = job_list_get(jobs, jobID);
    if (job == NULL) {
        printf("Job not found");
        return -1;
    }
    // 2, check if job is running in background
    if(job->status != JOB_BACKGROUND){
        fprintf(stderr, "Job index is for stopped process not background process\n");
        return -1;
    }
    // 3, wait for job to terminate
    int status;
    pid_t pid = waitpid(job -> pid, &status, WUNTRACED);
    if(pid == -1){
        perror("waitpid");
        return -1;
    }
    // 4, Update job status
    if(WIFEXITED(status) || WIFSIGNALED(status)){
        if(job_list_remove(jobs, jobID)==-1){ // by status ???
            perror("Can not remove job");
            return -1;
        }
    }
    return 0;
}


int await_all_background_jobs(job_list_t *jobs) {
    // TODO Task 6: Wait for all background jobs to stop or terminate
    // 1. Iterate through the jobs list, ignoring any stopped jobs
    // 2. For a background job, call waitpid() with WUNTRACED.
    // 3. If the job has stopped (check with WIFSTOPPED), change its
    //    status to JOB_STOPPED. If the job has terminated, do nothing until the
    //    next step (don't attempt to remove it while iterating through the list).
    // 4. Remove all background jobs (which have all just terminated) from jobs list.
    //    Use the job_list_remove_by_status() function.

    // first step
    int status;
    int i = 0;
    while(1){
        job_t *job = job_list_get(jobs, i);
        if(job==NULL){
            break;
        }  
        if(job->status == JOB_BACKGROUND){    // ignore stopped jobs
        // step 2
            pid_t pid = waitpid(job->pid, &status, WUNTRACED);
            if(pid == -1){
                perror("waitpid");
                return -1;
            }
            // step 3
            if(WIFSTOPPED(status)){
                job -> status = JOB_STOPPED;
            }
        }
    i++;
    }
    // step 4
    job_list_remove_by_status(jobs, JOB_BACKGROUND);
    return 0;
}
