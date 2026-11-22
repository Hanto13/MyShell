#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../include/func.h"
#include "../include/builtin.h"

#define INIT_SIZE_ARRAY 10
#define INIT_NBR_OF_STR 5

// call the env var from my current OS
extern char **environ;

int main(int argc, char *argv[])
{

    // ------- START OF SETTINGS OF ARRAY TO TAKE USER INPUT --------

    /* init the size array and malloc the array (the arrays has a lenght of 10 by default > INIT_SIZE_ARRAY)
    size_cmd = the number of string in the cmd array */
    size_t size_cmd = INIT_SIZE_ARRAY;
    size_t *size_cmd_ptr = &size_cmd;

    // ------ END OF SETTINGS OF ARRAY TO TAKE USER INPUT -------

    // ------- START OF SETTINGS OF ARRAY TO PIPE AND REDIRECT --------

    // size_base_cmd = the number of string in the base_cmd array
    size_t size_base_cmd = INIT_SIZE_ARRAY;
    size_t *size_base_cmd_ptr = &size_base_cmd;

    // size_redirection_symbol = the number of string in the redirection_symbol array
    size_t size_redirection_symbol = INIT_SIZE_ARRAY;
    size_t *size_redirection_symbol_ptr = &size_redirection_symbol;

    // size_cmd_to_redirect = the number of command in the cmd_to_redirect array
    // size_number_of_str = the number of string in the command of cmd_to_redirect array
    size_t size_cmd_to_redirect = INIT_SIZE_ARRAY;
    size_t *size_cmd_to_redirect_ptr = &size_cmd_to_redirect;

    size_t size_number_of_str = INIT_NBR_OF_STR;
    size_t *size_number_of_str_ptr = &size_number_of_str;

    // ------ END OF SETTINGS OF ARRAY TO PIPE AND REDIRECT -------

    /* TO UPDATE (MAKE A FUNC TO DEALLOCATE ALL ARRAY IN ONE TIME)
    size_all_array_allocated = the number of array allocated into the programm
    size_t size_all_array_allocated = INIT_SIZE_ARRAY;
    size_t *size_all_array_allocated_ptr = &size_all_array_allocated; */

    // var to follow the getchar() with 'c' and follow the array growth with 'i'
    int c;
    size_t i = 0;

    while (1)
    {

        char **cmd = calloc(size_cmd, sizeof(char *)); // points into array of char * >> cmd[0] = 1 str, cmd[1] = 1 str...

        char **base_cmd = calloc(size_base_cmd, sizeof(char *));
        char **arr_redirection_symbol = calloc(size_redirection_symbol, sizeof(char *));
        char ***arr_cmd_to_redirect = calloc(size_cmd_to_redirect, sizeof(char **));

        for (size_t i = 0; i < size_cmd_to_redirect; i++)
        {
            arr_cmd_to_redirect[i] = calloc(size_number_of_str, sizeof(char *));
        }



        if (cmd == NULL)
        {
            printf("Error: the cmd array is NULL");
            free_array(cmd, size_cmd_ptr);
            free_array(base_cmd, size_base_cmd_ptr);
            free_array(arr_redirection_symbol, size_redirection_symbol_ptr);
            free_array_up(arr_cmd_to_redirect, size_cmd_to_redirect_ptr, size_number_of_str_ptr);
            return 1;
        }

         if (base_cmd == NULL)
        {
            printf("Error: the base cmd array is NULL");
            free_array(cmd, size_cmd_ptr);
            free_array(base_cmd, size_base_cmd_ptr);
            free_array(arr_redirection_symbol, size_redirection_symbol_ptr);
            free_array_up(arr_cmd_to_redirect, size_cmd_to_redirect_ptr, size_number_of_str_ptr);
            return 1;
        }

        if (arr_redirection_symbol == NULL)
        {
            printf("Error: the redirection symbol array is NULL");
            free_array(cmd, size_cmd_ptr);
            free_array(base_cmd, size_base_cmd_ptr);
            free_array(arr_redirection_symbol, size_redirection_symbol_ptr);
            free_array_up(arr_cmd_to_redirect, size_cmd_to_redirect_ptr, size_number_of_str_ptr);
            return 1;
        }

        if (arr_cmd_to_redirect == NULL)
        {
            printf("Error: the cmd_to_redirect array is NULL");
            free_array(cmd, size_cmd_ptr);
            free_array(base_cmd, size_base_cmd_ptr);
            free_array(arr_redirection_symbol, size_redirection_symbol_ptr);
            free_array_up(arr_cmd_to_redirect, size_cmd_to_redirect_ptr, size_number_of_str_ptr);
            return 1;
        }

        // construct the working directory and print him
        char *current_dir = pwd_builtin();
        printf("Current DIR - %s\n", current_dir);
        free(current_dir);

        // MYSHELL INTRO
        printf("MyShell > ");

        // call the func to get the user input and put into the cmd array
        cmd = take_user_input(cmd, size_cmd_ptr, c, i);

        /* TO DEBUG >> PRINT USER COMMAND

        for(size_t i = 0; i < *size_cmd_ptr; i++) {
            printf("USER CMD %zu: %s\n",i,cmd[i]);
        }
        */

             

        // check the builtin func
        if (strcmp(cmd[0], "exit") == 0)
        {

            free_array(cmd, size_cmd_ptr);
            free_array(base_cmd, size_base_cmd_ptr);
            free_array(arr_redirection_symbol, size_redirection_symbol_ptr);
            free_array_up(arr_cmd_to_redirect, size_cmd_to_redirect_ptr, size_number_of_str_ptr);
            exit_builtin();
        }
        if (strcmp(cmd[0], "cd") == 0)
        {
            cd_builtin(cmd);
        }
        else
        {

            /* ENTER INTO THE EXECVE PROCESS

            all the var init for the execve command:
            - pathname = path for the binary file to execute. (EX: /bin/ls to exe "ls")
            - cmd = include all the arg for the new child process, the arg[0] should include the command name(EX: {cmd[0] = 'ls', cmd[1] = '-l', ..} )
            - environ = import a PATH that already exists or create new env. (EX: {key=value, key=value,...}
            - cmd and environ ends always with a NULL pointer

            */

            

            // HANDLE PIPE AND REDIRECTION

            size_t count = pipe_and_redirection(cmd,base_cmd,size_base_cmd_ptr, arr_redirection_symbol,size_redirection_symbol_ptr, arr_cmd_to_redirect,size_cmd_to_redirect_ptr,size_number_of_str_ptr);

            // TO DEBUG >> the number of redirection, the base command, the redirection symbol and the content of cmd to redirect 

            /* 
            printf("the redirect count is %zu\n", count);

            for (size_t i = 0; base_cmd[i] != NULL; i++) {

                printf("the base cmd[%zu] is %s\n",i, base_cmd[i]);
            }

            for (size_t i = 0; arr_redirection_symbol[i] != NULL; i++) {

                printf("the redirect symbol[%zu] is %s\n",i, arr_redirection_symbol[i]);
            }

            for (size_t i = 0; arr_cmd_to_redirect[i] != NULL; i++) {

                printf("the cmd to redirect [%zu] is %p\n",i,arr_cmd_to_redirect[i]);
                for (size_t j = 0; j < size_number_of_str; j++)
                {
                    printf("the cmd to redirect string [%zu][%zu] is %s\n",i,j,arr_cmd_to_redirect[i][j]);
                }
                
            } 
            */

            

            // ALL THE PIPE AND REDIRECTIONS PROCESS


            int status;
            // count all of pipe needed in the command
            size_t number_of_pipe = 0;

            for (size_t i = 0; i < count; i++) {
                if (strcmp(arr_redirection_symbol[i], "|") == 0) {
                    number_of_pipe++;
                }
                
            }

            // alloc a array to contains the PID of all process created in function of number of commands
            size_t number_of_commands = number_of_pipe + 1;
            size_t number_of_child = 0;
            pid_t *all_process_created = calloc(number_of_commands,sizeof(pid_t));

            if (all_process_created == NULL) {
                perror("calloc array for process created has failed");
                exit(EXIT_FAILURE);
            }

            // this is var to handle which command and args to execve()
            size_t idx_current_redirection = 0;
            char **current_command = base_cmd;

            // this is var for piping (the previous read will register the write of the last pipe)
            int previous_read = -1;
            int fd[2];

            /*
            create the child with fork() the current process (parent)
            -if fork return is -1, the fork unsuccesful and the program exit
            -else fork return 0

            so the -1 or 0 is to handle the child process
            the 0 is print when interact with the current child process
            the other fork returned positive value is to handle the
            parent process.

            when each pid is print, the print is call for each count of
            run process

            */

            /*
            to piping, use the fd[2] var (the kernel handle a copy for each process)
            - fd[0] is for read the actual pipe
            - fd[1] is for write into the actual pipe

            EXEMPLE: command1 fd[1]==(pipe)==fd[0] command2 fd[1]==(pipe)==fd[0] command3

            ! don't forget to close the fd after the use or not use !
            the pipe send a EOF to know when the communication is over 
            
            */


        
            // AT LEAST 1 PIPE
            if (number_of_pipe > 0) {

                for (size_t i = 0; i < number_of_commands; i++) {

                    //check if it's not last command
                    if (i != number_of_commands - 1)
                    {
                        // create the pipe (in the parent process (global shell))
                        // the child will herits of this pipe when fork()
                        if (pipe(fd) < 0) {
                            perror("error during piping process");
                            exit(EXIT_FAILURE);
                        }

                    }

                    // and fork to create a new child with unique PID (the child herits of copy of parent buffer (stdin, stdout))
                    pid_t new_child = fork();
                    

                    // FORK ERROR 
                    if (new_child < 0) {

                        perror("error during fork child for piping process");
                        exit(EXIT_FAILURE);

                    }

                    // CHILD PROCESS 
                    // where handle all redirection before execve the command for the current child
                    else if (new_child == 0) {

                        // if a precendent pipe exists
                        // the current child will recieve by it's stdin the result of the previous read
                        if (previous_read != -1) {
                            dup2(previous_read,STDIN_FILENO);
                            close(previous_read);
                        }
                        // if a next pipe exists
                        // the current child will send by it's stdout the result of what it will write in his fd
                        if (i != number_of_commands - 1) {

                            if (dup2(fd[1], STDOUT_FILENO) == -1)
                            {
                                perror("dup2 stdout");
                                _exit(EXIT_FAILURE);
                            }

                            close(fd[0]);
                            close(fd[1]);
                        }

                        // WILL HANDLE ALL REDIRECTION BEFORE THE NEXT PIPE SYMBOL
                        while (idx_current_redirection < count &&
                            strcmp(arr_redirection_symbol[idx_current_redirection], "|") != 0) {

                            if (strcmp(arr_redirection_symbol[idx_current_redirection], ">") == 0)
                            {

                                // use open to get the file descriptor of this file, > = Create a file with perm(644) if dont exists, write only and truncate the content
                                int fd = open(arr_cmd_to_redirect[idx_current_redirection][0], O_CREAT | O_WRONLY | O_TRUNC, 00644);
                                if (fd < 0) {
                                    perror("error during redirection (open) for a command");
                                    _exit(EXIT_FAILURE);
                                }
                                // TO DEBUG printf("fd > %d\n",fd);
                                // replace original stdout (the terminal) by the fd, STDOUT_FILENO = 1
                                if (dup2(fd, STDOUT_FILENO) == -1)
                                {
                                    perror("error during dup2 redirection stdout");
                                    close(fd);
                                    _exit(EXIT_FAILURE);
                                }
                                close(fd);
                            }

                            else if (strcmp(arr_redirection_symbol[idx_current_redirection], ">>") == 0)
                            {

                                // use open to get the file descriptor of this file, >> = Create a file with perm(644) if dont exists, write only and append the content
                                int fd = open(arr_cmd_to_redirect[idx_current_redirection][0], O_CREAT | O_WRONLY | O_APPEND, 00644);
                                if (fd < 0) {
                                    perror("error during redirection (open) for a command");
                                    _exit(EXIT_FAILURE);
                                }
                                // printf("fd > %d\n",fd);
                                // replace original stdout (the terminal) by the fd, STDOUT_FILENO = 1
                                if (dup2(fd, STDOUT_FILENO) == -1)
                                {
                                    perror("error during dup2 redirection stdout");
                                    close(fd);
                                    _exit(EXIT_FAILURE);
                                }
                                close(fd);
                            }

                            else if (strcmp(arr_redirection_symbol[idx_current_redirection], "<") == 0)
                            {

                                // use open to get the file descriptor of this file, < = read only
                                int fd = open(arr_cmd_to_redirect[idx_current_redirection][0], O_RDONLY);
                                if (fd < 0) {
                                    perror("error during redirection (open) for a command");
                                    _exit(EXIT_FAILURE);
                                }
                                // printf("fd > %d\n",fd);
                                // replace original stdin (the keyboard) by the fd, STDOUT_FILENO = 0
                                if (dup2(fd, STDIN_FILENO) == -1)
                                {
                                    perror("error during dup2 redirection stdin");
                                    close(fd);
                                    _exit(EXIT_FAILURE);
                                }
                                close(fd);
                            }

                            idx_current_redirection++;
                        }

                        // finally after all redirections extract the path from the current command and execute with all pipe and redirections (using execve)
                        char *pathname = extract_path(current_command);
                        // DEBUG printf("detected path >> %s\n",pathname);
                        if (pathname == NULL)
                        {

                            perror("Enter a valid command");
                            _exit(EXIT_FAILURE);
                        }

                        
                        execve(pathname, current_command, environ);

                        perror("execve error");
                        free(pathname);
                        _exit(EXIT_FAILURE);

                    }
                    // PARENT PROCESS (SHELL)
                    //
                    else if (new_child > 0) {

                        // add the new child into all process created array
                        all_process_created[number_of_child] = new_child;
                        number_of_child++;

                        // close the pipe for the parent process
                        if (previous_read != -1)
                        {
                            close(previous_read);
                        }

                        if (i != number_of_commands - 1)
                        {
                            close(fd[1]);
                            previous_read = fd[0];
                        }

                        // advance to the next pipe in the parent process
                        while (idx_current_redirection < count &&
                               strcmp(arr_redirection_symbol[idx_current_redirection], "|") != 0)
                        {
                            idx_current_redirection++;
                        }

                        // get the next command
                        if (idx_current_redirection < count)
                        {
                            current_command = arr_cmd_to_redirect[idx_current_redirection];
                            idx_current_redirection++;
                        }
                    }
                }

                // wait the end of all child process (suspend the parent waiting the child state change) before return into parent process and capture status
                for (size_t i = 0; i < number_of_child; i++) {

                    pid_t wait_status = waitpid(all_process_created[i], &status, 0);
                    if (wait_status == -1)
                    {
                        perror("waitpid error");
                        free_array(cmd, size_cmd_ptr);
                        free_array(base_cmd, size_base_cmd_ptr);
                        free_array(arr_redirection_symbol, size_redirection_symbol_ptr);
                        free_array_up(arr_cmd_to_redirect, size_cmd_to_redirect_ptr, size_number_of_str_ptr);
                        exit(EXIT_FAILURE);
                    }
                }
                free(all_process_created);
            }
            // IF THERE ARE NOT PIPE
            else{

                pid_t new_child = fork();

                // FORK ERROR
                if (new_child < 0) {

                    perror("error during fork child");
                    exit(EXIT_FAILURE);
                }

                // CHILD PROCESS
                else if (new_child == 0) {

                    
                    while (idx_current_redirection < count) {

                        if (strcmp(arr_redirection_symbol[idx_current_redirection], ">") == 0) {

                            // use open to get the file descriptor of this file, > = Create a file with perm(644) if dont exists, write only and truncate the content
                            int fd = open(arr_cmd_to_redirect[idx_current_redirection][0], O_CREAT | O_WRONLY | O_TRUNC, 00644);
                            if (fd < 0)
                            {
                                perror("error during redirection (open) for a command");
                                _exit(EXIT_FAILURE);
                            }
                            // TO DEBUG printf("fd > %d\n",fd);
                            // replace original stdout (the terminal) by the fd, STDOUT_FILENO = 1
                            dup2(fd, STDOUT_FILENO);
                            close(fd);
                        }

                        else if (strcmp(arr_redirection_symbol[idx_current_redirection], ">>") == 0) {

                            // use open to get the file descriptor of this file, >> = Create a file with perm(644) if dont exists, write only and append the content
                            int fd = open(arr_cmd_to_redirect[idx_current_redirection][0], O_CREAT | O_WRONLY | O_APPEND, 00644);
                            if (fd < 0)
                            {
                                perror("error during redirection (open) for a command");
                                _exit(EXIT_FAILURE);
                            }
                            // printf("fd > %d\n",fd);
                            // replace original stdout (the terminal) by the fd, STDOUT_FILENO = 1
                            dup2(fd, STDOUT_FILENO);
                            close(fd);
                        }

                        else if (strcmp(arr_redirection_symbol[idx_current_redirection], "<") == 0) {

                            // use open to get the file descriptor of this file, < = read only
                            int fd = open(arr_cmd_to_redirect[idx_current_redirection][0], O_RDONLY);
                            if (fd < 0)
                            {
                                perror("error during redirection (open) for a command");
                                _exit(EXIT_FAILURE);
                            }
                            // printf("fd > %d\n",fd);
                            // replace original stdin (the keyboard) by the fd, STDOUT_FILENO = 0
                            dup2(fd, STDIN_FILENO);
                            close(fd);
                        }

                        idx_current_redirection++;
                    }

                    // finally after all redirections extract the path from the current command and execute with all pipe and redirections (using execve)
                    char *pathname = extract_path(current_command);
                    // DEBUG printf("detected path >> %s\n",pathname);
                    if (pathname == NULL)
                    {

                        perror("Enter a valid command");
                        _exit(EXIT_FAILURE);
                    }

                    execve(pathname, current_command, environ);

                    perror("execve error");
                    free(pathname);
                    _exit(EXIT_FAILURE);
                }

                // wait the end of child process (suspend the parent waiting the child state change) before return into parent process and capture status
                pid_t shell_pid = waitpid(new_child, &status, 0);
                if (shell_pid == -1) {

                    perror("waitpid error");
                    free_array(cmd, size_cmd_ptr);
                    free_array(base_cmd, size_base_cmd_ptr);
                    free_array(arr_redirection_symbol, size_redirection_symbol_ptr);
                    free_array_up(arr_cmd_to_redirect, size_cmd_to_redirect_ptr, size_number_of_str_ptr);
                    exit(EXIT_FAILURE);

                }
            }
            
            
               
        }

        // free all array
        free_array(cmd, size_cmd_ptr);
        free_array(base_cmd, size_base_cmd_ptr);
        free_array(arr_redirection_symbol, size_redirection_symbol_ptr);
        free_array_up(arr_cmd_to_redirect, size_cmd_to_redirect_ptr, size_number_of_str_ptr);
    }

    return 0;
}
