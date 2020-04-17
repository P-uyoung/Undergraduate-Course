#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "ssu_score.h"

#define SECOND_TO_MICRO 1000000

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t);

int main(int argc, char *argv[])
{
	struct timeval begin_t, end_t;		//런타임 시간을 위한 구조체

	gettimeofday(&begin_t, NULL);		//프로그램 시작시간을 얻는다

	ssu_score(argc, argv);

	gettimeofday(&end_t, NULL);		//프로그램 종료시간을 얻는다
	ssu_runtime(&begin_t, &end_t);		//프로그램 실행시간 출력
	
	exit(0);
}

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t)
{
	end_t->tv_sec -= begin_t->tv_sec;	//timeval 구조체의 필드 tv_sec, tv_usec 이용

	if(end_t->tv_usec < begin_t->tv_usec){
		end_t->tv_sec--;
		end_t->tv_usec += SECOND_TO_MICRO;
	}

	end_t->tv_usec -= begin_t->tv_usec;
	printf("Runtime: %ld:%06ld(sec:usec)\n", end_t->tv_sec, end_t->tv_usec);
}
