#include "HLSM.h"
#include <math.h>



HLSM::HLSM()
{
}


HLSM::~HLSM()
{
}

HLSM::HLSM(vector<Schedule*> s, vector<Variable*> o) {
	schedules = s;
	allVars = o;
}

void HLSM::setAllVars(vector<Variable*> o) {
	allVars = o;
}

vector<Variable*> HLSM::getAllVars() {
	return allVars;
}

void HLSM::setSchedules(vector<Schedule*> s) {
	schedules = s;
}

vector<Schedule*> HLSM::getSchedules() {
	return schedules;
}

void HLSM::createHeader(ofstream &w_file) {
	bool first = true;
	//generate top part of file
	w_file << "`timescale 1ns / 1ps" << endl << endl;
	w_file << "module HLSM (Clk, Rst, Start, Done, ";

	//add inputs to module list
	for (int i = 0; i < allVars.size(); i++) {
		if (allVars[i]->getType().compare("input") == 0) {
			if (first) {
				w_file << allVars[i]->getVar();
				first = false;
			}
			else if (!first) {
				w_file << ", " << allVars[i]->getVar();
			}
		}
	}

	//add outputs to module list
	for (int i = 0; i < allVars.size(); i++) {
		if (allVars[i]->getType().compare("output") == 0) {
			w_file << ", " << allVars[i]->getVar();
		}
	}
	w_file << ");" << endl;

	//reset first
	first = true;

	//add 64 bit inputs
	if (areSize(64, "input")) {
		w_file << "\tinput [63:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("input") == 0 && allVars[i]->getSize() == 64) {
				if (first) {
					w_file << allVars[i]->getVar();
					first = false;
				}
				else if (!first) {
					w_file << ", " << allVars[i]->getVar();
				}
			}
		}
		w_file << ";" << endl;
	}
	first = true;
	//add 32 bit inputs
	if (areSize(32, "input")) {
		w_file << "\tinput [31:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("input") == 0 && allVars[i]->getSize() == 32) {
				if (first) {
					w_file << allVars[i]->getVar();
					first = false;
				}
				else if (!first) {
					w_file << ", " << allVars[i]->getVar();
				}
			}
		}
		w_file << ";" << endl;
	}
	first = true;
	//add 16 bit inputs
	if (areSize(16, "input")) {
		w_file << "\tinput [15:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("input") == 0 && allVars[i]->getSize() == 16) {
				if (first) {
					w_file << allVars[i]->getVar();
					first = false;
				}
				else if (!first) {
					w_file << ", " << allVars[i]->getVar();
				}
			}
		}
		w_file << ";" << endl;
	}
	first = true;
	//add 8 bit inputs
	if (areSize(8, "input")) {
		w_file << "\tinput [7:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("input") == 0 && allVars[i]->getSize() == 8) {
				if (first) {
					w_file << allVars[i]->getVar();
					first = false;
				}
				else if (!first) {
					w_file << ", " << allVars[i]->getVar();
				}
			}
		}
		w_file << ";" << endl;
	}
	first = true;
	//add 2 bit inputs
	if (areSize(2, "input")) {
		w_file << "\tinput [1:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("input") == 0 && allVars[i]->getSize() == 2) {
				if (first) {
					w_file << allVars[i]->getVar();
					first = false;
				}
				else if (!first) {
					w_file << ", " << allVars[i]->getVar();
				}
			}
		}
		w_file << ";" << endl;
	}
	first = true;
	//add 1 bit inputs
	if (areSize(1, "input")) {
		w_file << "\tinput ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("input") == 0 && allVars[i]->getSize() == 1) {
				if (first) {
					w_file << allVars[i]->getVar();
					first = false;
				}
				else if (!first) {
					w_file << ", " << allVars[i]->getVar();
				}
			}
		}
	}

	w_file << "\tinput Clk, Rst, Start;" << endl;

	first = true;
	//add 64 bit output
	if (areSize(64, "output")) {
		w_file << "\toutput [63:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("output") == 0 && allVars[i]->getSize() == 64) {
				if (first) {
					w_file << allVars[i]->getVar();
					first = false;
				}
				else if (!first) {
					w_file << ", " << allVars[i]->getVar();
				}
			}
		}
		w_file << ";" << endl;
	}
	first = true;
	//add 32 bit output
	if (areSize(32, "output")) {
		w_file << "\toutput [31:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("output") == 0 && allVars[i]->getSize() == 32) {
				if (first) {
					w_file << allVars[i]->getVar();
					first = false;
				}
				else if (!first) {
					w_file << ", " << allVars[i]->getVar();
				}
			}
		}
		w_file << ";" << endl;
	}
	first = true;
	//add 16 bit output
	if (areSize(16, "output")) {
		w_file << "\toutput [15:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("output") == 0 && allVars[i]->getSize() == 16) {
				if (first) {
					w_file << allVars[i]->getVar();
					first = false;
				}
				else if (!first) {
					w_file << ", " << allVars[i]->getVar();
				}
			}
		}
		w_file << ";" << endl;
	}
	first = true;
	//add 8 bit output
	if (areSize(8, "output")) {
		w_file << "\toutput [7:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("output") == 0 && allVars[i]->getSize() == 8) {
				if (first) {
					w_file << allVars[i]->getVar();
					first = false;
				}
				else if (!first) {
					w_file << ", " << allVars[i]->getVar();
				}
			}
		}
		w_file << ";" << endl;
	}
	first = true;
	//add 2 bit output
	if (areSize(2, "output")) {
		w_file << "\toutput [1:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("output") == 0 && allVars[i]->getSize() == 2) {
				if (first) {
					w_file << allVars[i]->getVar();
					first = false;
				}
				else if (!first) {
					w_file << ", " << allVars[i]->getVar();
				}
			}
		}
		w_file << ";" << endl;
	}
	first = true;
	//add 1 bit output
	if (areSize(1, "output")) {
		w_file << "\toutput ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("output") == 0 && allVars[i]->getSize() == 1) {
				if (first) {
					w_file << allVars[i]->getVar();
					first = false;
				}
				else if (!first) {
					w_file << ", " << allVars[i]->getVar();
				}
			}
		}
		w_file << ";" << endl;
	}

	w_file << "\toutput reg Done;" << endl;
}

