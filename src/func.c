#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/func.h"


#define COEFF_ADD_SIZE 2 
#define INIT_SIZE_PATH_EXTRACTED 40
#define INIT_SIZE_STRING 20


/* ----- FUNCTIONS ----- */

// FUNCTION 'free_array'

/* free the memory pointed by 'char **array' in args, begin with loop to free all
char * include using the size of array and after finish free the global array

args >> array = the array pointer (char **) which include all the char * (strings)
size_array = the pointer to the var which contains the size of array (used by the for loop)

no return*/

void free_array(char **array, size_t *size_array) {
    for(size_t i = 0; i < *size_array; i++) {
        free(array[i]);
    }
    free(array);


}

// FUNCTION 'free_array_up'

/* free the memory pointed by 'char **array' in args, begin with loop to free all
char * include using the size of array and after finish free the global array

args >> array = the array pointer (char **) which include all the char * (strings)
size_array = the pointer to the var which contains the size of array (used by the for loop)

no return*/

void free_array_up(char ***array, size_t *size_array, size_t *number_of_str_in_arr) {
    for(size_t i = 0; i < *size_array; i++) {
        for (size_t j = 0; j < *number_of_str_in_arr; j++)
        {
            free(array[i][j]);
        }
        
        free(array[i]);
    }
    free(array);


}


// FUNCTION 'take_user_input'

/* take the string user input and parsed him into token

args >> size = the initial size of the string user input (default: 10)
cmd = the array contains the command user wants to execute in the shell. format >> arr of string
c = var will stock each char input, one by one. c is int because the func getchar() return one different int for each char.
i = var to follow the memory growth of the cmd array and realloc him if needed

return >> many command line parsed delimited by spaces to interact with the pathname*/

char **take_user_input(char **cmd, size_t *size_array_ptr, int c, size_t i) {

    //the size by default of each string
    size_t size_string = INIT_SIZE_STRING;

    // init the getchar() func wich read all the char of the stdin one by one
    c = getchar();
    // read all the user input char by char, until a line break (ENTER pressed) or a 'End of File' (no more char)
    while (c != '\n' && c != EOF) {


        //continue if a space is detected
        if (c == ' ') {
            c = getchar();
            continue;
        }

        // create a new_string var to construct the string char by char
        size_t j = 0;
        char *new_string = malloc(size_string * sizeof(char));


        if (new_string == NULL) {
            free_array(cmd,size_array_ptr);
            return NULL;
        }

        // check if 'c' detect a space (word delimiter)
        while(c != ' ' && c != '\n' && c != EOF) {

            /* if the size of a string override the decided size (DEFAULT = INIT_SIZE_STRING), 
            create a temp and realloc new size (DEFAULT = COEFF_ADD SIZE) into the new_string var */
            if (j + 1 >= size_string) {

                size_string *= COEFF_ADD_SIZE;

                char *tmp = realloc(new_string, size_string * sizeof(char));
                
                // if temp alloc is NULL
                if (tmp == NULL) {
                    perror("The temp reallocation to modify the string size has failed");
                    free(new_string);
                    free_array(cmd,size_array_ptr);
                    return NULL;
                }
                // else replace the pointer into cmd by the temp with the new size
                new_string = tmp;
            
            }


            // else add the 'c' char into the new_string arr and forward with getchar()
            new_string[j] = c;
            j++;
            c = getchar();


        }

        // add null terminator to finish completely the string
        new_string[j] = '\0';

        /* if the size of the array override the decided size (DEFAULT = INIT_SIZE_ARRAY), 
        create a temp and realloc new size (default: COEFF_ADD SIZE) into the cmd */
        if (i + 1 >= *size_array_ptr) {
            *size_array_ptr *= COEFF_ADD_SIZE;
            char **temp = realloc(cmd, *size_array_ptr * sizeof(char *));

            // if temp alloc is NULL
            if (temp == NULL) {
                perror("The temp reallocation to modify the string size has failed");
                free_array(cmd,size_array_ptr);
                return NULL;
            }

            // else replace the pointer into cmd by the temp with the new size
            cmd = temp;
        }
        

        // else add the string into the cmd[i]
        cmd[i] = new_string;
        i++;
        
    }

    // add a NULL at the end of cmd
    cmd[i] = NULL;

    return cmd;
        
}

// FUNCTION 'extract_path'

