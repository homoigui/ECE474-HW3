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
	//set resource type to 1 for all applicable resources, creat unscheduled graph, find alap times
	SetResource();
	UnscheduleSequencingGraph();
	ALAP();

	//set initial resource counters
	int alu = 0;
	int mul = 0;

	//set initial time step
	int timestep = 1;
	vector<Operation*> candOps;
	vector<Operation*> scheduledOps;
	
	//set initial candidate ops from nop's children
	for (int i = 0; i < nop->getChild().size(); i++) {
		candOps.push_back(nop->getChild()[i]);
	}
	
	//until every vertex is scheduled
	while (!allScheduled) {
		//update cand ops
		for (int i = 0; i < scheduledOps.size(); i++) {
			if (timestep == scheduledOps[i]->getEndTime()) {
				scheduledOps[i]->isScheduled = true;
				for (int j = 0; j < scheduledOps[i]->getChild().size(); j++) { //add finished scheduled op's children to candOps
					candOps.push_back(scheduledOps[i]->getChild()[j]);
				}
				scheduledOps.erase(scheduledOps.begin() + i);
			}
		}
		
		//schedule cand ops
		for (int i = 0; i < resource.size(); i++) { //for each resource type
			refreshSlacks(timestep); //compute slacks
			for (int j = 0; j < candOps.size(); j++) {//schedule ops with zero slack, update resource counter, remove vertex and add it to the scheduled ops vector
				if (candOps[j]->getSlack == 0) {
					candOps[j]->setBeginTime(timestep); //schedule op
					candOps[j]->setEndTime(candOps[j]->getBeginTime() + candOps[j]->getDelay());
					scheduledOps.push_back(candOps[j]);
					candOps.erase(candOps.begin() + j); //remove the scheduled op from the candOps vector
					

					if (candOps[j]->getType().compare("a") == 0)//update resource counter
						alu++;
					else
						mul++;
				}
			}
			for (int j = 0; j < candOps.size(); j++) { //schedule ops requiring no additional resources, remove vertex and add it to teh scheduled ops vector
				if (candOps[j]->isALU() && resource[i].type == 'a' && alu < resource[i].amount) {
					candOps[j]->setBeginTime(timestep); //schedule op
					candOps[j]->setEndTime(candOps[j]->getBeginTime() + candOps[j]->getDelay());
					scheduledOps.push_back(candOps[j]);
					candOps.erase(candOps.begin() + j); //remove the scheduled op from the candOps vector
					alu++;
				}
				else if (candOps[j]->isMUL() && resource[i].type == 'm' && mul < resource[i].amount) {
					candOps[j]->setBeginTime(timestep); //schedule op
					candOps[j]->setEndTime(candOps[j]->getBeginTime() + candOps[j]->getDelay());
					scheduledOps.push_back(candOps[j]);
					candOps.erase(candOps.begin() + j); //remove the scheduled op from the candOps vector
					mul++;
				}
			}
		}
		timestep++;
	}
}

void Schedule::refreshSlacks(int timestep)
{
	//set the slack for the vertices in the graph
	for (int i = 0; i < vertex.size(); i++) {
		vertex[i]->setSlack(timestep);
	}
}


bool Schedule::allScheduled() {
	for (int i = 0; i < vertex.size(); i++) {
		if (vertex[i]->isScheduled == false) {
			return false;
		}
	}

	return true;
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

	for (int i = 0; i < nop->getChild().size(); i++) {
		USGSupport(nop->getChild()[i], v);
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
					o->AddChild(v[i]);
					USGSupport(v[i], v);
				}
			}
			else {
				if (!var.compare(v[i]->getInput1().getVar()) || !var.compare(v[i]->getInput2().getVar())) {
					o->AddChild(v[i]);
					USGSupport(v[i], v);
				}
			}
		}
	}
}