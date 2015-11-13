#pragma once
#ifndef OPERATION_H
#define OPERATION_H
#include <vector>
#include "Variable.h"


using namespace std;

class Operation {
protected:
	string type;
	Variable output;
	Variable input1;
	Variable input2;
	int delay;
	int vertex;
	vector<Operation> child;
public:
	Operation();
	Operation(string t, int d, int v, Variable i1, Variable i2, Variable o);
	~Operation();
	string getType();
	int getDelay();
	void setType(string t);
	void setDelay(int d);
	//virtual Variable GetSel(){}
};

class Mux : public Operation {
private:
	Variable sel;
public:
	Mux(string t, int d, int v, Variable i1, Variable i2, Variable o, Variable s);
	Variable GetSel();
};

#endif