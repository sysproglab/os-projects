#include "TSPServer.h"
#include "DataAccessObject.h"

int main(void)
{
	//DataAccessObjecct dao = DataAccessObjecct();
	//string hw = dao.getHardwareByLicenseKey("ca1e728d9d4c2f636f067f89cc14862c");//"eccbc87e4b5ce2fe28308fd9f2a7baf3");
	//printf("hw: %s\n", hw.c_str());
	//dao.addHardwareToLicenseKey("c81e728d9d4c2f636f067f89cc14862c", "Hello");
	//system("pause");

	TSPServer* server = new TSPServer();
	server->startServer();
	return 0;
}