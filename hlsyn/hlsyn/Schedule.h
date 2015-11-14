#pragma once
#ifndef SCHEDULE_H
#define SCHEDULE_H
#include <vector>
#include <string>
#include <algorithm>
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
	Operation *sink; //bottom
	void SetResource();
	bool ALAP(int latency);
	void UnscheduleSequencingGraph();
	void USGSupport(Operation *o, vector<Operation*> v);
	int checkMinCycle(Operation* o);
public:
	Schedule();
	Schedule(vector<Operation*> v, int latency);
	vector<Operation*> getVertices();
	void listR(int latency);
	void refreshSlacks(int timestep);
	bool allScheduled();
};


#endif