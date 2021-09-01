
#ifndef __EFFECT_OBJECT__
#define __EFFECT_OBJECT__

/*---------------------------------------------------------------

CBaseObject 를 상속받은 이펙트 객체.


---------------------------------------------------------------*/


class CEffectObject : public CBaseObject
{
public:

	////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	////////////////////////////////////////////////////////////////////
			CEffectObject(int iObjectType, int iObjectID, DWORD dwAttackID);
	virtual ~CEffectObject();



	// =================================================================
	// 오브젝트 공통 인터페이스 가상함수.
	//
	// =================================================================
	////////////////////////////////////////////////////////////////////
	// 오브젝트 액션 가상함수.
	//
	// Parameters: (DWORD)Param.
	// Return: (DWORD)Value.
	////////////////////////////////////////////////////////////////////
	virtual	DWORD		Action(DWORD dwParam);

	////////////////////////////////////////////////////////////////////
	// 오브젝트 그리기 가상함수.
	//
	// Parameters: (BYTE *)DestPtr. (int)DestWidth. (int)DestHeight. (int)DestPitch.
	// Return: (DWORD)Value.
	////////////////////////////////////////////////////////////////////
	virtual	DWORD		Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch);



protected:

	//------------------------------------------------------------
	// 공격자의 ID.
	// 공격자의 액션프레임에 맞게 이펙트를 터뜨려 주도록 하자.
	//
	// 입력된 ID 캐릭터를 검색하여 액션과, 프레임 수를 비교하여
	// 이펙트 에니메이션을 시작할지 결정된다.
	//------------------------------------------------------------
	DWORD		m_dwAttackID;

	BOOL		m_bEffectStart;


};

#endif