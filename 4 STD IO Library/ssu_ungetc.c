/* ungetc() 함수를 이용하여 파일로부터 입력 받은 숫자와 연산자를 분리하여 출력한다
isdigit() 함수로 number 먼저 받고,
EOF 이어서가 아니라, 문자여서 while문 빠져나온 경우, ungetc() 하고 operator로 다시 받기 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(void)
{
	char operator;
	FILE *fp;
	int character;
	int number = 0;		//자릿수

	if ((fp = fopen("ssu_expr.txt", "r")) == NULL) {
		fprintf(stderr, "fopen error for ssu_expr.txt\n");
		exit(1);
	}

	while (!feof(fp)) {
		while ((character = fgetc(fp)) != EOF && isdigit(character))
			number = 10 * number +character -48;	//문자처리 ASCII 값, 0: 48
		
		fprintf(stdout, "%d\n", number);
		number = 0;

		if (character != EOF) {
			ungetc(character, fp);		//읽은 문자 하나 되돌리기
			operator = fgetc(fp);
			printf("Operator => %c\n", operator);
		}
	}
	fclose(fp);
	exit(0);
}

/* 실행결과 이해 
 파일 끝에는 기본적으로 개행문자가 들어간다
 operator에 개행문자가 찍히고, number 값이 출력되고 while문을 빠져나간다. */
