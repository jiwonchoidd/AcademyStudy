#pragma once

class CRacoon
{
public:
	int x;				// x��ǥ
	int y;				// y��ǥ 
	char state;			// �ʱ��� ���� (���� , ������ , ���� ......) [�ʱ����������̵� ����]
	bool step;			// �����϶� ���ϸ��̼��� ���� 
	char speedx;		// �ӵ� 
	char speedy;		// �ӵ� 
public:
	CRacoon(void);
	~CRacoon(void);

	void Init();

	void HitLeftButton();
	void HitRightButton();
	void HitJumpButton();
};


//�����Ҷ� ��ǥ  
struct jump {
	char x;
	char y;
	char frame;	
};

// ���ڸ� ���� 
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

//���� ª�� ���� 
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

//���� �� ���� 
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