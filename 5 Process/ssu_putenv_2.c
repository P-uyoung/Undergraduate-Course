/* 특정 환경변수가 아닌, 환경변수 전체를 접근할 경우 getenv() 대신
environ 변수 혹은 main() 의 3번째 인자를 이용할 수 있다*/

#include <stdio.h>
#include <stdlib.h>

void ssu_printenv (char *label, char ***envpp);

extern char **environ;		// =*env[] 

int main (int argc, char *argv[], char *envp[])
{
	ssu_printenv("Initially", &envp);
	putenv("TZ=PST8PDT");
	
	ssu_printenv("After changing TZ", &envp);
	putenv("WARNING=Don't use envp after putenv()");
	
	ssu_printenv("After setting a new variable", &envp);
	printf("value of WARNING IS %s\n", getenv("WARNING"));
	
	exit(0);
}

//새로운 환경변수 등록 후, 라벨링하여 환경변수 리스트 출력
void ssu_printenv (char *label, char ***envpp) {	
	char ***ptr;

	printf("---- %s ---\n", label);
	printf("envp is at %8o and contains %8o\n", envpp, *envpp);
	printf("environ is at %8o and contains %8o\n", &environ, environ);
	printf("My environment variable are:\n");
	
	for (ptr = environ; *ptr; ptr++)
		printf("(%8o) = %8o -> %s\n", ptr, *ptr, *ptr);

	printf("(%8o) = %8o\n", ptr, *ptr);
}
