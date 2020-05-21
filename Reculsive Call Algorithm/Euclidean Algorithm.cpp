/* Euclidean Algorithm 을 이용한 GCD 구하기
(a,b) = (b,r)		a > b, r: a를 b로 나눈 나머지
(b,0) 일 때, 최대공약수는 b	*/

#include <iostream>
using namespace std;

int GCD(int a, int b);

int main(void)
{
	int a, b;

	cout << "최대공약수 a b:";
	cin >> a >> b;

	cout << GCD(a, b) << endl;

	return 0;
}

int GCD(int a, int b)
{
	if (b == 0)
		return a;
	
	else 
		return GCD(b, a%b);	// 재귀호출.. void 함수가 아니라면 return 해야한다
}
