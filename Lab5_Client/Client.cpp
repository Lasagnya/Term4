#include <windows.h>
#include <conio.h>
int main(int argc, char* argv[])
{
	HANDLE hWritePipe, hReadPipe;
	HANDLE hEnableRead;
	//hEnableRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpszEnableRead);
	hWritePipe = (HANDLE)atoi(argv[1]);
	hReadPipe = (HANDLE)atoi(argv[2]);
	int n, N, M;
	DWORD dwBytesRead;
	if (!ReadFile(hReadPipe, &n, sizeof(n), &dwBytesRead, NULL))
	{
		_cputs("Read from the pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	ReadFile(hReadPipe, &N, sizeof(N), &dwBytesRead, NULL);
	ReadFile(hReadPipe, &M, sizeof(M), &dwBytesRead, NULL);
	DWORD dwBytesWritten;
	for (int i = N; i <= M; i++) {
		int a = rand();
		if (!WriteFile(hWritePipe, &a, sizeof(a), &dwBytesWritten, NULL))
		{
			_cputs("Write to file failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
	}
}