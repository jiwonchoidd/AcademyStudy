#include "KFileIO.h"

void KFileIO::FileCreate()
{	
	std::cout << "\n::::::::::저장된 파일이 없어 새로 파일 생성 합니다.::::::::::\n" << std::endl;
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
	//파일의 유무를 따진다.
	else
	{
		int iReadObj = 0;
		fread(&iReadObj, sizeof(int), 1, fpRead);
		for (int iAdd = 0; iAdd < iReadObj; iAdd++)
		{
			KStudent* pStudent = new KStudent();
			memset(pStudent, 0, sizeof(KStudent));
			/*fread 함수는 줄바꿈문자(\n)와 상관없이 "읽어들일 바이트"수 만큼 문자를 읽어옵니다
			fgets함수는 "읽어들일 바이트"수 이전에 줄바꿈문자(\n)를 만나면  문자를 더 이상 읽어 오지 않습니다.
			즉 한줄을 읽어 올 때 사용하시면됩니다.*/
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
	// 블럭단위(덩어리) 입출력 함수
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