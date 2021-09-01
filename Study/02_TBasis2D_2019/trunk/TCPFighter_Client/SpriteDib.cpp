
/*---------------------------------------------------------------

BMP 파일을 읽어서 스프라이트 관리 클래스.

---------------------------------------------------------------*/
#include <windows.h>
#include "SpriteDib.h"

/*
// **************************************************************** 
// DIB 스프라이트 구조체. 
//
// 스프라이트 이미지와 사이즈 정보를 가짐.
// **************************************************************** 
typedef struct st_SPRITE
{
	BYTE	*bypImage;				// 스프라이트 이미지 포인터.
	int		iWidth;					// Widht
	int		iHeight;				// Height
};
*/
  

////////////////////////////////////////////////////////////////////
// 생성자, 파괴자.
//
// Parameters: (int)최대 스프라이트 개수. (DWORD)투명칼라.
////////////////////////////////////////////////////////////////////
CSpriteDib::CSpriteDib(int iMaxSprite, DWORD dwColorKey)
{
	m_iMaxSprite = iMaxSprite;
	m_dwColorKey = dwColorKey;

	m_stpSprite = NULL;

	//-----------------------------------------------------------------
	// 최대 읽어올 개수 만큼 미리 할당 받는다.
	//-----------------------------------------------------------------
	m_stpSprite = new st_SPRITE[m_iMaxSprite];
	memset(m_stpSprite, 0, sizeof(st_SPRITE) * m_iMaxSprite);
}

CSpriteDib::~CSpriteDib()
{
	int iCount;

	//-----------------------------------------------------------------
	// 전체를 돌면서 모두 지우자.
	//-----------------------------------------------------------------
	for ( iCount = 0; iCount > m_iMaxSprite; iCount++ )
	{
		ReleaseSprite(iCount);
	}
}


