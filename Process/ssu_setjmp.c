#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>

void ssu_nested_func (int loc_var, int loc_volatile, int loc_register);

static jmp_buf glob_buffer;

int main(void)
{									/* Type Qualifier 형 한정자	*/
	register int loc_register;		//restrict: 메모리 대신 CPU의 레지스터 사용 >> 메모리 접근횟수를 줄이는 최적화를 함
	volatile int loc_volatile;		//volatile: 해당 변수를 (레지스터가 아닌) 항상 메모리에 접근하도록 만든다
									// >> 즉, 이 변수는 언제든지 값이 바뀔 수 있으니까 항상 메모리에 접근하도록 함
	int loc_var;

	loc_var = 10;
	loc_volatile = 11;
	loc_register = 12;


	if (setjmp(glob_buffer) != 0) {		//직접 호출인 경우 0 리턴, longjmp()에 의한 호출인 경우 val 리턴
		printf("after longjmp, loc_var = %d, loc_volatile = %d,loc_register = %d\n", loc_var, loc_volatile, loc_register);	

		exit(0);
	}

	loc_var = 80;		
	loc_volatile = 81;	//최적화에 상관없이, 메모리에 저장되어 환원 X
	loc_register = 83;
	
	ssu_nested_func(loc_var, loc_volatile, loc_register);
	
	exit(0);
}

void ssu_nested_func (int loc_var, int loc_volatile, int loc_register) {

	printf("before longjmp, loc_var = %d, loc_volatile = %d,loc_register = %d\n", loc_var, loc_volatile, loc_register);

	longjmp(glob_buffer, 1);
}

/* 실행 결과 */
/* 1) 최적화 옵션을 사용한 컴파일	
before longjmp, loc_var = 80, loc_volatile = 81,loc_register = 83
after longjmp, loc_var = 10, loc_volatile = 81,loc_register = 83(12, 컴파일환경차이) */
/* 2) 사용하지 않은 컴파일 gcc ....
before longjmp, loc_var = 80, loc_volatile = 81,loc_register = 83
after longjmp, loc_var = 80, loc_volatile = 81,loc_register = 12 */

