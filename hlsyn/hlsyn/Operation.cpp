#include "Operation.h"

using namespace std;

Operation::Operation() {
	type = "unknown";
	latency = 999;
}

Operation::Operation(string t, int l) {
	type = t;
	latency = l;
}

Operation::~Operation() {};

void Operation::setType(string t) {
	type = t;
}

void Operation::setLatency(int l) {
	latency = l;
}

string Operation::getType() {
	return type;
}

int Operation::getLatency() {
	return latency;
}