///////////////////////////////////////////////////////
// LoadDibSprite. 
// BMP파일을 읽어서 하나의 프레임으로 저장한다.
//
// Parameters: (int)SpriteIndex. (char *)FileName. (int)CenterPointX. (int)CenterPointY.
// Return: (BOOL)TRUE, FALSE.
///////////////////////////////////////////////////////
BOOL CSpriteDib::LoadDibSprite(int iSpriteIndex, char *szFileName, int iCenterPointX, int iCenterPointY)
{
	HANDLE	hFile;
	DWORD	dwRead;
	int		iPitch;
	int		iImageSize;

	BITMAPFILEHEADER	stFileHeader;
	BITMAPINFOHEADER	stInfoHeader;
	//-----------------------------------------------------------------
	// 일단 파일을 열자.
	//-----------------------------------------------------------------
	hFile = CreateFile(szFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( INVALID_HANDLE_VALUE == hFile )
	{
		return FALSE;
	}

	//-----------------------------------------------------------------
	// 해당 스프라이트에 이미 데이터가 있다면 이를 지우자.
	//-----------------------------------------------------------------
	ReleaseSprite(iSpriteIndex);

	//-----------------------------------------------------------------
	// 비트맵 헤더를 열어 BMP 파일 확인.
	//-----------------------------------------------------------------
	ReadFile(hFile, &stFileHeader, sizeof(BITMAPFILEHEADER), &dwRead, NULL);
	if ( 0x4d42 == stFileHeader.bfType )
	{
		//-----------------------------------------------------------------
		// 인포헤더를 읽어서 저장 & 32비트 확인.
		//-----------------------------------------------------------------
		ReadFile(hFile, &stInfoHeader, sizeof(BITMAPINFOHEADER), &dwRead, NULL);
		if ( 32 == stInfoHeader.biBitCount )
		{
			//-----------------------------------------------------------------
			// 한줄, 한줄의 피치값을 구한다.
			//-----------------------------------------------------------------
			iPitch = (stInfoHeader.biWidth * 4) + 3 & ~3;
			//-----------------------------------------------------------------
			// 스프라이트 구조체에 크기 저장.
			//-----------------------------------------------------------------
			m_stpSprite[iSpriteIndex].iWidth = stInfoHeader.biWidth;
			m_stpSprite[iSpriteIndex].iHeight = stInfoHeader.biHeight;
			m_stpSprite[iSpriteIndex].iPitch = iPitch;

			//-----------------------------------------------------------------
			// 이미지에 대한 전체 크기를 구하고, 메모리 할당.
			//-----------------------------------------------------------------
			iImageSize = iPitch * stInfoHeader.biHeight;
			m_stpSprite[iSpriteIndex].bypImage = new BYTE[iImageSize];

			//-----------------------------------------------------------------
			// 이미지 부분은 스프라이트 버퍼로 읽어온다.
			//
			// DIB 는 뒤집어져 있으므로 이를 다시 뒤집자.
			// 임시 버퍼에 읽은 뒤에 뒤집으면서 복사한다.
			//-----------------------------------------------------------------
			BYTE *bypTempBuffer = new BYTE[iImageSize];
			BYTE *bypSpriteTemp = m_stpSprite[iSpriteIndex].bypImage;
			BYTE *bypTurnTemp;
			int iCount;

			ReadFile(hFile, bypTempBuffer, iImageSize, &dwRead, NULL);

			//-----------------------------------------------------------------
			// 한줄한줄 뒤집자.
			//-----------------------------------------------------------------
			bypTurnTemp = bypTempBuffer + iPitch * (stInfoHeader.biHeight - 1);

			for ( iCount = 0; iCount < stInfoHeader.biHeight; iCount++ )
			{
				memcpy(bypSpriteTemp, bypTurnTemp, iPitch);
				bypSpriteTemp += iPitch;
				bypTurnTemp -= iPitch;
			}
			delete[] bypTempBuffer;


			m_stpSprite[iSpriteIndex].iCenterPointX = iCenterPointX;
			m_stpSprite[iSpriteIndex].iCenterPointY = iCenterPointY;

			CloseHandle(hFile);
			return TRUE;
		}
	}

	CloseHandle(hFile);
	return FALSE;
}


///////////////////////////////////////////////////////
// ReleaseSprite. 
// 해당 스프라이트 해제.
//
// Parameters: (int)SpriteIndex.
// Return: (BOOL)TRUE, FALSE.
///////////////////////////////////////////////////////
void CSpriteDib::ReleaseSprite(int iSpriteIndex)
{
	//-----------------------------------------------------------------
	// 최대 할당된 스프라이트를 넘어서면 안되지롱
	//-----------------------------------------------------------------
	if ( m_iMaxSprite <= iSpriteIndex )
		return;

	if ( NULL != m_stpSprite[iSpriteIndex].bypImage )
	{
		//-----------------------------------------------------------------
		// 삭제 후 초기화.
		//-----------------------------------------------------------------
		delete[] m_stpSprite[iSpriteIndex].bypImage;
		memset(&m_stpSprite[iSpriteIndex], 0, sizeof(st_SPRITE));
	}
}

///////////////////////////////////////////////////////
// DrawSprite. 
// 특정 메모리 위치에 스프라이트를 출력한다. (클리핑 처리)
//
// Parameters: (int)SpriteIndex. (int)DrawX. (int)DrawY. (BYTE *)Dest. (int)DestWidth. (int)DestHeight. (int)DestPitch. (int)DrawLen.
// Return: 없음.
///////////////////////////////////////////////////////
void CSpriteDib::DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen)
{
	//-----------------------------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-----------------------------------------------------------------
	if ( iSpriteIndex >= m_iMaxSprite )
		return;

	if ( NULL == m_stpSprite[iSpriteIndex].bypImage )
		return;

	st_SPRITE *stpSprite = &m_stpSprite[iSpriteIndex];

	//-----------------------------------------------------------------
	// 스프라이트 출력을 위해 사이즈 저장.
	//-----------------------------------------------------------------
	int iSpriteWidth = stpSprite->iWidth;
	int iSpriteHeight = stpSprite->iHeight;
	int iCountX, iCountY;


	iSpriteWidth = iSpriteWidth * iDrawLen / 100;


	DWORD *dwpDest = (DWORD *)bypDest;
	DWORD *dwpSprite = (DWORD *)(stpSprite->bypImage);


	//-----------------------------------------------------------------
	// 출력 중점 처리.
	//-----------------------------------------------------------------
	iDrawX = iDrawX - stpSprite->iCenterPointX;
	iDrawY = iDrawY - stpSprite->iCenterPointY;


	//-----------------------------------------------------------------
	// 상단 에 대한 스프라이트 출력 위치 계산. (상단 클리핑)
	//-----------------------------------------------------------------
	if ( 0 > iDrawY )
	{
		iSpriteHeight = iSpriteHeight - (-iDrawY);
		dwpSprite = (DWORD *)(stpSprite->bypImage + stpSprite->iPitch * (-iDrawY));
		//-----------------------------------------------------------------
		// 윗쪽이 짤리는 경우이므로 스프라이트 시작 위치를 아래로 내려준다.
		//-----------------------------------------------------------------

		iDrawY = 0;
	}

	//-----------------------------------------------------------------
	// 하단에 사이즈 계산. (하단 클리핑)
	//-----------------------------------------------------------------
	if ( iDestHeight <= iDrawY + stpSprite->iHeight )
	{
		iSpriteHeight -= ((iDrawY + stpSprite->iHeight) - iDestHeight);
	}

	//-----------------------------------------------------------------
	// 왼쪽 출력 위치 계산. (좌측 클리핑)
	//-----------------------------------------------------------------
	if ( 0 > iDrawX )
	{
		iSpriteWidth = iSpriteWidth - (-iDrawX);
		dwpSprite = dwpSprite + (-iDrawX);
		//-----------------------------------------------------------------
		// 왼쪽이 조금 잘리므로 출력 시작 위치를 오른쪽으로 민다.
		//-----------------------------------------------------------------

		iDrawX = 0;
	}

	//-----------------------------------------------------------------
	// 오른쪽 출력 위치 계산. (우측 클리핑)
	//-----------------------------------------------------------------
	if ( iDestWidth <= iDrawX + stpSprite->iWidth )
	{
		iSpriteWidth -= ((iDrawX + stpSprite->iWidth) - iDestWidth);
	}

	//-----------------------------------------------------------------
	// 찍을 그림이 없다면 종료한다.
	//-----------------------------------------------------------------
	if ( iSpriteWidth <= 0 || iSpriteHeight <= 0 )
		return;

	//-----------------------------------------------------------------
	// 화면에 찍을 위치로 이동한다.
	//-----------------------------------------------------------------
	dwpDest = (DWORD *)(((BYTE *)(dwpDest + iDrawX) + (iDrawY * iDestPitch)));


	BYTE *bypDestOrigin = (BYTE *)dwpDest;
	BYTE *bypSpriteOrigin = (BYTE *)dwpSprite;

	//-----------------------------------------------------------------
	// 전체 크기를 돌면서 투명색 처리를 한다.
	//-----------------------------------------------------------------
	for ( iCountY = 0; iSpriteHeight > iCountY; iCountY++ )
	{
		for ( iCountX = 0; iSpriteWidth > iCountX; iCountX++ )
		{
			if ( m_dwColorKey != (*dwpSprite & 0x00ffffff) )
			{
				*dwpDest = *dwpSprite;
			}
			
			dwpDest++;
			dwpSprite++;
		}

		//-----------------------------------------------------------------
		// 다음줄로 이동, 화면과 스프라이트 모두..
		//-----------------------------------------------------------------
		bypDestOrigin = bypDestOrigin + iDestPitch;
		bypSpriteOrigin = bypSpriteOrigin + stpSprite->iPitch;

		dwpDest = (DWORD *)bypDestOrigin;
		dwpSprite = (DWORD *)bypSpriteOrigin;
	}
}


