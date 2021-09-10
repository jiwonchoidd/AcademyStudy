//�ൿƮ�� ���ڷ�
//http ://www.diva-portal.org/smash/get/diva2:907048/FULLTEXT01.pdf

#include <iostream>
#include <string>
#include <random>


#include "TActionNode.h"
#include "TActionExec.h"
#include "TLog.h"
using namespace std;

structlog LOGCFG = {};
TActionExec exec;

ReturnCode succ_fail()
{
	// ����Ʈ���� ��� ���� ������
	random_device rd;  
	//C++11 �⺻ ���� ���� ���� ���尪�� time(nullptr) �Ǵ� rd()�� �Ѵ�.	
	mt19937 gen(rd()); 

	// ��¥�Ⱓ ��� ����
	uniform_int_distribution<> dis(0, 10);// 0,1�� ����
	int true_false = dis(gen);
	if (true_false == 1)
	{
		TLog(DEBUG) << "---> success";
		return ReturnCode::SUCCESS;
	}
	else
	{
		TLog(DEBUG) << "---> failure";
		return ReturnCode::FAILURE;
	}
}

ReturnCode succ_runn()
{
	random_device rd;  
	mt19937 gen(rd()); 
	uniform_int_distribution<> dis(0, 1);
	int true_false = dis(gen);
	if (true_false == 1)
	{
		TLog(DEBUG) << "---> success";
		return ReturnCode::SUCCESS;
	}
	else
	{
		TLog(DEBUG) << "---> running";
		return ReturnCode::RUNNING;
	}
}


ReturnCode predOnComingCar()
{
	TLog(DEBUG) << "predOnComingCar:";
	return succ_fail();
}

ReturnCode predCarInFront()
{
	TLog(DEBUG) << "predCarInFront:";
	return succ_fail();
}

//ȸ��
ReturnCode actionAvoid()
{
	TLog(DEBUG) << "Avoiding car:";
	return exec.run();
	// return succ_runn();
}


ReturnCode actionTurnOut()
{
	TLog(DEBUG) << "Turning out:";
	return exec.run();
	// return succ_runn();
}
// �Ҹ�(�ð����)
ReturnCode actionPassCar()
{
	TLog(DEBUG) << "Passing car:";
	return exec.run();
	// return succ_runn();
}

ReturnCode actionTurnIn()
{
	TLog(DEBUG) << "Turning in:";
	return exec.run();
	// return succ_runn();
}
// ����
ReturnCode actionCruise()
{
	TLog(DEBUG) << "Cruising:";
	return exec.run();
	// return succ_runn();
}

// ���������� ���� �ൿƮ���� ����� �ǻ����
int main(int argc, char const *argv[])
{

	LOGCFG.headers = false;
	LOGCFG.level = DEBUG;

	TLog(INFO) << "Main executed with " << (argc - 1) << " arguments";	

	Selector *sel = new Selector();
	// �ٰ����� ����
	Sequence *seq_oncoming_car = new Sequence();
	sel->addChild(seq_oncoming_car);
		// �ٰ�����
		Conditional *cond_oncoming = new Conditional(predOnComingCar);
			seq_oncoming_car->addChild(cond_oncoming);
		// ȸ��
		Action *act_avoiding = new Action(actionAvoid);
			seq_oncoming_car->addChild(act_avoiding);

    // �߿�������
	Sequence *seq_overtake = new Sequence();
	sel->addChild(seq_overtake);
		// ���� ����
		Conditional *cond_car_infront = new Conditional(predCarInFront);
			seq_overtake->addChild(cond_car_infront);
		// �߿� ����
		Sequence *seq_overtake_exec = new Sequence();
			seq_overtake->addChild(seq_overtake_exec);
			// �ٱ������� 
			Action *act_turn_out = new Action(actionTurnOut);
				seq_overtake_exec->addChild(act_turn_out);
			// ��� ����
			Action *act_pass_car = new Action(actionPassCar);
				seq_overtake_exec->addChild(act_pass_car);
			// ��������
			Action *act_turn_in = new Action(actionTurnIn);
				seq_overtake_exec->addChild(act_turn_in);
    // ����
	Action *act_cruise = new Action(actionCruise);
	sel->addChild(act_cruise);


	ReturnCode result;
	result = sel->tick();
	// while (result != ReturnCode::SUCCESS)
	for (int i = 0; i < 15; ++i)
	{
		TLog(DEBUG) << "count: " << exec.getCounter();
		TLog(INFO) << "---------- new iteration ---------";
		result = sel->tick();
	}

	// Conditional cond (pred);
	// cout << (cond.tick() == ReturnCode::SUCCESS) << endl;
	TLog(INFO) << "code working so far";
	return 0;
}
