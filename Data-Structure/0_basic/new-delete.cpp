/* �����޸� �Ҵ�, ����� �ڷ� ũ�⸦ �̸� ���� ���� ��
new ������	//��ü�� �����ϰ� ������ ��ȯ

�������� �Ҵ�� �޸�(heap)�� ���� ������� �Ѵ�
(stack ������ �ڵ����� �������� ������, ���������� ����ϱ� ������, ���� ���� ���������)

delete ������ :���� �޸��� ���
delete []������ :�迭 �޸��� ���	*/

#include <iostream>
using namespace std;

int main()
{
	int size;
	int i, j;

	cout << "�Ҵ��ϰ��� �ϴ� �迭�� ũ��: ";
	cin >> size;

	int *arr = new int[size];		//in c, malloc(sizeof(int)*size);
	
	//����ó��
	if (arr = NULL) {
		cout << "�޸� �Ҵ� ����" << endl;
		return -1;
	}

	for (i = 0; i < size; i++)
		arr[i] = i;

	for (j = 0; j < size; j++)
		cout << "arr[" << i << "]= " << i << endl;

	delete[]arr;		//in c, free(arr);
}


