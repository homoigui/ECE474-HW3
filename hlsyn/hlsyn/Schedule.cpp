#include "Schedule.h"
#include "Operation.h"
#include <vector>

using namespace std;

Schedule::Schedule() {
	vector<Operation> vertices;
}

Schedule::Schedule(vector<Operation*> v) {
	vertex = v;
	nop = new Operation();
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
	ALAP();
}

bool Schedule::ALAP() {


	return true;
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
	USGSupport(nop, v);
}

void Schedule::USGSupport(Operation* o, vector<Operation*> &v) {
	if (!o->getOutput().getType().compare("output")) {
		return; //reached the end
	}
	else {
		for (int i = 0; i < o->getChild().size(); i++) { //check every child
			if (!o->getOutput().getType().compare("variable")) {
				for (int j = 0; j < v.size(); j++) { //compare against remaining vertex
					if (!v[j]->getType().compare("?")) { //if mux
						if (!o->getChild()[i]->getOutput().getVar().compare(v[j]->getInput1().getVar()) || !o->getChild()[i]->getOutput().getVar().compare(v[j]->getInput2().getVar()) || !o->getChild()[i]->getOutput().getVar().compare(static_cast<Mux*>(v[j])->GetSel().getVar())) {

						}
					}
					else {
						if (!o->getChild()[i]->getOutput().getVar().compare(v[j]->getInput1().getVar()) || !o->getChild()[i]->getOutput().getVar().compare(v[j]->getInput2().getVar())) {

						}
					}
					
				}
			}
			
		}
	}
}