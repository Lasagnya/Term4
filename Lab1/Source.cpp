#include <Windows.h>
#include <process.h>
#include <iostream>
#include <ctime>
using namespace std;

volatile int summ = 0;

struct Array {
	int* array;
	int lenght;

	Array(int* arr, int n) {
		array = arr;
		lenght = n;
	}

	Array(Array* copy) {
		array = copy->array;
		lenght = copy->lenght;
	}
};

DWORD WINAPI Add(LPVOID temp)
{
	Array* Arr = new Array((Array*)temp);
	for (int i = 0; i < Arr->lenght; i++) {
		summ += Arr->array[i];
		Sleep(12);
	}
	summ /= Arr->lenght;
	return summ;
}

UINT WINAPI thread(LPVOID temp) {
	return 0;
}

int main() {
	int n = 10;//rand() % 10;
	int* array = new int[n];
	for (int i = 0; i < n; i++) {
		array[i] = rand() % 10;
		cout << array[i] << " ";
	}
	cout << '\n';
	Array *Arr = new Array(array, n);
	HANDLE hThread;
	DWORD IDThread;
	UINT IDThread2;
	hThread = CreateThread(NULL, 0, Add, (void*)Arr, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();
	SuspendThread(hThread);
	ResumeThread(hThread);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	hThread = (HANDLE)
		_beginthreadex(NULL, 0, thread, 0, 0, &IDThread2);
	cout << summ;
	return 0;
}