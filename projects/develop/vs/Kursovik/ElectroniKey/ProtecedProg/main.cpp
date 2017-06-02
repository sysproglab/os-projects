#include <Windows.h>

int main(void)
{
	res = CreateProcess(
		NULL,                //  pointer to name of executable module  
		commandLine,         //  pointer to command line string  
		NULL,                //  pointer to process security attributes  
		NULL,                //  pointer to thread security attributes  
		TRUE,                //  handle inheritance flag  
		0,                   //  creation flags  
		NULL,                //  pointer to new environment block  
		NULL,                //  pointer to current directory name  
		&StartupInfo,        //  pointer to STARTUPINFO  
		&ProcessInfo         //  pointer to PROCESS_INFORMATION  
		);

	if (!res)
		//  process creation failed!
	{
		showError(commandLine);
		retVal = 1;
	}
	else if (waitForCompletion)
	{
		res = WaitForSingleObject(
			ProcessInfo.hProcess,
			INFINITE      // time-out interval in milliseconds  
			);
		GetExitCodeProcess(ProcessInfo.hProcess, &exitCode);

		retVal = (int)exitCode;
	}

	return 0;
}