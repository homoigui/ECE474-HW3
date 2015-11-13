#pragma once
#ifndef SCHEDULE_H
#define SCHEDULE_H
#include <vector>
#include <string>
#include "Operation.h"
#include "Variable.h"
//Connect operators here
using namespace std;

class Schedule {
private:
	struct resource_struct {
		char type;
		int amount;
	};
	vector<Operation*> vertex;
	vector<resource_struct> resource;
	Operation *nop; //top
	void SetResource();
	bool ALAP();
	void UnscheduleSequencingGraph();
	void USGSupport(Operation *o, vector<Operation*> v);
public:
	Schedule();
	Schedule(vector<Operation*> v);
	vector<Operation*> getVertices();
	void listR(int latency);
};


#endif