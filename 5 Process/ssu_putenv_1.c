//putenv(), getenv(): 특정 환경변수의 값을 알고 싶거나, 환경변수 값을 변경 혹은 생성하고자 할 때
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>		// environ 변수 선언되어 있다
#include <string.h>

void ssu_addone(void);

extern char **environ;		// '이름=값' 형태
char glob_var[] = "HOBBY=swimming";

int main(void)
{
	int i;
	for (i =0; environ[i] != NULL; i++)
		printf("environ[%d] : %s\n", i, environ[i]);	//등록 전, 환경변수 리스트 출력

	putenv(glob_var);	//전역변수로 선언된 환경변수 등록
	ssu_addone();		//지역변수	"
	
	printf("My hobby is %s\n", getenv("HOBBY"));	// >> 실행결과 제대로 출력
	printf("My lover is %s\n", getenv("LOVER"));	// >> 실행결과 제대로 출력 안됨(null)

	strcpy(glob_var+6, "fishing");		//"HOBBY=" 다음 열부터 strcpy

	for (i = 0; environ[i] != NULL; i++)
		printf("environ[%d] : %s\n", i, environ[i]);
	
	exit(0);
}

void ssu_addone(void) {
	char auto_var[10];

	strcpy(auto_var, "Lover=js");
	putenv(auto_var);	
}
