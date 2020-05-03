//blackbeard 구조체 변수를 fwrite() 호출하여, 파일에 입력하고, 새로운 pirate 구조체에 fread() 호출하여 저장한 후 출력
#include <stdio.h>
#include <stdlib.h>

struct ssu_pirate {
	unsigned long booty;
	unsigned int beard_length;
	char name[128];
};

int main(void)
{
	struct ssu_pirate blackbeard = {950, 48, "Edward Teach"}, pirate;		//fwrite,fread용
	char *fname = "ssu_data";
	FILE *fp1, *fp2;		//fwrite, fread용

	if ((fp2 = fopen(fname, "w")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	if (fwrite(&blackbeard, sizeof(struct ssu_pirate), 1, fp2) != 1) {	//fp2 파일에 구조체 변수 입력
		fprintf(stderr, "fwrite error\n");
		exit(1);
	}

	if (fclose(fp2)) {
		fprintf(stderr, "fclose error\n");
		exit(1);
	}

	if ((fp1 = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "fopen error\n");
		exit(1);
	}

	if (fread(&pirate, sizeof(struct ssu_pirate), 1, fp1) != 1) {		//fp1 파일에서 pirate 구조체에 입력
		fprintf(stderr, "fread error\n");
		exit(1);
	}

	if (fclose(fp1)) {
		fprintf(stderr, "fclose error\n");
		exit(1);
	}	

	printf("name=\"%s\" booty=%lu beard_length=%u\n", pirate.name, pirate.booty, pirate.beard_length);
	
	exit(0);
}	
