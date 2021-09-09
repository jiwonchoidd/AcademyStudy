#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cstdlib> // rand
#include <ctime> // time
#include <algorithm> // copy
using namespace std;
/*
공백 = 0
▦ = 1
■ = 2
이미 쌓인 블럭은 = 3
블럭이 쌓이는 맨 밑바닥 = 4
*/
#define TABLE_X 20 //테트리스 판 x 축 길이
#define TABLE_Y 38 //테트리스 판 y 축 길이
#define LEFT 75 // ←
#define RIGHT 77  // →
#define UP 72 // ↑
#define DOWN 80 // ↓
/*커서 숨기기(0) or 보이기(1) */
void CursorView(char show) {
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
/*콘솔 커서 위치 이동*/
void gotoxy(int x, int y) {
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
/*1번 블럭*/
const int block1[4][4][4] = {
        {
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {2, 2, 2, 2},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 2, 0},
                        {0, 0, 2, 0},
                        {0, 0, 2, 0},
                        {0, 0, 2, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {2, 2, 2, 2},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 2, 0},
                        {0, 0, 2, 0},
                        {0, 0, 2, 0},
                        {0, 0, 2, 0}
        },
};
/*2번 블럭*/
const int block2[4][4][4] = {
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 0},
                        {0, 2, 2, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 0},
                        {0, 2, 2, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 0},
                        {0, 2, 2, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 0},
                        {0, 2, 2, 0},
                        {0, 0, 0, 0}
        },
};
/*3번 블럭*/
const int block3[4][4][4] = {
        {
                        {0, 2, 0, 0},
                        {0, 2, 0, 0},
                        {0, 2, 2, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 2},
                        {0, 2, 0, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 2, 2, 0},
                        {0, 0, 2, 0},
                        {0, 0, 2, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 0, 2, 0},
                        {2, 2, 2, 0},
                        {0, 0, 0, 0}
        },
};
/*4번 블럭*/
const int block4[4][4][4] = {
        {
                        {0, 0, 0, 0},
                        {0, 2, 0, 0},
                        {0, 2, 2, 0},
                        {0, 0, 2, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 0},
                        {2, 2, 0, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 2, 0, 0},
                        {0, 2, 2, 0},
                        {0, 0, 2, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 0},
                        {2, 2, 0, 0},
                        {0, 0, 0, 0}
        },
};
/*5번 블럭*/
const int block5[4][4][4] = {
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 2},
                        {0, 0, 2, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 2, 0},
                        {0, 2, 2, 0},
                        {0, 0, 2, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 2, 0},
                        {0, 2, 2, 2},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 2, 0},
                        {0, 0, 2, 2},
                        {0, 0, 2, 0},
                        {0, 0, 0, 0}
        },
};
/*블럭 부모 클래스*/
class Block {
protected:
    int shape[4][4][4]; // shape[rotate][y][x]
    int x; // x좌표
    int y; // y좌표
    int rotationCount; // shape[0][y][x], shape[1][y][x], shape[2][y][x], shaoe[3][y][x]로 4가지 상태 표현
public:
    int getShape(int rotationCount, int y, int x) {
        return shape[rotationCount][y][x];
    }
    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
    int getRotationCount() {
        return rotationCount;
    }
    void down() { // 블럭 한 칸 아래 이동
        y++;
    }
    void left() { // 블럭 한 칸 왼쪽 이동
        x--;
    }
    void right() { // 블럭 한 칸 오른쪽 이동
        x++;
    }
    void rotate() { // 블럭 회전
        rotationCount = (rotationCount + 1) % 4; // 0 , 1, 2 , 3 으로 회전 표현
    }
    void setX(int x) {
        this->x = x;
    }
    void setY(int y) {
        this->y = y;
    }
    void setRotationCount(int r) {
        this->rotationCount = r;
    }
    void setShape(int r, int y, int x, int value) {
        this->shape[r][y][x] = value;
    }
};
/*1번 블럭 클래스*/
class Block1 : public Block {
public:
    Block1() {
        x = TABLE_X / 2 - 3; // 초기 생성 맨 위 중앙 쯤으로 맞춤
        y = 1;
        rotationCount = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    shape[i][j][k] = block1[i][j][k]; // 블럭 객체 정보 저장
                }
            }
        }
    }
};
/*2번 블럭 클래스*/
class Block2 : public Block {
public:
    Block2() {
        x = TABLE_X / 2 - 3; // 초기 생성 맨 위 중앙 쯤으로 맞춤
        y = 1;
        rotationCount = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    shape[i][j][k] = block2[i][j][k]; // 블럭 객체 정보 저장
                }
            }
        }
    }
};
/*3번 블럭 클래스*/
class Block3 : public Block {
public:
    Block3() {
        x = TABLE_X / 2 - 3; // 초기 생성 맨 위 중앙 쯤으로 맞춤
        y = 1;
        rotationCount = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    shape[i][j][k] = block3[i][j][k]; // 블럭 객체 정보 저장
                }
            }
        }
    }
};
/*4번 블럭 클래스*/
class Block4 : public Block {
public:
    Block4() {
        x = TABLE_X / 2 - 3; // 초기 생성 맨 위 중앙 쯤으로 맞춤
        y = 1;
        rotationCount = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    shape[i][j][k] = block4[i][j][k]; // 블럭 객체 정보 저장
                }
            }
        }
    }
};
/*5번 블럭 클래스*/
class Block5 : public Block {
public:
    Block5() {
        x = TABLE_X / 2 - 3; // 초기 생성 맨 위 중앙 쯤으로 맞춤
        y = 1;
        rotationCount = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    shape[i][j][k] = block5[i][j][k]; // 블럭 객체 정보 저장
                }
            }
        }
    }
};
/*메인 메뉴 클래스*/
class MainMenu {
public:
    MainMenu() {
        cout << "\n\n\n\n";
        cout << "\t\t"; cout << "@@@@@@@@@@@@  @@@@@@@@@   @@@@@@@@@@@  @@@@@@@@   @   @@@@@@@@@@@\n";
        cout << "\t\t"; cout << "      @       @                @       @      @   @   @          \n";
        cout << "\t\t"; cout << "      @       @                @       @      @   @   @          \n";
        cout << "\t\t"; cout << "      @       @@@@@@@@@        @       @     @    @   @@@@@@@@@@@\n";
        cout << "\t\t"; cout << "      @       @                @       @ @ @      @             @\n";
        cout << "\t\t"; cout << "      @       @                @       @     @    @             @\n";
        cout << "\t\t"; cout << "      @       @@@@@@@@@        @       @      @   @   @@@@@@@@@@@\n\n\n\n\n";
        cout << "\t\t"; cout << "                게임을 시작하려면 아무키나 누르세요.\n\n\n\n\n\n\n";
        cout << "\t\t"; cout << "                   TetrisGame1.3 By SeokJinLee\n";
        getchar(); // 아무키 입력 기다림
        system("cls"); // 콘솔 창 clear
    }
};
/*블럭, table 백업용 클래스*/
class backup {
public:
    /*블럭 백업*/
    static void updateBlock(Block* source, Block& backupBlock) {
        backupBlock.setX(source->getX()); // 블럭의 x좌표 백업
        backupBlock.setY(source->getY()); // 블럭의 y좌표 백업
        backupBlock.setRotationCount(source->getRotationCount()); // 블럭의 회전 상태 변수 백업
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    backupBlock.setShape(i, j, k, source->getShape(i, j, k)); // 블럭의 모양 백업
                }
            }
        }
    }
    /*table 백업*/
    static void updateTable(vector<vector<int> >& source, vector<vector<int> >& backupTable) {
        backupTable.resize(source.size(), vector<int>(source.size())); // 기존 table의 크기만큼 2차원 벡터 크기 할당
        copy(source.begin(), source.end(), backupTable.begin()); // 기존 table vector을 backupTable vector에 백업
    }
};
/*테트리스 게임 테이블 클래스*/
class GameTable {
private:
    int x; // 가로
    int y; // 세로
    Block* blockObject;
    vector<vector<int> > table; // 블럭 정보 저장시 table[y][x]
public:
    GameTable(int x, int y) { //테트리스 판 뼈대 생성
        blockObject = nullptr;
        this->x = x;
        this->y = y;
        for (int i = 0; i < y; i++) {
            vector<int> temp;
            for (int j = 0; j < x; j++) {
                temp.push_back(0);
            }
            table.push_back(temp);
        }
        //가장 자리 뼈대 색칠
        for (int i = 0; i < x; i++) {
            table[0][i] = 1;
            table[y - 1][i] = 1;
        }
        for (int i = 1; i < y - 1; i++) {
            table[i][0] = 1;
            table[i][x - 1] = 1;
        }
        /*맨 밑바닥 감지용 4*/
        for (int i = 1; i < x - 1; i++) {
            table[y - 1][i] = 4; // 맽 밑의 값을 4
        }
    }
    /*게임판 그리는 함수*/
    void DrawGameTable() {
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x; j++) {
                if (table[i][j] == 1 || table[i][j] == 4) cout << "▦";
                else if (table[i][j] == 2 || table[i][j] == 3) cout << "■";
                else cout << "  "; // 두 칸 띄우기
            }
            cout << "\n";
        }
    }
    /*블럭 생성*/
    void createBlock() {
        srand((unsigned int)time(NULL));
        int select = rand() % 5 + 1; // 1 ~ 5 블럭
        if (select == 1) blockObject = new Block1(); // 1번 블럭 생성
        else if (select == 2)blockObject = new Block2(); // 2번 블럭 생성
        else if (select == 3)blockObject = new Block3(); // 3번 블럭 생성
        else if (select == 4)blockObject = new Block4(); // 4번 블럭 생성
        else if (select == 5)blockObject = new Block5(); // 5번 블럭 생성
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int Y = j + blockObject->getY();
                int X = i + blockObject->getX();
                table[Y][X] = blockObject->getShape(blockObject->getRotationCount(), i, j);
                //블럭 객체를 테이블에 업데이트
            }
        }
    }
    /*블럭 이동*/
    void MoveBlock(int key) {
        /*백업*/
        Block backupBlock; // 백업용 Block 객체
        vector<vector<int> > backupTable; // 백업용 table vector
        backup::updateBlock(blockObject, backupBlock); // block 백업
        backup::updateTable(table, backupTable); // table 백업
        /*테이블에서 블럭 객체 지우기*/
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int Y = j + blockObject->getY();
                int X = i + blockObject->getX();
                if (table[Y][X] == 2) { // 만약 블럭이면
                    table[Y][X] = 0; // 테이블에서 지운다
                }
            }
        }
        /*블럭 이동*/
        if (key == DOWN) blockObject->down(); // 만약 인자로 들어온 key가 아랫 방향이면 블럭을 아래로 이동
        else if (key == LEFT) blockObject->left();  // 만약 인자로 들어온 key가 왼쪽 방향이면 블럭을 왼쪽으로 이동
        else if (key == RIGHT) blockObject->right(); // 만약 인자로 들어온 key가 오른쪽 방향이면 블럭을 오른쪽으로 이동
        /*이동한 블럭 상태를 테이블에 갱신 or 취소*/
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int Y = j + blockObject->getY();
                int X = i + blockObject->getX();
                int blockValue = blockObject->getShape(blockObject->getRotationCount(), i, j); //블럭 배열 값 얻기
                if (blockValue != 2) continue; // 블럭이 아니면 무시 (블럭은 2로 이루어져있음)
                if (table[Y][X] == 0) { // 빈공간이면 (갱신)
                    table[Y][X] = blockValue; // 블럭을 이동시킴
                }
                else if (table[Y][X] == 1) { // 블럭이 양 옆 벽면에 닿으면 (취소)
                    copy(backupTable.begin(), backupTable.end(), table.begin()); // table 백업
                    blockObject->setX(backupBlock.getX()); // 블럭 x 좌표 백업
                    blockObject->setY(backupBlock.getY()); // 블럭 y 좌표 백업
                    return; // 함수 종료
                }
                else if (table[Y][X] == 3) { // 이미 쌓여진 블럭과 접촉하면
                    copy(backupTable.begin(), backupTable.end(), table.begin()); // table 백업
                    blockObject->setX(backupBlock.getX()); // 블럭 x 좌표 백업
                    blockObject->setY(backupBlock.getY()); // 블럭 y 좌표 백업
                    if (key == DOWN) { // 만약 아랫 방향일 경우에
                        BuildBlock(); // 블럭을 쌓고
                        createBlock(); // 새로운 블럭을 만듬
                    }
                    return; // 함수 종료
                }
                else if (table[Y][X] == 4) { //만약에 맨 밑바닥에 접촉했으면
                    copy(backupTable.begin(), backupTable.end(), table.begin()); // table 백업
                    blockObject->setX(backupBlock.getX()); // 블럭 x 좌표 백업
                    blockObject->setY(backupBlock.getY()); // 블럭 y 좌표 백업
                    if (key == DOWN) { // 만약 아랫 방향일 경우에
                        BuildBlock(); // 블럭을 쌓고
                        createBlock(); // 새로운 블럭을 만듬
                    }
                    return; // 함수 종료
                }
            }
        }
    }
    /*블럭 회전*/
    void RotateBlock() {
        /*백업*/
        Block backupBlock; // 백업용 Block 객체
        vector<vector<int> > backupTable; // 백업용 table vector
        backup::updateBlock(blockObject, backupBlock); // block 백업
        backup::updateTable(table, backupTable); // table 백업
        /*테이블에서 블럭 객체 지우기*/
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int Y = j + blockObject->getY();
                int X = i + blockObject->getX();
                if (table[Y][X] == 2) { // 만약 블럭이면
                    table[Y][X] = 0; // 테이블에서 지운다
                }
            }
        }
        /*블럭 회전*/
        blockObject->rotate(); // 블럭을 회전
        /*회전한 블럭 상태를 테이블에 갱신 및 취소*/
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int Y = j + blockObject->getY();
                int X = i + blockObject->getX();
                int blockValue = blockObject->getShape(blockObject->getRotationCount(), i, j); //블럭 배열 값 얻기
                if (blockValue != 2) continue; // 블럭이 아니면 무시 (블럭은 2로 이루어져있음)
                if (table[Y][X] == 0) { //빈공간인 경우에 이동한 블럭 정보를 테이블에 갱신
                    table[Y][X] = blockObject->getShape(blockObject->getRotationCount(), i, j);
                }
                else if (table[Y][X] == 1 || table[Y][X] == 3 || table[Y][X] == 4) { // 블럭&블럭, 블럭&벽 닿을 시 취소
                    copy(backupTable.begin(), backupTable.end(), table.begin()); // table 백업
                    blockObject->setRotationCount(backupBlock.getRotationCount()); // 회전하기 전 상태로 백업
                    return; // 업데이트 취소
                }
            }
        }
    }
    /*블럭을 table에 쌓기*/
    void BuildBlock() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int Y = j + blockObject->getY();
                int X = i + blockObject->getX();
                int blockValue = blockObject->getShape(blockObject->getRotationCount(), i, j); //블럭 배열 값 얻기
                if (blockValue != 2) continue; // 블럭이 아니면 무시 (블럭은 2로 이루어져있음)
                table[Y][X] = 3;
            }
        }
    }

};
/*게임 시작 클래스*/
class GamePlay {
private:
    GameTable* gt;
public:
    GamePlay() {
        gt = new GameTable(TABLE_X, TABLE_Y); //게임 판 그리기 객체 생성
        gt->createBlock(); // 초기 블럭 생성
        gt->DrawGameTable(); // 게임판을 그린다.
        while (true) { // 방향키 입력 이벤트
            int nSelect;
            if (_kbhit()) {
                nSelect = _getch();
                if (nSelect == 224) {
                    nSelect = _getch();
                    switch (nSelect) {
                    case UP: // 화살표 위 눌렀을 때
                        gt->RotateBlock(); // 블럭을 90도 회전 
                        break;
                    case DOWN: // 화살표 아래 눌렀을 때
                        gt->MoveBlock(DOWN); // 블럭을 한 칸 아래로 이동
                        break;
                    case LEFT: // 화살표 왼쪽 눌렀을 떄
                        gt->MoveBlock(LEFT); // 블럭을 한 칸 왼쪽으로 이동
                        break;
                    case RIGHT: // 화살표 오른쪽 눌렀을 때
                        gt->MoveBlock(RIGHT); // 블럭을 한 칸 오른쪽으로 이동
                        break;
                    default:
                        break;
                    }
                }
            }
            gotoxy(0, 0); //system("cls") 안쓰고 (0, 0)으로 커서 이동 후
            gt->DrawGameTable(); // 다시 그리기
        }
    }
    ~GamePlay() { // 게임 종료 이벤트
        delete gt;
    }
};
int main(void) {
    CursorView(false); // 콘솔 화면 커서 제거 
    system("mode con cols=100 lines=40 | title 테트리스 게임"); // 콘솔창 크기 및 제목 설정
    GameTable gt(TABLE_X, TABLE_Y);
    MainMenu(); // 메인 메뉴 그리기 생성자 호출
    GamePlay(); // 게임 플레이
    getchar();
    return 0;
}
