#ifndef SSU_CROND_H
#define	SSU_CROND_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <dirent.h>
#include <sys/wait.h>

#define NAME_SIZE 100
#define BUFFER_SIZE 1024
#define PATHMAX 1024
#define FILE_NAME 1024
#define TIME_LEN 26
#define TRUE 1
#define FALSE 0
#define null 0

void command_separation(char *line, char (*token)[BUFFER_SIZE]);

#endif
