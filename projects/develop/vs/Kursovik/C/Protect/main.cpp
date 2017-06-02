#include "MyProgram.h"
#include "Protect.h"
#include "iostream"
#include "LocalRepository.h"

using namespace std;

int main(void)
{
	Protect protect = Protect();
	int res;
	do
	{
		res = protect.checkProtection();
		switch (res)
		{
		case 0:
		{
			cout << "License successful\n" << endl;
			break;
		}
		case -1:
		{
			cout << "Please, enter license key" << endl;
			string licenseKey;
			cin >> licenseKey;
			protect.setLicenseKey(licenseKey);
			break;
		}
		case -11:
		{
			cout << "Please, enter license key" << endl;
			string licenseKey;
			cin >> licenseKey;
			protect.setLicenseKey(licenseKey);
			break;
		}
		case -12:
		{
			cout << "Server not available, repeat later\n" << endl;
			system("pause");
			return -1;
			break;
		}
		case -13:
		{
			cout << "Wrong license key, please repeat\n" << endl;
			string licenseKey;
			cin >> licenseKey;
			protect.setLicenseKey(licenseKey);
			break;
		}
		case -14:
		{
			cout << "Wrong hardware for this license key\n" << endl;
			system("pause");
			return -2;
			break;
		}
		default:
			cout << "Unknown error\n" << endl;
			system("pause");
			return -2;
			break;
			
		}
	}	while (res != 0);

	MyProgram prog = MyProgram();
	prog.run();
	return 0;
}