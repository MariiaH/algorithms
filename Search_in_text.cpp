#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

const int  ALPHABET = 256;

//Алгоритм прямого пошуку (наївний)
int Naive(const string T, const string W) {
	int n = T.size();
	int m = W.size();
	for (int i = 0; i <= n - m; i++) {
		bool b = true;
		for (int j = 0; j < m; j++) {
			if (T[i + j] != W[j]) {
				b = false;
				break;
			}
		}
		if (b) return i;
	}
	return -1;
}


//порівняння
bool compare(string::const_iterator a, const string::const_iterator b, int num) {
	for (auto i = a, j = b; i < a + num; i++, j++) {
		if (*i != *j) return false;
	}
	return true;
}

//Алгоритм Хорспула
int Horspool(const string T, const string W)
{
	unsigned int A[ALPHABET];
	//заповнюємо масив максимальним значенням стрибка
	for (int i = 0; i < ALPHABET; i++)
		A[i] = W.size();

	//обчислюємо для кожного символа зразка стрибок
	for (int i = 0; i < W.size() - 1; i++)
		A[(int)W[i]] = W.size() - i - 1;

	//пошук
	int i = 0;
	while (i <= T.size() - W.size()) {
		if (T[i + W.size() - 1] == W[W.size() - 1])
			if (compare(T.begin() + i, W.begin(), W.size() - 2) == true)
				return i;
		/* 
		якщо ці два символи відрізняються один від одного,
		стрибаємо, використовуючи значення таблиці переходу в
		індекс символу тексту 
		*/
		i += A[(int)T[i + W.size() - 1]];
	}

	return -1;
}

//Алгоритм Р.Боуер і Д.Мура(БМ - пошук)
int BM(const string T, const string W) {
	//зсув хорошого суфікса
	vector<int> good_shift(W.size() + 1, W.size());
	//зсув поганго символа
	vector<int> bad_char(ALPHABET, W.size());

	//масив Z, кожен елемент якого Z [i] дорівнює довжині найдовшого префікса суфікса підрядка,
	//починається з позиції i в рядку S, який одночасно є і префіксом всього рядка S	
	vector<int> z(W.size(), 0);
	for (int j = 1, maxZidx = 0, maxZ = 0; j < W.size(); ++j) {
		if (j <= maxZ) z[j] = min(maxZ - j + 1, z[j - maxZidx]);
		while (j + z[j] < W.size() && T[W.size() - 1 - z[j]] == T[W.size() - 1 - (j + z[j])]) z[j]++;
		if (j + z[j] - 1 > maxZ) {
			maxZidx = j;
			maxZ = j + z[j] - 1;
		}
	}
	for (int j = W.size() - 1; j > 0; j--) 
		good_shift[W.size() - z[j]] = j; 
	for (int j = 1, r = 0; j <= W.size() - 1; j++) 
		if (j + z[j] == W.size())
			for (; r <= j; r++)
				if (good_shift[r] == W.size()) good_shift[r] = j;

	for (int j = 0; j < W.size() - 1; j++) bad_char[(int)W[j]] = W.size() - j - 1;

	int i = 0;
	while (i <= W.size() - T.size()) {
		int j = 0;
		for (j = W.size() - 1; j >= 0 && W[j] == T[i + j]; --j);
		if (j < 0) {
			return i;
		}
		else i += max((good_shift[j + 1]), (int)(bad_char[(int)T[i + j]] - W.size() + j + 1));
	}
	return -1;
}

//Алгоритм Д. Кнута, Д. Моріса і В. Пратта (КМП-пошук)
int KMP(const string T, const string W) {
	vector<int> z;
	z.resize(W.size());
	z[0] = 0;
	for (int i = 1; i < z.size(); ++i) {
		int position = z[i - 1];
		while (position > 0 && W[position] != W[i])
			position = z[position - 1];
		z[i] = position + (W[position] == W[i] ? 1 : 0);
	}

	int position = 0;
	for (int i = 0; i < T.size(); ++i) {
		while (position > 0 && (position >= W.size() || W[position] != T[i]))
			position = z[position - 1];
		if (T[i] == W[position]) position++;
		if (position == W.size())
			return (i - position + 1);
	}
	return -1;
}

//Алгоритм Рабіна-Карпа (РК-пошук)
int RK(const string T, const string W) {
	// рахуємо всі степені p, де p - просте число
	const int p = 31;
	vector<long long> p_pow(max(W.length(), T.length()));
	p_pow[0] = 1;
	for (size_t i = 1; i < p_pow.size(); ++i)
		p_pow[i] = p_pow[i - 1] * p;

	// рахуємо хеші від усіх префіксів рядка "T"
	vector<long long> h(T.length());
	for (size_t i = 0; i < T.length(); ++i){
		h[i] = (T[i] - 'a' + 1) * p_pow[i];
		if (i)  h[i] += h[i - 1];
	}

	// рахуємо хеш від рядка "W"
	long long h_s = 0;
	for (size_t i = 0; i < W.length(); ++i)
		h_s += (W[i] - 'a' + 1) * p_pow[i];

	// перебираємо всі підряди "T" довжини |W| та порівнюємо їх
	for (size_t i = 0; i + W.length() - 1 < T.length(); ++i){
		long long cur_h = h[i + W.length() - 1];
		if (i)  cur_h -= h[i - 1];
		// приводимо хеші до одного степеню та порівнюємо
		if (cur_h == h_s * p_pow[i])
			return i;
	}

	return -1;
}


int main() {
	ifstream fin("text8.txt");
	string w;//зразок, що будемо шукати
	string t;//рядок в якому шукаємо
	getline(fin, w);
	getline(fin, t);
	auto time = chrono::high_resolution_clock::now();

	cout << "Naive algorithm:" << endl;
	time = chrono::high_resolution_clock::now();
	cout << "position: " << Naive(t, w) << endl;
	cout << "time: " << chrono::duration_cast<chrono::nanoseconds>
		(chrono::high_resolution_clock::now() - time).count() << "ns\n\n";

	cout << "Horsool algorithm:" << endl;
	time = chrono::high_resolution_clock::now();
	cout << "position: " << Horspool(t, w) << endl;
	cout << "time: " << chrono::duration_cast<chrono::nanoseconds>
		(chrono::high_resolution_clock::now() - time).count() << "ns\n\n";

	cout << "Boyer-Moore algorithm:" << endl;
	time = chrono::high_resolution_clock::now();
	cout << "position: " << BM(t, w) << endl;
	cout << "time: " << chrono::duration_cast<chrono::nanoseconds>
		(chrono::high_resolution_clock::now() - time).count() << "ns\n\n";

	cout << "Knuth-Morris-Pratt algorithm:" << endl;
	time = chrono::high_resolution_clock::now();
	cout << "position: " << KMP(t, w) << endl;
	cout << "time: " << chrono::duration_cast<chrono::nanoseconds>
		(chrono::high_resolution_clock::now() - time).count() << "ns\n\n";

	cout << "Rabin-Karp algorithm:" << endl;
	time = chrono::high_resolution_clock::now();
	cout << "position: " << RK(t, w) << endl;
	cout << "time: " << chrono::duration_cast<chrono::nanoseconds>
		(chrono::high_resolution_clock::now() - time).count() << "ns\n\n";

	system("pause");
	
	return 0;
}