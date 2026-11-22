#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "../include/func.h"
#include "../include/builtin.h"

#define INIT_SIZE_CURRENTDIR 50
#define COEFF_ADD_SIZE 2 


/* ----- FUNCTIONS BUILT-IN ----- */

// FUNCTION 'cd_builtin'

/* implement the cd builtin using chdir() function

verify if args is given at the command :
- 0 command >> same effect than cd ~
- 1 command >> redirect into the correspondant dir
- if too many args so raises an error

take the char **cmd in arg to control the user input

no return
*/

void cd_builtin(char **cmd) {

    if (cmd[2] != NULL)
    {
        perror("cd: too many arguments");
        exit(EXIT_FAILURE);
    }
    if (cmd[1] == NULL || strcmp(cmd[1],"~") == 0) {
        char *path_total = getenv("HOME");
        chdir(path_total);
    }
    else {
        
        chdir(cmd[1]);

    }
    

}

// FUNCTION 'pwd_builtin'

/* 
implement the pwd builtin using getcwd() function and a buffer (current_dir) to contain the str

will verify if the buffer is to small to get the current dir, realloc if needed

return the str contains the current dir
*/

char *pwd_builtin() {

    size_t size_currentdir = INIT_SIZE_CURRENTDIR; 
    char *current_dir = malloc(sizeof(char) * size_currentdir);

    if (current_dir == NULL)
    {
        free(current_dir);
        return NULL;
    }
    

    

    // check while the current_dir is to small to get the getcwd() result
    while ( getcwd(current_dir, size_currentdir ) == NULL ) {


        //check if really a problem of size, else free
        if (errno != ERANGE)
        {
            free(current_dir);
            return NULL;
            
        }
        
        size_currentdir *= COEFF_ADD_SIZE;
        char *tmp = realloc(current_dir, size_currentdir * sizeof(char));

        if (tmp == NULL)
        {
            perror("The tmp reallocation to modify the current dir size has failed");
            free(current_dir);
            return NULL;
        }

        current_dir = tmp;
        


    }

    
        
    return current_dir;


}

// FUNCTION 'exit_builtin'

/* 
implement the exit builtin 

no return
*/


void exit_builtin() {

    exit(EXIT_SUCCESS);
    

}

