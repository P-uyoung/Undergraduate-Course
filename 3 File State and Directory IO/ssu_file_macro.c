/* 표준 테스트 매크로를 이용하여 인자로 받은 여러개의 파일들의 파일 종류를 출력한다.
stat 구조체의 st_mode 필드 이용한다 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	struct stat file_info;
	char *str;
	int i;

	for (i = 1; i < argc; i++) {		
		printf("name = %s, type = ",argv[i]);
		
		if (lstat(argv[i], &file_info) < 0) {
			fprintf(stderr, "lstat error\n");
			continue;
		}
	
		if (S_ISREG(file_info.st_mode))		//파일의 모드를 이용해서 표준매크로 사용 
			str = "regular";
	
		else if (S_ISDIR(file_info.st_mode))	
			str = "directory";  

		else if (S_ISCHR(file_info.st_mode))     	
			str = "character special";
	               
		else if (S_ISBLK(file_info.st_mode))    
       		        str = "block special";
        
		else if (S_ISFIFO(file_info.st_mode))     
        	        str = "FIFO";        		
	
		else if (S_ISLNK(file_info.st_mode))    	
			str = "symbolic link";
	     
       	else if (S_ISSOCK(file_info.st_mode))            		  
		   	str = "socket";
	
		else 
			str = "unknown mode";
	
		printf("%s\n", str);
	}
	exit(0);
}
