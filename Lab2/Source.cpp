#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include <iostream>
#include <ctime>
using namespace std;

int main() {
	char* lpszCommandLine = new char[255];
	lpszCommandLine[0] = '\0';
	strcat(lpszCommandLine, "Lab2_Child ");
	int n = rand()%10;
	char* temp = new char[255];
	_itoa(n, temp, 10);
	strcat(lpszCommandLine, strcat(temp, " "));
	double* array = new double[n];
	for (int i = 0; i < n; i++) {
		array[i] = rand() % 10;
		sprintf(temp, "%f", array[i]);
		strcat(lpszCommandLine, strcat(temp, " "));
		cout << array[i] << " ";
	}
	cout << "\n";
	int K = rand() % n;
	_itoa(K, temp, 10);
	strcat(lpszCommandLine, temp);
	strcat(lpszCommandLine, ".exe");
	//char lpszCommandLine[] = "Lab2_Child 5 1.0 2.0 3.0 4.0 5.0 4.exe";
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	if (!CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
	{
		_cputs("The new process is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		return 0;
	}
	WaitForSingleObject(piApp.hProcess, INFINITE);
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	return 0;
}