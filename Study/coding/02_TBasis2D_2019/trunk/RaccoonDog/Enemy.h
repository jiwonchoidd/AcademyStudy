#pragma once

class CEnemy
{
public:
	int x;				
	int y;				
	int alpha;			
	char speed;			
	bool state;			
	bool type;			
public:
	CEnemy(void);
	~CEnemy(void);

	void Init(int x,int y,int alpha,int speed,bool type);
	void Render();
	void Update();
	void Moving();
};
