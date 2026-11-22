#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char **take_user_input(char **cmd, size_t *size_array_ptr, int c,size_t i);
void free_array(char **array, size_t *size_array);
void free_array_up(char ***array, size_t *size_array, size_t *number_of_str_in_arr);
char *extract_path(char **cmd);
size_t pipe_and_redirection(char **cmd,char **base_cmd,size_t *size_base_cmd,char **redirection_symbol,size_t *size_redirection_symbol_ptr, char ***cmd_to_redirect, size_t *size_cmd_to_redirect_ptr, size_t *size_number_of_str_ptr);

#endif