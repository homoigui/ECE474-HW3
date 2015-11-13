#pragma once
#ifndef SCHEDULE_H
#define SCHEDULE_H
#include <vector>
#include "Operation.h"


using namespace std;

class Schedule {
private:
	vector<Operation> vertices;

public:
	Schedule();
	Schedule(vector<Operation> v);
	~Schedule();
	void setVertices(vector<Operation> v);
	vector<Operation> getVertices();
	void listR();
};


#endif