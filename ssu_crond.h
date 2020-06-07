#ifndef SSU_CROND_H
#define	SSU_CROND_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <dirent.h>
#include <sys/wait.h>
#include <pthread.h>

#define NAME_SIZE 100
#define BUFFER_SIZE 1024
#define PATHMAX 1024
#define FILE_NAME 1024
#define TIME_LEN 26
#define SECOND_TO_MICRO 1000000
#define TRUE 1
#define FALSE 0
#define null 0

void *ssu_thread(void *arg);

void command_separation(char *line, char (*token)[BUFFER_SIZE]);

void make_timetable(char (*token)[BUFFER_SIZE]);

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t);

#endif
