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

Operation::Operation(string t, int d, int v, Variable i1, Variable i2, Variable o, Variable s, char r) {
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



Variable Operation::GetSel() {
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

//class tree {
//public:
//	vector<Operation*> main;
//	int noIF;
//	int noELSE;
//	int noUNIQUE;
//	bool _else;
//	tree* left;
//	tree* right;
//	tree(vector<Operation*> m, int i, int e, int u, bool lse) {
//		main = m;
//		noIF = i;
//		noELSE = e;
//		noUNIQUE = u;
//		_else = lse;
//		left = NULL;
//		right = NULL;
//	}
//};
//void seperateOperatorHelper(vector<tree*> &t, tree* head) {
//	for (unsigned int i = 0; i < t.size(); i++) {
//		if (t[i]->noIF - 1 == head->noIF && !t[i]->_else && t[i]->noELSE == head->noELSE) {
//			head->left = t[i];
//			t.erase(t.begin() + i);
//			i--;
//			seperateOperatorHelper(t, head->left);
//			
//		}
//		else if (t[i]->noELSE - 1 == head->noELSE && t[i]->_else && t[i]->noIF == head->noIF) {
//			head->right = t[i];
//			t.erase(t.begin() + i);
//			i--;
//			seperateOperatorHelper(t, head->right);
//		}
//	}
//}
//
//void seperateOperatorHelper2(vector<vector<Operation*> > &o_list, tree* node, vector<Operation*> &list) {
//	list.insert(list.end(), node->main.begin(), node->main.end());
//	if (node->left == NULL && node->right == NULL) {
//		o_list.push_back(list);
//		for (unsigned int i = 0; i < node->main.size(); i++) {
//			list.pop_back();
//		}
//		return;
//	}
//	if (node->left != NULL) {
//		seperateOperatorHelper2(o_list, node->left, list);
//
//		
//		delete node->left;
//		node->left = NULL;
//		
//		
//
//		if (node->left == NULL && node->right == NULL) {
//			o_list.push_back(list);
//			for (unsigned int i = 0; i < node->main.size(); i++) {
//				list.pop_back();
//			}
//		}
//	}
//	if (node->right != NULL) {
//		seperateOperatorHelper2(o_list, node->right, list);
//
//		if (node->right != NULL) {
//			delete node->right;
//			node->right = NULL;
//		}
//
//		for (unsigned int i = 0; i < node->main.size(); i++) {
//			list.pop_back();
//		}
//	}
//}

void Operation::seperateOperator(vector<vector<Operation*> > &o_list, vector<Operation*> o) {

	int maxUnique = 0;
	for (unsigned int i = 0; i < o.size(); i++) {
		if (o[i]->uniqueNo > maxUnique) {
			maxUnique = o[i]->uniqueNo;
		}
	}
	for (unsigned int i = 0; i <= maxUnique; i++) {
		vector<Operation*> u;
		for (unsigned int j = 0; j < o.size(); j++) {
			if (o[j]->uniqueNo == i) {
				u.push_back(o[j]);
			}
		}
		if (u.size() != 0) {
			o_list.push_back(u);
		}
	}
	
	//vector<tree*> top;
	//int ifMax = 0;
	//int elseMax = 0;
	//int uniqueMax = 0;
	//for (unsigned int i = 0; i < o.size(); i++) {
	//	if (o[i]->getNumIF() > ifMax) {
	//		ifMax = o[i]->getNumIF();
	//	}
	//	if (o[i]->getNumElse() > elseMax) {
	//		elseMax = o[i]->getNumElse();
	//	}
	//	if (o[i]->uniqueNo > uniqueMax) {
	//		uniqueMax = o[i]->uniqueNo;
	//	}
	//}

	//for (unsigned int i = 0; i <= uniqueMax; i++) {
	//	for (unsigned int j = 0; j <= elseMax; j++) {
	//		for (unsigned int k = 0; k <= ifMax; k++) {
	//			vector<Operation*> stuff;
	//			bool _else = false;
	//			for (unsigned int l = 0; l < o.size(); l++) {
	//				if (o[l]->getNumElse() == j && o[l]->getNumIF() == k && o[l]->uniqueNo == i) { //Seperates operators with the same ID number to a list
	//					if (o[l]->_else) {
	//						_else = true;
	//					}
	//					stuff.push_back(o[l]);
	//				}
	//			}
	//			if (stuff.size() != 0) {
	//				if (_else) {
	//					top.push_back(new tree(stuff, k, j, i, true));
	//				}
	//				else {
	//					top.push_back(new tree(stuff, k, j, i, false));
	//				}
	//				
	//			}
	//		}
	//	}
	//}
	//tree* head = top[0];
	//top.erase(top.begin());
	//seperateOperatorHelper(top, head); //Creates an IF ELSE tree
	//vector<Operation*> list;
	//seperateOperatorHelper2(o_list, head, list); //Transverse the tree that make sense
}



