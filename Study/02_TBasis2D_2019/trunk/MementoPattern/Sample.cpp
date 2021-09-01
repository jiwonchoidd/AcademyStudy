//memento는 '(사람, 장소를 기억하기 위한) 기념품'이란 뜻이다.
//클래스 설계 관점에서 객체의 정보를 저장할 필요가 있을 때 적용할 수 있다.
#include <iostream>
#include <iomanip>
#include <list>
#include <iterator>
using namespace std;

#define GO_BOARD_SIZE     19

class GoMemento 
{
	friend class GoBoard;
public:
	GoMemento() 
	{
		memset(board_, 0, sizeof(int) * GO_BOARD_SIZE * GO_BOARD_SIZE);
		whiteDeadNum_ = blackDeadNum_ = 0;
		paePosX_ = paePosY_ = -1;
	}

	GoMemento(const GoMemento& rhs) { CopyBoard(rhs); }
	GoMemento& operator=(const GoMemento& rhs) { CopyBoard(rhs); }

	void GetOutDeadStone() 
	{
		// -- 확실히 죽은 돌을 골라낸다. 
		// -- 이때 whiteDeadNum_ 이나 blackDeadNum_ 값이 조정됨
	}

	bool IsPaePosition(int x, int y) 
	{
		// -- x,y 위치가 패이면 true 되돌림
		return false;
	}

protected:
	void CopyBoard(const GoMemento& src) 
	{
		memcpy(board_, &src, sizeof(int) * GO_BOARD_SIZE * GO_BOARD_SIZE);
		whiteDeadNum_ = src.whiteDeadNum_;
		blackDeadNum_ = src.blackDeadNum_;
	}
private:
	// -- 0 은 돌없음, 양수는 백, 음수는 흑, 절대값은 수순
	int board_[GO_BOARD_SIZE][GO_BOARD_SIZE];

	int whiteDeadNum_; // -- 흰돌 죽은 수
	int blackDeadNum_; // -- 검은돌 죽은 수

	int paePosX_;
	int paePosY_;
};

class GoBoard 
{   // -- 바둑판
public:
	GoBoard(int firstTurn = -1) 
	{
		pCurBoard_ = new GoMemento();
		whoseTurn_ = firstTurn;
		totalStoneNum_ = 0;
	}

	void PutStone(int x, int y) 
	{
		if (pCurBoard_->board_[x][y] != 0 ||
			(pCurBoard_->paePosX_ == x && pCurBoard_->paePosY_ == y)) 
		{
			cout << "Can't Be Put Stone There." << endl;
			return;
		}

		GoMemento *pNewBoard = new GoMemento(*pCurBoard_);
		totalStoneNum_++;
		pNewBoard->board_[x][y] = whoseTurn_ * totalStoneNum_;
		whoseTurn_ *= -1;
		if (pCurBoard_->IsPaePosition(x, y)) 
		{
			pCurBoard_->paePosX_ = x;
			pCurBoard_->paePosY_ = y;
		}
		else {
			pCurBoard_->paePosX_ = -1;
			pCurBoard_->paePosY_ = -1;
		}

		pNewBoard->GetOutDeadStone();

		historyList_.push_front(pCurBoard_);
		pCurBoard_ = pNewBoard;
	}

	void RetractStone(int cnt) 
	{
		// -------------------------------------------
		// cnt 값만큼 수를 물린다. 
		// 이때 1수를 물리는 것은 pCurBoard_ 값을 historyList_의 
		// 첫 항목과 대치하는 것과 동일하다. 
		// -------------------------------------------
		if (cnt <= 0) return;

		for (int i = 0; i < cnt - 1; i++) {
			GoMemento *pTmpBoard = historyList_.front();
			delete pTmpBoard;
			historyList_.pop_front();
			totalStoneNum_--;
		}

		delete pCurBoard_;
		totalStoneNum_--;
		if (historyList_.empty())
			pCurBoard_ = new GoMemento();
		else
			pCurBoard_ = historyList_.front();
	}

	void PrintBoard() 
	{		
		for (int i = 0; i < GO_BOARD_SIZE; i++) {
			for (int j = 0; j < GO_BOARD_SIZE; j++)
				cout << pCurBoard_->board_[i][j] << " ";
			cout << endl;
		}
		cout << "--- total stone --- " << totalStoneNum_ << endl;
	}
private:
	list<GoMemento*> historyList_;
	GoMemento* pCurBoard_;
	int whoseTurn_;    // -- 다음 둘 차례(1 은 백, -1 은 흑)
	int totalStoneNum_;
};

void main()
{
	GoBoard board;

	board.PutStone(3, 3);
	board.PutStone(16, 16);
	board.PutStone(16, 3);
	board.PutStone(3, 16);
	board.PrintBoard();

	board.RetractStone(2);
	board.PrintBoard();
}
