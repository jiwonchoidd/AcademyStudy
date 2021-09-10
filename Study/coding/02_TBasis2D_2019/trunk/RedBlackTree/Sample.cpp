#include "TRBTree.h"

int main()
{
	int ch, y = 0;
	RBTree obj1, obj2;
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		//obj1.insertValue(iCnt);
		obj2.insertValue(10-iCnt);
	}
	obj1.insertValue(7);
	obj1.insertValue(5);
	obj1.insertValue(9);
	obj1.insertValue(12);
	obj1.insertValue(8);
	obj1.insertValue(11);
	obj1.insertValue(10);

	do
	{
		cout << "\n\t RED BLACK TREE ";
		cout << "\n 1. 추가";
		cout << "\n 2. 삭제";
		cout << "\n 3. 검색";
		cout << "\n 4. 출력";
		cout << "\n 5. 종료";
		cout << "\nEnter Your Choice: ";
		cin >> ch;
		switch (ch)
		{
		case 1: 
		{
			int z;
			cout << "\nEnter key of the node to be inserted: ";
			cin >> z;
			obj1.insertValue(z);
			cout << "\nNode Inserted.\n";
		}break;
		case 2: obj1.deleteValue(5);
			break;
		case 3: obj1.search(5);
			break;
		case 4: obj1.inorder();
			break;
		case 5: y = 1;
			break;
		default: cout << "\nEnter a Valid Choice.";
		}
		cout << endl;

	} while (y != 1);

	obj1.merge(obj2);
	obj1.preorder();
	return 1;
}
