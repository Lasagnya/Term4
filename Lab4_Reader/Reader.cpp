#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

HANDLE A, B, endReader;
HANDLE hSemaphore;

int main(int argc, char* argv[])
{
	A = CreateEvent(NULL, FALSE, FALSE, "A");
	if (A == NULL)
		return GetLastError();
	B = CreateEvent(NULL, FALSE, FALSE, "B");
	if (B == NULL)
		return GetLastError();
	endReader = CreateEvent(NULL, FALSE, FALSE, "endR");
	if (endReader == NULL)
		return GetLastError();
	hSemaphore = CreateSemaphore(NULL, 2, 2, "SemaphoreR");
	if (hSemaphore == NULL)
		return GetLastError();
	int n = atoi(argv[0]);
	int m_count = atoi(argv[1]);
	cout << "Reader #" << n << "\n";
	WaitForSingleObject(hSemaphore, INFINITE);
	HANDLE mass[] = { A, B };
	for (int i = 0; i < 2 * m_count; i++)
	{
		int ind = WaitForMultipleObjects(2, mass, FALSE, INFINITE) - WAIT_OBJECT_0;
		if (ind == 0)
		{
			cout << "Get message A from Writer\n";
			ResetEvent(A);
		}
		else if (ind == 1)
		{
			cout << "Get message B from Writer\n";
			ResetEvent(B);
		}
	}
	cout << "Завершение работы..." << endl;
	ReleaseSemaphore(hSemaphore, 1, NULL);
	CloseHandle(A);
	CloseHandle(B);
	WaitForSingleObject(endReader, INFINITE);
	return 0;
}