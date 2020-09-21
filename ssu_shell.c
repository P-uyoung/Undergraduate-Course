#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line);

int main(int argc, char* argv[]) 
{
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;

	FILE* fp;
	if(argc == 2) 
	{
		fp = fopen(argv[1],"r");
		if(fp == NULL) 
		{
			printf("File doesn't exists.");
			
			return -1;
		}
	}

	while(1) 
	{			
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		if(argc == 2)	// batch mode
		{ 
			if(fgets(line, sizeof(line), fp) == NULL) { // file reading finished
				break;	
			}
			line[strlen(line) - 1] = '\0';
		} 
		else	// interactive mode
		{ 
			printf("$ ");
			scanf("%[^\n]", line);
			getchar();
		}
//		printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line

		/* BEGIN: 구분자 '|' 토큰화하기 */
		char **a_tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
		char *a_token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
		int tokenIndex = -1;
		int tokenNo = 0;	// 토큰개수

		for (i=0; i<strlen(line); i++)
		{
			char readChar= line[i];

			if (readChar =='|')
			{
				a_token[tokenIndex] = '\0';
				if (tokenIndex != -1)
				{
					a_tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
					strcpy(a_tokens[tokenNo++], a_token);
					i++;
					tokenIndex = -1;
				}
			} else
				a_token[++tokenIndex] = readChar;
		}
		if (tokenIndex != -1)
		{
			a_token[tokenIndex] = '\0';
			a_tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
			strcpy(a_tokens[tokenNo++], a_token);
		}

		free(a_token);
		printf("%d\n", tokenNo);
		a_tokens[tokenNo] = NULL;
		for(i=0; a_tokens[i]!=NULL;i++)	
		{
			printf("pipe token %s (remove this debug output later)\n", a_tokens[i]);
		}	
		/* END: 구분자 '|' 토큰화하기 */

		
		/* BEGIN: 파이프가 있는 경우 */
		if (tokenNo > 1)
		{
			pid_t pid;
			int pipe_fd[2];
			int count = tokenNo-1; // 자식프로세스 생성개수
			char buf[MAX_INPUT_SIZE] = {0};

			if (pipe(pipe_fd) == -1)
			{
				fprintf(stderr, "pipe error\n");
				exit(1);
			}

			if (count > 0)
				child_fork();
			for (int j = 0; j < tokenNo; j++)
			{
				//pipe 초기화
				// buf초기화
				tokens = tokenize(a_tokens[j]);

				dup2(pipe_fd[1],1);
//				dup2(stdin,pipe_fd[0]);


				if (execvp(tokens[0], tokens) < 0) 
				{
				
					
					sprintf(a_usr_path, "/usr%s", a_path);
					printf("%d 프로세스 (/usr)경로명 %s\n", j+1, a_usr_path);

					if (execv(a_usr_path, tokens) < 0)
					{
						printf("SSUShell : Incorrect command\n");
						exit(1);
					}
				}

				i = 0;
				while(1) 
				{
					if (read(pipe_fd[1], &buf[i], 1) > 0)
						i++;
					
					else
					{
						buf[i] = '\0';
						break;
					}
				}
				
				printf("%s\n", buf);				


				

			}

			
			
			
			
			// Freeing the allocated memory	
			for(i=0;a_tokens[i]!=NULL;i++){
				free(a_tokens[i]);
			}	
			free(a_tokens);

			continue;
		}
		/* END: 파이프가 있는 경우 */


		/* BEGIN: 파이프가 아닌 경우 */
		tokens = tokenize(line);
   
       //do whatever you want with the commands, here we just print them
		

		for(i=0;tokens[i]!=NULL;i++)	
		{
			printf("found token %s (remove this debug output later)\n", tokens[i]);
			
		}			
		
		// fork() > exec() > wait()
		pid_t pid;
		int status;
		
		if ((pid = fork()) < 0) {
			fprintf(stderr, "fork error\n"); 
			exit(1);
		}

		else if (pid == 0)
		{	
			if (execvp(tokens[0], tokens) < 0) 
			{
				if(tokens[0][0] == '\n')
					exit(1);

				printf("SSUShell : Incorrect command\n");
				exit(1);
			}
		}

		else
		{
			wait(&status);	// 자식프로세스 회수
//			printf("자식 프로세스 회수 완료\n");
			// ...
		}

		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}

char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++)
  {
    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t')
	{
      token[tokenIndex] = '\0';
      if (tokenIndex != 0)
	  {
		tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
		strcpy(tokens[tokenNo++], token);
		tokenIndex = 0; 
      }
    } 
	else 
	{
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  
  return tokens;
}


void child_fork(int n, int *p_fd, int **p_token )	// 자식 프로세스 개수
{
	pid_t pid;
	char buf;

	if ((pid=fork()) < 0){
			fprintf(stderr,"fork error\n");
			exit(1);
	}

	else if (pid == 0) {
		if (n > 1)
			child_fork(n-1, p_fd, p_token);
		
		close(p_fd[1]);
		close(STDIN_FILENO);
		
		int new_stdin = dup(p_fd[0]);
		execlp(p_token[

			
	}
		

	if (n > 1) 
	{
		if((pid=fork()) < 0){
			fprintf(stderr,"fork error\n");
			exit(1);
		}
		
		child_fork(n-1);
	
	}else
	{
		if((pid = fork()
		
	
	
	}

	





}
