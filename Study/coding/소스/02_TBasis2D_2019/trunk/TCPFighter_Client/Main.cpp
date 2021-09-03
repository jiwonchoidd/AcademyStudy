
/////////////////////////////////////////////////////////////////////
// ���� ���� ����...........
//
//
/////////////////////////////////////////////////////////////////////
#include "CommonInclude.h"


#include "BaseObject.h"
#include "PlayerObject.h"

#include "ScreenDib.h"
#include "FrameSkip.h"

#include "DefineEnum.h"
#include "SpriteDib.h"


#include "Main.h"

// **************************************************************** 
// ������Ʈ STL ����Ʈ ������ ���� ����ü. 
//
// **************************************************************** 
struct stLISTsort
{
//	bool stLISTsort(const CBaseObject *pObject1, const CBaseObject *pObject2)
	bool operator() (const CBaseObject *pObject1, const CBaseObject *pObject2) const
	{
		//------------------------------------------------------------
		// ������Ʈ Ÿ���� ���� ��쿡�� ��ǥ�� �˻�
		//------------------------------------------------------------
		if ( pObject1->GetObjectType() == pObject2->GetObjectType() )
		{
			//------------------------------------------------------------
			// Y ���� ū��� �ڷ�
			//------------------------------------------------------------
			if ( pObject1->GetCurY() < pObject2->GetCurY() )
				return TRUE;

			return FALSE;
		}


		//------------------------------------------------------------
		// ������Ʈ Ÿ���� Ŭ ��� �ڷ�.
		//------------------------------------------------------------
		if ( pObject1->GetObjectType() < pObject2->GetObjectType() )
			return TRUE;

		return FALSE;
	}
};




//------------------------------------------------------------
// ȭ�����, �����ӽ�ŵ, ��������Ʈ Ŭ����
//------------------------------------------------------------
CScreenDib g_cScreenDib(dfSCREEN_WIDTH, dfSCREEN_HEIGHT, dfSCREEN_BIT);
CFrameSkip g_cFrameSkip(50);
CSpriteDib g_cSprite(eSPRITE_MAX, 0x00ffffff);


//------------------------------------------------------------
// ������Ʈ ����Ʈ
// ȭ�鿡 �������� ��� ��ü���� �� ����Ʈ���� ó���ȴ�.
//------------------------------------------------------------
list<CBaseObject *> g_stlObjectList;

//------------------------------------------------------------
// �÷��̾� ������Ʈ�� ���� �ϳ��� ��ü�� �˾Ƶд�
//------------------------------------------------------------
CBaseObject *g_pPlayerObject = NULL;