///////////////////////////////////////////////////////
// DrawSprite. 
// 특정 메모리 위치에 스프라이트를 출력한다. (클리핑 처리)
//
// Parameters: (int)SpriteIndex. (int)DrawX. (int)DrawY. (BYTE *)Dest. (int)DestWidth. (int)DestHeight. (int)DestPitch.
// Return: 없음.
///////////////////////////////////////////////////////
void CSpriteDib::DrawSpriteRed(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	//-----------------------------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-----------------------------------------------------------------
	if ( iSpriteIndex >= m_iMaxSprite )
		return;

	if ( NULL == m_stpSprite[iSpriteIndex].bypImage )
		return;

	st_SPRITE *stpSprite = &m_stpSprite[iSpriteIndex];

	//-----------------------------------------------------------------
	// 스프라이트 출력을 위해 사이즈 저장.
	//-----------------------------------------------------------------
	int iSpriteWidth = stpSprite->iWidth;
	int iSpriteHeight = stpSprite->iHeight;
	int iCountX, iCountY;

	DWORD *dwpDest = (DWORD *)bypDest;
	DWORD *dwpSprite = (DWORD *)(stpSprite->bypImage);


	//-----------------------------------------------------------------
	// 출력 중점 처리.
	//-----------------------------------------------------------------
	iDrawX = iDrawX - stpSprite->iCenterPointX;
	iDrawY = iDrawY - stpSprite->iCenterPointY;


	//-----------------------------------------------------------------
	// 상단 에 대한 스프라이트 출력 위치 계산. (상단 클리핑)
	//-----------------------------------------------------------------
	if ( 0 > iDrawY )
	{
		iSpriteHeight = iSpriteHeight - (-iDrawY);
		dwpSprite = (DWORD *)(stpSprite->bypImage + stpSprite->iPitch * (-iDrawY));
		//-----------------------------------------------------------------
		// 윗쪽이 짤리는 경우이므로 스프라이트 시작 위치를 아래로 내려준다.
		//-----------------------------------------------------------------

		iDrawY = 0;
	}

	//-----------------------------------------------------------------
	// 하단에 사이즈 계산. (하단 클리핑)
	//-----------------------------------------------------------------
	if ( iDestHeight <= iDrawY + stpSprite->iHeight )
	{
		iSpriteHeight -= ((iDrawY + stpSprite->iHeight) - iDestHeight);
	}

	//-----------------------------------------------------------------
	// 왼쪽 출력 위치 계산. (좌측 클리핑)
	//-----------------------------------------------------------------
	if ( 0 > iDrawX )
	{
		iSpriteWidth = iSpriteWidth - (-iDrawX);
		dwpSprite = dwpSprite + (-iDrawX);
		//-----------------------------------------------------------------
		// 왼쪽이 조금 잘리므로 출력 시작 위치를 오른쪽으로 민다.
		//-----------------------------------------------------------------

		iDrawX = 0;
	}

	//-----------------------------------------------------------------
	// 오른쪽 출력 위치 계산. (우측 클리핑)
	//-----------------------------------------------------------------
	if ( iDestWidth <= iDrawX + stpSprite->iWidth )
	{
		iSpriteWidth -= ((iDrawX + stpSprite->iWidth) - iDestWidth);
	}

	//-----------------------------------------------------------------
	// 찍을 그림이 없다면 종료한다.
	//-----------------------------------------------------------------
	if ( iSpriteWidth <= 0 || iSpriteHeight <= 0 )
		return;

	//-----------------------------------------------------------------
	// 화면에 찍을 위치로 이동한다.
	//-----------------------------------------------------------------
	dwpDest = (DWORD *)(((BYTE *)(dwpDest + iDrawX) + (iDrawY * iDestPitch)));


	BYTE *bypDestOrigin = (BYTE *)dwpDest;
	BYTE *bypSpriteOrigin = (BYTE *)dwpSprite;

	BYTE byRed, byGreen, byBlue;

	//-----------------------------------------------------------------
	// 전체 크기를 돌면서 투명색 처리를 한다.
	//-----------------------------------------------------------------
	for ( iCountY = 0; iSpriteHeight > iCountY; iCountY++ )
	{
		for ( iCountX = 0; iSpriteWidth > iCountX; iCountX++ )
		{
			if ( m_dwColorKey != (*dwpSprite & 0x00ffffff) )
			{
				byRed = *((BYTE *)dwpSprite + 2);
				byGreen = *((BYTE *)dwpSprite + 1);
				byBlue = *((BYTE *)dwpSprite + 0);

				*((BYTE *)dwpDest + 2) = byRed;
				*((BYTE *)dwpDest + 1) = byGreen / 2;
				*((BYTE *)dwpDest + 0) = byBlue / 2;

//				*dwpDest = *dwpSprite;
			}
			
			dwpDest++;
			dwpSprite++;
		}

		//-----------------------------------------------------------------
		// 다음줄로 이동, 화면과 스프라이트 모두..
		//-----------------------------------------------------------------
		bypDestOrigin = bypDestOrigin + iDestPitch;
		bypSpriteOrigin = bypSpriteOrigin + stpSprite->iPitch;

		dwpDest = (DWORD *)bypDestOrigin;
		dwpSprite = (DWORD *)bypSpriteOrigin;
	}
}



