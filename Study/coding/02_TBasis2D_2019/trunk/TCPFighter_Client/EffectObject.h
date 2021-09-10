
#ifndef __EFFECT_OBJECT__
#define __EFFECT_OBJECT__

/*---------------------------------------------------------------

CBaseObject �� ��ӹ��� ����Ʈ ��ü.


---------------------------------------------------------------*/


class CEffectObject : public CBaseObject
{
public:

	////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//
	////////////////////////////////////////////////////////////////////
			CEffectObject(int iObjectType, int iObjectID, DWORD dwAttackID);
	virtual ~CEffectObject();



	// =================================================================
	// ������Ʈ ���� �������̽� �����Լ�.
	//
	// =================================================================
	////////////////////////////////////////////////////////////////////
	// ������Ʈ �׼� �����Լ�.
	//
	// Parameters: (DWORD)Param.
	// Return: (DWORD)Value.
	////////////////////////////////////////////////////////////////////
	virtual	DWORD		Action(DWORD dwParam);

	////////////////////////////////////////////////////////////////////
	// ������Ʈ �׸��� �����Լ�.
	//
	// Parameters: (BYTE *)DestPtr. (int)DestWidth. (int)DestHeight. (int)DestPitch.
	// Return: (DWORD)Value.
	////////////////////////////////////////////////////////////////////
	virtual	DWORD		Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch);



protected:

	//------------------------------------------------------------
	// �������� ID.
	// �������� �׼������ӿ� �°� ����Ʈ�� �Ͷ߷� �ֵ��� ����.
	//
	// �Էµ� ID ĳ���͸� �˻��Ͽ� �׼ǰ�, ������ ���� ���Ͽ�
	// ����Ʈ ���ϸ��̼��� �������� �����ȴ�.
	//------------------------------------------------------------
	DWORD		m_dwAttackID;

	BOOL		m_bEffectStart;


};

#endif