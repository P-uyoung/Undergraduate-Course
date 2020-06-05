#ifndef SSU_RSYNC_H
#define	SSU_RSYNC_H

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
#include <utime.h>

#define NAME_SIZE 100
#define BUFFER_SIZE 1024
#define PATHMAX 1024
#define FILE_NAME 1024
#define TIME_LEN 26
#define DIRECTORY_SIZE MAXNAMLEN
#define TRUE 1
#define FALSE 0
#define null 0


void command_separation(char *line, char (*argv)[BUFFER_SIZE]);

int check_argv(char (*argv)[BUFFER_SIZE]);

int rsync_file(char (*argv)[BUFFER_SIZE]);

int rsync_dir(char (*argv)[BUFFER_SIZE]);

void rsync(char (*argv)[BUFFER_SIZE]);

void ssu_signal_handler(int signo);

void remove_swp(void);

void write_log(char (*argv)[BUFFER_SIZE]);


#endif
