/* ssu_exam.txt 파일에 ID, name 을 사용자입력으로 입력받아,
test1 구조체에 fwrite()로 입력하고,
test2 구조체에 fread()로 저장하고 printf()로 출력한다.  */

#include <stdio.h>
#include <stdlib.h>

struct ssu_id {
	char name[64];
	int id;
};

int main(void)
{
	struct ssu_id test1, test2;	//입력용, 출력용
	char *fname = "ssu_exam.txt";
	FILE *fp;

	if ((fp = fopen(fname, "w")) == NULL) {		//쓰기용
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	printf("Input ID >> ");
	scanf("%d", &test1.id);
	printf("Input name>> ");
	scanf("%s", &test1.name);

	if (fwrite(&test1, sizeof(struct ssu_id), 1, fp) != 1) {
		fprintf(stderr, "fwrite error\n");
		exit(1);
	}

	fclose(fp);

	if ((fp = fopen(fname, "r")) == NULL) {		//읽기용
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}
	if (fread(&test2, sizeof(struct ssu_id), 1, fp) != 1) {
		fprintf(stderr, "fread error\n");
		exit(1);
	}

	printf("\nID	name\n");
	printf(" ===========\n");
	printf("%d      %s\n", test2.id, test2.name);
	fclose(fp);
	exit(0);
}
