//fscanf(), fprintf() 이용하여 파일 수정하기
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char *fname = "ssu_test.dat";
	char name[BUFFER_SIZE];
	FILE *fp;
	int age;

	fp = fopen(fname, "r");
	fscanf(fp, "%s%d", name, &age);		//fp 파일에서, name, age 값 입력받기
	//printf("%s%d", name, age);		//터미널에 해당 값 출력된다
	fclose(fp);
	fp = fopen(fname, "w");
	fprintf(fp, "%s is %d years old\n", name, age);		//fp 파일에, name, age 값 출력하기
	fclose(fp);
	exit(0);
}
