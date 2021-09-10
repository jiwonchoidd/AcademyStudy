
/////////////////////////////////////////////////////////////////////
// 게임 메인 파일...........
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
// 오브젝트 STL 리스트 정렬을 위한 구조체. 
//
// **************************************************************** 
struct stLISTsort
{
//	bool stLISTsort(const CBaseObject *pObject1, const CBaseObject *pObject2)
	bool operator() (const CBaseObject *pObject1, const CBaseObject *pObject2) const
	{
		//------------------------------------------------------------
		// 오브젝트 타입이 같은 경우에만 좌표로 검사
		//------------------------------------------------------------
		if ( pObject1->GetObjectType() == pObject2->GetObjectType() )
		{
			//------------------------------------------------------------
			// Y 축이 큰경우 뒤로
			//------------------------------------------------------------
			if ( pObject1->GetCurY() < pObject2->GetCurY() )
				return TRUE;

			return FALSE;
		}


		//------------------------------------------------------------
		// 오브젝트 타입이 클 경우 뒤로.
		//------------------------------------------------------------
		if ( pObject1->GetObjectType() < pObject2->GetObjectType() )
			return TRUE;

		return FALSE;
	}
};




//------------------------------------------------------------
// 화면버퍼, 프레임스킵, 스프라이트 클래스
//------------------------------------------------------------
CScreenDib g_cScreenDib(dfSCREEN_WIDTH, dfSCREEN_HEIGHT, dfSCREEN_BIT);
CFrameSkip g_cFrameSkip(50);
CSpriteDib g_cSprite(eSPRITE_MAX, 0x00ffffff);


//------------------------------------------------------------
// 오브젝트 리스트
// 화면에 보여지는 모든 객체들은 이 리스트에서 처리된다.
//------------------------------------------------------------
list<CBaseObject *> g_stlObjectList;

//------------------------------------------------------------
// 플레이어 오브젝트에 대해 하나의 객체를 알아둔다
//------------------------------------------------------------
CBaseObject *g_pPlayerObject = NULL;




/////////////////////////////////////////////////////////////////////
// 게임초기화 함수.
//
// Parameters: 없음.
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
// Parameters: 없음.
// Return: 없음.
/////////////////////////////////////////////////////////////////////
void GameRelease(void)
{
	list<CBaseObject *>::iterator listIter = g_stlObjectList.begin();
	CBaseObject *pObject;

	//------------------------------------------------------------
	// 전체 오브젝트 목록을 객체를 삭제한다.
	//------------------------------------------------------------
	while ( g_stlObjectList.end() != listIter )
	{
		pObject = *listIter;
		delete pObject;
			
		listIter++;
	}

	//------------------------------------------------------------
	// 리스트 아이템을 삭제한다.
	//------------------------------------------------------------
	g_stlObjectList.clear();

/*
vector는 해당 원소가 삭제되면 Iterator의 공백을 삭제된 원소
다음에 있는 원소 위치로 채워지게 된다.
이는 RandomAccessIterator(멋대로 접근 가능 지시자)라 가능한 듯.

허나 list는 BidirectionalIterator(순서대로 차근차근 접근 가능)이므로
해당 원소가 삭제되면 Iterator는 그 다음 위치를 잃어버리게 되는 것이다.
결국 그 다음 지시자의 위치는 까마득한 별나라로...

이를 위한 해결 방법
가장 확실한 방법은 erase 함수의 리턴값을 이용하는 것이다.
erase 함수의 리턴값이 바로 삭제 원소 다음에 있는 원소 iterator이기 때문...

for( Iterator = List.begin(); Iterator != List.end(); ) 
{
    Iterator = List.erase(Iterator);
}

뭐 굳이 ++을 쓰고 싶다면...
for( Iterator = List.begin(); Iterator != List.end(); ) 
{
    List.erase(Iterator++);
}
*/


}




/////////////////////////////////////////////////////////////////////
// KeyProcess.
//
// Parameters: 없음.
// Return: 없음.
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
	// 공격은 한번에 한가지만 가능하므로 = 로
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
	// 어떤 동작이 입력됐는지 알려준다.
	//------------------------------------------------------------
	g_pPlayerObject->ActionInput(dwAction);


}


/////////////////////////////////////////////////////////////////////
// Action.
//
// Parameters: 없음.
// Return: 없음.
/////////////////////////////////////////////////////////////////////
void Action(void)
{
	list<CBaseObject *>::iterator listIter = g_stlObjectList.begin();
	CBaseObject *pObject;

	//------------------------------------------------------------
	// 전체 오브젝트 목록을 돌면서 Action() 호출
	//------------------------------------------------------------
	while ( g_stlObjectList.end() != listIter )
	{
		pObject = *listIter;

		//------------------------------------------------------------
		// 액션의 리턴값이 TRUE 라면 파괴한다.
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
	// 리스트 정렬. //Y좌표 버블정렬
	//------------------------------------------------------------
	g_stlObjectList.sort(stLISTsort());
}





/////////////////////////////////////////////////////////////////////
// Draw.
//
// Parameters: 없음.
// Return: 없음.
/////////////////////////////////////////////////////////////////////
void Draw(void)
{
	//------------------------------------------------------------
	// 출력 버퍼 포인터.
	//------------------------------------------------------------
	BYTE *bypDest = g_cScreenDib.GetDibBuffer();
	int iDestWidth = g_cScreenDib.GetWidth();
	int iDestHeight = g_cScreenDib.GetHeight();
	int iDestPitch = g_cScreenDib.GetPitch();
	
	//------------------------------------------------------------
	// 1. 맵 화면 출력
	//------------------------------------------------------------
	{
		g_cSprite.DrawImage(eMAP, 0, 0, bypDest, iDestWidth, iDestHeight, iDestPitch);
	}

	//------------------------------------------------------------
	// 2. 스프라이트 출력
	//------------------------------------------------------------
	{
		list<CBaseObject *>::iterator listIter = g_stlObjectList.begin();
		CBaseObject *pObject;

		//------------------------------------------------------------
		// 전체 오브젝트 목록을 돌면서 화면에 출력한다.
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
// 메인 함수.
//
/////////////////////////////////////////////////////////////////////
void Main(void)
{
	//------------------------------------------------------------
	// 키보드 입력 처리
	//------------------------------------------------------------
	if ( g_bActiveApp )
		KeyProcess();

	//------------------------------------------------------------
	// 객체 액션 처리
	//------------------------------------------------------------
	Action();

	//------------------------------------------------------------
	// 맵, 객체... 화면에 그리기
	//------------------------------------------------------------
	if ( g_cFrameSkip.FrameSkip() )
	{
		Draw();

		//------------------------------------------------------------
		// ScreenDib 를 화면으로 플립
		//------------------------------------------------------------
		g_cScreenDib.DrawBuffer(g_hWnd);
	}

	Sleep(0);
}


