//symlink()를 이용하여 /home/oslab/oslab 파일을 /bin/에 바로가기 만들기
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "usage : %s<actualname><symname>\n", argv[0]);
		exit(1);
	}

	if (symlink(argv[1], argv[2]) < 0) {		//argv[2]가 argv[1]의 심볼릭링크파일 이 된다
		fprintf(stderr, "symlink error\n");
		exit(1);
	}

	else
		printf("symlink : %s-> %s\n", argv[2], argv[1]);	//argv[2] :(Windows)바로가기 파일과 동일

	exit(0);
}
