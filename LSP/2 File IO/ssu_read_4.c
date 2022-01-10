/* 특정라인 출력한다.
 
 각 줄에 글자수와 (오프셋)이전까지의 글자수가 필요하다
 구조체_ length와 offset을 필드로 구조체 선언한다
 한 글자씩 읽으면서 개행문자가 오기전까지 length 증가
 개행문자오면 entry +1
 
 (옵션) 디버깅
 
 출력하고 싶은 line number 입력받으면,
 lseek   오프셋(그 전줄까지의 length의 합)을 해준다 
 read    해당 줄의 length 만큼 읽어준다 (-1 혹은 0 치면 프로그램 빠져나온다) 
 printf로 출력해줘야 하므로, buf에 0을 삽입 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define TABLE_SIZE 128
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	static struct {
		long offset;
		int length;
	} table [TABLE_SIZE];	//구조체배열 선언 및 객체선언
	char buf[BUFFER_SIZE];
	long offset;
	int entry;
	int i;
	int length;	//파일에서 read한 글자수
	int fd;		

	if (argc < 2) {
		fprintf(stderr, "usage : %s <file>\n", argv[0]);
		exit(1);
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr,"open error for %s\n", argv[1]);
		exit(1);
	}

	entry = 0;
	offset = 0;
	while ((length = read(fd, buf, BUFFER_SIZE)) > 0) {
		for (i = 0 ; i <length ; i++) {
			table[entry].length++;
			offset++;	//n번째 테이블의 length를 증가시킨다
			
			if (buf[i] == '\n')	//개행문자를 만나면,테이블의 오프셋을 설정한다
				table[++entry].offset = offset;	
		}
	}

#ifdef DEBUG		//디버깅
	for (i = 0; i < entry; i++)
		printf("%d : %ld, %d\n", i+1, table[i].offset, table[i].length);
#endif		

	while (1) {	//특정라인 출력하기
		printf("Enter line number : ");
		scanf("%d", &length);

		if (--length < 0)	
			break;

		lseek(fd, table[length].offset, 0);
		
		if (read(fd, buf, table[length].length) <= 0)	//읽을 바이트가 없을 때
			continue;

		buf[table[length].length] = '\0';
		printf("%s", buf);
	}
	
	close(fd);
	exit(0);
}

