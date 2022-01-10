#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void ssu_charatatime(char *str);

int main(void)
{
	pid_t pid;

	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	else if (pid == 0)
		ssu_charatatime("output from child\n");

	else
		ssu_charatatime("output from parent\n");

	exit(0);
}

static void ssu_charatatime(char *str) {
	char *ptr;
	int print_char;

	setbuf(stdout, NULL);		//버퍼링 없어짐

	for (ptr = str; (print_char = *ptr++) != 0; ) {
		putc(print_char, stdout);	//바로 표준출력으로 출력
		usleep(10);
	}
}
