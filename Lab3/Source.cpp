#include <Windows.h>
#include <process.h>
#include <iostream>
#include <ctime>
using namespace std;

CRITICAL_SECTION cs;
HANDLE event1, event2;


struct Array {
	char* array;
	int length;
	int k;
	int sum = 0;

	Array(char* arr, int n, int k) {
		array = arr;
		length = n;
		this->k = k;
	}

	Array(Array* copy) {
		array = copy->array;
		length = copy->length;
		this->k = copy->k;
	}
};


DWORD WINAPI summElement(LPVOID value)
{
	WaitForSingleObject(event2, INFINITE);
	EnterCriticalSection(&cs);
	Array* Arr = (Array*)value;
	for (int i = 0; i < Arr->k; i++) {
		Arr->sum += Arr->array[i];
	}
	Arr->sum /= Arr->k;
	LeaveCriticalSection(&cs);
	return 0;
}


DWORD WINAPI work(LPVOID value)
{
	Array* Arr = (Array*)value;
//	int s;
//	cin >> s;
	int count = 0;
	bool ind = false;
	for (int i = 0; i < Arr->length; i++) {
		for (int j = i + 1; j < Arr->length; j++) {
			if (Arr->array[j] == Arr->array[i]) {
				++count;
				char temp = Arr->array[Arr->length - count];
				Arr->array[Arr->length - count] = Arr->array[j];
				Arr->array[j] = temp;
				ind = true;
			}
			Sleep(50);
		}
		if (ind) {
			char temp = Arr->array[Arr->length - count];
			Arr->array[Arr->length - count] = Arr->array[i];
			Arr->array[i] = temp;
			ind = false;
		}
	}
	SetEvent(event1);
	return 0;
}


int main() {
	int n = 10;//rand() % 10;
	char* array = new char[n];
	for (int i = 0; i < n; i++) {
		array[i] = rand() % 10;
		cout << array[i] << " ";
	}
	cout << '\n';
	event1 = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (event1 == NULL)
		return GetLastError();
	event2 = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (event2 == NULL)
		return GetLastError();
	InitializeCriticalSection(&cs);
	int k;
	cin >> k;
	Array* Arr = new Array(array, n, k);
	HANDLE hThread;
	DWORD IDThread;
	HANDLE hThread2;
	DWORD IDThread2;
	hThread = CreateThread(NULL, 0, work, (void*)Arr, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();
	hThread2 = CreateThread(NULL, 0, summElement, (void*)Arr, 0, &IDThread2);
	if (hThread2 == NULL)
		return GetLastError();
	WaitForSingleObject(event1, INFINITE);
	for (int i = 0; i < Arr->length; i++) {
		cout << Arr->array[i] << " ";
	}
	cout << '\n';
	SetEvent(event2);
	Sleep(10);
	EnterCriticalSection(&cs);
	cout << Arr->sum << '\n';
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
	CloseHandle(hThread);
	CloseHandle(hThread2);
}