//fgets() 함수를 통해 표준입력으로부터 명령어 입력받고, 입력 받은 문자열이 NULL이 아니면 system() 호출한다

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_MAX 256

int main(void)
{
	char command[BUFFER_MAX];
	char *prompt = "Prompt>>";

	while (1) {
		fputs(prompt, stdout);

		if (fgets(command, sizeof(command), stdin) == NULL)
			break;

		system(command);
	}

	fprintf(stdout, "Good bye...\n");
	fflush(stdout);		//while문의 fgets()의 에러발생 대비
	exit(0);
}
/* 실행결과 이해
system()을 통해, ls /, ssu_fgets_2  명령어 실행한다
키보드 입력을 통해 EOF를 넣어준다(Ctrl-d) 
ssu_fgets_2 두번 실행했으므로, Good bye,, 두번 출력된다 */
