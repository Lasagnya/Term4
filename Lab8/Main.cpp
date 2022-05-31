#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <time.h>
#include <process.h>
#include <conio.h>
using namespace std;


class MonitorStack
{
public:
	char* mass;
	int size;
	int vol = -1;
	MonitorStack(int n) : size(n) {
		mass = new char[size];
		for (int i = 0; i < size; i++) {
			mass[i] = ' ';
		}
	}
	~MonitorStack() {
		delete[] mass;
	};
	bool isEmpty() {
		if (vol == -1) return true;
		return false;
	}
	bool isFull() {
		if (vol == size - 1) return true;
		return false;
	}
	void push(char x) {
		if (isFull())
			cout << "Stack is full!!!";
		vol++;
		mass[vol] = x;
	}
	char pop() {
		if (isEmpty())
			cout << "Stack is empty!!!";
		vol--;
		return mass[vol + 1];
	}
	MonitorStack& operator = (const MonitorStack& B)
	{
		if (this == &B)
			return *this;
		size = B.size;
		vol = B.vol;
		delete[] mass;
		mass = new char[size];
		for (int i = 0; i < size; i++)
		{
			mass[i] = B.mass[i];
		}
		return *this;
	}
};


MonitorStack S(1);

DWORD WINAPI threadConsumer(LPVOID mass)
{
	HANDLE SemPr, SemCons, hMutex1, SemLock;
	SemPr = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreProd");
	if (SemPr == NULL)
	{
		cout << "Open semaphore failed. Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}
	SemCons = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreCons");
	if (SemCons == NULL)
	{
		cout << "Open semaphore failed. Press any key to exit." << endl;
		cin.get();
		CloseHandle(SemPr);
		return GetLastError();
	}
	SemLock = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreLock");
	if (SemLock == NULL)
	{
		cout << "Open semaphore failed. Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}
	hMutex1 = OpenMutex(SYNCHRONIZE, FALSE, "Mutex1");
	if (hMutex1 == NULL)
	{
		cout << "Open mutex failed. Press any key to exit." << endl;
		cin.get();
		CloseHandle(SemCons);
		CloseHandle(SemPr);
		return GetLastError();
	}
	int* mass1 = (int*)mass;
	int numb = mass1[0];
	int count = mass1[1];
	for (int i = 0; i < count; i++) {
		WaitForSingleObject(SemCons, INFINITE);
		//WaitForSingleObject(hMutex1, INFINITE);
		WaitForSingleObject(SemLock, INFINITE);
		char prod = S.pop();
		cout << "Потребитель №" << numb << " потребил продукт " << prod << endl;
		//ReleaseMutex(hMutex1);
		ReleaseSemaphore(SemLock, 1, NULL);
		ReleaseSemaphore(SemPr, 1, NULL);
	}
	CloseHandle(hMutex1);
	CloseHandle(SemCons);
	CloseHandle(SemPr);
	CloseHandle(SemLock);
	return 0;
}

DWORD WINAPI threadProducer(LPVOID mass)
{
	HANDLE SemPr, SemCons, hMutex1, SemLock;
	SemPr = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreProd");
	if (SemPr == NULL)
	{
		cout << "Open semaphore failed. Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}
	SemCons = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreCons");
	if (SemCons == NULL)
	{
		cout << "Open semaphore failed. Press any key to exit." << endl;
		cin.get();
		CloseHandle(SemPr);
		return GetLastError();
	}
	SemLock = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreLock");
	if (SemLock == NULL)
	{
		cout << "Open semaphore failed. Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}
	hMutex1 = OpenMutex(SYNCHRONIZE, FALSE, "Mutex1");
	if (hMutex1 == NULL)
	{
		cout << "Open mutex failed. Press any key to exit." << endl;
		cin.get();
		CloseHandle(SemCons);
		CloseHandle(SemPr);
		return GetLastError();
	}
	srand(time(0));
	int* mass1 = (int*)mass;
	int numb = mass1[0];
	int count = mass1[1];
	for (int i = 0; i < count; i++) {
		WaitForSingleObject(SemPr, INFINITE);
		//WaitForSingleObject(hMutex1, INFINITE);
		WaitForSingleObject(SemLock, INFINITE);
		char prod = (char)rand();
		S.push(prod);
		cout << "Производитель №" << numb << " произвёл продукт " << prod << endl;
		//ReleaseMutex(hMutex1);
		ReleaseSemaphore(SemLock, 1, NULL);
		ReleaseSemaphore(SemCons, 1, NULL);
	}
	CloseHandle(hMutex1);
	CloseHandle(SemCons);
	CloseHandle(SemPr);
	CloseHandle(SemLock);
	return 0;
}


int main()
{
	setlocale(LC_ALL, "ru");
	HANDLE SemPr, SemCons, hMutex1, SemLock;
	srand(time(0));
	int size;
	cout << "Введите размер стека:" << endl;
	cin >> size;
	MonitorStack S1(size);
	S = S1;
	SemPr = CreateSemaphore(NULL, 0, size, "SemaphoreProd");
	if (SemPr == NULL)
	{
		cout << "Create semaphore failed. Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}
	SemCons = CreateSemaphore(NULL, 0, size, "SemaphoreCons");
	if (SemCons == NULL)
	{
		cout << "Create semaphore failed. Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}
	SemLock = CreateSemaphore(NULL, 1, 1, "SemaphoreLock");
	if (SemLock == NULL)
	{
		cout << "Create semaphore failed. Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}
	hMutex1 = CreateMutex(NULL, FALSE, "Mutex1");
	if (hMutex1 == NULL)
	{
		cout << "Create mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}
	int n_cons, n_prod, m_count;
	cout << "Введите количество потоков Producer:" << endl;
	cin >> n_prod;
	cout << "Введите количество потоков Consumer:" << endl;
	cin >> n_cons;
	DWORD	IDThreadConsumer;
	DWORD	IDThreadProducer;
	HANDLE* H = new HANDLE[n_prod + n_cons];
	int Summ_prod = 0, Summ_cons = 0;
	while (true) {
		for (int i = 0; i < n_prod; i++) {
			cout << "Введите количество произведенных продуктов для потока Producer " << i + 1 << ":\n";
			int count_prod;
			cin >> count_prod;
			Summ_prod += count_prod;
			int* mass = new int[2];
			mass[0] = i + 1;
			mass[1] = count_prod;
			H[i] = CreateThread(NULL, 0, threadProducer, (void*)mass, 0, &IDThreadProducer);
		}
		for (int i = 0; i < n_cons; i++) {
			cout << "Введите количество потребленных продуктов для потока Consumer " << i + 1 << ":\n";
			int count_cons;
			cin >> count_cons;
			Summ_cons += count_cons;
			int* mass = new int[2];
			mass[0] = i + 1;
			mass[1] = count_cons;
			H[i + n_prod] = CreateThread(NULL, 0, threadConsumer, (void*)mass, 0, &IDThreadConsumer);
		}
		if (Summ_cons > Summ_prod) {
			cout << "Продуктов недостаточно. Попробуйте ввести данные еще раз." << endl;
		}
		else if (Summ_prod > Summ_cons + size) {
			cout << "Слишком много продуктов. Попробуйте ещё раз." << endl;
		}
		else break;
	}
	cout << "Потоки начали работу." << endl;
	ReleaseSemaphore(SemPr, size, NULL);
	WaitForMultipleObjects(n_cons + n_prod, H, true, INFINITE);
	for (int i = 0; i < n_cons + n_prod; i++) {
		CloseHandle(H[i]);
	}
	CloseHandle(hMutex1);
	CloseHandle(SemCons);
	CloseHandle(SemPr);
	CloseHandle(SemLock);
	return 0;
}