#include "HLSM.h"
#include <math.h>
#include <sstream>


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
	
	//add signed variables
	//add 64 bit inputs
	if (areSize(64, "input", true)) {
		w_file << "\tinput signed [63:0] ";
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
	if (areSize(32, "input", true)) {
		w_file << "\tinput signed [31:0] ";
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
	if (areSize(16, "input", true)) {
		w_file << "\tinput signed [15:0] ";
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
	if (areSize(8, "input", true)) {
		w_file << "\tinput signed [7:0] ";
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
	if (areSize(2, "input", true)) {
		w_file << "\tinput signed [1:0] ";
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
	if (areSize(1, "input", true)) {
		w_file << "\tinput signed ";
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
	if (areSize(64, "output", true)) {
		w_file << "\toutput reg signed [63:0] ";
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
	if (areSize(32, "output", true)) {
		w_file << "\toutput reg signed [31:0] ";
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
	if (areSize(16, "output", true)) {
		w_file << "\toutput reg signed [15:0] ";
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
	if (areSize(8, "output", true)) {
		w_file << "\toutput reg signed [7:0] ";
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
	if (areSize(2, "output", true)) {
		w_file << "\toutput reg signed [1:0] ";
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
	if (areSize(1, "output", true)) {
		w_file << "\toutput reg signed ";
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

	first = true;
	//add 64 bit reg
	if (areSize(64, "variable", true)) {
		w_file << "\treg signed [63:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("variable") == 0 && allVars[i]->getSize() == 64) {
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
	//add 32 bit reg
	if (areSize(32, "variable", true)) {
		w_file << "\treg signed [31:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("variable") == 0 && allVars[i]->getSize() == 32) {
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
	//add 16 bit reg
	if (areSize(16, "variable", true)) {
		w_file << "\treg signed [15:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("variable") == 0 && allVars[i]->getSize() == 16) {
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
	//add 8 bit reg
	if (areSize(8, "variable", true)) {
		w_file << "\treg signed [7:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("variable") == 0 && allVars[i]->getSize() == 8) {
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
	//add 2 bit reg
	if (areSize(2, "variable", true)) {
		w_file << "\treg signed [1:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("variable") == 0 && allVars[i]->getSize() == 2) {
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
	//add 1 bit reg
	if (areSize(1, "variable", true)) {
		w_file << "\treg signed ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("variable") == 0 && allVars[i]->getSize() == 1) {
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

	//unsigned
	//add 64 bit inputs
	if (areSize(64, "input", false)) {
		w_file << "\tinput unsigned [63:0] ";
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
	if (areSize(32, "input", false)) {
		w_file << "\tinput unsigned [31:0] ";
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
	if (areSize(16, "input", false)) {
		w_file << "\tinput unsigned [15:0] ";
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
	if (areSize(8, "input", false)) {
		w_file << "\tinput unsigned [7:0] ";
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
	if (areSize(2, "input", false)) {
		w_file << "\tinput unsigned [1:0] ";
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
	if (areSize(1, "input", false)) {
		w_file << "\tinput unsigned ";
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
	if (areSize(64, "output", false)) {
		w_file << "\toutput reg unsigned [63:0] ";
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
	if (areSize(32, "output", false)) {
		w_file << "\toutput reg unsigned [31:0] ";
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
	if (areSize(16, "output", false)) {
		w_file << "\toutput reg unsigned [15:0] ";
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
	if (areSize(8, "output", false)) {
		w_file << "\toutput reg unsigned [7:0] ";
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
	if (areSize(2, "output", false)) {
		w_file << "\toutput reg unsigned [1:0] ";
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
	if (areSize(1, "output", false)) {
		w_file << "\toutput reg unsigned ";
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

	first = true;
	//add 64 bit reg
	if (areSize(64, "variable", false)) {
		w_file << "\treg unsigned [63:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("variable") == 0 && allVars[i]->getSize() == 64) {
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
	//add 32 bit reg
	if (areSize(32, "variable", false)) {
		w_file << "\treg unsigned [31:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("variable") == 0 && allVars[i]->getSize() == 32) {
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
	//add 16 bit reg
	if (areSize(16, "variable", false)) {
		w_file << "\treg unsigned [15:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("variable") == 0 && allVars[i]->getSize() == 16) {
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
	//add 8 bit reg
	if (areSize(8, "variable", false)) {
		w_file << "\treg unsigned [7:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("variable") == 0 && allVars[i]->getSize() == 8) {
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
	//add 2 bit reg
	if (areSize(2, "variable", false)) {
		w_file << "\treg unsigned [1:0] ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("variable") == 0 && allVars[i]->getSize() == 2) {
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
	//add 1 bit reg
	if (areSize(1, "variable", false)) {
		w_file << "\treg unsigned ";
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("variable") == 0 && allVars[i]->getSize() == 1) {
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
	int numStates = 0;
	for (int i = 0; i < schedules.size(); i++) {
		numStates = numStates + schedules[i]->getVertices().back()->getTime() + schedules[i]->getVertices().back()->getDelay() - 1;
	}

	//take log base 2 of number of states, then call ceil() to round up to next int
	int bitWidth = ceil(log2(numStates+2)) - 1;

	//write state names
	int nameCount = 0;
	vector<string> names;
	names.push_back("STATE_WAIT");

	for (int i = 1; i < numStates + 2 - 1; i++) {
		names.push_back("STATE_" + to_string(i));
	}

	names.push_back("STATE_FINAL");
	w_file << endl << "\tlocalparam STATE_WAIT = " << bitWidth << "\'d0," << endl << "\t           ";
	for (int i = 1; i < names.size() - 1; i++) {
		w_file << names[i] << " = " << bitWidth << "\'d" << i << "," << endl << "\t           ";
	}
	w_file << "STATE_FINAL = " << bitWidth << "\'d" << names.size() - 1 << ";" << endl;

	//write state regs
	w_file << endl << "\treg [" << bitWidth << ":0] State;" << endl;

	return numStates;
}

int HLSM::createHSM(char* file) {
	ofstream w_file(file);

	if (w_file) {
		int vertex = 0;
		int schedule = 0;
		int cycle = 1;
		bool opFound = false;
		stringstream opLine;
		int latestVertex = 0;
		int latestSchedule = 0;
		
		//write module
		createHeader(w_file);
		//write state header
		int states = createStateReg(w_file);
		stringstream stateLogic;

		//create always @
		stateLogic << endl << "\talways@ (posedge Clk) begin" << endl;
		stateLogic << "\t\tif (Rst == 1'b1) begin" << endl << "\t\t\tState <= STATE_WAIT;" << endl << "\t\t\tDone <= 0;" << endl;
		//assign all outputs zero
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("output") == 0) {
				stateLogic << "\t\t\t" << allVars[i]->getVar() << " <= 0;" << endl;
			}
		}
		stateLogic << "\t\tend" << endl << "\t\telse begin" << endl;
		stateLogic << "\t\tcase (State)" << endl;
		
		//write wait state
		stateLogic << "\t\t\tSTATE_WAIT: begin" << endl << "\t\t\t\tDone <= 0;" << endl << "\t\t\t\tif (Start == 1'b1) begin" << endl << "\t\t\t\t\tState <= STATE_1;" << endl << "\t\t\t\tend" << endl << "\t\t\t\telse begin" << endl <<
			"\t\t\t\t\tState <= STATE_WAIT;" << endl << "\t\t\t\tend" << endl << "\t\t\tend" << endl;

		//write operations states
		for (int i = 1; i <= states; i++) { //for each state check to see if a vertex starts or ends in it
			stateLogic << "\t\t\tSTATE_" << i << ": begin" << endl;
			for (schedule = 0; schedule < schedules.size(); schedule++) {
				cycle = i - getPrevCycles(schedule);
				for (vertex = 0; vertex < schedules[schedule]->getVertices().size(); vertex++) {
					//find all vertexes with start time = current state and add to operation stringbuffer
					if (schedules[schedule]->getVertices()[vertex]->getTime() == cycle && schedules[schedule]->getVertices()[vertex]->getType().compare("?") == 0) {//if vertex start time = state and vertex is mux
						opLine << "\t\t\t\t" << schedules[schedule]->getVertices()[vertex]->getOutput().getVar() << " <= " << schedules[schedule]->getVertices()[vertex]->GetSel().getVar() << " " + schedules[schedule]->getVertices()[vertex]->getType() <<
							" " + schedules[schedule]->getVertices()[vertex]->getInput1().getVar() << " : " + schedules[schedule]->getVertices()[vertex]->getInput2().getVar() + ";" << endl;
					}
					else if (schedules[schedule]->getVertices()[vertex]->getTime() == cycle && schedules[schedule]->getVertices()[vertex]->getType().compare("?") != 0) { //any other op
						opLine << "\t\t\t\t" << schedules[schedule]->getVertices()[vertex]->getOutput().getVar() << " <= " << schedules[schedule]->getVertices()[vertex]->getInput1().getVar() <<
							" " + schedules[schedule]->getVertices()[vertex]->getType() << " " + schedules[schedule]->getVertices()[vertex]->getInput2().getVar() + ";" << endl;
					}

					//check if vertex ends this state
					int endsTime = schedules[schedule]->getVertices()[vertex]->getTime() + schedules[schedule]->getVertices()[vertex]->getDelay() -1 + getPrevCycles(schedule);
					if (endsTime == i) {
						latestVertex = vertex;
						latestSchedule = schedule;
						opFound = true;
					}
				}
			}
			//write state ops
			stateLogic << opLine.str();
			opLine.str("");

			//determine next state
			//if this is the last vertex in the schedule and this isnt the last schedule
			if ((latestVertex == schedules[latestSchedule]->getVertices().size()-1) && latestSchedule < schedules.size() - 1 && opFound) {
				if (schedules[latestSchedule + 1]->getVertices()[0]->getConditionIF().compare("no condition") != 0) {//check to see if entering if
																											   //code has reached an if statement, now check how many cycles the if lasts for
					stringstream waka;
					int ifcycles = 0;
					string indent = "";

					int f = schedules[latestSchedule + 1]->getVertices()[0]->getNumIF();
					ifcycles = schedules[latestSchedule + f]->getVertices().back()->getTime() + schedules[latestSchedule + f]->getVertices().back()->getDelay() - 1;
					waka << endl << "\t\t\t\tif (" << schedules[latestSchedule + f]->getVertices()[0]->getConditionIF() << ") begin" << endl << waka.str() <<
						endl << "\t\t\t\tend" << endl << "\t\t\t\telse begin" << endl <<
						"\t\t\t\t\tState <= STATE_" << i + f + ifcycles << ";" << endl << "\t\t\t\tend" << endl << "\t\t\tend" << endl;
					
					for (f = schedules[latestSchedule + 1]->getVertices()[0]->getNumIF() - 1; f > 0; f--) { //create nested ifs if needed
						ifcycles = schedules[latestSchedule + f]->getVertices().back()->getTime() + schedules[latestSchedule + f]->getVertices().back()->getDelay() - 1;
						waka << endl << "\t\t\t\tif (" << schedules[latestSchedule + f]->getVertices()[0]->getConditionIF() << ") begin" << endl << waka.str() <<
							endl << "\t\t\t\tend" << endl << "\t\t\t\telse begin" << endl << "\t\t\t\t\tState <= STATE_" << i + f + ifcycles << ";" << endl << "\t\t\t\tend" 
							<< endl << "\t\t\tend" << endl;
					}
					stateLogic << waka.str();
					opFound = false;
				}
				else if (schedules[latestSchedule + 1]->getVertices()[0]->getConditionIF().compare("no condition") == 0) { //leaving a branch, just go to next vertex
					stateLogic << endl << "\t\t\t\tState <= STATE_" << i + 1 << ";" << endl << "\t\t\tend" << endl;
					opFound = false;
				}
			}
			else if ((latestVertex < schedules[latestSchedule]->getVertices().size()-1) && opFound) {//else if this is a vertex in the middle of a schedule go to next state
				stateLogic << endl << "\t\t\t\tState <= STATE_" << i + 1 << ";" << endl << "\t\t\tend" << endl;
				opFound = false;
			}
			else if ((latestVertex == schedules[latestSchedule]->getVertices().size()-1) && (latestSchedule == schedules.size()-1) && opFound){ //else this is the last vertex and schedule, go to done state
				stateLogic << endl << "\t\t\t\tState <= STATE_FINAL;" << endl << "\t\t\tend" << endl;
				opFound = false;
			}
			else if (!opFound) { //else if no operation finishes this cycle just go to next state
				stateLogic << endl << "\t\t\t\tState <= STATE_" << i + 1 << ";" << endl << "\t\t\tend" << endl;
			}
		}

		//write final state
		stateLogic << "\t\t\tSTATE_FINAL: begin" << endl << "\t\t\t\tDone <= 1'b1;" << endl << "\t\t\t\tState <= STATE_WAIT;" << endl << "\t\t\tend" << endl;

		//write default
		stateLogic << "\t\t\tdefault: State <= STATE_WAIT;" << endl;


		
		//write state logic
		w_file << stateLogic.str();

		//finish off module
		w_file << "\t\tendcase" << endl; //endcase
		w_file << "\t\tend" << endl; //end else
		w_file << "\tend" << endl; //end always
		w_file << "endmodule";

		w_file.close();
		return 0;
	}
	else {
		cout << "cannot open / create file for output" << endl;
		return -1;
	}
}

bool HLSM::areSize(int s, string t, bool sign) {
	for (int i = 0; i < allVars.size(); i++) {
		if (allVars[i]->getSize() == s && allVars[i]->getType() == t && allVars[i]->getSign() == sign) {
			return true;
		}
	}
	return false;
}

int HLSM::getPrevCycles(int schedule) {
	int cycles = 0;
	if (schedule == 0) {
		return 0;
	}
	else{
		for (int k = 0; k < schedule; k++) {
			cycles = cycles + schedules[k]->getVertices().back()->getTime() + schedules[k]->getVertices().back()->getDelay() - 1;
		}
		return cycles;
	}
}

