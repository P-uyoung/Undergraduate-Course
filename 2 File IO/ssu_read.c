/* 구조체를 통해 employeefile(인자로 받을)에서 record_num를 인덱스로 직원-봉급 정보를 읽어온다 
읽기전용 파일로 open 한다
while문에서 lseek으로 record 크기만큼 오프셋을 해서 하나씩 읽어온다
(record_num < 0 인 경우 빠져나온다)
printf 함수를 이용하여 "Employee :  Salary : " 출력한다
읽기 오류시, "Record %d not found" 해줄 것 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssu_employee.h"

int main(int argc, char *argv[])
{
    struct ssu_employee record;
    int fd;
    int record_num;             //읽어드릴 이름-봉급 정보의 순서가 된다

    if (argc < 2) {
        fprintf(stderr, "Usage :%s <file>\n", argv[0]);
        exit(1);
    }

   if ((fd = open(argv[1], O_RDONLY)) < 0) {
	   fprintf(stderr, "open error for  %s\n", argv[1]);
	   exit(1);
   }

   while (1) {			//record_num으로 구조체 정보를 차례대로 읽는다
	   printf("Enter record number : ");
	   scanf("%d", &record_num);

	   if (record_num < 0)
		   break;

	   if (lseek(fd,(long)record_num*sizeof(record),0) < 0) {
		   fprintf(stderr, "lseek error\n");
		   exit(1);
	   }

	   if (read(fd,(char *)&record, sizeof(record)) > 0) 
		  printf("Employee : %s Salary :%d\n", record.name, record.salary);
	   else  
		  printf("Record %d not found\n", record_num);
   }

   close(fd);
   exit(0);
}
