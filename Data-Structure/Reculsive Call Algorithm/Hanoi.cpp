/* Hanoi Tower Algorithm
*  ���1�� �ִ� n ���� ������ ���2�� �̿��Ͽ� ���3���� �ű�� �˰���
*  Hanoi �Լ��� else ��
*  1 ���1���� n-1���� ������ ���3�� �̿��Ͽ� ���2�� �ű��
*  2 ���1���� ���� 1���� ������ ���3���� �ű��
*  3 ���2���� n-1���� ������ ���1�� �̿��Ͽ� ���3���� �ű�� */

#include <iostream>
using namespace std;

void Hanoi(int n, char from, char by, char to);

int main() {
	int n;		// ���� ����

	cout << "disk number: ";
	cin >> n;

	Hanoi(n, 'A', 'B', 'C');

	return 0;
}

void Hanoi(int n, char from, char by, char to)
{
	if (n == 1)
		cout << from << "=>" << to << endl;
	else {
		Hanoi(n - 1, from, to, by);		
		cout << from << "=>" << to << endl;
		Hanoi(n - 1, by, from, to);
	}
}