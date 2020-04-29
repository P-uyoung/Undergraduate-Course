#include <iostream>
using namespace std;

int min(int x, int y) {
	return x > y ? y : x;
}

int main()
{
	cout << (5 > 6 ? 6 : 5) << endl;	//in c, min(5,6);
	cout << (3 > 2 ? 2 : 3) << endl;

	return 0;
}