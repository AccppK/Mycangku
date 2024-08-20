#include <windows.h>  
#include <iostream>  

// 函数原型，用于执行系统命令  
bool ExecuteCommand(const std::wstring& command) {  
	STARTUPINFO si;  
	PROCESS_INFORMATION pi;  
	
	ZeroMemory(&si, sizeof(si));  
	si.cb = sizeof(si);  
	ZeroMemory(&pi, sizeof(pi));  
	
	// 将命令转换为适合CreateProcess的格式  
	std::wstring cmd = L"cmd.exe /c " + command;  
	LPCWSTR cmdStr = cmd.c_str();  
	
	// 创建进程  
	if (!CreateProcess(NULL,   // No module name (use command line)  
		const_cast<LPWSTR>(cmdStr), // Command line  
		NULL,           // Process handle not inheritable  
		NULL,           // Thread handle not inheritable  
		FALSE,          // Set handle inheritance to FALSE  
		0,              // No creation flags  
		NULL,           // Use parent's environment block  
		NULL,           // Use parent's starting directory   
		&si,            // Pointer to STARTUPINFO structure  
		&pi)           // Pointer to PROCESS_INFORMATION structure  
		) {  
		std::cerr << "CreateProcess failed (" << GetLastError() << ").\n";  
		return false;  
	}  
	
	// 等待进程结束  
	WaitForSingleObject(pi.hProcess, INFINITE);  
	
	// 关闭进程和线程句柄  
	CloseHandle(pi.hProcess);  
	CloseHandle(pi.hThread);  
	
	return true;  
}  

int main() {  
	// 注意：这里的"/m \\远程计算机名" 和 "/s /t 0" 是shutdown命令的参数  
	// 但由于安全限制，直接这样使用通常不会成功，除非你有适当的权限和配置  
	// 这里仅作为演示，你应该在实际应用中调整这些参数  
	std::wstring command = L"shutdown /m \\\\远程计算机名 /s /t 0 /f /c \"Forced shutdown by C++ program\"";  
	
	if (ExecuteCommand(command)) {  
		std::cout << "Command executed successfully.\n";  
	} else {  
		std::cout << "Failed to execute command.\n";  
	}  
	
	return 0;  
}
