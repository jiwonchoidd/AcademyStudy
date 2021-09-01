//������ �����Ǿ� �ִ� Ŭ����(�ձ� ����� ��)�� �׶��׶� �ʿ��� ���(���ݸ�, ġ��, ��ũ��)�� 
//�߰�(���, ����)�س����� ���� ������ decorator �����̶�� �Ѵ�.
//�̰��� ��� Ȯ���� �ʿ��� �� ����� ������� ����Ѵ�.

#include <iostream>
using namespace std;

// �Ʒ��� �ڵ�� �⺻�ڵ�
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
//			// -- ���� ���� ����
//			cout << "�������" << endl;
//		}
//		if (direction_ & REAR_DIRECTION) {
//			// -- �Ĺ� ���� ����
//			cout << "�Ĺ����" << endl;
//		}//
//		// -- ���� ���� ����
//		cout << "�������" << endl;
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
		std::cout << "�������" << std::endl;
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
		std::cout << "�������" << std::endl;
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
		std::cout << "�Ĺ����" << std::endl;
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
