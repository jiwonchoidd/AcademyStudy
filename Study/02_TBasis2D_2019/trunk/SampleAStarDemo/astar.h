/////////////////////////////////////////////////////////////////////////////
//////////////// ASTAR.h ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifndef ASTAR_H
#define ASTAR_H
#include <stdio.h>
#include <stdlib.h>
#define  SHIFT 4        //Ÿ�ϻ����� 16(2*2*2*2)����Ʈ ���� 
#define TILESIZE 16     //Ÿ�ϻ�����  
#define COLS 30         //���� �迭�� 
#define ROWS 40         //���� �迭�� 

//A*  ��� ���� 
class ASTAR
{
	private:
		//��ǥ������ ��θ� �����ϱ� ���� ����ü 
		struct NODE 
		{    
			long f, h;           // f=���Լ�(��ǥ�Ÿ� +��߰Ÿ� ),h=��ǥ�Ÿ�(���� ��ġ���� ��ǥ��ġ������ �Ÿ�)
	   		int  g, tmpg;        // g=��߰Ÿ�(��������� �� ��ġ������ ����) tmpg=�ӽ� ��߰Ÿ� ����(���ϱ� ���Ͽ�) 
			int x, y;            // �ӽú��� 
			int NodeNum;         // Ÿ�Ϲ�ȣ 
   			NODE *Parent;        // �θ� ��� ������ 
    	    NODE *Child[4];      // �ڽ� ��� ���� (4���� üũ) 
			NODE *NextNode;      // ���� ��� ������ 
		};
		//OPEN,CLOSED�� �ѹ� ���İ� Ÿ���� �ٽ� �� ������ �ϱ� ���Ͽ� ���� 
		NODE *OPEN;              // �ּ��� ���Լ��� ���� ��带 �����ϱ� ���� ��ҷ� ��� 
		NODE *CLOSED;            // �ߺ��� ��带 �����ϱ� ���� ��ҷ� ��� 
		NODE *PATH;              // A*���� �ּ��� ��尡 ����ȴ�. �̰��� ������ ����� �����Ѵ�.

		
		struct STACK                
		{     
	   		NODE *NodePtr;
			STACK *NextStackPtr;
		};
		STACK *Stack;
	    BOOL CHECKPATH;                                             //4������ 1���̶� ���� ������ ���� 1�� �ȴ�.
	

      int Rows,Cols,TOTAL_TILES;                                    //����,����,�� Ÿ�ϼ�
      int TileMap[COLS][ROWS];                                      //�ʹ迭�� �޾� �����Ѵ�.
	 

	public:
   		ASTAR(int TMP[COLS][ROWS],int cols,int rows);               //������ 
		~ASTAR();                                                   //�Ҹ��� 
		BOOL NewPath(int sx, int sy, int dx, int dy);                  //sx,sy�����ǥ ,dx,dy��ǥ ��ǥ (���콺 �Է½ø� �����.)
		BOOL NewPath2(int sx, int sy, int dx, int dy,int x,int y);     //NewPath() �� ���� ����� �ϳ�, �� ���� ȸ�Ǹ� ���Ͽ�  
		                                                               //�� ������ǥ(x,y)�� �μ��� �Ѵ�. 
		                                                               //�� ������ġ�� Ÿ���� ���� ���� Ÿ�Ϸ� �ȴ�.
		BOOL ReachedGoal(void);                                        //��ǥ�� �����Ͽ����� üũ�Ѵ�.
		                                                               //�߿��� ���� ���� ��ǥ�Ӹ��ƴ϶� ���� ��忡 �����Ͽ��� ���� ���ȴ�.
		                                                               //���� �����Ͽ��ٸ� ���� ��带 �ٽ� ���� �´�.

		void PathNextNode(void) { PATH=PATH->Parent; }                 // ���� ������ ���(��ġ)�� ���� �´�
		
		int NodeGetX(void)      { return PATH->x; }                    //���� ����� x ��ǥ.
		int NodeGetY(void)      { return PATH->y; }                    //���� ����� y ��ǥ.
		int TileNum(int x, int y);                                     //Ÿ�Ϲ�ȣ�� �����Ѵ�.
		int FreeTile(int x, int y);                                    //x,y��ġ�� Ÿ���� ���� �ִ� Ÿ������ �˾ƺ���.
	private:
       void FreeNodes(void);                                           //��� ��带 ����.(�ʱ�ȭ)
       void FindPath(int sx, int sy, int dx, int dy);                  //���� ã�´�.(������ǥ�� ������ǥ��ǥ ) 
	   void DIRECTIONS(NODE *BestNode, int dx, int dy);                //Ÿ�Ͽ��� ���� �ִ� 4������ Ÿ���� ���� �ִ� Ÿ������ �˾ƺ���.
	   void DIRECTIONS_SUCCESS(NODE *BestNode,int x, int y, int dx, int dy);   //���� ����� ���� �ִ� Ÿ���̶�� �� Ÿ���� ������ ���Ѵ�.
	   void Insert(NODE *Successor);                                   //���ÿ��� ��带 �����´�,
	   void CHILD_NODE_CREATE(NODE *Old);                              //�ڽĳ�带 �����Ѵ�.
       void Push(NODE *Node);                                          //���ÿ� ���� �ִ´�.
	   NODE *Pop(void);
	   NODE *ReturnBestNode(void);                                     //�ּ��� ��带 ���Ѵ�.
	   NODE *CheckOPEN(int tilenum);                                   //OPEN ��忡 �ִ��� �˾ƺ���.
	   NODE *CheckCLOSED(int tilenum);                                 //CLOSED��忡 �ִ��� �˾ƺ���.
	   
};
#endif 