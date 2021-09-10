/////////////////////////////////////////////////////////////////////////////
//////////////// ASTAR.h ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifndef ASTAR_H
#define ASTAR_H
#include <stdio.h>
#include <stdlib.h>
#define  SHIFT 4        //타일사이즈 16(2*2*2*2)시프트 연산 
#define TILESIZE 16     //타일사이즈  
#define COLS 30         //세로 배열수 
#define ROWS 40         //가로 배열수 

//A*  헤더 파일 
class ASTAR
{
	private:
		//목표까지의 경로를 저장하기 위한 구조체 
		struct NODE 
		{    
			long f, h;           // f=평가함수(목표거리 +출발거리 ),h=목표거리(현재 위치에서 목표위치까지의 거리)
	   		int  g, tmpg;        // g=출발거리(출발점에서 현 위치가지의 깊이) tmpg=임시 출발거리 저장(비교하기 위하여) 
			int x, y;            // 임시변수 
			int NodeNum;         // 타일번호 
   			NODE *Parent;        // 부모 노드 포인터 
    	    NODE *Child[4];      // 자식 노드 저장 (4방향 체크) 
			NODE *NextNode;      // 다음 노드 포인터 
		};
		//OPEN,CLOSED는 한번 거쳐간 타일을 다시 안 가도록 하기 위하여 선언 
		NODE *OPEN;              // 최선의 평가함수를 가진 노드를 선택하기 위한 장소로 사용 
		NODE *CLOSED;            // 중복된 노드를 제거하기 위한 장소로 사용 
		NODE *PATH;              // A*에서 최선의 노드가 저장된다. 이것을 길찻기 결과로 리턴한다.

		
		struct STACK                
		{     
	   		NODE *NodePtr;
			STACK *NextStackPtr;
		};
		STACK *Stack;
	    BOOL CHECKPATH;                                             //4방향중 1곳이라도 갈수 없으면 값이 1인 된다.
	

      int Rows,Cols,TOTAL_TILES;                                    //가로,세로,총 타일수
      int TileMap[COLS][ROWS];                                      //맵배열을 받아 저장한다.
	 

	public:
   		ASTAR(int TMP[COLS][ROWS],int cols,int rows);               //생성자 
		~ASTAR();                                                   //소멸자 
		BOOL NewPath(int sx, int sy, int dx, int dy);                  //sx,sy출발좌표 ,dx,dy목표 좌표 (마우스 입력시만 실행됨.)
		BOOL NewPath2(int sx, int sy, int dx, int dy,int x,int y);     //NewPath() 와 같은 기능을 하나, 적 유닛 회피를 위하여  
		                                                               //적 유닛좌표(x,y)를 인수로 한다. 
		                                                               //적 유닛위치의 타일은 갈수 없는 타일로 된다.
		BOOL ReachedGoal(void);                                        //목표에 도착하였는지 체크한다.
		                                                               //중요한 것은 최종 목표뿐만아니라 다음 노드에 도착하였을 때도 사용된다.
		                                                               //만약 도착하였다면 다음 노드를 다시 가져 온다.

		void PathNextNode(void) { PATH=PATH->Parent; }                 // 다음 가야할 노드(위치)를 가져 온다
		
		int NodeGetX(void)      { return PATH->x; }                    //다음 노드의 x 좌표.
		int NodeGetY(void)      { return PATH->y; }                    //다음 노드의 y 좌표.
		int TileNum(int x, int y);                                     //타일번호를 리턴한다.
		int FreeTile(int x, int y);                                    //x,y위치의 타일이 갈수 있는 타일인지 알아본다.
	private:
       void FreeNodes(void);                                           //모든 노드를 비운다.(초기화)
       void FindPath(int sx, int sy, int dx, int dy);                  //길을 찾는다.(시작좌표와 최종목표좌표 ) 
	   void DIRECTIONS(NODE *BestNode, int dx, int dy);                //타일에서 갈수 있는 4방향의 타일중 갈수 있는 타일인지 알아본다.
	   void DIRECTIONS_SUCCESS(NODE *BestNode,int x, int y, int dx, int dy);   //위의 결과로 갈수 있는 타일이라면 그 타일의 정보를 구한다.
	   void Insert(NODE *Successor);                                   //스택에서 노드를 가져온다,
	   void CHILD_NODE_CREATE(NODE *Old);                              //자식노드를 생성한다.
       void Push(NODE *Node);                                          //스택에 집어 넣는다.
	   NODE *Pop(void);
	   NODE *ReturnBestNode(void);                                     //최선의 노드를 구한다.
	   NODE *CheckOPEN(int tilenum);                                   //OPEN 노드에 있는지 알아본다.
	   NODE *CheckCLOSED(int tilenum);                                 //CLOSED노드에 있는지 알아본다.
	   
};
#endif 