int HLSM::createStateReg(ofstream &w_file) {
	//calculate number of states (states = maxmim time for each schedle + wait + done)
	int numStates = 2;
	for (int i = 0; i < schedules.size(); i++) {
		numStates = numStates + schedules[i]->getVertices().back()->getEndTime();
	}

	//take log base 2 of number of states, then call ceil() to round up to next int
	int bitWidth = ceil(log2(numStates)) - 1;

	//write state names
	int nameCount = 0;
	vector<string> names;
	names.push_back("STATE_WAIT");

	for (int i = 1; i < numStates - 1; i++) {
		names.push_back("STATE_" + to_string(i));
	}

	names.push_back("STATE_FINAL");
	w_file << endl << "\tlocalparam STATE_WAIT = " << bitWidth << "\'d0," << endl << "\t           ";
	for (int i = 1; i < names.size() - 1; i++) {
		w_file << names[i] << " = " << bitWidth << "\'d" << i << "," << endl << "\t           ";
	}
	w_file << "STATE_FINAL = " << bitWidth << "\'d" << names.size() - 1 << ";" << endl;

	//write state regs
	w_file << endl << "\treg [" << bitWidth << ":0] State" << endl;
	w_file << "\treg [" << bitWidth << ":0] nextState" << endl << endl;

	return numStates;
}

int HLSM::createHSM(char* file) {
	ofstream w_file(file);

	if (w_file) {
		createHeader(w_file);
		int numStates = createStateReg(w_file);

		//create state logic
		w_file << "\talways@ (State, nextState) begin" << endl;
		w_file << "\t\tcase (State)" << endl;
		
		//write wait state
		w_file << "\t\t\tSTATE_WAIT: begin" << endl << "\t\t\t\tif (Start = 1'b1) begin" << endl << "\t\t\t\t\tnextState = STATE_1" << endl << "\t\t\t\tend" << endl << "\t\t\t\telse begin" << endl <<
				"\t\t\t\t\tnextState = STATE_WAIT" << endl << "\t\t\t\tend" << endl << "\t\t\tend" << endl;

		//write operations states
	

		w_file.close();
		return 0;
	}
	else {
		cout << "cannot open / create file for output" << endl;
		return -1;
	}
}

bool HLSM::areSize(int s, string t) {
	for (int i = 0; i < allVars.size(); i++) {
		if (allVars[i]->getSize() == s && allVars[i]->getType() == t) {
			return true;
		}
	}
	return false;
}

