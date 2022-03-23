#include <Windows.h>
#include <process.h>
#include <iostream>
#include <ctime>
#include <algorithm>
using namespace std;

CRITICAL_SECTION cs;
HANDLE event1, event2;


struct Array {
	int* array;
	int length;
	int k;
	int sum = 0;

	Array(int* arr, int n, int k) {
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

bool comp(int i, int j) { return (i < j); }

DWORD WINAPI work(LPVOID value)
{
	Array* Arr = (Array*)value;
//	int s;
//	cin >> s;
	/*int count = 0;
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
			Sleep(12);
		}
		if (ind) {
			char temp = Arr->array[Arr->length - count];
			Arr->array[Arr->length - count] = Arr->array[i];
			Arr->array[i] = temp;
			ind = false;
		}
	}*/

	sort(Arr->array, Arr->array + Arr->length, comp);
	int* array = new int[Arr->length];
	int t = 0;
	for (int i = 0; i < Arr->length; i++) {
		if (Arr->array[i] != Arr->array[i + 1] && Arr->array[i] != Arr->array[i - 1]) {
			array[t] = Arr->array[i];
			++t;
		}
	}
	for (int i = 0; i < Arr->length; i++) {
		int count = 0;
		if (Arr->array[i] == Arr->array[i + 1])
			for (int j = i; Arr->array[j] == Arr->array[i] && j < Arr->length - 1; j++, t++, count++)
				array[t] = Arr->array[j];
		i += count;
	}
	Arr->array = array;
	SetEvent(event1);
	return 0;
}


int main() {
	int n = 10;//rand() % 10;
	cout << "Starting massiv\n";
	int* array = new int[n];
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
	int k = 5;
	cout << "Input k\n";
//	cin >> k;
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
	cout << "New massiv\n";
	WaitForSingleObject(event1, INFINITE);
	for (int i = 0; i < Arr->length; i++) {
		cout << Arr->array[i] << " ";
	}
	cout << '\n';
	SetEvent(event2);
	Sleep(10);
	EnterCriticalSection(&cs);
	cout << "Summ\n";
	cout << Arr->sum << '\n';
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
	CloseHandle(hThread);
	CloseHandle(hThread2);
}