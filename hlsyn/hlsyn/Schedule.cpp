#include "Schedule.h"
#include "Operation.h"
#include <vector>

using namespace std;

Schedule::Schedule() {
	vector<Operation> vertices;
}

Schedule::Schedule(vector<Operation> v) {
	for (int i = 0; i < v.size(); i++) {
		vertices.push_back(v[i]);
	}
}

Schedule::~Schedule() {};

void Schedule::setVertices(vector<Operation> v) {
	for (int i = 0; i < v.size(); i++) {
		vertices.push_back(v[i]);
	}
}

vector<Operation> Schedule::getVertices() {
	return vertices;
}

void Schedule::listR() {
	//performs scheduling task listR
}


