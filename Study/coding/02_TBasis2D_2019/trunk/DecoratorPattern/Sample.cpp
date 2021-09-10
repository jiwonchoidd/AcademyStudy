//기존에 구현되어 있는 클래스(둥근 모양의 빵)에 그때그때 필요한 기능(초콜릿, 치즈, 생크림)을 
//추가(장식, 포장)해나가는 설계 패턴을 decorator 패턴이라고 한다.
//이것은 기능 확장이 필요할 때 상속의 대안으로 사용한다.

#include <iostream>
using namespace std;

// 아래의 코드는 기본코드
//class Airplane 
//{
//public:
//	void AddItem(Item* pItem) {
//		direction_ += pItem->GetDirection();
//	}
//
//	void Attack() 
//	{
//		if (direction_ & SIDE_DIRECTION) { 
//			// -- 측방 공격 수행
//			cout << "측방공격" << endl;
//		}
//		if (direction_ & REAR_DIRECTION) {
//			// -- 후방 공격 수행
//			cout << "후방공격" << endl;
//		}//
//		// -- 전방 공격 수행
//		cout << "전방공격" << endl;
//	}
//
//private:
//	int direction_;
//};

#include <iostream>
#include <vector>
using namespace std;
class Airplane
{
public:
	virtual void Attack() = 0;
};
class FrontAttackAirplane : public Airplane
{
public:
	void Attack()
	{
		std::cout << "전방공격" << std::endl;
	}
};
class Decorator : public Airplane
{
public:
	Decorator(Airplane* pObj)
	{
		m_AttackList.push_back(pObj);
	}
	virtual void Attack()
	{
		for (int iAttack = 0; iAttack < m_AttackList.size();
			iAttack++)
		{
			m_AttackList[iAttack]->Attack();
		}
	}
	void AddAttack(Airplane* pObj)
	{
		m_AttackList.push_back(pObj);
	}
	void DelAttack(Airplane* pObj)
	{
		std::vector<Airplane*>::iterator iter;
		iter = find(m_AttackList.begin(),
			m_AttackList.end(),
			pObj);
		m_AttackList.erase(iter);
	}
protected:
	std::vector<Airplane*> 	m_AttackList;
};
class SideAttackAirplane : public Decorator
{
public:
	SideAttackAirplane(Airplane* pAirPlane) :
		Decorator(pAirPlane)
	{
	}
	void Attack()
	{
		Decorator::Attack();
		std::cout << "측방공격" << std::endl;
	}
};
class RearAttackAirplane : public Decorator
{
public:
	RearAttackAirplane(Airplane* pAirPlane) :
		Decorator(pAirPlane)
	{
	}
	void Attack()
	{
		Decorator::Attack();
		std::cout << "후방공격" << std::endl;
	}
};

class User
{
	Airplane* m_pAttack;
public:
	User() {
		m_pAttack = 0;
	}
	void DeleteAttack()
	{
		delete m_pAttack;
		m_pAttack = 0;
	}
	void SetAttack(Airplane* pAirPlane)
	{
		m_pAttack = pAirPlane;
	}
	void AddAttack(Airplane* pAirPlane)
	{
		Decorator* pAdd = (Decorator*)m_pAttack;
		pAdd->AddAttack(pAirPlane);
	}
	void DelAttack(Airplane* pAirPlane)
	{
		Decorator* pAdd = (Decorator*)m_pAttack;
		pAdd->DelAttack(pAirPlane);
	}
	void AttackAction()
	{
		m_pAttack->Attack();
	}
};
void main()
{
	Airplane* userFront = new FrontAttackAirplane;
	Airplane* userSide = new SideAttackAirplane(userFront);
	Airplane* rearSide = new RearAttackAirplane(userSide);

	User user;
	user.SetAttack(userFront);
	user.AttackAction();

	user.SetAttack(userSide);
	user.AddAttack(userFront);
	user.AttackAction();
	user.DelAttack(userFront);
	user.AttackAction();

	user.SetAttack(rearSide);
	user.AttackAction();

	delete userFront;
	delete userSide;
	delete rearSide;
}
