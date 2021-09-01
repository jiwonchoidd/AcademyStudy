#include <iostream>
using namespace std;

struct PERSON {   
	int age;   
	long ss;
	float weight;
	char name[25];
} family_member;   

struct CELL {   
	unsigned short character : 8;   //00000000 ????????
	unsigned short foreground : 3;  //00000??? 00000000
	unsigned short intensity : 1;   //0000?000 00000000
	unsigned short background : 3;  //0???0000 00000000
	unsigned short blink : 1;       //?0000000 00000000
};       

int main() {
	struct PERSON sister;   
	PERSON brother;   
	sister.age = 13;   
	brother.age = 7;
	cout << "sister.age = " << sister.age << '\n';
	cout << "brother.age = " << brother.age << '\n';
	
	CELL my_cell;
	my_cell.character = 255;
	my_cell.foreground = 7;
	my_cell.intensity = 1;
	my_cell.background = 7;
	my_cell.blink = 1;
	
	cout << "my_cell.character = " << my_cell.character << endl;
	cout << "my_cell.character = " << my_cell.foreground << endl;
	cout << "my_cell.character = " << my_cell.intensity << endl;
	cout << "my_cell.character = " << my_cell.background << endl;
	cout << "my_cell.character = " << my_cell.blink << endl;
}
// Output:
// sister.age = 13
// brother.age = 7
// my_cell.character = 1
