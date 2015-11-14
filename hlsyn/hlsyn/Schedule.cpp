#include "Schedule.h"
#include "Operation.h"
#include <vector>

using namespace std;

Schedule::Schedule() {
	vector<Operation> vertices;
}

Schedule::Schedule(vector<Operation*> v, int latency) {
	vertex = v;
	nop = new Operation();
	sink = new Operation();
	sink->setTime(latency + 1);
}

void Schedule::SetResource() {
	bool mul = false;
	bool alu = false;
	resource_struct *a;
	for (int i = 0; i < vertex.size(); i++) {
		if (vertex[i]->getType().compare("*") == 0) {
			mul = true;
		}
		else {
			alu = true;
		}
	}
	if (mul) {
		a = new resource_struct;
		a->type = 'm';
		a->amount = 1;
		resource.push_back(*a);
		delete a;
	}
	if (alu) {
		a = new resource_struct;
		a->type = 'a';
		a->amount = 1;
		resource.push_back(*a);
		delete a;
	}
}

vector<Operation*> Schedule::getVertices() {
	return vertex;
}


void Schedule::listR(int latency) {	//performs scheduling task listR
	SetResource();
	UnscheduleSequencingGraph();
	ALAP(latency);
}

//Add a check for ALAP
bool Schedule::ALAP(int latency) {
	int count = checkMinCycle(nop);
	if (count > latency) {
		return false;
	}
	vector<Operation*> unscheduled = vertex;
	vector < Operation*> scheduled;
	scheduled.push_back(sink);
	while (unscheduled.size() != 0) {
		for (int i = 0; i < unscheduled.size(); i++) {
			for (int j = 0; j < scheduled.size(); j++) {
				if (unscheduled[i]->getChild()[0] == scheduled[j]) {
					unscheduled[i]->setTime(scheduled[j]->getTime() - unscheduled[i]->getDelay());
					scheduled.push_back(unscheduled[i]);
					unscheduled.erase(unscheduled.begin() + i);
					i--;
					break;
				}
			}
		}
	}

	return true;
}

int Schedule::checkMinCycle(Operation* o) {
	int current = 0;
	if (o->getChild().size() == 0) { //therminal node
		return o->getDelay();
	}
	else {
		for (int i = 0; i < o->getChild().size(); i++) {
			int n = checkMinCycle(o->getChild()[i]);
			if (n > current) {
				current = n;
			}
		}
		return o->getDelay() + current;
	}
}

void Schedule::UnscheduleSequencingGraph() {
	//Schedule Input First
	vector<Operation*> v = vertex; //Make copy //Unschedule list
	for (int i = 0; i < v.size(); i++) {
		if (!v[i]->getType().compare("?")) { //Its a mux!
			if (v[i]->getInput1().getType().compare("input") == 0 && v[i]->getInput2().getType().compare("input") == 0 && static_cast<Mux*>(v[i])->GetSel().getType().compare("input") == 0) {
				nop->AddChild(v[i]);
				v.erase(v.begin() + i);
				i--;
			}
		}
		else{ //Everything else
			if (v[i]->getInput1().getType().compare("input") == 0 && v[i]->getInput2().getType().compare("input") == 0) {
				nop->AddChild(v[i]);
				v.erase(v.begin() + i);
				i--;
			}
		}
	}

	for (int i = 0; i < nop->getChild().size(); i++) {
		USGSupport(nop->getChild()[i], v);
	}
	for (int i = 0; i < vertex.size(); i++) {
		if (!vertex[i]->getOutput().getType().compare("output")) {
			vertex[i]->AddChild(sink);
		}
	}
}

void Schedule::USGSupport(Operation *o, vector<Operation*> v) {
	if (!o->getOutput().getType().compare("output")) {
		return; //reached the end
	}
	else {
		for (int i = 0; i < v.size(); i++) {
			string var = o->getOutput().getVar();
			if (!v[i]->getType().compare("?")) {
				if (!var.compare(v[i]->getInput1().getVar()) || !var.compare(v[i]->getInput2().getVar()) || !var.compare(static_cast<Mux*>(v[i])->GetSel().getVar())) {
					bool exist = false;
					for (int j = 0; j < o->getChild().size(); j++) {
						if (!o->getChild()[j]->getOutput().getVar().compare(v[i]->getOutput().getVar())) {
							exist = true;
						}
						else {
							exist = false;
						}
					}
					if (!exist) {
						o->AddChild(v[i]);
						USGSupport(v[i], v);
					}
				}
			}
			else {
				if (!var.compare(v[i]->getInput1().getVar()) || !var.compare(v[i]->getInput2().getVar())) {
					bool exist = false;
					for (int j = 0; j < o->getChild().size(); j++) {
						if (!o->getChild()[j]->getOutput().getVar().compare(v[i]->getOutput().getVar())) {
							exist = true;
						}
						else {
							exist = false;
						}
					}
					if (!exist) {
						o->AddChild(v[i]);
						USGSupport(v[i], v);
					}
				}
			}
		}
	}
}