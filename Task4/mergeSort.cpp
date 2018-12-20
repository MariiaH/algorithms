#include<iostream>
#include<fstream>
using namespace std;

const int SIZE_INT = sizeof(int);//������� (� ������) ���� int

//������ ���� ��������� �����
void merge(const char*FileName, int l, int m, int r) {
	
	// ��������� ���� � ������� � �������� ������
	fstream file(FileName, fstream::in | fstream::out | fstream::binary);

	//������������ ��������� �� l ������� �����
	file.seekg(SIZE_INT*l, ios::beg);

	fstream auxiliary1("aux1.txt", fstream::out | fstream::binary);
	int k1 = m - l + 1;//������� �������� � 1 ����
	//����� �������� � 1 ����
	for (int i = 0; i < k1; i++) {
		int date;
		file.read((char*)&date, SIZE_INT);
		auxiliary1.write((char*)&date, SIZE_INT);
	}
	auxiliary1.close();

	fstream auxiliary2("aux2.txt", fstream::out | fstream::binary);
	int k2 = r - m;//������� �������� � 2 ����
	//����� �������� � 2 ����
	for (int i = 0; i < k2; i++) {
		int date;
		file.read((char*)&date, SIZE_INT);
		auxiliary2.write((char*)&date, SIZE_INT);
	}
	auxiliary2.close();

	//�������� ��������� ������� ����� � ����� �������
	auxiliary1.open("aux1.txt", fstream::in | fstream::binary);
	auxiliary2.open("aux2.txt", fstream::in | fstream::binary);

	//������������ ��������� �� ������� l
	file.seekg(SIZE_INT*l, ios::beg);

	int i1 = 0;
	int i2 = 0;

	//������, ���� ������ ���� �� ����� ������
	while (i1 < k1 && i2 < k2) {
		int date1, date2;
		//������� �� �������� � ������� ���������� �����
		auxiliary1.read((char*)&date1, SIZE_INT);
		auxiliary2.read((char*)&date2, SIZE_INT);
		//��������� ��
		if (date1 <= date2) {
			file.write((char*)&date1, SIZE_INT);//����� � �������� ����
			i1++;
			auxiliary2.seekg(SIZE_INT*(-1), ios::cur);
		}
		else {
			file.write((char*)&date2, SIZE_INT);//����� � �������� ����
			i2++;
			auxiliary1.seekg(SIZE_INT*(-1), ios::cur);
		}
	}
	//����� ������� 1 �����
	while (i1 < k1) {
		int date1;
		auxiliary1.read((char*)&date1, SIZE_INT);//������� �������� ������� � ����� 1
		file.write((char*)&date1, SIZE_INT);//������ � �������� ����
		i1++;
	}
	//����� ������� 2 �����
	while (i2 < k2) {
		int date2;
		auxiliary2.read((char*)&date2, SIZE_INT);//������� �������� ������� � ����� 2
		file.write((char*)&date2, SIZE_INT);//����� � �������� ����
		i2++;
	}

	file.close();

	auxiliary1.close();
	auxiliary2.close();

	//��������� ��������� �����
	remove("aux1.txt");
	remove("aux2.txt");
}

//���������� �������� ����� 
void mergeSort(const char* FileName, int l, int r) {
	if (l < r) {
		int m = l + (r - l) / 2;//��������
		mergeSort(FileName, l, m);//��� ����� ��������
		mergeSort(FileName, m + 1, r);//��� ����� ��������
		merge(FileName, l, m, r);//������ ���� ���������
	}
}

int main() {
	//��������� �������� ����� ��� ������ (������ �� �������� �� ����������� ���, ������� ����� � �����������)
	fstream fileB("Bin.txt", fstream::out | fstream::binary | ios_base::trunc);

	// ��������� ������� ���� � ������� � ���������� ������ ��� �������
	ifstream fin("file1.txt");

	cout << "Elements of a non-sorted file: " << endl;
	int date;
	while (fin >> date) {
		cout << date << " ";
		//����� �������� � ������� ����
		fileB.write((char*)&date, sizeof(date));
	}
	cout << endl;
	fileB.close();
	fin.close();

	fileB.open("Bin.txt", fstream::in | fstream::binary);

	//������� �������� � ����
	int kol = fileB.seekg(0, ios::end).tellg() / SIZE_INT;

	fileB.close();

	//����������
	mergeSort("Bin.txt", 0, kol - 1);

	fileB.open("Bin.txt", fstream::in | fstream::binary);

	//��������� ��������� ���� ��� ������ ������������ �����
	ofstream fout("file2.txt", ios_base::trunc);

	cout << "Elements of the sorted file: " << endl;
	//�������� �������� ���� ���������� (�� ����� � � file2.txt)
	while (fileB.read((char*)&date, SIZE_INT)) {
		fout << date << " ";
		cout << date << " ";
	}
	cout << endl;
	fout.close();
	fileB.close();

	//��������� ���������� ������� �����
	remove("Bin.txt");

	system("pause");

	return 0;
}