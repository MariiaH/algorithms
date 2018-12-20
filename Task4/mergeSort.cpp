#include<iostream>
#include<fstream>
using namespace std;

const int SIZE_INT = sizeof(int);//довжина (в байтах) типу int

//злиття двох допоміжних файлів
void merge(const char*FileName, int l, int m, int r) {
	
	// відкриваємо файл з числами у бінарному вигляді
	fstream file(FileName, fstream::in | fstream::out | fstream::binary);

	//встановлення вказівника на l елемент файлу
	file.seekg(SIZE_INT*l, ios::beg);

	fstream auxiliary1("aux1.txt", fstream::out | fstream::binary);
	int k1 = m - l + 1;//кількість елементів у 1 файлі
	//запис елементів в 1 файл
	for (int i = 0; i < k1; i++) {
		int date;
		file.read((char*)&date, SIZE_INT);
		auxiliary1.write((char*)&date, SIZE_INT);
	}
	auxiliary1.close();

	fstream auxiliary2("aux2.txt", fstream::out | fstream::binary);
	int k2 = r - m;//кількість елементів у 2 файлі
	//запис елементів в 2 файл
	for (int i = 0; i < k2; i++) {
		int date;
		file.read((char*)&date, SIZE_INT);
		auxiliary2.write((char*)&date, SIZE_INT);
	}
	auxiliary2.close();

	//відкриття допоміжних бінарних файлів у режимі читання
	auxiliary1.open("aux1.txt", fstream::in | fstream::binary);
	auxiliary2.open("aux2.txt", fstream::in | fstream::binary);

	//встановлення вказівника на позицію l
	file.seekg(SIZE_INT*l, ios::beg);

	int i1 = 0;
	int i2 = 0;

	//злиття, поки якийсь файл не стане пустим
	while (i1 < k1 && i2 < k2) {
		int date1, date2;
		//зчитуємо по елементу з кожного допоміжного файлу
		auxiliary1.read((char*)&date1, SIZE_INT);
		auxiliary2.read((char*)&date2, SIZE_INT);
		//порівнюємо їх
		if (date1 <= date2) {
			file.write((char*)&date1, SIZE_INT);//запис у головний файл
			i1++;
			auxiliary2.seekg(SIZE_INT*(-1), ios::cur);
		}
		else {
			file.write((char*)&date2, SIZE_INT);//запис у головний файл
			i2++;
			auxiliary1.seekg(SIZE_INT*(-1), ios::cur);
		}
	}
	//допис залишку 1 файлу
	while (i1 < k1) {
		int date1;
		auxiliary1.read((char*)&date1, SIZE_INT);//зчитуємо черговий елемент з файлу 1
		file.write((char*)&date1, SIZE_INT);//записо у головний файл
		i1++;
	}
	//допис залишку 2 файлу
	while (i2 < k2) {
		int date2;
		auxiliary2.read((char*)&date2, SIZE_INT);//зчитуємо черговий елемент з файлу 2
		file.write((char*)&date2, SIZE_INT);//запис у головний файл
		i2++;
	}

	file.close();

	auxiliary1.close();
	auxiliary2.close();

	//видалення допоміжних файлів
	remove("aux1.txt");
	remove("aux2.txt");
}

//рекурсивне розбиття файлу 
void mergeSort(const char* FileName, int l, int r) {
	if (l < r) {
		int m = l + (r - l) / 2;//середина
		mergeSort(FileName, l, m);//для першої половини
		mergeSort(FileName, m + 1, r);//для другої половини
		merge(FileName, l, m, r);//злиття двох половинок
	}
}

int main() {
	//створення бінарного файлу для запису (доступ до елементів за константний час, завдяки роботі з вказівниками)
	fstream fileB("Bin.txt", fstream::out | fstream::binary | ios_base::trunc);

	// відкриваємо вхідний файл з числами у текстовому вигляді для читання
	ifstream fin("file1.txt");

	cout << "Elements of a non-sorted file: " << endl;
	int date;
	while (fin >> date) {
		cout << date << " ";
		//запис елементів у бінарний файл
		fileB.write((char*)&date, sizeof(date));
	}
	cout << endl;
	fileB.close();
	fin.close();

	fileB.open("Bin.txt", fstream::in | fstream::binary);

	//кількість елементів у файлі
	int kol = fileB.seekg(0, ios::end).tellg() / SIZE_INT;

	fileB.close();

	//сортування
	mergeSort("Bin.txt", 0, kol - 1);

	fileB.open("Bin.txt", fstream::in | fstream::binary);

	//створюємо текстовий файл для запису відсортованих чисел
	ofstream fout("file2.txt", ios_base::trunc);

	cout << "Elements of the sorted file: " << endl;
	//виводимо елементи після сортування (на екран і у file2.txt)
	while (fileB.read((char*)&date, SIZE_INT)) {
		fout << date << " ";
		cout << date << " ";
	}
	cout << endl;
	fout.close();
	fileB.close();

	//видалення допоміжного бінарний файлу
	remove("Bin.txt");

	system("pause");

	return 0;
}