/////////////////////////////////////////////////////////////////////
// �����ʱ�ȭ �Լ�.
//
// Parameters: ����.
// Return: (BOOL)TRUE, FALSE.
/////////////////////////////////////////////////////////////////////
BOOL GameInit(void)
{
	if ( !g_cSprite.LoadDibSprite(eMAP, "../../Data/res/_Map.bmp", 0, 0) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_STAND_L01, "../../Data/res/Stand_L_01.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_STAND_L02, "../../Data/res/Stand_L_02.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_STAND_L03, "../../Data/res/Stand_L_03.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_STAND_L04, "../../Data/res/Stand_L_02.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_STAND_L05, "../../Data/res/Stand_L_01.bmp", 71, 90) )
		return FALSE;

	if ( !g_cSprite.LoadDibSprite(ePLAYER_STAND_R01, "../../Data/res/Stand_R_01.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_STAND_R02, "../../Data/res/Stand_R_02.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_STAND_R03, "../../Data/res/Stand_R_03.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_STAND_R04, "../../Data/res/Stand_R_02.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_STAND_R05, "../../Data/res/Stand_R_01.bmp", 71, 90) )
		return FALSE;

	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_L01, "../../Data/res/Move_L_01.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_L02, "../../Data/res/Move_L_02.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_L03, "../../Data/res/Move_L_03.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_L04, "../../Data/res/Move_L_04.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_L05, "../../Data/res/Move_L_05.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_L06, "../../Data/res/Move_L_06.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_L07, "../../Data/res/Move_L_07.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_L08, "../../Data/res/Move_L_08.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_L09, "../../Data/res/Move_L_09.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_L10, "../../Data/res/Move_L_10.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_L11, "../../Data/res/Move_L_11.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_L12, "../../Data/res/Move_L_12.bmp", 71, 90) )
		return FALSE;

	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_R01, "../../Data/res/Move_R_01.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_R02, "../../Data/res/Move_R_02.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_R03, "../../Data/res/Move_R_03.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_R04, "../../Data/res/Move_R_04.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_R05, "../../Data/res/Move_R_05.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_R06, "../../Data/res/Move_R_06.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_R07, "../../Data/res/Move_R_07.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_R08, "../../Data/res/Move_R_08.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_R09, "../../Data/res/Move_R_09.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_R10, "../../Data/res/Move_R_10.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_R11, "../../Data/res/Move_R_11.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_MOVE_R12, "../../Data/res/Move_R_12.bmp", 71, 90) )
		return FALSE;

	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK1_L01, "../../Data/res/Attack1_L_01.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK1_L02, "../../Data/res/Attack1_L_02.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK1_L03, "../../Data/res/Attack1_L_03.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK1_L04, "../../Data/res/Attack1_L_04.bmp", 71, 90) )
		return FALSE;

	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK1_R01, "../../Data/res/Attack1_R_01.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK1_R02, "../../Data/res/Attack1_R_02.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK1_R03, "../../Data/res/Attack1_R_03.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK1_R04, "../../Data/res/Attack1_R_04.bmp", 71, 90) )
		return FALSE;

	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK2_L01, "../../Data/res/Attack2_L_01.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK2_L02, "../../Data/res/Attack2_L_02.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK2_L03, "../../Data/res/Attack2_L_03.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK2_L04, "../../Data/res/Attack2_L_04.bmp", 71, 90) )
		return FALSE;

	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK2_R01, "../../Data/res/Attack2_R_01.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK2_R02, "../../Data/res/Attack2_R_02.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK2_R03, "../../Data/res/Attack2_R_03.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK2_R04, "../../Data/res/Attack2_R_04.bmp", 71, 90) )
		return FALSE;

	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK3_L01, "../../Data/res/Attack3_L_01.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK3_L02, "../../Data/res/Attack3_L_02.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK3_L03, "../../Data/res/Attack3_L_03.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK3_L04, "../../Data/res/Attack3_L_04.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK3_L05, "../../Data/res/Attack3_L_05.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK3_L06, "../../Data/res/Attack3_L_06.bmp", 71, 90) )
		return FALSE;


	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK3_R01, "../../Data/res/Attack3_R_01.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK3_R02, "../../Data/res/Attack3_R_02.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK3_R03, "../../Data/res/Attack3_R_03.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK3_R04, "../../Data/res/Attack3_R_04.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK3_R05, "../../Data/res/Attack3_R_05.bmp", 71, 90) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(ePLAYER_ATTACK3_R06, "../../Data/res/Attack3_R_06.bmp", 71, 90) )
		return FALSE;


	if ( !g_cSprite.LoadDibSprite(eEFFECT_SPARK_01, "../../Data/res/xSpark_1.bmp", 70, 70) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(eEFFECT_SPARK_02, "../../Data/res/xSpark_2.bmp", 70, 70) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(eEFFECT_SPARK_03, "../../Data/res/xSpark_3.bmp", 70, 70) )
		return FALSE;
	if ( !g_cSprite.LoadDibSprite(eEFFECT_SPARK_04, "../../Data/res/xSpark_4.bmp", 70, 70) )
		return FALSE;


	if ( !g_cSprite.LoadDibSprite(eGUAGE_HP, "../../Data/res/HPGuage.bmp", 0, 0) )
		return FALSE;

	if ( !g_cSprite.LoadDibSprite(eSHADOW, "../../Data/res/Shadow.bmp", 32, 4) )
		return FALSE;
