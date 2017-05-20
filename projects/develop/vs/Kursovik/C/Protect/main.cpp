#include "MyProgram.h"
#include "Protect.h"
#include "iostream"
#include "LocalRepository.h"

using namespace std;

int main(void)
{
	Protect protect = Protect();
	string licemseKey = "LIC";
	protect.setLicenseKey(licemseKey);

	if (int a = protect.checkProtection() != 0)
	{
		std::cout << "License failed Error:" << a << std::endl;
		system("pause");
		return -1;
	}

	MyProgram prog = MyProgram();
	prog.run();
	return 0;
}