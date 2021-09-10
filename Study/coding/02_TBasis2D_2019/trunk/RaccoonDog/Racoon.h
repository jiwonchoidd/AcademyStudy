#pragma once

class CRacoon
{
public:
	int x;				// x좌표
	int y;				// y좌표 
	char state;			// 너구리 상태 (왼쪽 , 오른쪽 , 점프 ......) [너구리상태전이도 참고]
	bool step;			// 움직일때 에니메이션을 위해 
	char speedx;		// 속도 
	char speedy;		// 속도 
public:
	CRacoon(void);
	~CRacoon(void);

	void Init();

	void HitLeftButton();
	void HitRightButton();
	void HitJumpButton();
};


//점프할때 좌표  
struct jump {
	char x;
	char y;
	char frame;	
};

// 제자리 점프 
static jump _StandJump[10] = {
	{ 1,-5,0},			
	{ 1,-5,0},
	{-1,-5,0},
	{-1,-5,1},
	{-1,-5,1},	
	{-1, 5,1},
	{ 1, 5,1},
	{ 1, 5,0},
	{ 0, 5,0},		
	{ 0, 5,0}
};

//왼쪽 짧은 점프 
static jump _LeftShortJump[11] = {
	{-5,-5,0},			
	{-5,-4,0},
	{-5,-4,1},
	{-5,-4,1},
	{-5,-3,1},
	{ 0, 0,1},	
	{-5, 3,2},
	{-5, 4,2},
	{-5, 4,2},
	{-5, 4,2},		
	{-5, 5,3}
};

//왼쪽 긴 점프 
static jump _LeftLongJump[17] = {
	{-5,-6,0},			
	{-5,-6,0},
	{-5,-5,0},
	{-5,-5,1},
	{-5,-4,1},
	{-5,-3,1},
	{-5,-2,1},
	{-5,-1,1},	
	{ 0, 0,1},	
	{-5,1,2},			
	{-5,2,2},
	{-5,3,2},
	{-5,4,2},
	{-5,5,2},
	{-5,5,2},
	{-5,6,2},
	{-5,6,3}
};