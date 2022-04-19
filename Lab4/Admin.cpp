#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include <iostream>
using namespace std;

int main() {
	char* lpszCommandLine = new char[255];
	HANDLE A, B, endWriter, endReader;
//	HANDLE hMutex;
	HANDLE hSemaphore;
	A = CreateEvent(NULL, FALSE, FALSE, "A");
	if (A == NULL)
		return GetLastError();
	B = CreateEvent(NULL, FALSE, FALSE, "B");
	if (B == NULL)
		return GetLastError();
	endWriter = CreateEvent(NULL, FALSE, FALSE, "endW");
	if (endWriter == NULL)
		return GetLastError();
	endReader = CreateEvent(NULL, FALSE, FALSE, "endR");
	if (endReader == NULL)
		return GetLastError();
//	hMutex = CreateMutex(NULL, FALSE, "Mutex");
	hSemaphore = CreateSemaphore(NULL, 0, 10, NULL);
	char lpszAppNameR[] = "Reader.exe";
	char lpszAppNameW[] = "Writer.exe";
	int n, m_count;
	cout << "Number of process\n";
	cin >> n;
	cout << "Number of messages\n" << endl;
	cin >> m_count;
	HANDLE* Readers = new HANDLE[n];
	HANDLE* Writers = new HANDLE[n];
	PROCESS_INFORMATION piApp;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	for (int i = 0; i < n; i++) {
		char* ch = new char[1];
		char* lpCommLine = new char[1];
		lpCommLine = strcpy(lpCommLine, "");
		ch = _itoa((i + 1), ch, 10);
		lpCommLine = strcat(lpCommLine, ch);
		ch = _itoa(m_count, ch, 10);
		lpCommLine = strcat(lpCommLine, " ");
		lpCommLine = strcat(lpCommLine, ch);
		if (!CreateProcess(lpszAppNameW, lpCommLine, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
		{
			_cputs("The new process is not created.\n");
			_cputs("Check a name of the process.\n");
			_cputs("Press any key to finish.\n");
			return 0;
		}
		Writers[i] = piApp.hProcess;
		if (!CreateProcess(lpszAppNameR, lpCommLine, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
		{
			_cputs("The new process is not created.\n");
			_cputs("Check a name of the process.\n");
			_cputs("Press any key to finish.\n");
			return 0;
		}
		Readers[i] = piApp.hProcess;
	}
}