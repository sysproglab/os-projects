
#include <iostream>
#include <string>

using namespace std;

bool ReplacementFor_password_is_valid(string ReplacementFor_password)
{
	string ReplacementFor_valid_pass = "qwerty123";
	if (ReplacementFor_valid_pass == ReplacementFor_password)
		return true;
	else
		return false;
}

void ReplacementFor_get_pass()
{
	string ReplacementFor_user_pass;
	cout << "������� ������: ";
	getline(cin, ReplacementFor_user_pass);
	if (!ReplacementFor_password_is_valid(ReplacementFor_user_pass)) {
		cout << "�������� ������!" << endl;
		ReplacementFor_get_pass(); 
	}
	else {
		cout << "������ ��������." << endl;
	}
}

int main()
{
	ReplacementFor_get_pass();
	return 0;
}
