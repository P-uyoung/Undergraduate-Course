// ���� ���� ���α׷�
// fputc, read, write �� ��, ������ �ƽ�Ű�ڵ��ȣ�� �̿��ؼ� ���ڷ� �ٲ�� �Ѵ�

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {

	int i;
	int array[50];
	int d;
	FILE *fp;

	for (i = 0; i < 50; i++) {
		array[i] = rand() % 10;
		printf("%d", array[i]);
	}

	if ((fp = fopen("ssu_test1.txt", "w+")) == NULL) {
		fprintf(stderr, "fopen error\n");
		exit(1);
	}

	for (i = 0; i < 50; i++) {
		fputc(array[i], fp);		// �̷��� �����ϸ�, �̻��� ���鸸 ����.. array[i]+48 �� �ٲ�� �Ѵ�

	}

	fclose(fp);
	exit(0);
}