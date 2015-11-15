#pragma once
#ifndef OPERATION_H
#define OPERATION_H
#include <vector>
#include <algorithm>
#include "Variable.h"


using namespace std;

class Operation {
protected:
	string type;
	char resourceType;
	Variable output;
	Variable input1;
	Variable input2;
	int delay;
	int vertex;
	int time;
	int beginTime;
	int endTime;
	int slack;
	vector<Operation*> child;
	vector<Operation*> parent;
public:
	struct slackCompare {
		bool operator()(const Operation *left, const Operation *right) {
			return left->slack < right->slack;
		}
	};
	struct timeCompare {
		bool operator()(const Operation *left, const Operation *right) {
			return left->time < right->time;
		}
	};
	Operation();
	Operation(string t, int d, int v, Variable i1, Variable i2, Variable o, char r);
	~Operation();
	string getType();
	int getDelay();
	void setType(string t);
	void setDelay(int d);
	void AddChild(Operation* node);
	vector<Operation*> getChild();
	void AddParent(Operation* node);
	vector<Operation*> getParent();
	void setTime(int t);
	int getTime();
	void setSlack(int timestep);
	int getSlack();
	Variable getInput1();
	Variable getInput2();
	Variable getOutput();
	//virtual Variable GetSel(){}
	bool isScheduled;
	int getBeginTime();
	int getEndTime();
	void setBeginTime(int t);
	void setEndTime(int t);
	bool isALU();
	bool isMUL();
	char getResourceType();
	char color;
	int dist;
};

class Mux : public Operation {
private:
	Variable sel;
public:
	Mux(string t, int d, int v, Variable i1, Variable i2, Variable o, Variable s, char r);
	Variable GetSel();
};

#endif