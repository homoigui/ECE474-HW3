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



void Operation::setNumIF(int i) {
	numIF = i;
}

int Operation::getNumIF() {
	return numIF;
}

void Operation::setConditionIF(string s) {
	conditionIF = s;
}

string Operation::getConditionIF() {
	return conditionIF;
}

void Operation::setNumElse(int e) {
	numElse = e;
}

int Operation::getNumElse() {
	return numElse;
}

void Operation::seperateOperator(vector<vector<Operation*> > &o_list, vector<Operation*> o) {
	vector<vector<Operation*> > seperation;
	int ifMax = 0;
	int elseMax = 0;
	int uniqueMax = 0;
	for (unsigned int i = 0; i < o.size(); i++) {
		if (o[i]->getNumIF() > ifMax) {
			ifMax = o[i]->getNumIF();
		}
		if (o[i]->getNumElse() > elseMax) {
			elseMax = o[i]->getNumElse();
		}
		if (o[i]->uniqueNo > uniqueMax) {
			uniqueMax = o[i]->uniqueNo;
		}
	}

	for (unsigned int i = 0; i <= uniqueMax; i++) {
		for (unsigned int j = 0; j <= elseMax; j++) {
			for (unsigned int k = 0; k <= ifMax; k++) {
				vector<Operation*> stuff;
				for (unsigned int l = 0; l < o.size(); l++) {
					if (o[l]->getNumElse() == j && o[l]->getNumIF() == k && o[l]->uniqueNo == i) {
						stuff.push_back(o[l]);
					}
				}
				if (stuff.size() != 0) {
					seperation.push_back(stuff);
				}
			}
		}
	}
	vector<Operation*> start = seperation[0];
	seperation.erase(seperation.begin());
	while (seperation.size() != 0) {
		for (unsigned int i = 0; i < seperation.size(); i++) {
			if (seperation[i][0]->getNumIF() - 1 == start[0]->getNumIF()) {
				
			}
		}
	}
	for (unsigned int i = 0; i < seperation.size(); i++) {

	}
	cout << endl;
}
void seperateOperatorHelper(vector<vector<Operation*> > &seperation, vector<Operation*> &start) {

}
/*if (o[j]->getNumIF() == 0 && o[j]->getNumElse() == 0) {
temp.push_back(o[j]);
}
if (o[j]->getNumElse() - 1 == i && o[j]->getNumIF() - 1 == i) {
temp.push_back(o[j]);
}
if (o[j]->getNumIF() <= i && o[j]->getConditionIF().compare("no condition") != 0 && o[j]->getNumElse() - 1 != i) {
temp.push_back(o[j]);
}*/

	/*int max = 0;
	for (unsigned int i = 0; i < o.size(); i++) {
		if (o[i]->getNumIF() > max) {
			max = o[i]->getNumIF();
		}
	}
	for (unsigned int i = 0; i < o.size(); i++) {
		o[i]->maxNest = max;
	}
	for (unsigned int i = 0; i <= max; i++) {
		vector<Operation*> stuff;
		for (unsigned int j = 0; j < o.size(); j++) {
			if (o[j]->getNumIF() == i) {
				stuff.push_back(o[j]);
			}
			if (o[j]->getNumIF() == i && o[j]->getNumElse() - 1 == i) {
				stuff.push_back(o[j]);
			}
		}
		list.push_back(stuff);
	}

	vector<Operation*> base;
	for (unsigned int j = 0; j < o.size(); j++) {
		if (o[j]->getNumIF() == 0 && o[j]->getNumElse() == 0) {
			base.push_back(o[j]);
		}
	}
	o_list.push_back(base);
	for (unsigned int i = 1; i <= max; i++) {
		vector<Operation*> ifelse = base;
		for (unsigned int j = 0; j < o.size(); j++) {
			if (o[j]->getNumIF() <= i) {
				ifelse.push_back(o[j]);
			}
		}

	}*/