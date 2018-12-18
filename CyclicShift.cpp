#include <iostream>
#include <string>

using namespace std;

//створює таблицю
void prefix(string Tp, int tapl[]){
	int m = Tp.size();
	int k;
	tapl[0] = -1;
	for (int i = 1; i < m; i++){
		k = tapl[i - 1];
		while (k >= 0){
			if (Tp[k] == Tp[i - 1])	break;
			else	k = tapl[k];
		}
		tapl[i] = k + 1;
	}
}

//реалізація алгоритму КМП, яка перевіряє, чи є Т* (Tp) підрядком Т
bool is_contains(string Tp, string T){
	int m = Tp.size();
	int n = T.size();
	int* tapl = new int[m];
	prefix(Tp, tapl);
	int i = 0;
	int k = 0;
	while (i < n){
		if (k == -1){
			i++;
			k = 0;
		}
		else if (T[i] == Tp[k]){
			i++;
			k++;
			if (k == m)
				return true;
		}
		else
			k = tapl[k];
	}
	return false;
}

int main()
{
	string s1 = "abbaba";
	int n1 = s1.size();
	string s1_2;

	for (int i = 0; i < n1; i++) 
		s1_2.push_back(s1[i]);
	
	for (int i = 0; i < n1; i++) 
		s1_2.push_back(s1[i]);
	
	string s2 = "abaabb";

	cout << s2;
	if (is_contains(s2, s1_2))	cout << " is a cyclic shift ";
	else	cout << " is not a cyclic shift ";
	cout << s1 << endl;

	system("pause");
	return 0;
}