#include<iostream>
#include<vector>
#include <string>

using namespace std;

const int d = 50;
const int q = 101;

//вивід матриці на екран
void Cout(vector<string> v) {
	int size = v[0].size();
	for (int i = 0; i < size; i++) {
		for (int j = 0; j <= size; j++) {
			cout << v[i][j] << " ";
		}  cout << endl;
	}
}

//порівняння матриць починаючи від заданих індексів
bool compare_matrix(vector<string> matrix, vector<string> model, int row, int col, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (matrix[row + i][col + j] != model[i][j]) return false;
		}
	}
	return true;
}

//порівняння рядків починаючи від заданих індексів
bool compare_rows(vector<int> s1, vector<int> s2, int ind) {
	int size = s2.size();
	for (int i = 0; i < size; i++)
		if (s1[i + ind] != s2[i]) return false;
	return true;

}

//метод Рабіна-Карпа
int RK(vector<string> matrix, vector<string> model, vector<int> hashMatrix, vector<int> hashModel, int row) {
	int N = matrix[0].size();
	int M = model[0].size();

	int h = 1;
	for (int i = 0; i < M - 1; i++)	h = (h * d) % q;

	int mod = 0;
	int mat = 0;
	for (int i = 0; i < M; i++) {
		mod = (d * mod + hashModel[i]) % q;
		mat = (d * mat + hashMatrix[i]) % q;
	}

	for (int i = 0; i <= N - M; i++) {
		if (mod == mat)
			if (compare_rows(hashMatrix, hashModel, i) && compare_matrix(matrix, model, row, i, M))	return i;
		if (i < N - M) {
			mat = (d * (mat - hashMatrix[i] * h) + hashMatrix[i + M]) % q;
			if (mat < 0)
				mat += q;
		}
	}
	return -1;
}

//розрахунок хешів
void culculationHashes(vector<string> v, vector<int> &hashes, int width, int height) {
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			hashes[j] = (d * hashes[j] + v[i][j]) % q;
		}
	}
}

//перерахунок хешів
void reculculationHashes(vector<string> v, vector<int> &hashes, int width, int height, int row) {
	int h = 1;
	for (int i = 0; i < height - 1; i++)
		h = (h * d) % q;
	for (int j = 0; j < width; j++) {
		hashes[j] = (d * (hashes[j] - v[row][j] * h) + v[row + height][j]) % q;
		if (hashes[j] < 0)
			hashes[j] = (hashes[j] + q);
	}
}

//узагальнення для матриць
void generalizationRK(vector<string> matrix, vector<string> model) {

	int N = matrix[0].size();
	int M = model[0].size();

	int h = 1;
	for (int i = 0; i < M - 1; i++)
		h = (h * d) % q;

	vector<int> hashMatrix(N, 0);
	vector<int> hashModel(M, 0);

	culculationHashes(model, hashModel, M, M);
	culculationHashes(matrix, hashMatrix, N, M);

	int column = 0;
	for (int j = 0; j <= N - M; j++) {
		column = RK(matrix, model, hashMatrix, hashModel, j);
		if (column != -1) {
			cout << "Find at row = " << j + 1 << ", column = " << column + 1 << endl;
			return;
		}
		else if (j != N - M) {
			reculculationHashes(matrix, hashMatrix, N, M, j);
		}
	}

	cout << "Model wasn't found" << endl;

}


int main() {
	vector<string> matrix = {
		"hhvvn",
		"dfsfd",
		"cmnxi",
		"ncjdk",
		"cnjdj"
	};

	vector<string> model = {
		"fsf",
		"mnx",
		"cjd"
	};

	cout << "model:" << endl;
	Cout(model);
	cout << endl;
	cout << "matrix:" << endl;
	Cout(matrix);
	cout << endl;

	generalizationRK(matrix, model);

	system("pause");
	return 0;
}
