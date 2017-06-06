/*Mangle-It C++ Source Code Obfuscator*/
#include <iostream>
#include <string>
using namespace std;
bool _debug_symbol_password_is_valid(string password)
{
string _debug_symbol_valid_pass =  decrypt::_debug_symbol_dec_debug(_T( "_debug_qwerty123"));
if (_debug_symbol_valid_pass == password)
return true;
else
return false;
}
void _debug_symbol_get_pass()
{
string _debug_symbol_user_pass;
_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Введите пароль: "));
getline(_debug_symbol_cin, _debug_symbol_user_pass);
if (!_debug_symbol_password_is_valid(_debug_symbol_user_pass)) {
_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Неверный пароль!")) << _debug_symbol_endl;
_debug_symbol_get_pass();
}
else {
_debug_symbol_cout <<  decrypt::_debug_symbol_dec_debug(_T( "_debug_Доступ разрешен.")) << _debug_symbol_endl;
}
}
int main()
{
_debug_symbol_get_pass();
return 0;
}
