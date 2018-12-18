#include <iostream>
#include <vector>

using namespace std;

//час роботи O(n), алгоритм стійкий
void sort1(vector<int> &A){
	vector<int> B(A.size());
	int x = -1; 
	//підраховує кількість нулів в масиві
	for (int i = 0; i < A.size(); ++i)
		if (A[i] == 0) x++;
	int y = A.size() - 1;
	for (int i = A.size() - 1; i >= 0; --i){
		if (A[i] == 0){
			B[x] = A[i];
			x--;
		}
		else{
			B[y] = A[i];
			y--;
		}
	}
	A = B;
}

//Час роботи O(n), сортування проводиться на місці
void sort2(vector<int> &A){
	int c = 0;
	for (int i = 0; i < A.size(); ++i){
		if (A[i] == 0){
			swap(A[i], A[c]);
			c++;
		}
	}
}

//алгоритм стійкий, сортування на місці
void sort3(vector<int> &A)
{
	int i = -1; //ндекс останнього нуля у вже відсортованій послідовності
	for (int j = 0; j < A.size(); ++j){
		if (A[j] == 0){
			for (int k = j; (k - i - 1) > 0; k--){
				swap(A[k], A[k - 1]);
			}
			i++;
		}
	}
}

void CoutVector(vector<int> v){
	for (int i = 0; i < v.size(); ++i)
		cout << v[i] << " ";
	cout << endl;
}

int main()
{
	vector<int> A = { 1,0,1,0,1,0,1 };
	vector<int> B = { 1,1,1,1,1,1,0,0,0,0,0,0,1,1};
	vector<int> C = { 0,0,0,0,1,1,0,0,1,0,1 };
	cout << "Sort1: ";
	sort1(A);
	CoutVector(A);
	cout << "Sort2: ";
	sort2(B);
	CoutVector(B);
	cout << "Sort3: ";
	sort1(C);
	CoutVector(C);
	cin.get();
	return 0;
}
