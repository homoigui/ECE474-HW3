#include "Operation.h"

using namespace std;

Operation::Operation() {
	type = "nop";
	delay = 0;
	vertex = -1;
	time = 0;
	slack = -1;
	isScheduled = false;
}

Operation::Operation(string t, int d, int v, Variable i1, Variable i2, Variable o, char r) {
	type = t;
	delay = d;
	input1 = i1;
	input2 = i2;
	output = o;
	vertex = v;
	time = -1;
	slack = -1;
	isScheduled = false;
	resourceType = r;
}

Operation::~Operation() {};

Mux::Mux(string t, int d, int v, Variable i1, Variable i2, Variable o, Variable s, char r) {
	type = t;
	delay = d;
	input1 = i1;
	input2 = i2;
	output = o;
	vertex = v;
	sel = s;
	slack = -1;
	isScheduled = false;
	resourceType = r;
}

Variable Mux::GetSel() {
	return sel;
}
void Operation::setType(string t) {
	type = t;
}

void Operation::setDelay(int d) {
	delay = d;
}

string Operation::getType() {
	return type;
}

int Operation::getDelay() {
	return delay;
}

void Operation::AddChild(Operation* node) {
	child.push_back(node);
}

vector<Operation*> Operation::getChild() {
	return child;
}

void Operation::AddParent(Operation* node) {
	parent.push_back(node);
}

vector<Operation*> Operation::getParent() {
	return parent;
}

void Operation::setTime(int t) {
	time = t;
}

int Operation::getTime() {
	return time;
}

Variable Operation::getInput1() {
	return input1;
}

Variable Operation::getInput2() {
	return input2;
}

Variable Operation::getOutput() {
	return output;
}


void Operation::setSlack(int timestep) {
	slack = time - timestep;
}

int Operation::getSlack() {
	return slack;
}

void Operation::setBeginTime(int t) {
	beginTime = t;
}

void Operation::setEndTime(int t) {
	endTime = t;
}

int Operation::getBeginTime() {
	return beginTime;
}

int Operation::getEndTime() {
	return endTime;
}

bool Operation::isALU() {
	if (type.compare("*") == 0)
		return false;
	else
		return true;
}

bool Operation::isMUL() {
	if (type.compare("*") == 0)
		return true;
	else
		return false;

}

char Operation::getResourceType() {
	return resourceType;
}