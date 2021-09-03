#pragma once
#include "TBitmapManager.h"
#include "TCollision.h"

typedef std::vector<RECT>  RECT_ARRAY;

enum TObjectType {
	BACKGROUND = 0,
	UICONTROL,
	USER,
	NPC,
};
enum TControlState {
	DEFAULT = 0,
	PUSH,
	SELECT,
	DISABLE,
};
enum TOverlapState {
	OVERLAP_NONE = 0,
	OVERLAP_BEGIN,
	OVERLAP_STAY,
	OVERLAP_END,
};
struct TObjAttribute
{
	int iObjType;
	wstring szName;
	wstring szParentName;
	wstring colorbitmap;
	wstring pushbitmap;
	wstring selectbitmap;
	wstring disbitmap;
	wstring maskbitmap;
	TPoint  pos;
	RECT   rtSrc;
	RECT   rtDesk;
	bool   bColorKey;
	DWORD  dwColor;
	int	   iVisible;
	int	   iEnable;
};
struct TSpriteInfo
{
	wstring		colorbitmap;
	wstring		maskbitmap;
	RECT_ARRAY	rtArray;
	wstring		szName;
	wstring		szParentName;
};
enum TCollisionType
{
	T_IGNORE = 0,		/// 무시한다.
	T_OVERLAP,			/// 겹칠 수 있다.
	T_BLOCKING			/// 중첩될 수 없고 블록킹 된다.
};
enum TSelectState
{
	T_DEFAULT	= 0,  /// 마우스가 위에 없을 때( T_FOCUS상태에서 다른 곳이 T_ACTIVE 이후 상태가 되었을 때 전환된다.)
	T_HOVER		= 1,	/// 마우스가 위에 있을 때	
	T_FOCUS		= 2,	/// T_ACTIVE에서 마우스를 왼쪽 버튼을 다른 곳에서 놓았을 때( 또는 탭, 엔터키 적용 대상, 기타 경우)	
	T_ACTIVE	= 4,	/// 마우스 좌측 버튼을 누르고 있을 때(키 상태의 KEY_HOLD와 같다)	
	T_SELECTED	= 8, /// T_ACTIVE에서 마우스를 왼쪽 버튼을 위에서 놓았을 때( 이후에는 T_FOCUS가 된다.)
};
class TObject
{
public:
	int			m_iIndex;			 /// 모든 오브젝트에 발급되는 고유한 인덱스	
	DWORD		m_dwSvrID;			 /// 서버에 등록 된 고유한 인덱스
	int			m_iObjectType;	     /// TObjectType 오브젝트 타입
public:
	int			m_iCollisionObjectID;/// 오브젝트 메니져에 등록 된 충돌 인덱스
	int			m_iSelectObjectID;	 /// 오브젝트 메니져에 등록된 마우스 선택 인덱스
	bool		m_bCollisionEnabled; /// 충돌처리여부 판단(마우스 선택은 처리된다.)
	int			m_iCollisionType;	 /// TCollisionType에 따라 처리한다.
	DWORD		m_dwOverlapState;	 /// 오버랩 상태
	DWORD		m_dwSelectState;	 /// 마우스 선택 상태
	int			m_iImageState;
public:	
	wstring     m_szName;
	wstring		m_szParentName;
	TObject*	m_pParentObject;
	TObjAttribute m_Attribute;
	std::vector<TObject*>	m_pChildObjects;
public:
	TBitmap*	m_pMaskBmp;
	TBitmap*	m_pColorBmp;
	std::vector<TBitmap*> m_StateBitmap;
	RECT		m_rtSrc;		/// 원본 이미지 영역
	RECT		m_rtDesk;		/// 초기 화면 드로우 영역
	RECT		m_rtDraw;		/// 최종 드로우 영역
	RECT		m_rtCollide;	/// 최종 충돌 영역
	bool		m_bColorKey;	/// 컬러키 사용 유무
	DWORD		m_dwColorKey;	/// 컬러키 컬러 값
	int			m_iSortValue;	/// Z값 정렬됨
public:	
	TPoint		m_ptPos;
	float		m_fSpeed;
	TPoint		m_ptDirection;
	TPoint		m_ptPreDirection;
	bool		m_bDead;
	bool		m_bSelect;
	float		m_fLifeTime;
	float		m_fAttackTimer;
	float		m_fAlphaBlend;
	bool		m_bDrawDebugRect;
public:
	/// object Index, overlap State
	std::map<int, DWORD>	m_OverlapState;
	DWORD					Overlap(int iIndex);
	virtual void			HitOverlap(TObject* pDesk, DWORD dwState);
	virtual void			MouseOverlap(POINT ptMousepos, DWORD dwState);
public:
	virtual TObject* Clone();
	virtual bool  Reset();
	virtual bool  Init();
	virtual bool  PreFrame();
	virtual bool  Frame();
	virtual bool  PostFrame();
	virtual bool  PreRender();
	virtual bool  Render();
	virtual bool  PostRender();
	virtual bool  Release();
	virtual void  Update();
	virtual void  SetDir(float* p);
	virtual void  SetPos(float* d);
	virtual void  SetDir(TPoint p);
	virtual void  SetPos(TPoint p);
	virtual void  Set(TPoint p, RECT rtSrc);

	virtual void  SetPosTL(TPoint p);
	virtual void  SetSpeed(float fSpeed);
	virtual void  Set(RECT rtSrc, RECT rtDesk);
	virtual bool  Load( const TCHAR* color,
						const TCHAR* mask =nullptr,
						DWORD dwColor= RGB(255,0,255));
	virtual void  DrawColorKey();
public:
	virtual bool Damage() { return false; };
	virtual bool Dead() { return true; };
	virtual void Attack(TObject* pPlayer) {};
public:
	virtual void SetTransition(DWORD dwEevnt) {};
public:
	TObject();
	virtual ~TObject();
};

