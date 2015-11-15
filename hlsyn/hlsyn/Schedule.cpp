#include "Schedule.h"
#include "Operation.h"
#include <vector>
#include <climits>

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
	for (unsigned int i = 0; i < vertex.size(); i++) {
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


int Schedule::listR(int latency) {	//performs scheduling task listR
	//set resource type to 1 for all applicable resources, creat unscheduled graph, find alap times
	SetResource();
	UnscheduleSequencingGraph();
	if (!ALAP(latency)) {
		return -1;
	}
	std::sort(vertex.begin(), vertex.end(), Operation::timeCompare());

	for (int i = 0; i < vertex.size(); i++) {
		cout << vertex[i]->getType() << "   Time: " << vertex[i]->getTime() << endl;
	}
	cout << endl << endl;
	//set initial resource counters
	int alu = 0;
	int mul = 0;

	//set initial time step
	int timestep = 1;
	vector<Operation*> candOps;
	vector<Operation*> scheduledOps;
	vector<Operation*> allOps = vertex;
	
	//set initial candidate ops from nop's children
	for (unsigned int i = 0; i < nop->getChild().size(); i++) {
		candOps.push_back(nop->getChild()[i]);
	}
	for (unsigned int i = 0; i < candOps.size(); i++) {
		for (unsigned int j = 0; j < allOps.size(); j++) {
			if (allOps[j] == candOps[i]) {
				allOps.erase(allOps.begin()+j);
				j--;
			}
		}
	}
	
	//until every vertex is scheduled
	while (!allScheduled() && timestep <= latency || allOps.size() != 0) {
		//update cand ops
		
		alu = 0;
		mul = 0;
		for (unsigned int i = 0; i < scheduledOps.size(); i++) {
			if (timestep == scheduledOps[i]->getEndTime()) {
				scheduledOps[i]->isScheduled = true;
			}
		}
		if (scheduledOps.size() != 0) { //at least one of the initial is scheduled
			for (unsigned int i = 0; i < allOps.size(); i++) {
				bool isParentScheduled = true;
				for (unsigned int j = 0; j < allOps[i]->getParent().size(); j++) {
					if (allOps[i]->getParent()[j]->isScheduled == false) {
						isParentScheduled = false;
						break;
					}
				}
				if (isParentScheduled) {
					candOps.push_back(allOps[i]);
					allOps.erase(allOps.begin() + i);
					i--;
				}
			}
		}
		//for (int i = 0; i < scheduledOps.size(); i++) {
		//	if (timestep == scheduledOps[i]->getEndTime()) { //infinite loop ? h is not scheduled//////////////////////////////////////////////////////////////////
		//		scheduledOps[i]->isScheduled = true;	
		//	}
		//	for (int j = 0; j < scheduledOps[i]->getChild().size(); j++) { //add finished scheduled op's children to candOps
		//		for (int k = 0; k < scheduledOps[i]->getChild()[j]->getParent().size(); k++) {
		//			if (scheduledOps[i]->getChild()[j]->getParent()[k]->isScheduled == false) { //All of the parent must be scheduled to be a candidate
		//				isParentScheduled = false;
		//				break;
		//			}
		//		}
		//		if (isParentScheduled) {
		//			candOps.push_back(scheduledOps[i]->getChild()[j]);
		//		}
		//	}
		//}
		
		//schedule cand ops
		for (unsigned int i = 0; i < resource.size(); i++) { //for each resource type
			refreshSlacks(timestep); //compute slacks
			std::sort(candOps.begin(), candOps.end(), Operation::slackCompare()); //put least slack first
			for (unsigned int j = 0; j < candOps.size(); j++) {//schedule ops with zero slack, update resource counter, remove vertex and add it to the scheduled ops vector
				if (candOps[j]->getSlack() == 0) {
					candOps[j]->setBeginTime(timestep); //schedule op
					candOps[j]->setEndTime(candOps[j]->getBeginTime() + candOps[j]->getDelay());
					scheduledOps.push_back(candOps[j]);
					if (candOps[j]->getResourceType() == 'a') {//update resource counter
						alu++;
						if (resource[i].type == 'a' && alu > resource[i].amount) {
							resource[i].type = alu;
						}	
					}
					else if(candOps[j]->getResourceType() == 'm') {
						mul++;
						if (resource[i].type == 'm' && mul > resource[i].amount) {
							resource[i].type = mul;
						}						
					}
					candOps.erase(candOps.begin() + j); //remove the scheduled op from the candOps vector
					j--;
				}
			}
			for (unsigned int j = 0; j < candOps.size(); j++) { //schedule ops requiring no additional resources, remove vertex and add it to teh scheduled ops vector
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
	std::sort(vertex.begin(), vertex.end(), Operation::timeCompare());
	return 0;
}

void Schedule::refreshSlacks(int timestep){
	//set the slack for the vertices in the graph
	for (unsigned int i = 0; i < vertex.size(); i++) {
		vertex[i]->setSlack(timestep);
	}
}


bool Schedule::allScheduled() {
	for (unsigned int i = 0; i < vertex.size(); i++) {
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
		int vertexWithAllChildScheduled;
		for (unsigned int i = 0; i < unscheduled.size(); i++) {
			int size = 0;
			for (unsigned int j = 0; j < unscheduled[i]->getChild().size(); j++) {
				for (unsigned int k = 0; k < scheduled.size(); k++) {
					if (unscheduled[i]->getChild()[j] == scheduled[k]) {
						size++;
					}
				}
			}
			if (size == unscheduled[i]->getChild().size()) {
				vertexWithAllChildScheduled = i;
				break;
			}
		} //Now vertexWithAllchild is selected
		int minimum = INT_MAX;
		if (unscheduled[vertexWithAllChildScheduled]->getChild().size() == 1) {
			unscheduled[vertexWithAllChildScheduled]->setTime(unscheduled[vertexWithAllChildScheduled]->getChild()[0]->getTime() - unscheduled[vertexWithAllChildScheduled]->getDelay());
			scheduled.push_back(unscheduled[vertexWithAllChildScheduled]);
			unscheduled.erase(unscheduled.begin() + vertexWithAllChildScheduled);
		}
		else {
			for (unsigned int i = 0; i < unscheduled[vertexWithAllChildScheduled]->getChild().size() - 1; i++) {
				minimum = min(minimum, unscheduled[vertexWithAllChildScheduled]->getChild()[i]->getTime());
			}
			unscheduled[vertexWithAllChildScheduled]->setTime(minimum - unscheduled[vertexWithAllChildScheduled]->getDelay());
			scheduled.push_back(unscheduled[vertexWithAllChildScheduled]);
			unscheduled.erase(unscheduled.begin() + vertexWithAllChildScheduled);
		}
		
	}
	//for (unsigned int i = 0; i < unscheduled.size(); i++) { //For Each Scheduled Vertex
	//	for (unsigned int j = 0; j < scheduled.size(); j++) { //Finds something that is scheduled
	//	
	//			if (unscheduled[i]->getChild()[0] == scheduled[j]) {
	//				unscheduled[i]->setTime(scheduled[j]->getTime() - unscheduled[i]->getDelay()); //time = time of sucessor - delay
	//				scheduled.push_back(unscheduled[i]);
	//				unscheduled.erase(unscheduled.begin() + i);
	//				i--;
	//				break;
	//			}
	//	}
	//}
	return true;
}

int Schedule::checkMinCycle(Operation* o) {
	vector<Operation*> L; //sorted Topologicallist
	TopologicalSort(L);
	for (unsigned int i = 0; i < L.size(); i++) {

	}
	//int current = 0;
	//if (o->getChild().size() == 0) { //therminal node
	//	return o->getDelay();
	//}
	//else {
	//	for (unsigned int i = 0; i < o->getChild().size(); i++) {
	//		int n = checkMinCycle(o->getChild()[i]);
	//		if (n > current) {
	//			current = n;
	//		}
	//	}
	//	return o->getDelay() + current;
	//}
	return 0;
}

void Schedule::TopologicalSort(vector<Operation*> &L) {
	for (unsigned int i = 0; i < vertex.size(); i++) {
		vertex[i]->color = 'w';
	}
	for (unsigned int i = 0; i < vertex.size(); i++) {
		if (vertex[i]->color == 'w') {
			TSVisit(L, vertex[i]);
		}
	}
}

void Schedule::TSVisit(vector<Operation*> &L, Operation* u) {
	Operation* v;
	u->color = 'g';

	for (unsigned int i = 0; i < u->getChild().size(); i++) {
		v = u->getChild()[i];
		if (v->color == 'w') {
			TSVisit(L, v);
		}
	}
	u->color = 'b';
	L.insert(L.begin(), u);
}

void Schedule::UnscheduleSequencingGraph() {
	//Schedule Input First
	vector<Operation*> v = vertex; //Make copy //Unschedule list
	for (unsigned int i = 0; i < v.size(); i++) {
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

	for (unsigned int i = 0; i < nop->getChild().size(); i++) {
		USGSupport(nop->getChild()[i], v);
	}
	for (unsigned int i = 0; i < vertex.size(); i++) {
		if (!vertex[i]->getOutput().getType().compare("output")) {
			vertex[i]->AddChild(sink);
			sink->AddParent(vertex[i]);
		}
	}
}

void Schedule::USGSupport(Operation *o, vector<Operation*> v) {
	if (!o->getOutput().getType().compare("output")) {
		return; //reached the end
	}
	else {
		for (unsigned int i = 0; i < v.size(); i++) {
			string var = o->getOutput().getVar();
			if (!v[i]->getType().compare("?")) {
				if (!var.compare(v[i]->getInput1().getVar()) || !var.compare(v[i]->getInput2().getVar()) || !var.compare(static_cast<Mux*>(v[i])->GetSel().getVar())) {
					bool exist = false;
					for (unsigned int j = 0; j < o->getChild().size(); j++) {
						if (!o->getChild()[j]->getOutput().getVar().compare(v[i]->getOutput().getVar())) {
							exist = true;
							break;
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
					for (unsigned int j = 0; j < o->getChild().size(); j++) {
						if (!o->getChild()[j]->getOutput().getVar().compare(v[i]->getOutput().getVar())) {
							exist = true;
							break;
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