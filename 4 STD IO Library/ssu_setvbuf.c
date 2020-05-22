#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void ssu_setbuf(FILE *fp, char *buf);		//파일에 따라 다른 mode로 setvbuf() 

int main(void)
{
	char buf[BUFFER_SIZE];
	char *fname = "/dev/pts/0";		//현재 사용중인 터미널의 파일명
	FILE *fp;

	if ((fp = fopen(fname, "w")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	ssu_setbuf(fp, buf);
	fprintf(fp, "Hello, ");
	sleep(1);
	fprintf(fp, "UNIX!!");
	sleep(1);
	fprintf(fp, "\n");
	sleep(1);
	
	ssu_setbuf(fp, NULL);
	fprintf(fp, "HOW");
	sleep(1);
	fprintf(fp, " ARE");
	sleep(1);
	fprintf(fp, " YOU?");
	sleep(1);
	fprintf(fp, "\n");
	sleep(1);
	exit(0);

} 

void ssu_setbuf(FILE *fp, char *buf) {
	size_t size;
	int fd;
	int mode;

	fd = fileno(fp);		/* stream 을 검사해 연관된 파일핸들 리턴한다
							  
							  #include <stdio.h>
								
							  int fileno(FILE *stream);		//리턴 값: stdin 0, stdout 1, stderr 2 	*/

	
	if (isatty(fd))			/* filedes 가 터미널(TTY, teletypewriter)에 연결되어 있는지 확인한다
							
							#include <unistd.h>
					   
							int isatty(int filedes);	//리턴 값: 터미널에 연결 1, 그렇지 않을 경우 0		*/
		mode = _IOLBF;

	else
		mode = _IOFBF;

	if (buf == NULL) {
		mode = _IONBF;
		size = 0;
	}

	else 
		size = BUFFER_SIZE;

	setvbuf(fp, buf, mode, size);

}
