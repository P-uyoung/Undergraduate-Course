#include "Student.h"

Student::Student(void)
{
	num = 0;
	name = "";
}
Student::Student(int num)
{
	this->num = num;		// this->num: Student 클래스의 멤버변수
	name = "";
}
Student::Student(int num, string name)
{
	this->num = num;
	this->name = name;
}
void Student::View()
{
	if (num)
		cout << "번호: " << num;
	else
		cout << "번호: N/A";
	if (name != "")
		cout << " 이름: " << name << endl;
	else
		cout << " 이름: N/A" << endl;

}

/* Ref :http://ehpub.co.kr/%EB%94%94%EB%94%A4%EB%8F%8C-c-16-%EC%83%9D%EC%84%B1%EC%9E%90-%EC%86%8C%EB%A9%B8%EC%9E%90/ */