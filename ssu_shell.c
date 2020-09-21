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
void multiple_pipe(char ***cmd);
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
		char ***pipe_tokens = (char ***)malloc(MAX_NUM_TOKENS *sizeof(char**));
		char *a_token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
		int i, a_index = 0, a_tokenNo = 0, pipe_counter=0;

		for (i=0; i<strlen(line); i++)
		{
			char readChar= line[i];

			if (readChar == ' ' || readChar == '\n' || readChar == '\t')
			{
				a_token[a_index] = '\0';
				if (a_index != 0)
				{
					if (a_tokenNo==0)
						pipe_tokens[pipe_counter] = (char **)malloc(MAX_TOKEN_SIZE*sizeof(char));
					pipe_tokens[pipe_counter][a_tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
					strcpy(pipe_tokens[pipe_counter][a_tokenNo++], a_token);
					a_index = 0;
				}
			}

			else if (readChar == '|')
			{
				pipe_tokens[pipe_counter][a_tokenNo] = NULL;
				pipe_counter++;
				a_tokenNo = 0;
			}

			else
				a_token[a_index++] = readChar;
		}

		free(a_token);
		pipe_tokens[pipe_counter+1] = NULL;

		printf("Debug) pipe count :%d\n",pipe_counter);

		for(i=0; pipe_tokens[i]!=NULL;i++){	
			for(int j = 0; pipe_tokens[i][j] != NULL; j++)
				printf("Debug) pipe_tokens[%d][%d] :%s\n",i,j, pipe_tokens[i][j]);
		}
		/* END: 구분자 '|' 토큰화하기 */


		/* BEGIN: 파이프가 있는 경우 */
		if (pipe_counter > 0)
		{

			/*			char ***pipe_tokens = (char***)calloc(pipe_countera_tokenNo,sizeof(char**));
						for(i = 0; i < a_tokenNo; i++){
			 *(pipe_tokens+i) = (char**)calloc(MAX_NUM_TOKENS,sizeof(char*));
			 for (int j = 0; j < MAX_NUM_TOKENS; j++){
			 *(*(pipe_tokens+i)+j) = (char*)calloc(MAX_TOKEN_SIZE,sizeof(char));
			 }
			 }

			 for (i = 0; i < a_tokenNo; i++){
			 strcpy(line, a_tokens[i]);
			 *(pipe_tokens+i) = tokenize(line);
			 }

			 for (i = 0; i <a_tokenNo; i++)
			 for (int j= 0; j < 2; j++)
			 printf("pipe_tokens[%d][%d][]=%s\n",i,j,pipe_tokens[i][j]);
			 */
			
			multiple_pipe(pipe_tokens);			

			// Freeing the allocated memory	
			for (i=0;i< pipe_counter;i++){
				for(int j=0; j < MAX_TOKEN_SIZE;j++){
					free(*(*(pipe_tokens+i)+j));
				}
				free(*(pipe_tokens+i));
			}
			free(pipe_tokens);


			continue;
		}
		/* END: 파이프가 있는 경우 */


		/* BEGIN: 파이프가 아닌 경우 */
		tokens = tokenize(line);
		//	printf("Debug) tokenNo = %d\n", tokenNo);
		//do whatever you want with the commands, here we just print them

		for(i=0;tokens[i]!=NULL;i++)	
		{
			printf("Debug) found token %s\n", tokens[i]);

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
  int i, tokenIndex = 0;
  int tokenNo = 0;

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

void multiple_pipe(char ***cmd)
{
	int fd[2];
	pid_t pid;
	int fd_in = 0;

	while (*cmd != NULL)
	{
		printf("Debug) in multiple_pipe\n");
		
		if (pipe(fd)==-1)
		{
			fprintf(stderr, "pipe error\n");
			exit(1);
		}
		
		if ((pid = fork()) == -1)
		{
			fprintf(stderr, "fork error\n");
			exit(EXIT_FAILURE);
		}

		else if (pid == 0)
		{
			printf("Debug) child proces\n");
			dup2(fd_in, 0);	// change the input according to the old one
			printf("Debug) before dup2..\n");
			if(*(cmd+1) != NULL)
				dup2(fd[1], 1);
		
			printf("Debug) before close..\n");
			
			close(fd[0]);
			
			printf("Debug) before execvp\n");

			if(execvp((*cmd)[0], *cmd) < 0)
			exit(EXIT_FAILURE);
		}

		else
		{
			wait(NULL);
			close(fd[1]);
			fd_in = fd[0];	// save the input for the next command
			cmd++;
		}
	}	
}



