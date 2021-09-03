#include "TFiniteState.h"
#include "TFiniteStateMachine.h"

TFiniteState::TFiniteState( DWORD state )
	: m_dwState( state )
{
}

TFiniteState::~TFiniteState()
{
	std::map < DWORD, DWORD >::iterator	iter, iterPrev;

	//  ��� ������ �����Ѵ�.
	iter = m_Map.begin();
	while( iter != m_Map.end() )
	{
		iterPrev = iter++;

		m_Map.erase( iterPrev );
	}
}

void TFiniteState::AddTransition( DWORD inputEvent, DWORD outputStateID )
{
	m_Map[inputEvent] = outputStateID;
}

void TFiniteState::DeleteTransition( DWORD inputEvent )
{
	m_Map.erase( inputEvent );
}
// �Լ��� ���� �ڿ� throw(..)�� ���̸� � ���ܸ� ������ �Լ����� �� �� �ִ�.
// throw() : ���ܰ� ����.
// throw(char*, int) : ���ܸ� char*�� �� int�� �����Եȴ�.
//���� �������� ���� ���ܰ� �߻��Ѵٸ� �̶��� unexpected��� �Լ��� ȣ��Ǿ� ������ ���ܸ� ó���Ѵ�.
//unexcepted�� ����Ʈ�� terminate�� ȣ���Ͽ� ���α׷��� ������ �����ϴµ� ���� �Լ��� ����ϸ� ��ó�� ���� �ڵ鷯�� ������ �� �ִ�.
// SampleCode
// unexpected_handler set_unexpected(unexpected_handler ph)
//#include <exception>
//using namespace std;
//void myunex(){	puts("�߻��ؼ��� �ȵǴ� ���� �߻�");	exit(-2);}
//void calc() throw(int){	throw "string";}
//void main(){
//	set_unexpected(myunex);
//	try {	calc();	}	catch (int) {puts("������ ���� �߻�");}
//	puts("���α׷� ����");
//}

DWORD TFiniteState::Output( DWORD inputEvent ) throw ( InvalidInputEventException* )
{
	std::map < DWORD, DWORD >::iterator	iter;

	//  ���� ���ǿ� �ش��ϴ� ���� ���¸� ã�Ƴ���.
	iter = m_Map.find( inputEvent );
	if( iter == m_Map.end() )
		throw ( new InvalidInputEventException( inputEvent ) );

	return m_Map[inputEvent];
}

DWORD TFiniteState::GetCount()
{
	return m_Map.size();
}