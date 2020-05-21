/* Fibonacci Sequnece Algorithm
토끼의 번식데 대한 문제 제시 알고리즘 */

#include <iostream>
using namespace std;

int Fibo(int n);

int main(void)
{
	int n;		// 수열의 항

	cout << "수열의 n번째 항:";
	cin >> n;

	Fibo(n);

	cout << "n번째 항:";
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
