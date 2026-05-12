#include "header.h"


int main() {
	string hex1 = "";
	string hex2 = "";
	string line;

	ifstream Hex1("hex1.txt");
	if (!Hex1) {
		cout << "File is not open\n";
		return -1;
	}

	ifstream Hex2("hex2.txt");
	if (!Hex2) {
		cout << "File is not open\n";
		return -1;
	}

	ofstream Result("result.txt");
	if (!Result) {
		cout << "Failed to create write file\n";
		return -1;
	}

	while (getline(Hex1, line)) {
		hex1 += line;
	}

	while (getline(Hex2, line)) {
		hex2 += line;
	}

	Hex1.close();
	Hex2.close();

	
	Result.close();

	return 0;
}