char *extract_path(char **cmd) {

    /* init all var and alloc char * to contain the 'good value' of the PATH 
    >> the const VAR_NAME is to indicate, we search the PATH of the system 
    >> path_total is the complete system PATH we get from getenv() (into one str)
    >> path_extracted is the allocated string we use to stock and test the different parts of the PATH and finally obtain the best part

    >> the twice var named 'idx_...' is to follow the iteration progress
    >> capacity_max_extracted is to follow when the path_extracted size will override and realloc her
    */
    const char *VAR_NAME = "PATH";
    char *path_total = getenv(VAR_NAME);
    char *path_extracted = calloc(INIT_SIZE_PATH_EXTRACTED, sizeof(char));

    size_t idx_total_path = 0;
    size_t idx_extract_path = 0;
    size_t capacity_max_extracted = INIT_SIZE_PATH_EXTRACTED;

    if(path_total == NULL) {

        perror("getenv PATH return nothing\n");
        return NULL;
    }

    if (path_extracted == NULL) {

        perror("bad calloc init for 'path_extracted'\n");
        return NULL;
    }

    
    if (cmd[0] == NULL)
    {
        perror("there are not command to extract");
        return NULL;

    }
    


    // while still character in path_total
    while(path_total[idx_total_path] != '\0') {

        // while the current character is not ':'
        while(path_total[idx_total_path] != ':' && path_total[idx_total_path] != '\0') {

            // transfer path_total char into path_extracted
            path_extracted[idx_extract_path] = path_total[idx_total_path];
            idx_total_path++;
            idx_extract_path++;

            // realloc path_extracted to double his size if needed
            if(idx_extract_path + 1 >= capacity_max_extracted) {

                capacity_max_extracted *= COEFF_ADD_SIZE;
                char *tmp = realloc(path_extracted, capacity_max_extracted);
                if(tmp == NULL) {

                    perror("The temp reallocation to modify the path extracted string size has failed");
                    free(path_extracted);
                    return NULL;
                }
                path_extracted = tmp;
            }
        }
        // once a ':' is detected
        if (path_total[idx_total_path] == ':')
        {
            idx_total_path++;
        }
        
        path_extracted[idx_extract_path] = '\0';
        // the required size is the actual length of 'path_extracted' + 1 (for '/') + length of command (cmd[0] + 1 (for '\0'))
        size_t required_size = strlen(path_extracted) + 1 + strlen(cmd[0]) + 1;
        if(required_size >= capacity_max_extracted) {

            while (capacity_max_extracted < required_size) {

                capacity_max_extracted *= COEFF_ADD_SIZE;

            }
            char *tmp = realloc(path_extracted, capacity_max_extracted);

            if(tmp == NULL) {

                perror("The temp reallocation to modify the path extracted string size has failed");
                free(path_extracted);
                return NULL;
            }

            path_extracted = tmp;

        }


        strcat(path_extracted, "/");
        strcat(path_extracted, cmd[0]);
        
        // check if a file is really a file and if he executable
        int file_value = access(path_extracted, F_OK | X_OK);

        if(file_value == -1 ) {

            //reset_string_arr(&path_extracted, capacity_max_extracted);
            memset(path_extracted,0,capacity_max_extracted);
            idx_extract_path = 0;
        }
        else {

            return path_extracted;

        }



    }


}

// FUNCTION 'pipe_and_redirection'

