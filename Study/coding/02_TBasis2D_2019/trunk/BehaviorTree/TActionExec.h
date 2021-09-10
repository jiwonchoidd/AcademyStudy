#ifndef ACTIONEXEC_H
#define ACTIONEXEC_H

#include "TLog.h"

class TActionExec 
{
private:
	int counter;
public:
	TActionExec() { counter = 0; };
	~TActionExec() {};

	ReturnCode run();

	int getCounter() { return counter; }
};

#endif // ACTIONEXEC_H
