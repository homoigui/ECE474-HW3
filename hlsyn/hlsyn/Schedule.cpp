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
	nop->isScheduled = true;
	sink = new Operation();
	sink->setTime(latency + 1);
	sink->isScheduled = true;
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
	ALAP(latency);

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
	while (!allScheduled()) {
		//update cand ops
		bool isParentScheduled = true;
		alu = 0;
		mul = 0;
		for (int i = 0; i < scheduledOps.size(); i++) {
			if (timestep == scheduledOps[i]->getEndTime()) {
				scheduledOps[i]->isScheduled = true;
				for (int j = 0; j < scheduledOps[i]->getChild().size(); j++) { //add finished scheduled op's children to candOps
					for (int k = 0; k < scheduledOps[i]->getChild()[j]->getParent().size(); k++) {
						if (scheduledOps[i]->getChild()[j]->getParent()[k]->isScheduled == false) { //All of the parent must be scheduled to be a candidate
							isParentScheduled = false;
						}
					}
					if (isParentScheduled) {
						candOps.push_back(scheduledOps[i]->getChild()[j]);
					}			
				}
			}
		}
		sort(candOps.begin(), candOps.end()); //put least slack first
		//schedule cand ops
		for (int i = 0; i < resource.size(); i++) { //for each resource type
			refreshSlacks(timestep); //compute slacks
			for (int j = 0; j < candOps.size(); j++) {//schedule ops with zero slack, update resource counter, remove vertex and add it to the scheduled ops vector
				if (candOps[j]->getSlack() == 0) {
					candOps[j]->setBeginTime(timestep); //schedule op
					candOps[j]->setEndTime(candOps[j]->getBeginTime() + candOps[j]->getDelay());
					scheduledOps.push_back(candOps[j]);
					candOps.erase(candOps.begin() + j); //remove the scheduled op from the candOps vector
					j--;
					if (candOps[j]->getType().compare("a") == 0) {//update resource counter
						alu++;
						if (resource[i].type == 'a' && alu > resource[i].amount) {
							resource[i].type = alu;
						}	
					}
					else {
						mul++;
						if (resource[i].type == 'm' && mul > resource[i].amount) {
							resource[i].type = mul;
						}						
					}
				}
			}
			for (int j = 0; j < candOps.size(); j++) { //schedule ops requiring no additional resources, remove vertex and add it to teh scheduled ops vector
				if (candOps[j]->isALU() && resource[i].type == 'a' && alu < resource[i].amount) {
					candOps[j]->setBeginTime(timestep); //schedule op
					candOps[j]->setTime(timestep);
					candOps[j]->setEndTime(candOps[j]->getBeginTime() + candOps[j]->getDelay());
					scheduledOps.push_back(candOps[j]);
					candOps.erase(candOps.begin() + j); //remove the scheduled op from the candOps vector
					alu++;
					j--;
				}
				else if (candOps[j]->isMUL() && resource[i].type == 'm' && mul < resource[i].amount) {
					candOps[j]->setBeginTime(timestep); //schedule op
					candOps[j]->setTime(timestep);
					candOps[j]->setEndTime(candOps[j]->getBeginTime() + candOps[j]->getDelay());
					scheduledOps.push_back(candOps[j]);
					candOps.erase(candOps.begin() + j); //remove the scheduled op from the candOps vector
					mul++;
					j--;
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
		for (int i = 0; i < unscheduled.size(); i++) { //For each vertex
			for (int j = 0; j < scheduled.size(); j++) { //Finds something that is scheduled
				if (unscheduled[i]->getChild()[0] == scheduled[j]) {
					unscheduled[i]->setTime(scheduled[j]->getTime() - unscheduled[i]->getDelay()); //time = time of sucessor - delay
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
				v[i]->AddParent(nop);
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
					v[i]->AddParent(o);
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
					v[i]->AddParent(o);
					USGSupport(v[i], v);
				}
			}
		}
	}
}
}