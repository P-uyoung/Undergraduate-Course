/*  인자로 받는 파일의 파일모드 변경
비트연산자 이용 (or, xor)  */

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/stat.h> 

#define MODE_EXEC (S_IXUSR|S_IXGRP|S_IXOTH) 

int main(int argc, char *argv[]) 
{ 	
	struct stat statbuf; 
	int i; 
	if (argc < 2) { 
		fprintf(stderr, "usage: %s <file1> <file2> ... <fileN>\n", argv[0]); 
		exit(1); 
	} 
	
	for (i= 1; i< argc; i++) { 
		if (stat(argv[i], &statbuf) < 0) { 
			fprintf(stderr, "%s : stat error\n", argv[i]); 
			continue; 
		}

		statbuf.st_mode |= MODE_EXEC; //or 연산 후 할당, 실행모드 켜기
		statbuf.st_mode ^= (S_IXGRP|S_IXOTH); //xor  연산 후 할당, % umask 011    

		if (chmod(argv[i], statbuf.st_mode) < 0) 
			fprintf(stderr, "%s : chmoderror\n", argv[i]); 
		
		else 
			printf("%s : file permission was changed.\n", argv[i]); 
	}  
       
	exit(0);
}

