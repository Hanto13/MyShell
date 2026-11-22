#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void cd_builtin(char **cmd);
char *pwd_builtin();
void exit_builtin();
#endif