#include <iostream>
#include <string>

using namespace std;

bool password_is_valid(string password)
{
	string valid_pass = "qwerty123";
	if (valid_pass == password)
		return true;
	else
		return false;
}

void get_pass()
{
	string user_pass;
	cout << "Введите пароль: ";
	getline(cin, user_pass);
	if (!password_is_valid(user_pass)) {
		cout << "Неверный пароль!" << endl;
		get_pass(); // Здесь делаем рекурсию
	}
	else {
		cout << "Доступ разрешен." << endl;
	}
}

int main()
{
	get_pass();
	return 0;
}