#include <iostream>
#include <fstream>	//for) ofstream, iostream Ŭ������ �̿�
using namespace std;

int main() {
	ofstream outFile("my.out", ios::out);		//ofstream : in c, FILE ����ü
												//ios::out : ���¸�� �÷��� ��¸��� 
	if (!outFile) {
		cerr << "cannot open my.out" << endl;	//cerr :ǥ�� ���� ��ġ
		return -1;
	}

	int n = 50;
	float f = 20.3;
	outFile << "n: " << n << endl;	//ofstream�� ��ü�� : ���� �Է�
	outFile << "f: " << f << endl;

	return 0;
}