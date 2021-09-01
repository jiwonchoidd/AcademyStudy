#ifndef NODE_H
#define NODE_H

#include <vector>
#include <functional>
#include <iostream>
#include "TLog.h"
using namespace std;

class TActionNode
{
public:
	TActionNode() {};
	~TActionNode() {};
	virtual ReturnCode tick() 
	{ 
		return ReturnCode::SUCCESS;
	};
};
class Leaf : public TActionNode
{
};

class Action : public Leaf
{
private:
	function<ReturnCode()> action;
public:
	Action(function<ReturnCode()> act) { action = act; };
	~Action() {};
	ReturnCode tick();
};
class Conditional : public Leaf
{
private:
	function<ReturnCode()> predicate;
public:
	Conditional(function<ReturnCode()> pred) { predicate = pred; };
	~Conditional() {};
	ReturnCode tick();
};
class Composite : public TActionNode
{
protected:
	int currentChild;
	vector<TActionNode*> children;
public:
	Composite() { currentChild = 0; };
	~Composite() {};
	void addChild(TActionNode *n) { children.push_back(n); }
	virtual ReturnCode tick() { return ReturnCode::SUCCESS; };
};

class Sequence : public Composite
{
public:
	Sequence() {};
	~Sequence() {};
	ReturnCode tick();
};

class Selector : public Composite
{
public:
	Selector() {};
	~Selector() {};
	ReturnCode tick();
};

#endif // NODE_H
