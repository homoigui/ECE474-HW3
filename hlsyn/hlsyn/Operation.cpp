#include "Operation.h"

using namespace std;

Operation::Operation() {
	type = "unknown";
	delay = 999;
	input1 = "unknown";
	input2 = "unknown";
	output = "unknown";
	vertex = 999;
}

Operation::Operation(string t, int d, int v, string i1, string i2, string o) {
	type = t;
	delay = d;
	input1 = i1;
	input2 = i2;
	output = o;
	vertex = v;
}

Operation::~Operation() {};

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
