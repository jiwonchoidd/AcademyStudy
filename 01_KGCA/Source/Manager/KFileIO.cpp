#include "KFileIO.h"

void KFileIO::FileCreate()
{	
	std::cout << "\n::::::::::����� ������ ���� ���� ���� ���� �մϴ�.::::::::::\n" << std::endl;
	FileSave();
}


bool KFileIO::FileLoad()
{
	FILE* fpRead = fopen("GradeManager.txt", "rb");
	if (fpRead == NULL)
	{
		FileCreate();
		return false;
	}
	//������ ������ ������.
	else
	{
		int iReadObj = 0;
		fread(&iReadObj, sizeof(int), 1, fpRead);
		for (int iAdd = 0; iAdd < iReadObj; iAdd++)
		{
			KStudent* pStudent = new KStudent();
			memset(pStudent, 0, sizeof(KStudent));
			/*fread �Լ��� �ٹٲ޹���(\n)�� ������� "�о���� ����Ʈ"�� ��ŭ ���ڸ� �о�ɴϴ�
			fgets�Լ��� "�о���� ����Ʈ"�� ������ �ٹٲ޹���(\n)�� ������  ���ڸ� �� �̻� �о� ���� �ʽ��ϴ�.
			�� ������ �о� �� �� ����Ͻø�˴ϴ�.*/
			fread(pStudent, sizeof(KStudent), 1, fpRead);
			m_List.AddLink(pStudent);
		}
	}
	fclose(fpRead);
	return true;
}

bool KFileIO::FileSave()
{
	FILE* fpWrite = fopen("GradeManager.txt", "wb");
	// ������(���) ����� �Լ�
	int iCouner = m_List.GetAmount();
	fwrite(&iCouner, sizeof(int), 1, fpWrite);
	for (KNode<KStudent>* pNode = m_List.m_pHead->m_pNext;
		pNode != m_List.m_pTail;
		pNode = pNode->m_pNext)
	{
		fwrite(pNode->m_pValue, sizeof(KStudent), 1, fpWrite);
	}
	fclose(fpWrite);
	return true;
}

void KFileIO::FileDeleteAll()
{
	for (KNode<KStudent>* pNode = m_List.m_pHead->m_pNext;
		pNode != m_List.m_pTail;
		pNode = pNode->m_pNext)
	{
		delete pNode;
	}
	FileSave();
}
void KFileIO::FileDraw()
{
	m_List.PrintAll();
}