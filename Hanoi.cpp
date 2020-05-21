/* Hanoi Tower Algorithm
*  기둥1에 있는 n 개의 원판을 기둥2를 이용하여 기둥3으로 옮기는 알고리즘
*  Hanoi 함수의 else 문
*  1 기둥1에서 n-1개의 원판을 기둥3을 이용하여 기둥2로 옮긴다
*  2 기둥1에서 남은 1개의 원판을 기둥3으로 옮긴다
*  3 기둥2에서 n-1개의 원판을 기둥1을 이용하여 기둥3으로 옮긴다 */

#include <iostream>
using namespace std;

void Hanoi(int n, char from, char by, char to);

int main() {
	int n;		// 원판 개수

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