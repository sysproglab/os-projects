#include <tchar.h>
#include <Windows.h>

#include <string>
#include <fstream>

SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle;
LPTSTR serviceName = TEXT("MyServiceCPP");
LPTSTR servicePath;

int addLogMessage(std::string str) {
	std::ofstream out;
	out.open("c:\\log.txt", std::ofstream::out | std::ofstream::app);
	out << str << std::endl;
	if (!out) return -1;
	out.close();
	return 0;
}


void ControlHandler(DWORD request) {
	switch (request)
	{
	case SERVICE_CONTROL_STOP:
		addLogMessage("Stopped.");

		serviceStatus.dwWin32ExitCode = 0;
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		return;

	case SERVICE_CONTROL_SHUTDOWN:
		addLogMessage("Shutdown.");

		serviceStatus.dwWin32ExitCode = 0;
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		return;

	default:
		break;
	}

	SetServiceStatus(serviceStatusHandle, &serviceStatus);

	return;
}

void ServiceMain(int argc, char** argv) {
	int error;
	int i = 0;

	serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwCurrentState = SERVICE_START_PENDING;
	serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	serviceStatus.dwWin32ExitCode = 0;
	serviceStatus.dwServiceSpecificExitCode = 0;
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;

	serviceStatusHandle = RegisterServiceCtrlHandler(serviceName, (LPHANDLER_FUNCTION)ControlHandler);
	if (serviceStatusHandle == (SERVICE_STATUS_HANDLE)0) {
		return;
	}

	error = 0;
	if (error) {
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		serviceStatus.dwWin32ExitCode = -1;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		return;
	}

	serviceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);

	while (serviceStatus.dwCurrentState == SERVICE_RUNNING)
	{
		char buffer[255];
		sprintf_s(buffer, "%u", i);
		int result = addLogMessage(buffer);
		if (result) {
			serviceStatus.dwCurrentState = SERVICE_STOPPED;
			serviceStatus.dwWin32ExitCode = -1;
			SetServiceStatus(serviceStatusHandle, &serviceStatus);
			return;
		}
		i++;
		//—вой код
	}
	
	return;
}
int InstallService() {
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!hSCManager) {
		addLogMessage("Error: Can't open Service Control Manager");
		return -1;
	}

	SC_HANDLE hService = CreateService(
		hSCManager,
		serviceName,
		serviceName,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		servicePath,
		NULL, NULL, NULL, NULL, NULL
	);

	if (!hService) {
		int err = GetLastError();
		switch (err) {
		case ERROR_ACCESS_DENIED:
			addLogMessage("Error: ERROR_ACCESS_DENIED");
			break;
		case ERROR_CIRCULAR_DEPENDENCY:
			addLogMessage("Error: ERROR_CIRCULAR_DEPENDENCY");
			break;
		case ERROR_DUPLICATE_SERVICE_NAME:
			addLogMessage("Error: ERROR_DUPLICATE_SERVICE_NAME");
			break;
		case ERROR_INVALID_HANDLE:
			addLogMessage("Error: ERROR_INVALID_HANDLE");
			break;
		case ERROR_INVALID_NAME:
			addLogMessage("Error: ERROR_INVALID_NAME");
			break;
		case ERROR_INVALID_PARAMETER:
			addLogMessage("Error: ERROR_INVALID_PARAMETER");
			break;
		case ERROR_INVALID_SERVICE_ACCOUNT:
			addLogMessage("Error: ERROR_INVALID_SERVICE_ACCOUNT");
			break;
		case ERROR_SERVICE_EXISTS:
			addLogMessage("Error: ERROR_SERVICE_EXISTS");
			break;
		default:
			addLogMessage("Error: Undefined");
		}
		CloseServiceHandle(hSCManager);
		return -1;
	}
	CloseServiceHandle(hService);

	CloseServiceHandle(hSCManager);
	addLogMessage("Success install service!");
	return 0;
}

int RemoveService() {
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCManager) {
		addLogMessage("Error: Can't open Service Control Manager");
		return -1;
	}
	SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_STOP | DELETE);
	if (!hService) {
		addLogMessage("Error: Can't remove service");
		CloseServiceHandle(hSCManager);
		return -1;
	}

	DeleteService(hService);
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	addLogMessage("Success remove service!");
	return 0;
}

int StartService() {
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_START);
	if (!StartService(hService, 0, NULL)) {
		CloseServiceHandle(hSCManager);
		addLogMessage("Error: Can't start service");
		return -1;
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	return 0;
}

int _tmain(int argc, _TCHAR* argv[]) {

	servicePath = LPTSTR(argv[0]);

	if (argc - 1 == 0) {
		SERVICE_TABLE_ENTRY ServiceTable[1];
		ServiceTable[0].lpServiceName = serviceName;
		ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

		if (!StartServiceCtrlDispatcher(ServiceTable)) {
			addLogMessage("Error: StartServiceCtrlDispatcher");
		}
	}
	else if (wcscmp(argv[argc - 1], _T("install")) == 0) {
		InstallService();
	}
	else if (wcscmp(argv[argc - 1], _T("remove")) == 0) {
		RemoveService();
	}
	else if (wcscmp(argv[argc - 1], _T("start")) == 0) {
		StartService();
	}
}
/*
int _tmain(int argc, _TCHAR* argv[]) {
	SERVICE_TABLE_ENTRY ServiceTable[1];
	ServiceTable[0].lpServiceName = serviceName;
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

	StartServiceCtrlDispatcher(ServiceTable);
}*/