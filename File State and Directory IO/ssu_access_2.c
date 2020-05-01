/* 인자로 지정한 파일에 대해 접근 가능한지 파일의 권한을 출력한다 2
구조체 배열을 사용한다 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TABLE_SIZE (sizeof(table)/sizeof(*table))
/* sizeof(*table) : 8	//구조체를 정렬할 때, 멤버 중에서 가장 큰 자료형의 크기의 배수로 정렬
   sizeof(table) : 32
   sizeof(TABLE_SIZE) : 4 */
int main(int argc, char *argv[])
{
	struct {	//구조체 선언 및 할당
		char *text;
		int mode;
	} table[] = {
		{"exists", 0},
		{"execute", 1},
		{"write", 2},
		{"read", 4}
	};	

	int i;

	if (argc < 2) {
		fprintf(stderr, "usage : %s <file>\n", argv[0]);
		exit(1);
	}

	for (i = 0; i < TABLE_SIZE; i++) {	//4번 반복문 돈다
		if (access(argv[1], table[i].mode) != -1)
			printf("%s -ok\n", table[i].text);

		else 
			printf("%s\n", table[i].text);
	}

	exit(1);
}	
