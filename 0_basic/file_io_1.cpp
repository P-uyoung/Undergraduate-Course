#include <iostream>
#include <fstream>	//for) ofstream, iostream 클래스를 이용
using namespace std;

int main() {
	ofstream outFile("my.out", ios::out);		//ofstream : in c, FILE 구조체
												//ios::out : 오픈모드 플래그 출력모드로 
	if (!outFile) {
		cerr << "cannot open my.out" << endl;	//cerr :표준 오류 장치
		return -1;
	}

	int n = 50;
	float f = 20.3;
	outFile << "n: " << n << endl;	//ofstream형 객체명 : 파일 입력
	outFile << "f: " << f << endl;

	return 0;
}