/*
	g_pPlayerObject = new CPlayerObject(eTYPE_PLAYER, 0);
	g_pPlayerObject->SetSprite(ePLAYER_STAND_L01, ePLAYER_STAND_L_MAX, dfDELAY_STAND);

	g_pPlayerObject->SetPosition(200, 200);
	//g_pPlayerObject->SetHP(100);

	g_stlObjectList.push_back(g_pPlayerObject);
*/
	return TRUE;

}



/////////////////////////////////////////////////////////////////////
// Release
//
// Parameters: ����.
// Return: ����.
/////////////////////////////////////////////////////////////////////
void GameRelease(void)
{
	list<CBaseObject *>::iterator listIter = g_stlObjectList.begin();
	CBaseObject *pObject;

	//------------------------------------------------------------
	// ��ü ������Ʈ ����� ��ü�� �����Ѵ�.
	//------------------------------------------------------------
	while ( g_stlObjectList.end() != listIter )
	{
		pObject = *listIter;
		delete pObject;
			
		listIter++;
	}

	//------------------------------------------------------------
	// ����Ʈ �������� �����Ѵ�.
	//------------------------------------------------------------
	g_stlObjectList.clear();

/*
vector�� �ش� ���Ұ� �����Ǹ� Iterator�� ������ ������ ����
������ �ִ� ���� ��ġ�� ä������ �ȴ�.
�̴� RandomAccessIterator(�ڴ�� ���� ���� ������)�� ������ ��.

�㳪 list�� BidirectionalIterator(������� �������� ���� ����)�̹Ƿ�
�ش� ���Ұ� �����Ǹ� Iterator�� �� ���� ��ġ�� �Ҿ������ �Ǵ� ���̴�.
�ᱹ �� ���� �������� ��ġ�� ����� �������...

�̸� ���� �ذ� ���
���� Ȯ���� ����� erase �Լ��� ���ϰ��� �̿��ϴ� ���̴�.
erase �Լ��� ���ϰ��� �ٷ� ���� ���� ������ �ִ� ���� iterator�̱� ����...

for( Iterator = List.begin(); Iterator != List.end(); ) 
{
    Iterator = List.erase(Iterator);
}

�� ���� ++�� ���� �ʹٸ�...
for( Iterator = List.begin(); Iterator != List.end(); ) 
{
    List.erase(Iterator++);
}
*/


}




/////////////////////////////////////////////////////////////////////
// KeyProcess.
//
// Parameters: ����.
// Return: ����.
/////////////////////////////////////////////////////////////////////
void KeyProcess(void)
{
	if ( NULL == g_pPlayerObject )
		return;

	DWORD dwAction = dfACTION_STAND;

	if ( (GetAsyncKeyState(VK_LEFT) & 0x8000) &&
		 (GetAsyncKeyState(VK_UP) & 0x8000) )
	{
		dwAction = dfACTION_MOVE_LU;
	}
	else if ( (GetAsyncKeyState(VK_LEFT) & 0x8000) &&
		 (GetAsyncKeyState(VK_DOWN) & 0x8000) )
	{
		dwAction = dfACTION_MOVE_LD;
	}
	else if ( (GetAsyncKeyState(VK_RIGHT) & 0x8000) &&
		 (GetAsyncKeyState(VK_UP) & 0x8000) )
	{
		dwAction = dfACTION_MOVE_RU;
	}
	else if ( (GetAsyncKeyState(VK_RIGHT) & 0x8000) &&
		 (GetAsyncKeyState(VK_DOWN) & 0x8000) )
	{
		dwAction = dfACTION_MOVE_RD;
	}
	else if ( (GetAsyncKeyState(VK_RIGHT) & 0x8000) )
	{
		dwAction = dfACTION_MOVE_RR;
	}
	else if ( (GetAsyncKeyState(VK_LEFT) & 0x8000) )
	{
		dwAction = dfACTION_MOVE_LL;
	}
	else if ( (GetAsyncKeyState(VK_DOWN) & 0x8000) )
	{
		dwAction = dfACTION_MOVE_DD;
	}
	else if ( (GetAsyncKeyState(VK_UP) & 0x8000) )
	{
		dwAction = dfACTION_MOVE_UU;
	}



	//------------------------------------------------------------
	// ������ �ѹ��� �Ѱ����� �����ϹǷ� = ��
	//------------------------------------------------------------
	if ( GetAsyncKeyState(0x5a) & 0x8000 )	// Z
	{
		dwAction = dfACTION_ATTACK1;
	}
	else if ( GetAsyncKeyState(0x58) & 0x8000 )	// X
	{
		dwAction = dfACTION_ATTACK2;
	}
	else if ( GetAsyncKeyState(0x43) & 0x8000 )	// C	
	{
		dwAction = dfACTION_ATTACK3;
	}


	//------------------------------------------------------------
	// � ������ �Էµƴ��� �˷��ش�.
	//------------------------------------------------------------
	g_pPlayerObject->ActionInput(dwAction);


}


