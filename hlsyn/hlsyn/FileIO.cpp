#include "FileIO.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include "Variable.h"


int readfile(char* file, vector<Variable> &v) {
	ifstream a_file(file);
	string line;


	if (a_file) {
		//If File exist
		while (getline(a_file, line)) {
			int comment = line.find("//"); //Erasees comment
			if (comment > 0) {
				line.erase(line.begin() + comment, line.end());
			}
			istringstream iss(line);

			vector<string> tokens;
			copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens)); //splits each word into its own vector
			string keyword;

			if (tokens.size() == 0) { //if empty line do nothing and move to next line
				keyword = "nothing here skip to end";
			}
			else {
				keyword = tokens[0]; //First keyword
			}


			if (keyword.compare("input") == 0 || keyword.compare("output") == 0 || keyword.compare("wire") == 0 || keyword.compare("register") == 0) { // Input is found
				vector<string> var;

				for (vector<int>::size_type i = 2; i != tokens.size(); i++) { //Puts the variable in getline into its own vector
					var.push_back(tokens[i]);
				}
				for (vector<int>::size_type i = 0; i != var.size(); i++) { //Erase Commas
					var[i].erase(remove(var[i].begin(), var[i].end(), ','), var[i].end());
				}
				//Var is done

				for (vector<int>::size_type i = 0; i != var.size(); i++) {
					Variable *temp = new Variable();
					temp->var = var[i];
					temp->type = keyword;
					temp->sign = parseInt(tokens[1]).getSign();
					temp->size = parseInt(tokens[1]).getBits();
					v.push_back(*temp);
				}

			}
			else if (tokens.size() != 0) {
				//The rest of the commands
				
				if (tokens.size() < 4) { //means REG =
					if (tokens[1].compare("=") != 0) {
						return -2;
					}
					else {
						temp->setOp("=");
						temp->var.push_back(tokens[0]);
						temp->var.push_back(tokens[2]);

						temp->addPort(v);

						top.push_back(*temp);
					}


				}
				else if (tokens[1].compare("=") != 0) {
					return -3;
				}
				else if (tokens.size() < 6) {//ITS ADD OR SUB OR MUL OR COMPO AND SHIFT
					if (tokens[3].compare("+") == 0 || tokens[3].compare("-") == 0 || tokens[3].compare("*") == 0 || tokens[3].compare("<") == 0 || tokens[3].compare(">") == 0
						|| tokens[3].compare("==") == 0 || tokens[3].compare("<<") == 0 || tokens[3].compare(">>") == 0) {

					
					}
					else {
						return -2;
					}
				}
				else if (tokens.size() == 7) { //ITS MUX 
					if (tokens[3].compare("?") != 0 && tokens[5].compare(":") != 0) {
						return -2;
					}
					else {
						temp->setOp("?");
						temp->var.push_back(tokens[0]);
						temp->var.push_back(tokens[2]);
						temp->var.push_back(tokens[4]);
						temp->var.push_back(tokens[6]);

						temp->addPort(v);

						top.push_back(*temp);
					}
				}
				else {
					return -3;
				}
			}
		}

		return 0;
	}
	else {
		//Else file does not exist
		return -1;
	}

	a_file.close();
}

