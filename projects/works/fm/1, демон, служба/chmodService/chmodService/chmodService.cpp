#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "acl.h"
#include <iostream>
#include <locale>

#include <winsvc.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

using namespace std;

typedef struct ARGS {
	DWORD argc;
	LPTSTR *argv;
};

SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
SERVICE_STATUS        g_ServiceStatus = { 0 };
HANDLE                g_ServiceStopEvent = INVALID_HANDLE_VALUE;

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
void WINAPI ServiceCtrlHandler(DWORD);
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);

#define SERVICE_NAME  _T("My Chmod Service")

int _tmain(int argc, TCHAR *argv[])
{
	SERVICE_TABLE_ENTRY ServiceTable[] =
	{
		{ SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};

	if (StartServiceCtrlDispatcher(ServiceTable) == FALSE)
	{
		return GetLastError();
	}

	return 0;
}

DWORD WINAPI ServiceWorkerThread(LPVOID lpParam)
{
	//Sleep(20000);
	ARGS *pArgs = (ARGS*)lpParam;
	acl aclController = acl();
	setlocale(LC_ALL, "rus");
	FILE * f = fopen("E:\\всё\\5 курс\\Проектирование ОС и компонентов\\отчеты\\лаб 1, демон, служба\\results.txt", "a+");
	/*fprintf(f, "The main thread works!\n");
	for (int i = 0; i < pArgs->argc; i++)
	{
		string option = pArgs->argv[i];
		fprintf(f, "Option %s\n", option.c_str());
	}*/
	aclController.start(pArgs->argc, pArgs->argv, f);
	fclose(f);
	Sleep(3000);
	return ERROR_SUCCESS;
}

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
	DWORD Status = E_FAIL;

	// Register our service control handler with the SCM
	g_StatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);

	if (g_StatusHandle == NULL)
	{
		return;
	}

	// Tell the service controller we are starting
	ZeroMemory(&g_ServiceStatus, sizeof(g_ServiceStatus));
	g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_ServiceStatus.dwControlsAccepted = 0;
	g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwServiceSpecificExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;

	if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
	{
		OutputDebugString(_T(
			"My Chmod Service: ServiceMain: SetServiceStatus returned error"));
	}

	// Perform tasks necessary to start the service here

	// Create a service stop event to wait on later
	g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (g_ServiceStopEvent == NULL)
	{
		// Error creating event
		// Tell service controller we are stopped and exit
		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		g_ServiceStatus.dwWin32ExitCode = GetLastError();
		g_ServiceStatus.dwCheckPoint = 1;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
		{
			OutputDebugString(_T(
				"My Chmod Service: ServiceMain: SetServiceStatus returned error"));
		}
		return;
	}

	// Tell the service controller we are started
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;

	if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
	{
		OutputDebugString(_T(
			"My Chmod Service: ServiceMain: SetServiceStatus returned error"));
	}

	// Start a thread that will perform the main task of the service
	ARGS args = {argc, argv};
	HANDLE hThread = CreateThread(NULL, 0, ServiceWorkerThread, &args, 0, NULL);

	// Wait until our worker thread exits signaling that the service needs to stop
	WaitForSingleObject(hThread, INFINITE);


	// Perform any cleanup tasks

	CloseHandle(g_ServiceStopEvent);

	// Tell the service controller we are stopped
	g_ServiceStatus.dwControlsAccepted = 0;
	g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 3;

	if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
	{
		OutputDebugString(_T(
			"My Chmod Service: ServiceMain: SetServiceStatus returned error"));
	}

	return;
}

void WINAPI ServiceCtrlHandler(DWORD CtrlCode)
{
	switch (CtrlCode)
	{
	case SERVICE_CONTROL_STOP:

		if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
			break;

		//Perform tasks necessary to stop the service here

		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		g_ServiceStatus.dwWin32ExitCode = 0;
		g_ServiceStatus.dwCheckPoint = 4;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
		{
			OutputDebugString(_T(
				"My Chmod Service: ServiceCtrlHandler: SetServiceStatus returned error"));
		}

		// This will signal the worker thread to start shutting down
		SetEvent(g_ServiceStopEvent);

		break;

	default:
		break;
	}
}

