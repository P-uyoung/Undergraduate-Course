#include "Student.h"
int main(void)
{
	Student *stu1 = new Student();
	Student *stu2 = new Student(3);
	Student *stu3 = new Student(3, "ȫ�浿");

	stu1->View();
	stu2->View();
	stu3->View();

	delete stu1;
	delete stu2;
	delete stu3;

	return 0;
}

/* ���� ���
* ��ȣ: N/A �̸� : N/A
* ��ȣ: 3 �̸� : N/A
* ��ȣ: 3 �̸� : ȫ�浿 */
	