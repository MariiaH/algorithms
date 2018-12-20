#include <iostream>
#include <vector>

using namespace std;

int d;

//батько і-го вузла
int Parent(int i){
	return (i - 1) / d;
}

//к-й син
int Son(int parent, int k){
	return parent * d + k;
}

//підтримка властивості незростання піраміди
void Max_Heapify(vector<int> &A, int i){
	int largest = i;
	for (int j = 1; j <= d; ++j){
		int child = Son(i, j);
		if (child < A.size() && A[child] > A[largest]){
			largest = child;
		}
	}
	if (largest != i){
		swap(A[i], A[largest]);
		Max_Heapify(A, largest);
	}
}

//повертає максимум
int Extract_Max(vector<int> &A){
	if (A.size() < 1){
		cout << "Error" << endl;
		return -1;
	}
	int Max = A[0];
	A[0] = A[A.size() - 1];
	A.pop_back();
	Max_Heapify(A, 0);
	return Max;
}

//збільшує ключ і-го вузла(ставить його рівним key)
void Increase_Key(vector<int> &A, int i, int key){
	if (key < A[i]){
		cout << "Key is less then current" << endl;
		return;
	}
	A[i] = key;
	while (i > 0 && A[Parent(i)] < A[i]){
		swap(A[i], A[Parent(i)]);
		i = Parent(i);
	}
}

//вставляє елемент key у піраміду
void Insert(vector<int> &A, int key){
	A.push_back(-1000);
	Increase_Key(A, (int)A.size() - 1, key);
}

int main()
{
	d = 3;
	vector<int> A;
	A = { 10, 29, 28, 27, 26, 25, 20, 19, 17, 15 };
	Max_Heapify(A, 0);
	for (int i = 0; i < A.size(); ++i)
		cout << A[i] << " ";
	cout << endl;
	Increase_Key(A, 6, 42);
	cout << "After Increase_Key for i = 6, key = 42" << endl;
	for (int i = 0; i < A.size(); ++i)
		cout << A[i] << " ";
	cout << endl;
	Insert(A, 31);
	cout << "After Insert for key = 31" << endl;
	for (int i = 0; i < A.size(); ++i)
		cout << A[i] << " ";

	cout << endl;
	int m = Extract_Max(A);
	cout << "max = " << m << endl;
	cout << "After Extract_Max" << endl;
	for (int i = 0; i < A.size(); ++i)
		cout << A[i] << " ";
	cout << endl;

	system("pause");
	return 0;
}
