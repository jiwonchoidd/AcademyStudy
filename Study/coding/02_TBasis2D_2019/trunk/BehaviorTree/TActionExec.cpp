#include "TActionExec.h"

ReturnCode TActionExec::run()
{
	counter += 1;
	if (counter > 4)
	{
		counter = 0;
		TLog(DEBUG) << "---> success";
		return ReturnCode::SUCCESS;
	}
	else
	{
		TLog(DEBUG) << "---> running";
		return ReturnCode::RUNNING;
	}
}
