/* 인자로 지정한 파일에 대해 접근 가능한지 파일의 권한을 출력한다.
프로토타입  int access(const char *pathname, int mode);
mode 인자 상수  F_OK, R_OK, W_OK, X_OK	*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int i;

	if (argc < 2) {
		fprintf(stderr, "usage : %s <file1><file2>..<fileN>\n", argv[0]);
		exit(1);
	}

	for (i = 1; i < argc; i++) {
		if (access(argv[i], F_OK) < 0) { 	//파일이 존재하는지
			fprintf(stderr, "%s doesn't exist.\n", argv[i]);
			continue;
		}

                if (access(argv[i], R_OK) == 0)         //파일은 읽기권한이 있음
                        printf("User can read %s\n", argv[i]);

				if (access(argv[i], W_OK) == 0)         //파일은 쓰기권한이 있음
                        printf("User can write %s\n", argv[i]);

                if (access(argv[i], X_OK) == 0)         //파일은 실행권한이 있음
                        printf("User can execute %s\n", argv[i]);
	}

	exit(0);
}

