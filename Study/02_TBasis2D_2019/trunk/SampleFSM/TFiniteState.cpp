#include "TFiniteState.h"
#include "TFiniteStateMachine.h"

TFiniteState::TFiniteState( DWORD state )
	: m_dwState( state )
{
}

TFiniteState::~TFiniteState()
{
	std::map < DWORD, DWORD >::iterator	iter, iterPrev;

	//  모든 정보를 제거한다.
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
// 함수의 원형 뒤에 throw(..)를 붙이면 어떤 예외를 던지는 함수인지 알 수 있다.
// throw() : 예외가 없다.
// throw(char*, int) : 예외를 char*형 및 int로 던지게된다.
//만약 지정하지 않은 예외가 발생한다면 이때는 unexpected라는 함수가 호출되어 미지정 예외를 처리한다.
//unexcepted는 디폴트로 terminate를 호출하여 프로그램을 강제로 종료하는데 다음 함수를 사용하면 미처리 예외 핸들러를 변경할 수 있다.
// SampleCode
// unexpected_handler set_unexpected(unexpected_handler ph)
//#include <exception>
//using namespace std;
//void myunex(){	puts("발생해서는 안되는 에러 발생");	exit(-2);}
//void calc() throw(int){	throw "string";}
//void main(){
//	set_unexpected(myunex);
//	try {	calc();	}	catch (int) {puts("정수형 예외 발생");}
//	puts("프로그램 종료");
//}

DWORD TFiniteState::Output( DWORD inputEvent ) throw ( InvalidInputEventException* )
{
	std::map < DWORD, DWORD >::iterator	iter;

	//  전이 조건에 해당하는 전이 상태를 찾아낸다.
	iter = m_Map.find( inputEvent );
	if( iter == m_Map.end() )
		throw ( new InvalidInputEventException( inputEvent ) );

	return m_Map[inputEvent];
}

DWORD TFiniteState::GetCount()
{
	return m_Map.size();
}