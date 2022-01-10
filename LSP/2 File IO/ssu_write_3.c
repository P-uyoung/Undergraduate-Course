{/* 구조체에 <직원명> <봉금> 입력받고 바로 write로 파일에 입력한다
 (구조체 배열을 사용하지 않는다) ('.' 치면 빠져나온다) 
 구조체 구현부는 employee.h 파일에 따로 구현한다 
 main 함수에 empolyeefile(새파일)이 인자로 들어온다 
 open 함수로 O_CREAT | O_EXCL 640 모드로 파일 연다 
 while문으로 입력 다 받으면, close로 파일 닫고 정상종료한다	*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssu_employee.h"

int main(int argc, char *argv[])
{
    struct ssu_employee record;
    int fd;

    if(argc < 2) {
        fprintf(stderr,"Usage : %s <file>\n", argv[0]);
        exit(1);
    }

    if((fd = open(argv[1],O_WRONLY | O_CREAT | O_EXCL, 0640)) < 0) {
        fprintf(stderr, "open error for %s\n", argv[1]);
        exit(1);
    }

    while (1) {     //구조체에 scanf를 통해 입력는다
        printf("Enter employee name <SPACE> salary: ");
        scanf("%s", record.name);

        if(record.name[0] == '.')
            break;              //'.'문자 입력받으면 while문 빠져나온다

        scanf("%d", &record.salary);
        record.pid = getpid();
        write(fd,(char *)&record, sizeof(record));  //구조체에 입력받은 값 write()로 파일에 쓰기
    }

    close(fd);
    exit(0);
}
