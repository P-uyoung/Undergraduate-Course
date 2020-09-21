#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

int a_tokenNo;	// | 구분자로 토큰화한 토큰개수

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
		int a_tokenIndex = -1;

		for (i=0; i<strlen(line); i++)
		{
			char readChar= line[i];

			if (readChar =='|')
			{
				a_token[a_tokenIndex] = '\0';
				if (a_tokenIndex != -1)
				{
					a_tokens[a_tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
					strcpy(a_tokens[a_tokenNo++], a_token);
					i++;
					a_tokenIndex = -1;
				}
			} else
				a_token[++a_tokenIndex] = readChar;
		}
		if (a_tokenIndex != -1)
		{
			a_token[a_tokenIndex] = '\0';
			a_tokens[a_tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
			strcpy(a_tokens[a_tokenNo++], a_token);
		}

		free(a_token);

		printf("Debug) 파이프구분자 토큰개수 :%d\n", a_tokenNo);
		
		a_tokens[a_tokenNo] = NULL;
		for(i=0; a_tokens[i]!=NULL;i++)	
		{
			printf("Debug) pipe token :%s\n", a_tokens[i]);
		}	
		/* END: 구분자 '|' 토큰화하기 */

		
		/* BEGIN: 파이프가 있는 경우 */
		if (a_tokenNo > 1)
		{
			pid_t pid;
			int status;
			int pipe_counter = a_tokenNo-1;
			int *pipe_fd = (int *)calloc(pipe_counter*2, sizeof(int));
			int *ptr_fd = pipe_fd;
//			int count = a_tokenNo; // 재귀변수

			for (i = 0; i < pipe_counter; i++) {
				if (pipe(ptr_fd) == -1)
				{
					fprintf(stderr, "pipe error\n");
					exit(1);
				}
				ptr_fd +=2;
			}

			int j = 0; // .. 0초기화
			for (i = 0; i < a_tokenNo; i++) 
			{
				printf("Debug) %d 번째 루프\n", i+1);

				if((pid=fork()) < 0)
				{
					fprintf(stderr, "fork error\n");
					exit(1);
				}
				else if (pid == 0)
				{/* child */
					if (i < pipe_counter && dup2(pipe_fd[j+1],1) < 0)	// If this is not the last remaining command
						exit(1);

					if (j != 0 && dup2(pipe_fd[j-2],0) < 0)	// If this is not the first command
						exit(1);

					for (int k = 0; k < 2*pipe_counter; k++)
						close(pipe_fd[k]);

					tokens = tokenize(a_tokens[i]);
					if (execvp(tokens[0],tokens) < 0)	// Execute and if it returns anything, exit!
					{
						printf("SSUShell : Incorrect command\n");
						exit(1);
					}
					else{
						printf("Debug) exec 정상실행 ");
						while(tokens[i] != NULL)
							printf("%s ", tokens[i++]);

					}
				} j += 2;
			}

			for (i = 0; i < 2*pipe_counter++; i++)
				close(pipe_fd[i]);

			for (i = 0; i < a_tokenNo; i++)		// The only program that gets to this point is
				wait(&status);					// the parent process, which should wait for completeion.

			// Freeing the allocated memory	
			for(i=0;tokens[i]!=NULL;i++){
				free(tokens[i]);
			}	
			free(tokens);

			for(i=0;a_tokens[i]!=NULL;i++){
				free(a_tokens[i]);
			}	
			free(a_tokens);

			for (i = 0; i < a_tokenNo; i++)
				printf("Debug) pipe_fd[%d] :%d\n", i,pipe_fd[i]);

//			child_fork(count, pipe_fd, a_tokens);
		//	
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
		/* END: 파이프가 아닌 경우 */
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
/*

void child_fork(int n, int *p_fd, int **p_tokens )	// 자식 프로세스 개수
{
	pid_t pid;
	char buf;
	char **tokens = tokenize(p_tokens[a_tokenNo-n]);

	if ((pid=fork()) < 0){
			fprintf(stderr,"fork error\n");
			exit(1);
	}

	else if (pid == 0) {
		if (n > 1)
			child_fork(n-1, p_fd, p_tokens);
		
		close(p_fd[1]);
		close(STDIN_FILENO);
		
		int new_stdin = dup(p_fd[0]);
		if (execvp(tokens[0], tokens) < 0)
		{
			printf("SSUShell : Incorrect command\n");
			exit(1);
		}

		close(p_fd[0]);
		close(new_stdin);
		
		exit(0);
	}

	else
	{
		close(p_fd[0]);
		close(STDOUT_FILENO);

		int new_stdout = dup(p_fd[1]);

	
	}


	// Freeing the allocated memory	
	for(i=0;a_tokens[i]!=NULL;i++){
		free(a_tokens[i]);
	}	
	free(a_tokens);
}*/
