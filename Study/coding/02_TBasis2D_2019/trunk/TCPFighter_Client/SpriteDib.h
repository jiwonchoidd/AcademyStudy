
#ifndef __SPRITE_DIB__
#define __SPRITE_DIB__

/*---------------------------------------------------------------

BMP ������ �о ��������Ʈ ���� Ŭ����.

---------------------------------------------------------------*/

class CSpriteDib
{
public:

	// **************************************************************** 
	// DIB ��������Ʈ ����ü. 
	//
	// ��������Ʈ �̹����� ������ ������ ����.
	// **************************************************************** 
	typedef struct st_SPRITE
	{
		BYTE	*bypImage;				// ��������Ʈ �̹��� ������.
		int		iWidth;					// Widht
		int		iHeight;				// Height
		int		iPitch;					// Pitch

		int		iCenterPointX;			// ���� X
		int		iCenterPointY;			// ���� Y
	};

    
	////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//
	// Parameters: (int)�ִ� ��������Ʈ ����. (DWORD)����Į��.
	////////////////////////////////////////////////////////////////////
			CSpriteDib(int iMaxSprite, DWORD dwColorKey);
	virtual ~CSpriteDib();


	///////////////////////////////////////////////////////
	// LoadDibSprite. 
	// BMP������ �о �ϳ��� ���������� �����Ѵ�.
	//
	// Parameters: (int)SpriteIndex. (char *)FileName. (int)CenterPointX. (int)CenterPointY.
	// Return: (BOOL)TRUE, FALSE.
	///////////////////////////////////////////////////////
	BOOL LoadDibSprite(int iSpriteIndex, char *szFileName, int iCenterPointX, int iCenterPointY);


	///////////////////////////////////////////////////////
	// ReleaseSprite. 
	// �ش� ��������Ʈ ����.
	//
	// Parameters: (int)SpriteIndex.
	// Return: (BOOL)TRUE, FALSE.
	///////////////////////////////////////////////////////
	void ReleaseSprite(int iSpriteIndex);


	///////////////////////////////////////////////////////
	// DrawSprite. 
	// Ư�� �޸� ��ġ�� ��������Ʈ�� ����Ѵ�. (Į��Ű, Ŭ���� ó��)
	//
	// Parameters: (int)SpriteIndex. (int)DrawX. (int)DrawY. (BYTE *)Dest. (int)DestWidth. (int)DestHeight. (int)DestPitch. (int)DrawLen.
	// Return: ����.
	///////////////////////////////////////////////////////
	void DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100);
	void DrawSprite50(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch);
	void DrawSpriteRed(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch);

	///////////////////////////////////////////////////////
	// DrawImage. 
	// Ư�� �޸� ��ġ�� �̹����� ����Ѵ�. (Ŭ���� ó��)
	//
	// Parameters: (int)SpriteIndex. (int)DrawX. (int)DrawY. (BYTE *)Dest. (int)DestWidth. (int)DestHeight. (int)DestPitch. (int)DrawLen.
	// Return: ����.
	///////////////////////////////////////////////////////
	void DrawImage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100);


protected:

	//------------------------------------------------------------------
	// Sprite �迭 ����.
	//------------------------------------------------------------------
	int			m_iMaxSprite;
	st_SPRITE	*m_stpSprite;

	//------------------------------------------------------------------
	// ���� �������� ����� �÷�.
	//------------------------------------------------------------------
	DWORD		m_dwColorKey;

};

#endif

