#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char *arg_directory = NULL;
	char *arg_prefix = NULL;

	if (argc != 3) {
		fprintf(stderr, "usage : %s <directory> <prefix>\n", argv[0]);
		exit(0);
	}

	arg_directory = argv[1][0] != ' ' ? argv[1] : NULL;		//�ӽ������� ������ ���丮 ����
	arg_prefix = argv[2][0] != ' ' ? argv[2] : NULL;		//�ӽ� ���� �̸��� ���ξ� ����
	printf("created : %s\n", tempnam(arg_directory, arg_prefix));	//�ӽ� ���� ����
	exit(0);
}
