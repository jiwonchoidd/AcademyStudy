/*3�� û�ҳ������ �׷� 3�� ���̺� ��ġ�ϰ� �ٸ� �۾�����
������ �������� �̿��ϸ� �״�� �̿� �����ϴ�.

�Ϸ�Ǹ� �񵿱� ����� �۾���� ���� ť�� ����
������� ������ �����Ͱ� ������ ��

�����忡���� �Ϸ�ť�� �����Ͱ� ������ �۾��Ѵ�.

�Ϸ� ��Ʈ ���� ��ü ����
�����ҋ��� ù��°�� INVALID_HANDLE_VALUE �ʱ�ȭ ����.
�񵿱� ��� �۾��� �����̵ȴ�. hiocp�� �����ؼ� 
3�������� �����������̴�. Completionport �Ϸ� ��Ʈ
���� ��Ʈ�� �Ϸᰡ �Ǹ� ������ 1000���̶�� ��ȣ�� ������
�̰� 1000���� �Ϸ� ��Ʈ��� �Ѵ�. 1000�� �۾� ������ �������ش�.

GetQueuedCompletionStatus, iocp �ڵ�, Ʈ������, Ű��, ������, 1
Ű���� 1000���̸� �񵿱� �۾� �бⰡ �Ϸ�Ǹ� ���⼭ ��ȯ
Transfer�� ��ŭ �о��µ� ��ŭ ������
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
		//�۾�
	}
	else
	{
		//����
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
