#include "Operation.h"

using namespace std;

Operation::Operation() {
	type = "unknown";
	delay = 999;
	vertex = 999;
}

Operation::Operation(string t, int d, int v, Variable i1, Variable i2, Variable o) {
	type = t;
	delay = d;
	input1 = i1;
	input2 = i2;
	output = o;
	vertex = v;
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
