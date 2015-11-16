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
	w_file << endl << "\treg [" << bitWidth << ":0] State;" << endl;

	return numStates;
}

int HLSM::createHSM(char* file) {
	ofstream w_file(file);

	if (w_file) {
		createHeader(w_file);
		int numStates = createStateReg(w_file);

		//create state logic
		w_file << endl << "\talways@ (posedge Clk) begin" << endl;
		w_file << "\t\tif (Rst == 1'b1) begin" << endl << "\t\t\tState <= STATE_WAIT;" << endl << "\t\t\tDone <= 0;" << endl;
		//assign all outputs zero
		for (int i = 0; i < allVars.size(); i++) {
			if (allVars[i]->getType().compare("output") == 0) {
				w_file << "\t\t\t" << allVars[i]->getVar() << " <= 0;" << endl;
			}
		}
		w_file << "\t\tend" << endl << "\t\telse begin" << endl;
		w_file << "\t\tcase (State)" << endl;
		
		//write wait state
		w_file << "\t\t\tSTATE_WAIT: begin" << endl << "\t\t\t\tif (Start = 1'b1) begin" << endl << "\t\t\t\t\tState = STATE_1;" << endl << "\t\t\t\tend" << endl << "\t\t\t\telse begin" << endl <<
			"\t\t\t\t\tState = STATE_WAIT;" << endl << "\t\t\t\tend" << endl << "\t\t\tend" << endl;

		//write operations states
		int vertex = 0;
		int schedule = 0;
		int latestVertex;
		int latestSchedule;
		int actualState;
		for (int i = 1; i < numStates - 1; i++) {
			w_file << "\t\t\tSTATE_" << i << ": begin" << endl;
			latestVertex = -1;
			latestSchedule = -1;
			for (schedule = 0; schedule < schedules.size(); schedule++) { //iterate through each schedule 
				for (vertex = 0; vertex < schedules[schedule]->getVertices().size(); vertex++) { //iterate through each vertex in the schedule to search for a vertex that starts at [i]
					//if the vertex start time (normalized with the number of states) is the same as the state counter, print the operations into that state
					actualState = getPrevCycles(schedule);
					if (schedules[schedule]->getVertices()[vertex]->getBeginTime() + actualState == i && schedules[schedule]->getVertices()[vertex]->getType().compare("?") == 0) {//mux
						w_file << "\t\t\t\t" << schedules[schedule]->getVertices()[vertex]->getOutput().getVar() << " <= " << "sel " << schedules[schedule]->getVertices()[vertex]->getType() <<
							" " + schedules[schedule]->getVertices()[vertex]->getInput1().getVar() << " : " + schedules[schedule]->getVertices()[vertex]->getInput2().getVar() + ";" << endl;
						latestVertex = vertex;
						latestSchedule = schedule;
					}
					else if (schedules[schedule]->getVertices()[vertex]->getBeginTime() + actualState == i && schedules[schedule]->getVertices()[vertex]->getType().compare("?") != 0) {//any other op
						w_file << "\t\t\t\t" << schedules[schedule]->getVertices()[vertex]->getOutput().getVar() << " <= " << schedules[schedule]->getVertices()[vertex]->getInput1().getVar() <<
							" " + schedules[schedule]->getVertices()[vertex]->getType() << " " + schedules[schedule]->getVertices()[vertex]->getInput2().getVar() + ";" << endl;
						latestVertex = vertex;
						latestSchedule = schedule;
					}
				}
			}
			//determine next state
		}

		//write final state
		w_file << "\t\t\tSTATE_FINAL: begin" << endl << "\t\t\t\tDone <= 1'b1;" << endl << "\t\t\t\tState <= STATE_WAIT;" << endl << "\t\t\tend" << endl;

		//write default
		w_file << "\t\t\tdefault: State <= STATE_WAIT;" << endl;

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

bool HLSM::areSize(int s, string t) {
	for (int i = 0; i < allVars.size(); i++) {
		if (allVars[i]->getSize() == s && allVars[i]->getType() == t) {
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
			cycles = cycles + schedules[k]->getVertices().back()->getEndTime() - 1;
		}
		return cycles;
	}
}

