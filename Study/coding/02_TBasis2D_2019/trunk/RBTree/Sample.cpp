#include "Sample.h"
#include "Sample.h"

void main()
{
	node obj;
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		node* pData = new node();

		pData->key = iCnt;
		insert_repair_tree(pData);
	}
}