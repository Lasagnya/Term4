#include <Windows.h>
#include <process.h>
#include <iostream>
#include <ctime>
using namespace std;

struct Array {
	char* array;
	int length;
	int k;

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
		}
		if (ind) {
			char temp = Arr->array[Arr->length - count];
			Arr->array[Arr->length - count] = Arr->array[i];
			Arr->array[i] = temp;
			ind = false;
		}
	}
}

int main() {
	int n = 10;//rand() % 10;
	char* array = new char[n];
	for (int i = 0; i < n; i++) {
		array[i] = rand() % 10;
		cout << array[i] << " ";
	}
	cout << '\n';
	CRITICAL_SECTION cs;
	HANDLE event1, event2;
	int k;
	cin >> k;
	Array* Arr = new Array(array, n, k);
}