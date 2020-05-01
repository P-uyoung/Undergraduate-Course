//remove()로 파일 삭제
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc != 3 ) {
		fprintf(stderr, "usage : %s <oldname><newname>\n", argv[0]);
		exit(1);
	}

	if (link(argv[1],argv[2]) < 0) {	// abcd.txt파일을 새로운 파일명 edfg.txt파일로 link
		fprintf(stderr, "link error\n");
		exit(1);
	}

	else
		printf("step1 passed.\n");

	if (remove(argv[1]) < 0) {		//open하지 않았으므로 참조계수 0, 따라서 파일삭제
		fprintf(stderr, "remove error\n");
		remove(argv[2]);
		exit(1);
	}

	else 
		printf("step2 passed.\n");

	printf("Success!\n");
	exit(0);
}
