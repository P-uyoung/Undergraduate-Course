/* Fibonacci Sequnece Algorithm
�䳢�� ���ĵ� ���� ���� ���� �˰��� */

#include <iostream>
using namespace std;

int Fibo(int n);

int main(void)
{
	int n;		// ������ ��

	cout << "������ n��° ��:";
	cin >> n;

	Fibo(n);

	cout << "n��° ��:";
	cout << Fibo(n) << endl;

	return 0;
}

int Fibo(int n)
{
	if (n == 0)
		return 0;

	else if (n == 1)
		return 1;
	else
		return Fibo(n - 1) + Fibo(n - 2);
}
