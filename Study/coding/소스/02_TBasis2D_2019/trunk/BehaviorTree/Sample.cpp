//행동트리 논문자료
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
	// 소프트웨어 기반 난수 생성기
	random_device rd;  
	//C++11 기본 난수 엔진 난수 씨드값을 time(nullptr) 또는 rd()로 한다.	
	mt19937 gen(rd()); 

	// 날짜기간 계산 랜덤
	uniform_int_distribution<> dis(0, 10);// 0,1의 분포
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

//회피
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
// 소멸(시간경과)
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
// 순항
ReturnCode actionCruise()
{
	TLog(DEBUG) << "Cruising:";
	return exec.run();
	// return succ_runn();
}

// 자율주행을 위한 행동트리를 사용한 의사결정
int main(int argc, char const *argv[])
{

	LOGCFG.headers = false;
	LOGCFG.level = DEBUG;

	TLog(INFO) << "Main executed with " << (argc - 1) << " arguments";	

	Selector *sel = new Selector();
	// 다가오는 차량
	Sequence *seq_oncoming_car = new Sequence();
	sel->addChild(seq_oncoming_car);
		// 다가간다
		Conditional *cond_oncoming = new Conditional(predOnComingCar);
			seq_oncoming_car->addChild(cond_oncoming);
		// 회피
		Action *act_avoiding = new Action(actionAvoid);
			seq_oncoming_car->addChild(act_avoiding);

    // 추월시퀀스
	Sequence *seq_overtake = new Sequence();
	sel->addChild(seq_overtake);
		// 앞쪽 차량
		Conditional *cond_car_infront = new Conditional(predCarInFront);
			seq_overtake->addChild(cond_car_infront);
		// 추월 차량
		Sequence *seq_overtake_exec = new Sequence();
			seq_overtake->addChild(seq_overtake_exec);
			// 바깥쪽으로 
			Action *act_turn_out = new Action(actionTurnOut);
				seq_overtake_exec->addChild(act_turn_out);
			// 통과 차량
			Action *act_pass_car = new Action(actionPassCar);
				seq_overtake_exec->addChild(act_pass_car);
			// 안쪽으로
			Action *act_turn_in = new Action(actionTurnIn);
				seq_overtake_exec->addChild(act_turn_in);
    // 순항
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
