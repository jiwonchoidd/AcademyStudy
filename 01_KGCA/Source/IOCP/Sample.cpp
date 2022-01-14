/*3번 청소끝났어요 그럼 3번 테이블 배치하고 다른 작업못함
파일을 소켓으로 이용하면 그대로 이용 가능하다.

완료되면 비동기 입출력 작업결과 저장 큐가 있음
입출력이 끝나면 데이터가 저장이 됨

스레드에서는 완료큐에 데이터가 있으면 작업한다.

완료 포트 관리 객체 생성
생성할떄는 첫번째에 INVALID_HANDLE_VALUE 초기화 값임.
비동기 결과 작업이 저장이된다. hiocp로 연결해서 
3문장으로 끝나버린것이다. Completionport 완료 포트
파일 포트가 완료가 되면 앞으로 1000번이라는 번호를 남겨줘
이걸 1000번을 완료 포트라고 한다. 1000번 작업 끝났어 리턴해준다.

GetQueuedCompletionStatus, iocp 핸들, 트랜스퍼, 키값, 오버랩, 1
키값이 1000번이면 비동기 작업 읽기가 완료되면 여기서 반환
Transfer는 얼만큼 읽었는디 얼마큼 썻는지
*/
#include <windows.h>
#define MAX_WORKER_THREAD 3
#define BlockSizes 4096

HANDLE h_hIOCP;
DWORD WINAPI WorkerThread(LPVOID param)
{
	BOOL bRet = ::GetQueuedCompletionStatus();
	if (bRet == true)
	{
		//작업
	}
	else
	{
		//오류
	}
}
int main()
{
	for (int i = 0; i < MAX_WORKER_THREAD; i++)
	{
		DWORD id;

		CloseHandle(::CreateThread(0, 0, WorkerThread, nullptr, 0, &id));

	}
}