int writefile(char* file, vector<Variable> &v, vector<Module> &top) {
	ofstream w_file(file);
	int addCount = 1;
	int subCount = 1;
	int mulCount = 1;
	int compCount = 1;
	int shrCount = 1;
	int shlCount = 1;
	int regCount = 1;
	int muxCount = 1;

	int scCount = 1;
	int sshrCount = 1;
	int srCount = 1;

	bool first = true;
	bool writegt = false;
	bool writelt = false;
	bool writeeq = false;
	bool existComp = false;

	

	//check to see if signals exists for gt, lt, eq
	for (int i = 0; i < top.size(); i++) {
		if (top[i].getOp().compare(">") == 0) {
			existComp = true;
			writelt = true;
			writeeq = true;
		}
		else if (top[i].getOp().compare("<") == 0) {
			existComp = true;
			writegt = true;
			writeeq = true;
		}
		else if (top[i].getOp().compare("==") == 0) {
			existComp = true;
			writegt = true;
			writelt = true;
		}
	}

	//initialize all the module sizes
	for (int i = 0; i < top.size(); i++) {
		top[i].setSize();
	}

	if (w_file) {
		//generate top part of file
		w_file << "`timescale 1ns / 1ps" << endl << endl;
		w_file << "module circuit(";

		//add inputs to module list
		for (int i = 0; i < v.size(); i++) {
			if (v[i].type.compare("input") == 0) {
				if (first) {
					w_file << v[i].var;
					first = false;
				}
				else if (!first) {
					w_file << ", " << v[i].var;
				}
			}
		}
		w_file << ", Clk, Rst";

		//add outputs to module list
		for (int i = 0; i < v.size(); i++) {
			if (v[i].type.compare("output") == 0) {
				w_file << ", " << v[i].var;
			}
		}
		w_file << ");" << endl;

		//reset first
		first = true;

		//add 64 bit inputs
		if (areSize(64, v, "input")) {
			w_file << "\tinput [63:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("input") == 0 && v[i].size == 64) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 32 bit inputs
		if (areSize(32, v, "input")) {
			w_file << "\tinput [31:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("input") == 0 && v[i].size == 32) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 16 bit inputs
		if (areSize(16, v, "input")) {
			w_file << "\tinput [15:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("input") == 0 && v[i].size == 16) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 8 bit inputs
		if (areSize(8, v, "input")) {
			w_file << "\tinput [7:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("input") == 0 && v[i].size == 8) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 2 bit inputs
		if (areSize(2, v, "input")) {
			w_file << "\tinput [1:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("input") == 0 && v[i].size == 2) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 1 bit inputs
		if (areSize(1, v, "input")) {
			w_file << "\tinput ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("input") == 0 && v[i].size == 1) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << "Clk, Rst;" << endl;
		}

		//add 1 bit inputs
		if (!areSize(1, v, "input")) {
			w_file << "\tinput Clk, Rst;" << endl;
		}

		first = true;
		//add 64 bit output
		if (areSize(64, v, "output")) {
			w_file << "\toutput [63:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("output") == 0 && v[i].size == 64) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 32 bit output
		if (areSize(32, v, "output")) {
			w_file << "\toutput [31:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("output") == 0 && v[i].size == 32) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 16 bit output
		if (areSize(16, v, "output")) {
			w_file << "\toutput [15:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("output") == 0 && v[i].size == 16) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 8 bit output
		if (areSize(8, v, "output")) {
			w_file << "\toutput [7:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("output") == 0 && v[i].size == 8) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 2 bit output
		if (areSize(2, v, "output")) {
			w_file << "\toutput [1:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("output") == 0 && v[i].size == 2) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 1 bit output
		if (areSize(1, v, "output")) {
			w_file << "\toutput ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("output") == 0 && v[i].size == 1) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 64 bit wire
		if (areSize(64, v, "wire")) {
			w_file << "\twire [63:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("wire") == 0 && v[i].size == 64) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 32 bit wire
		if (areSize(32, v, "wire")) {
			w_file << "\twire [31:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("wire") == 0 && v[i].size == 32) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 16 bit wire
		if (areSize(16, v, "wire")) {
			w_file << "\twire [15:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("wire") == 0 && v[i].size == 16) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 8 bit wire
		if (areSize(8, v, "wire")) {
			w_file << "\twire [7:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("wire") == 0 && v[i].size == 8) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 2 bit wire
		if (areSize(2, v, "wire")) {
			w_file << "\twire [1:0] ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("wire") == 0 && v[i].size == 2) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}
			w_file << ";" << endl;
		}
		first = true;
		//add 1 bit wire
		if (areSize(1, v, "wire")) {
			w_file << "\twire ";
			for (int i = 0; i < v.size(); i++) {
				if (v[i].type.compare("wire") == 0 && v[i].size == 1) {
					if (first) {
						w_file << v[i].var;
						first = false;
					}
					else if (!first) {
						w_file << ", " << v[i].var;
					}
				}
			}

			//add missing signals to comparator if needed
			if (writegt && existComp) w_file << ", gt";
			if (writelt && existComp) w_file << ", lt";
			if (writeeq && existComp) w_file << ", eq";
			w_file << ";" << endl;
		}
		if (!areSize(1, v, "wire") && existComp) {
			bool gtUsed = false;
			bool ltUsed = false;
			bool eqUsed = false;

			w_file << "\twire ";
			if (writegt && first) {
				w_file << "gt";
				first = false;
				gtUsed = true;
			}
			if (writelt && first) {
				w_file << "lt";
				first = false;
				ltUsed = true;
			}
			if (writeeq && first) {
				w_file << "eq";
				first = false;
				eqUsed = true;
			}
			if (!gtUsed && !first && writegt) {
				w_file << ", gt";
				gtUsed = true;
			}
			if (!ltUsed && !first && writelt) {
				w_file << ", lt";
				ltUsed = true;
			}
			if (!eqUsed && !first && writeeq) {
				w_file << ", eq;" << endl;
				eqUsed = true;
			}
		}


		//generate modules
		w_file << endl;
		for (int m = 0; m < top.size(); m++) {
			top[m].generateModule(w_file, addCount, subCount, mulCount, compCount, shrCount, shlCount, regCount, muxCount, scCount, sshrCount, srCount);
		}

		//end module
		w_file << "endmodule";

		return 0;
	}
	else {
		cout << "cannot open / create file for output" << endl;
		return -1;
	}
}

Bits parseInt(string s) {
	Bits bits;
	if (s.compare("Int1") == 0) {
		bits.setBits(1);
		bits.setSign(true);
	}
	else if (s.compare("UInt1") == 0) {
		bits.setBits(1);
		bits.setSign(false);
	}
	else if (s.compare("Int2") == 0) {
		bits.setBits(2);
		bits.setSign(true);
	}
	else if (s.compare("UInt2") == 0) {
		bits.setBits(2);
		bits.setSign(false);
	}
	else if (s.compare("Int8") == 0) {
		bits.setBits(8);
		bits.setSign(true);
	}
	else if (s.compare("UInt8") == 0) {
		bits.setBits(8);
		bits.setSign(false);
	}
	else if (s.compare("Int16") == 0) {
		bits.setBits(16);
		bits.setSign(true);
	}
	else if (s.compare("UInt16") == 0) {
		bits.setBits(16);
		bits.setSign(false);
	}
	else if (s.compare("Int32") == 0) {
		bits.setBits(32);
		bits.setSign(true);
	}
	else if (s.compare("UInt32") == 0) {
		bits.setBits(32);
		bits.setSign(false);
	}
	else if (s.compare("Int64") == 0) {
		bits.setBits(64);
		bits.setSign(true);
	}
	else if (s.compare("UInt64") == 0) {
		bits.setBits(64);
		bits.setSign(false);
	}
	return bits;
}

bool areSize(int s, vector<Variable> &v, string t) {
	for (int i = 0; i < v.size(); i++) {
		if (v[i].size == s && v[i].type == t) {
			return true;
		}
	}
	return false;
}

