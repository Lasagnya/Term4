#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	int n = atoi(argv[1]);
	int k = atoi(argv[2 + n]);
	double* array = new double[n];
	for (int i = 0; i < n; i++) {
		array[i] = atof(argv[i + 2]);
		cout << array[i] << " ";
	}
	cout << "\n";
	double summ = 0;
	for (int i = 0; i < k; i++) {
		summ += array[i];
	}
	cout << "Summ " << summ;
	cin >> n;
	return 0;
}
