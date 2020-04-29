/* 동적메모리 할당, 사용할 자료 크기를 미리 알지 못할 때
new 연산자	//객체를 생성하고 포인터 반환

동적으로 할당된 메모리(heap)는 직접 지워줘야 한다
(stack 정보도 자동으로 지워지진 않지만, 순차적으로 사용하기 때문에, 이전 값을 덮어버린다)

delete 변수명 :단일 메모리일 경우
delete []변수명 :배열 메모리일 경우	*/

#include <iostream>
using namespace std;

int main()
{
	int size;
	int i, j;

	cout << "할당하고자 하는 배열의 크기: ";
	cin >> size;

	int *arr = new int[size];		//in c, malloc(sizeof(int)*size);
	
	//예외처리
	if (arr = NULL) {
		cout << "메모리 할당 실패" << endl;
		return -1;
	}

	for (i = 0; i < size; i++)
		arr[i] = i;

	for (j = 0; j < size; j++)
		cout << "arr[" << i << "]= " << i << endl;

	delete[]arr;		//in c, free(arr);
}


