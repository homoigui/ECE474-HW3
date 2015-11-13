#include "Operation.h"

using namespace std;

Operation::Operation() {
	type = "nop";
	delay = -1;
	vertex = -1;
	time = 0;
}

Operation::Operation(string t, int d, int v, Variable i1, Variable i2, Variable o) {
	type = t;
	delay = d;
	input1 = i1;
	input2 = i2;
	output = o;
	vertex = v;
	time = -1;
}

Operation::~Operation() {};

Mux::Mux(string t, int d, int v, Variable i1, Variable i2, Variable o, Variable s) {
	type = t;
	delay = d;
	input1 = i1;
	input2 = i2;
	output = o;
	vertex = v;
	sel = s;
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

vector<Operation*> Operation::getChild() {
	return child;
}