///////////////////////////////////////////////////////
// DrawSprite. 
// 특정 메모리 위치에 스프라이트를 출력한다. (클리핑 처리)
//
// Parameters: (int)SpriteIndex. (int)DrawX. (int)DrawY. (BYTE *)Dest. (int)DestWidth. (int)DestHeight. (int)DestPitch.
// Return: 없음.
///////////////////////////////////////////////////////
void CSpriteDib::DrawSprite50(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	//-----------------------------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-----------------------------------------------------------------
	if ( iSpriteIndex >= m_iMaxSprite )
		return;

	if ( NULL == m_stpSprite[iSpriteIndex].bypImage )
		return;

	st_SPRITE *stpSprite = &m_stpSprite[iSpriteIndex];

	//-----------------------------------------------------------------
	// 스프라이트 출력을 위해 사이즈 저장.
	//-----------------------------------------------------------------
	int iSpriteWidth = stpSprite->iWidth;
	int iSpriteHeight = stpSprite->iHeight;
	int iCountX, iCountY;

	DWORD *dwpDest = (DWORD *)bypDest;
	DWORD *dwpSprite = (DWORD *)(stpSprite->bypImage);


	//-----------------------------------------------------------------
	// 출력 중점 처리.
	//-----------------------------------------------------------------
	iDrawX = iDrawX - stpSprite->iCenterPointX;
	iDrawY = iDrawY - stpSprite->iCenterPointY;


	//-----------------------------------------------------------------
	// 상단 에 대한 스프라이트 출력 위치 계산. (상단 클리핑)
	//-----------------------------------------------------------------
	if ( 0 > iDrawY )
	{
		iSpriteHeight = iSpriteHeight - (-iDrawY);
		dwpSprite = (DWORD *)(stpSprite->bypImage + stpSprite->iPitch * (-iDrawY));
		//-----------------------------------------------------------------
		// 윗쪽이 짤리는 경우이므로 스프라이트 시작 위치를 아래로 내려준다.
		//-----------------------------------------------------------------

		iDrawY = 0;
	}

	//-----------------------------------------------------------------
	// 하단에 사이즈 계산. (하단 클리핑)
	//-----------------------------------------------------------------
	if ( iDestHeight <= iDrawY + stpSprite->iHeight )
	{
		iSpriteHeight -= ((iDrawY + stpSprite->iHeight) - iDestHeight);
	}

	//-----------------------------------------------------------------
	// 왼쪽 출력 위치 계산. (좌측 클리핑)
	//-----------------------------------------------------------------
	if ( 0 > iDrawX )
	{
		iSpriteWidth = iSpriteWidth - (-iDrawX);
		dwpSprite = dwpSprite + (-iDrawX);
		//-----------------------------------------------------------------
		// 왼쪽이 조금 잘리므로 출력 시작 위치를 오른쪽으로 민다.
		//-----------------------------------------------------------------

		iDrawX = 0;
	}

	//-----------------------------------------------------------------
	// 오른쪽 출력 위치 계산. (우측 클리핑)
	//-----------------------------------------------------------------
	if ( iDestWidth <= iDrawX + stpSprite->iWidth )
	{
		iSpriteWidth -= ((iDrawX + stpSprite->iWidth) - iDestWidth);
	}

	//-----------------------------------------------------------------
	// 찍을 그림이 없다면 종료한다.
	//-----------------------------------------------------------------
	if ( iSpriteWidth <= 0 || iSpriteHeight <= 0 )
		return;

	//-----------------------------------------------------------------
	// 화면에 찍을 위치로 이동한다.
	//-----------------------------------------------------------------
	dwpDest = (DWORD *)(((BYTE *)(dwpDest + iDrawX) + (iDrawY * iDestPitch)));


	BYTE *bypDestOrigin = (BYTE *)dwpDest;
	BYTE *bypSpriteOrigin = (BYTE *)dwpSprite;

	BYTE bySrcRed, bySrcGreen, bySrcBlue;
	BYTE byDestRed, byDestGreen, byDestBlue;

	DWORD dwSprite;

	//-----------------------------------------------------------------
	// 전체 크기를 돌면서 투명색 처리를 한다.
	//-----------------------------------------------------------------
	for ( iCountY = 0; iSpriteHeight > iCountY; iCountY++ )
	{
		for ( iCountX = 0; iSpriteWidth > iCountX; iCountX++ )
		{
			if ( m_dwColorKey != (*dwpSprite & 0x00ffffff) )
			{
				dwSprite = (*dwpSprite >> 1) & 0x7f7f7f7f;
				*dwpDest = (*dwpDest >> 1) & 0x7f7f7f7f;

				*dwpDest = *dwpDest + dwSprite;
/*
				bySrcRed = *((BYTE *)dwpSprite + 2);
				bySrcGreen = *((BYTE *)dwpSprite + 1);
				bySrcBlue = *((BYTE *)dwpSprite + 0);

				byDestRed = *((BYTE *)dwpDest + 2);
				byDestGreen = *((BYTE *)dwpDest + 1);
				byDestBlue = *((BYTE *)dwpDest + 0);

				*((BYTE *)dwpDest + 2) = (byDestRed + bySrcRed) / 2;
				*((BYTE *)dwpDest + 1) = (bySrcGreen + byDestGreen) / 2;
				*((BYTE *)dwpDest + 0) = (bySrcBlue + byDestBlue) / 2;

//				*dwpDest = *dwpSprite;
*/
			}
			
			dwpDest++;
			dwpSprite++;
		}

		//-----------------------------------------------------------------
		// 다음줄로 이동, 화면과 스프라이트 모두..
		//-----------------------------------------------------------------
		bypDestOrigin = bypDestOrigin + iDestPitch;
		bypSpriteOrigin = bypSpriteOrigin + stpSprite->iPitch;

		dwpDest = (DWORD *)bypDestOrigin;
		dwpSprite = (DWORD *)bypSpriteOrigin;
	}
}



