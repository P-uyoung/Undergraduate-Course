#pragma once	// �����Ϸ� ����ĵ ����
#include <iostream>
#include <string>
using namespace std;

class Student
{
	int num;
	string name;

public:
	// ������ �ߺ� ����
	Student(void);
	Student(int num);
	Student(int num, string name);
	void View();
};

/* Ref :http://ehpub.co.kr/%EB%94%94%EB%94%A4%EB%8F%8C-c-16-%EC%83%9D%EC%84%B1%EC%9E%90-%EC%86%8C%EB%A9%B8%EC%9E%90/ */