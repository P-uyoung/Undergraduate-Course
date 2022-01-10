/* <read_3A.c> 백그라운드로 실행하면서 <read_3B.c> 실행한다   
<read_3A.c> 문자 하나씩 읽으면서 화면에 출력한다 (getchar)   
"ssu_test.txt" 파일 읽기전용파일로 오픈한다
<read_3B.c> 문자 하나씩 읽으면서, line_count 출력한다   
("Total line : ")										*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	char c;		
	int fd;

	if ((fd = open("ssu_test.txt", O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", "ssu_test.txt");
		exit(1);
	}

	while (1) {		//파일에 문자를 하나씩 읽은 후, 화면에 출력
		
		if (read(fd, &c, 1) > 0)
			putchar(c);
	else
		break;
	}

	exit(0);
}
