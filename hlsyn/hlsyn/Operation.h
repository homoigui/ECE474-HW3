#pragma once
#ifndef OPERATION_H
#define OPERATION_H
#include <vector>


using namespace std;

class Operation {
private:
	string type;
	string output;
	string input1;
	string input2;
	int delay;
	int vertex;
	vector<Operation> child;
public:
	Operation();
	Operation(string t, int d, int v, string i1, string i2, string o);
	~Operation();
	string getType();
	int getDelay();
	void setType(string t);
	void setDelay(int d);
};

class Mux : private Operation {
private:
	string sel;
};

#endif