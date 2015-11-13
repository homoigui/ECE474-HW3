#pragma once
#ifndef OPERATION_H
#define OPERATION_H
#include <vector>


using namespace std;

class Operation {
private:
	string type;
	int latency;

public:
	Operation();
	Operation(string t, int l);
	~Operation();
	string getType();
	int getLatency();
	void setType(string t);
	void setLatency(int l);
};


#endif