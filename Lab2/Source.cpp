#include <Windows.h>
#include <process.h>
#include <iostream>
#include <ctime>
using namespace std;

int main() {
	int n = 10;
	int* array = new int[n];
	for (int i = 0; i < n; i++) {
		array[i] = rand() % 10;
		cout << array[i] << " ";
	}
}