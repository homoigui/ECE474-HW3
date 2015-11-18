#include "Schedule.h"
#include "Operation.h"
#include <vector>
#include <climits>

using namespace std;

Schedule::Schedule() {

}

Schedule::Schedule(int latency) {
	nop = new Operation();
	nop->isScheduled = true;
	sink = new Operation();
	sink->setTime(latency + 1);
	sink->isScheduled = true;
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

	if (vertex.size() == 1) {
		vertex[0]->setTime(1);
		vertex[0]->setBeginTime(1);
		vertex[0]->setEndTime(vertex[0]->getDelay());
		return 0;
	}
	//Debug purpose
	//for (int i = 0; i < vertex.size(); i++) {
	//	cout << vertex[i]->getType() << " time:  " << vertex[i]->getTime() << endl;
	//}

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
	int done = 0;
	bool isMulDone = true;
	//until every vertex is scheduled
	while (!allScheduled() && timestep <= latency || allOps.size() != 0) {
		//update cand ops
		
		alu = 0;
		mul = 0;
		for (unsigned int i = 0; i < scheduledOps.size(); i++) { //Checks if it reached its end time. Set it as scheduled when it does
			if (timestep == scheduledOps[i]->getEndTime()) {
				scheduledOps[i]->isScheduled = true;
			}
		}
		if (scheduledOps.size() != 0) { //at least one of the initial is scheduled
			for (unsigned int i = 0; i < allOps.size(); i++) { //check all the unscheduled ops
				bool isParentScheduled = true;
				for (unsigned int j = 0; j < allOps[i]->getParent().size(); j++) { //First make sure if all the parents are scheduled
					if (allOps[i]->getParent()[j]->isScheduled == false) { 
						isParentScheduled = false;
						break;
					}
				}
				if (isParentScheduled) { //Set it as a candiate op if all the parents are scheduled
					candOps.push_back(allOps[i]);
					allOps.erase(allOps.begin() + i);
					i--;
				}
			}
		}
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
							resource[i].amount = alu;
						}	
					}
					else if(candOps[j]->getResourceType() == 'm') {
						mul++;
						isMulDone = false;
						done = 0;
						if (resource[i].type == 'm' && mul > resource[i].amount) {
							resource[i].amount = mul;
						}						
					}
					candOps.erase(candOps.begin() + j); //remove the scheduled op from the candOps vector
					j--;
				}
			}
			if (done == 2) {
				isMulDone = true;
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
					if (isMulDone) {
						candOps[j]->setBeginTime(timestep); //schedule op
						candOps[j]->setTime(timestep);
						candOps[j]->setEndTime(candOps[j]->getBeginTime() + candOps[j]->getDelay());
						scheduledOps.push_back(candOps[j]);
						candOps.erase(candOps.begin() + j); //remove the scheduled op from the candOps vector
						mul++;
						j--;
						isMulDone = false;
						done = 0;
					}
					
				}
			}
		}
		timestep++;
		done++; //Makes sure that the multiplier is finished first
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
	vector<Operation*> unscheduled = vertex; //List of unscheduled veretex for alap
	vector < Operation*> scheduled; //List of already scheduled vertex
	scheduled.push_back(sink); //Set sink node first
	while (unscheduled.size() != 0) { //keep going until unscheduled is empty
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
	L.push_back(sink);
	for (unsigned int i = 0; i < L.size(); i++) {
		L[i]->dist = 0;
	}
	Operation* v;
	for (unsigned int i = 0; i < L.size(); i++) {
		for (unsigned int j = 0; j < L[i]->getChild().size(); j++) {
			v = L[i]->getChild()[j];
			if ((L[i]->dist + L[i]->getDelay()) > v->dist) {
				v->dist = L[i]->dist + L[i]->getDelay();
			}
		}
	}
	int max = 0;
	for (unsigned int i = 0; i < L.size(); i++) {
		if (L[i]->dist > max) {
			max = L[i]->dist;
		}
	}
	return max;
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

void Schedule::UnscheduleSequencingGraph() { //This function just connects stuff together
	//Schedule Input First
	vector<Operation*> v = vertex; //Make copy //Unschedule list
	if (v.size() == 1) {
		v[0]->AddChild(sink);
		sink->AddParent(v[0]);
		v[0]->AddParent(nop);
		nop->AddChild(v[0]);
		return;
	}

	//for (unsigned int i = 0; i < v.size(); i++) {
	//	if (!v[i]->getType().compare("?")) { //Its a mux!
	//		if (v[i]->getInput1().getType().compare("input") == 0 && v[i]->getInput2().getType().compare("input") == 0 && static_cast<Mux*>(v[i])->GetSel().getType().compare("input") == 0) {
	//			nop->AddChild(v[i]);
	//			v.erase(v.begin() + i);
	//			i--;
	//			hasnop = true;
	//		}
	//	}
	//	else { //Everything else
	//		if (v[i]->getInput1().getType().compare("input") == 0 && v[i]->getInput2().getType().compare("input") == 0) {
	//			nop->AddChild(v[i]);
	//			v[i]->AddParent(nop);
	//			v.erase(v.begin() + i);
	//			i--;
	//			hasnop = true;
	//		}
	//	}
	//}
	for (int i = 0; i < v.size(); i++) {
		v[i]->connected = false;
	}
	//nop->AddChild(v[0]);
	//v.erase(v.begin());
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v.size(); j++) {
			string output = v[i]->getOutput().getVar();
			string input1 = v[j]->getInput1().getVar();
			string input2 = v[j]->getInput2().getVar();
			if (!v[j]->getType().compare("?")) {
				if (output.compare(v[j]->GetSel().getVar()) == 0 || output.compare(input1) == 0 || output.compare(input2) == 0) {
					v[j]->connected = true;
				}
			}
			else {
				if (output.compare(input1) == 0 || output.compare(input2) == 0) {
					v[j]->connected = true;
				}
			}
		}
	}
	for (int i = 0; i < v.size(); i++) {
		if (v[i]->connected == false) {
			nop->AddChild(v[i]);
			v.erase(v.begin() + i);
			i--;
		}
	}

	
		

	
	//Operation* head = NULL;
	//Operation* end= NULL;
	//if (!hasnop) { //Middle parts or stuff with no two inputs
	//	for (unsigned int i = 0; i < v.size(); i++) {
	//		for (unsigned int j = 0; j < v.size(); j++) {
	//			if (v[i]->getOutput().getVar().compare(v[j]->getInput1().getVar()) == 0) {
	//				v[i]->AddChild(v[j]);
	//				v[j]->AddParent(v[i]);
	//				if (head == NULL) {
	//					head = v[i];
	//				}
	//				nop->AddChild(head);
	//			}
	//		}
	//	}

	//}

	for (unsigned int i = 0; i < nop->getChild().size(); i++) {
		USGSupport(nop->getChild()[i], v);
	}

	for (int i = 0; i < vertex.size(); i++) {
		if (vertex[i]->getChild().size() == 0) {
			vertex[i]->AddChild(sink);
			sink->AddParent(vertex[i]);
		}
	}

	/*for (unsigned int i = 0; i < vertex.size(); i++) {
		if (!vertex[i]->getOutput().getType().compare("output")) {
			vertex[i]->AddChild(sink);
			sink->AddParent(vertex[i]);
		}
	}*/
	//new code
	for (unsigned int i = 0; i < vertex.size(); i++) {
		if (vertex[i]->getChild().size() == 0) {
			vertex[i]->AddChild(sink);
			sink->AddParent(vertex[i]);
		}
	}
}

void Schedule::USGSupport(Operation *o, vector<Operation*> v) { //recursive function to connect the middle parts together
	if (!o->getOutput().getType().compare("output")) {
		return; //reached the end
	}
	else {
		for (unsigned int i = 0; i < v.size(); i++) {
			string var = o->getOutput().getVar();
			if (!v[i]->getType().compare("?")) {
				if (!var.compare(v[i]->getInput1().getVar()) || !var.compare(v[i]->getInput2().getVar()) || !var.compare(v[i]->GetSel().getVar())) {
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

void Schedule::renewOperations(vector<vector<Operation*> > &o_list) {
	vector<vector<Operation*> > o_list_new;
	for (unsigned int i = 0; i < o_list.size(); i++) {
		vector<Operation*> op_new;
		for (unsigned int j = 0; j < o_list[i].size(); j++) {
			if (o_list[i][j]->getType().compare("?") == 0) { //if mux
				Operation* op = new Operation();
				*op = *o_list[i][j];
				op_new.push_back(op);
			}
			else {
				Operation* op = new Operation();
				*op = *o_list[i][j];
				op_new.push_back(op);
			}
		}
		o_list_new.push_back(op_new);
	}
	o_list.swap(o_list_new);
}