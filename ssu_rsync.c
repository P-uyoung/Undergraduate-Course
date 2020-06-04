#include "ssu_rsync.h"

int main(void)
{
	while(1)
	{
		char line[BUFFER_SIZE];  //command
		char argv[3][BUFFER_SIZE] = {0};
		
		memset(argv, 0, sizeof(argv));

		fgets(line, sizeof(line), stdin);

		command_separation(line, argv); // 명령어 분리

		if(check_argv(argv) < 0) { // src dst 디렉토리 검사
			print("/* USAGE */\n");
			printf("rsync <src> <dst>\n");
			printf("<src> is file or directory.\n");
			printf("<dst> is directory.\n");
			exit(1);
		}
	
	}


}

// 명령어를 분리하는 함수
void command_separation(char *line, char (*argv)[BUFFER_SIZE])
{
	int argc = 0;
	
	for(int i = 0; i < (int)strlen(line) ; i++)
	{
		for(int j = 0;;j++, i++)
		{

			if (line[i] == ' ')
			{
				argv[argc][j] = '\0';
				argc++;
				break;
			}
		}

		if(line[i] == '\n')
			break;

		argv[argc][j] = line[i];
	}
}

// 입력 포맷 검사하는 함수
int check_argv(char (*argv)[BUFFER_SIZE])
{
	char *temp;
	char *tmp;
	int i;

	temp = malloc(sizeof(char)*PATHMAX);
	getcwd(temp, PATHMAX);
	strcat(t
	
	// argv[0] 입력 검사
	if(argv[0] != "ssu_rysnc")
		return -1;
	
	// argv[1] 입력 검사
	if (access(argv[1], F_OK) < 0) 
		return -1;

	if (access(argv[1], R_OK) < 0)
		return -1;

	// argv[2] 입력 검사
	if (access(argv[2], F_OK) < 0)
		return -1;

	if (access(argv[2], R_OK) < 0)
		return -1;

	if (access(argv[2], W_OK) < 0)
		return -1;



}