/////////////////////////////////////////////////////////////////////
// Action.
//
// Parameters: ����.
// Return: ����.
/////////////////////////////////////////////////////////////////////
void Action(void)
{
	list<CBaseObject *>::iterator listIter = g_stlObjectList.begin();
	CBaseObject *pObject;

	//------------------------------------------------------------
	// ��ü ������Ʈ ����� ���鼭 Action() ȣ��
	//------------------------------------------------------------
	while ( g_stlObjectList.end() != listIter )
	{
		pObject = *listIter;

		//------------------------------------------------------------
		// �׼��� ���ϰ��� TRUE ��� �ı��Ѵ�.
		//------------------------------------------------------------
		if ( pObject->Action(0) )
		{
			listIter = g_stlObjectList.erase(listIter);
			delete pObject;
		}
		else
		{
			listIter++;
		}
	}

	//------------------------------------------------------------
	// ����Ʈ ����. //Y��ǥ ��������
	//------------------------------------------------------------
	g_stlObjectList.sort(stLISTsort());
}





/////////////////////////////////////////////////////////////////////
// Draw.
//
// Parameters: ����.
// Return: ����.
/////////////////////////////////////////////////////////////////////
void Draw(void)
{
	//------------------------------------------------------------
	// ��� ���� ������.
	//------------------------------------------------------------
	BYTE *bypDest = g_cScreenDib.GetDibBuffer();
	int iDestWidth = g_cScreenDib.GetWidth();
	int iDestHeight = g_cScreenDib.GetHeight();
	int iDestPitch = g_cScreenDib.GetPitch();
	
	//------------------------------------------------------------
	// 1. �� ȭ�� ���
	//------------------------------------------------------------
	{
		g_cSprite.DrawImage(eMAP, 0, 0, bypDest, iDestWidth, iDestHeight, iDestPitch);
	}

	//------------------------------------------------------------
	// 2. ��������Ʈ ���
	//------------------------------------------------------------
	{
		list<CBaseObject *>::iterator listIter = g_stlObjectList.begin();
		CBaseObject *pObject;

		//------------------------------------------------------------
		// ��ü ������Ʈ ����� ���鼭 ȭ�鿡 ����Ѵ�.
		//------------------------------------------------------------
		while ( g_stlObjectList.end() != listIter )
		{
			pObject = *listIter;
			pObject->Draw(bypDest, iDestWidth, iDestHeight, iDestPitch);
			listIter++;
		}
	}
}




/////////////////////////////////////////////////////////////////////
// ���� �Լ�.
//
/////////////////////////////////////////////////////////////////////
void Main(void)
{
	//------------------------------------------------------------
	// Ű���� �Է� ó��
	//------------------------------------------------------------
	if ( g_bActiveApp )
		KeyProcess();

	//------------------------------------------------------------
	// ��ü �׼� ó��
	//------------------------------------------------------------
	Action();

	//------------------------------------------------------------
	// ��, ��ü... ȭ�鿡 �׸���
	//------------------------------------------------------------
	if ( g_cFrameSkip.FrameSkip() )
	{
		Draw();

		//------------------------------------------------------------
		// ScreenDib �� ȭ������ �ø�
		//------------------------------------------------------------
		g_cScreenDib.DrawBuffer(g_hWnd);
	}

	Sleep(0);
}