size_t pipe_and_redirection(char **cmd,char **base_cmd,size_t *size_base_cmd,char **redirection_symbol,size_t *size_redirection_symbol_ptr, char ***cmd_to_redirect, size_t *size_cmd_to_redirect_ptr, size_t *size_number_of_str_ptr) {

    size_t count = 0;
    size_t loop_ext = 0;
    size_t loop_int = 0;
    size_t idx_base_cmd = 0;
    size_t idx_redirection_symbol = 0;
    size_t idx_cmd_to_redirect_ext = 0;
    size_t idx_cmd_to_redirect_int = 0;

    
    

    while (cmd[loop_ext] != NULL) {


        if (strcmp(cmd[loop_ext], ">") != 0 && strcmp(cmd[loop_ext], ">>") != 0 && strcmp(cmd[loop_ext], "<") != 0 && strcmp(cmd[loop_ext], "|") != 0 && strcmp(cmd[loop_ext], "&") != 0 && strcmp(cmd[loop_ext], "&&") != 0 && strcmp(cmd[loop_ext], "||") != 0 && count == 0) {

            if (idx_base_cmd + 1 >= *size_base_cmd) {

                *size_base_cmd *= COEFF_ADD_SIZE;

                char **tmp = realloc(base_cmd, *size_base_cmd * sizeof(char *));
                
                // if temp alloc is NULL
                if (tmp == NULL) {
                    perror("The temp reallocation to modify the base cmd array has failed");
                    free_array(base_cmd, size_base_cmd);
                    return 0;
                }
                // else replace the pointer into base cmd by the temp with the new size
                base_cmd = tmp;
            }
            

            base_cmd[idx_base_cmd] = strdup(cmd[loop_ext]);
            idx_base_cmd++;
            
        }
        
        if (strcmp(cmd[loop_ext], ">") == 0 || strcmp(cmd[loop_ext], ">>") == 0 || strcmp(cmd[loop_ext], "<") == 0 || strcmp(cmd[loop_ext], "|") == 0 || strcmp(cmd[loop_ext], "&") == 0 || strcmp(cmd[loop_ext], "&&") == 0 || strcmp(cmd[loop_ext], "||") == 0) {

            if (idx_redirection_symbol+ 1 >= *size_redirection_symbol_ptr) {

                *size_redirection_symbol_ptr *= COEFF_ADD_SIZE;

                char **tmp = realloc(redirection_symbol, *size_redirection_symbol_ptr * sizeof(char *));
                
                // if temp alloc is NULL
                if (tmp == NULL) {
                    perror("The temp reallocation to modify the redirection symbol array has failed");
                    free_array(redirection_symbol, size_redirection_symbol_ptr);
                    return 0;
                }
                // else replace the pointer by the temp with the new size
                redirection_symbol = tmp;
            }
            
            redirection_symbol[idx_redirection_symbol] = strdup(cmd[loop_ext]);
            idx_redirection_symbol++;
            count++;
            loop_int = loop_ext + 1;
            if (idx_cmd_to_redirect_ext + 1 >= *size_cmd_to_redirect_ptr) {

                *size_cmd_to_redirect_ptr *= COEFF_ADD_SIZE;

                char ***tmp = realloc(cmd_to_redirect, *size_cmd_to_redirect_ptr * sizeof(char **));
                    
                // if temp alloc is NULL
                if (tmp == NULL) {
                    perror("The temp reallocation to modify the cmd to redirect array has failed");
                    free_array_up(cmd_to_redirect,size_cmd_to_redirect_ptr, size_number_of_str_ptr);
                    return 0;
                }
                // else replace the pointer by the temp with the new size
                cmd_to_redirect = tmp;
            }
            while (cmd[loop_int] != NULL && strcmp(cmd[loop_int], ">") != 0 && strcmp(cmd[loop_int], ">>") != 0 && strcmp(cmd[loop_int], "<") != 0 && strcmp(cmd[loop_int], "|") != 0 && strcmp(cmd[loop_int
            ], "&") != 0 && strcmp(cmd[loop_int], "&&") != 0 && strcmp(cmd[loop_int], "||") != 0)
            {
              
                if (idx_cmd_to_redirect_int + 1 >= *size_number_of_str_ptr) {

                    *size_number_of_str_ptr *= COEFF_ADD_SIZE;

                    char **tmp = realloc(cmd_to_redirect[idx_cmd_to_redirect_ext], *size_number_of_str_ptr * sizeof(char *));
                    
                    // if temp alloc is NULL
                    if (tmp == NULL) {
                        perror("The temp reallocation to modify the string size in a cmd to redirect has failed");
                        free_array(cmd_to_redirect[idx_cmd_to_redirect_ext], size_number_of_str_ptr);
                        return 0;
                    }
                    // else replace the pointer  by the temp with the new size
                    cmd_to_redirect[idx_cmd_to_redirect_ext] = tmp;
                }
                cmd_to_redirect[idx_cmd_to_redirect_ext][idx_cmd_to_redirect_int] = strdup(cmd[loop_int]);

                loop_int++;
                idx_cmd_to_redirect_int++;
            }
            idx_cmd_to_redirect_ext++;
            idx_cmd_to_redirect_int = 0;
            
        }

        loop_ext++;
    }

    return count;
    

}





