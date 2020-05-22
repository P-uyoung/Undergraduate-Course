/* <ssu__exit.c> */
/* _exit() 표준입출력 버퍼를 비우지 않고 종료, 아무것도 출력하지 않는다 */

#include <stdio.h>
#include <unistd.h>

int main(void)
{
	printf("Good afternoon?");	//출력되지 않고, fflush() 된다
	_exit(0);	
}




/* <ssu_exit.c> */
/* _exit() 표준입출력 버퍼에 남아있는 문자를 표준 출력하고 종료 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("Good afternoon");
	exit(0);	//표준입출력 정리
}
