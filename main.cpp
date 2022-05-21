#include <Windows.h>
#include <iostream>
#include "memory.hpp"
#include <string>

using namespace std;

char namedll[32];

Injector inj;

DWORD pid;

bool DoesFileExist(const char* name) {
	if (FILE* file = fopen(name, "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

void bypass()
{
	// Restore original NtOpenFile from external process
	LPVOID ntOpenFile = GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");
	if (ntOpenFile) {
		char originalBytes[5];
		memcpy(originalBytes, ntOpenFile, 5);
		WriteProcessMemory(inj.process, ntOpenFile, originalBytes, 5, NULL);
	} 
	else
	{
		cout << "Unable to bypass.\n";
	}
}

int main()
{
	SetConsoleTitle("t.me/lpsopg");

	cout << "the injector is from https://t.me/lpsopg\n" << endl;

	inj.hwndproc = FindWindowA(0, "Counter-Strike: Global Offensive - Direct3D 9");

	GetWindowThreadProcessId(inj.hwndproc, &pid);
	inj.process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	inj.clientDLL = inj.GetModule(pid, "client.dll");

	cout << "write your dll:" << endl;

	scanf_s("%s", namedll);
	

	if (DoesFileExist(namedll)) {
		bypass();

			if (inj.inject(pid, namedll)) {
				cout << "No errors check your csgo!\nJoin: t.me/lpsopg" << endl;
					Sleep(5000);
					exit(0);
			}
			else
			{
				cout << "Injection failed\n\n" << endl;
					system("pause");
			}

	}
	else
	{
		cout << "Cannot found: " << namedll <<"\n";
		system("pause");
	}

	return 0;
}