///////////////////////////////////////////////////////
// DrawImage. 
// 특정 메모리 위치에 이미지를 출력한다. (클리핑 처리)
//
// Parameters: (int)SpriteIndex. (int)DrawX. (int)DrawY. (BYTE *)Dest. (int)DestWidth. (int)DestHeight. (int)DestPitch. (int)DrawLen.
// Return: 없음.
///////////////////////////////////////////////////////
void CSpriteDib::DrawImage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen)
{
	//-----------------------------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-----------------------------------------------------------------
	if ( iSpriteIndex >= m_iMaxSprite )
		return;

	if ( NULL == m_stpSprite[iSpriteIndex].bypImage )
		return;

	st_SPRITE *stpSprite = &m_stpSprite[iSpriteIndex];

	//-----------------------------------------------------------------
	// 스프라이트 출력을 위해 사이즈 저장.
	//-----------------------------------------------------------------
	int iSpriteWidth = stpSprite->iWidth;
	int iSpriteHeight = stpSprite->iHeight;
	int iCountY;

	iSpriteWidth = iSpriteWidth * iDrawLen / 100;

	DWORD *dwpDest = (DWORD *)bypDest;
	DWORD *dwpSprite = (DWORD *)(stpSprite->bypImage);

	//-----------------------------------------------------------------
	// 상단 에 대한 스프라이트 출력 위치 계산. (상단 클리핑)
	//-----------------------------------------------------------------
	if ( 0 > iDrawY )
	{
		iSpriteHeight = iSpriteHeight - (-iDrawY);
		dwpSprite = (DWORD *)(stpSprite->bypImage + stpSprite->iPitch * (-iDrawY));
		//-----------------------------------------------------------------
		// 윗쪽이 짤리는 경우이므로 스프라이트 시작 위치를 아래로 내려준다.
		//-----------------------------------------------------------------

		iDrawY = 0;
	}

	//-----------------------------------------------------------------
	// 하단에 사이즈 계산. (하단 클리핑)
	//-----------------------------------------------------------------
	if ( iDestHeight < iDrawY + stpSprite->iHeight )
	{
		iSpriteHeight -= ((iDrawY + stpSprite->iHeight) - iDestHeight);
	}

	//-----------------------------------------------------------------
	// 왼쪽 출력 위치 계산. (좌측 클리핑)
	//-----------------------------------------------------------------
	if ( 0 > iDrawX )
	{
		iSpriteWidth = iSpriteWidth - (-iDrawX);
		dwpSprite = dwpSprite + (-iDrawX);
		//-----------------------------------------------------------------
		// 왼쪽이 조금 잘리므로 출력 시작 위치를 오른쪽으로 민다.
		//-----------------------------------------------------------------

		iDrawX = 0;
	}

	//-----------------------------------------------------------------
	// 오른쪽 출력 위치 계산. (우측 클리핑)
	//-----------------------------------------------------------------
	if ( iDestWidth < iDrawX + stpSprite->iWidth )
	{
		iSpriteWidth -= ((iDrawX + stpSprite->iWidth) - iDestHeight);
	}

	//-----------------------------------------------------------------
	// 찍을 그림이 없다면 종료한다.
	//-----------------------------------------------------------------
	if ( iSpriteWidth <= 0 || iSpriteHeight <= 0 )
		return;

	//-----------------------------------------------------------------
	// 화면에 찍을 위치로 이동한다.
	//-----------------------------------------------------------------
	dwpDest = (DWORD *)(((BYTE *)(dwpDest + iDrawX) + (iDrawY * iDestPitch)));

	//-----------------------------------------------------------------
	// 전체 크기를 돌면서 복사한다.
	//-----------------------------------------------------------------
	for ( iCountY = 0; iSpriteHeight > iCountY; iCountY++ )
	{
		memcpy(dwpDest, dwpSprite, iSpriteWidth * 4);
		//-----------------------------------------------------------------
		// 다음줄로 이동, 화면과 스프라이트 모두..
		//-----------------------------------------------------------------
		dwpDest = (DWORD *)((BYTE *)dwpDest + iDestPitch);
		dwpSprite = (DWORD *)((BYTE *)dwpSprite + stpSprite->iPitch);
	}
}

