// Polinoms.cpp : Defines the entry point for the console application.
//

#include "TList.h"
#include <iostream>

using namespace std;

void main()
{
	setlocale(LC_ALL, "Russian");
	int input, num;
	string str1;
	string str2;
	Polinom p1(10);
	Polinom p2(p1);
	Polinom p3;

	cout << "������� ������ �������: ";
	cin >> str1;
	p1.Input(str1);

	cout << "������� ������ �������: ";
	cin >> str2;
	p2.Input(str2);

	cout << "\n1 - ������������ ���������\n2 - ����� ���������\n3 - �������� ���������\n4 - ��������� �� ���������\n5 - �����\n";
	cin >> input;
	switch (input)
	{
	case 1:
		cout << "������������ ���������: ";
		p3 = p1*p2;
		cout << p3.Output() << endl;
		break;
	case 2:
		cout << "����� ���������: ";
		p3 = p1 + p2;
		cout << p3.Output() << endl;
		break;
	case 3:
		cout << "�������� ���������: ";
		p3 = p1 - p2;
		cout << p3.Output() << endl;
		break;
	case 4:
		do {
			cout << "�������� �������: ";
			cin >> num;
		} while (num != 1 && num != 2);
		int cnst;
		cout << "������� ���������: ";
		cin >> cnst;
		if (num == 1)
		{
			p1 = p1 * cnst;
			cout << p1.Output() << endl;
		}
		else
		{
			p2 = p2 * cnst;
			cout << p2.Output() << endl;
		}
		break;
	default:
		break;
	}
}

