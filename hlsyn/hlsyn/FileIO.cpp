#include "FileIO.h"


int readfile(char* file, vector<Variable*> &v, vector<Operation*> &o) {
	ifstream a_file(file);
	string line;
	int vertex = 1;
	int numIF = 0;
	int numElse = 0;
	int uniqueNo = 0;
	int noWrite = 0;
	vector<int> un;
	string conditionIF = "no condition";
	vector<string> conditions;
	vector<bool> el;
	bool _else = false;
	if (a_file) {
		//If File exist
		while (getline(a_file, line)) {
			int comment = line.find("//"); //Erases comment
			if (comment > 0) {
				line.erase(line.begin() + comment, line.end());
			}
			comment = line.find("\t"); //tabs
			if (comment >= 0) {
				line.erase(line.begin() + comment, line.begin() + comment + 1);
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
			
			//tokens[0] = input / output / variable, tokens[1] = dataType
			if (keyword.compare("input") == 0 || keyword.compare("output") == 0 || keyword.compare("variable") == 0) { // Input is found
				vector<string> var;

				for (vector<int>::size_type i = 2; i != tokens.size(); i++) { //Puts the variable in getline into its own vector
					var.push_back(tokens[i]);
				}
				for (vector<int>::size_type i = 0; i != var.size(); i++) { //Erase Commas
					var[i].erase(remove(var[i].begin(), var[i].end(), ','), var[i].end());
				}
				//Var is done

				for (vector<int>::size_type i = 0; i != var.size(); i++) {
					Variable *temp = new Variable(var[i], tokens[0], tokens[1]);
					v.push_back(temp);
				}

			}
			else if (keyword.compare("if") == 0){ //an if statement
				conditions.push_back(conditionIF);
				conditionIF = tokens[2];

				numIF++;
				un.push_back(noWrite);
				uniqueNo++;
				noWrite = uniqueNo;
				if (_else) {
					el.push_back(_else);
					_else = false;
				}
			}
			else if (keyword.compare("}") == 0) {
				if (!_else) {
					numIF--;
					
					conditionIF = conditions.back();
					conditions.pop_back();
					
					if (el.size() != 0) {
						_else = el.back();
						el.pop_back();
					}
				}
				else {
					_else = false;
					if (el.size() != 0) {
						_else = el.back();
						el.pop_back();
					}
					numElse--;
				}
				uniqueNo++;
				noWrite = uniqueNo;
			}
			else if (keyword.compare("else") == 0) {
				numElse++;
				un.push_back(noWrite);
				uniqueNo++;
				noWrite = uniqueNo;
				if (_else) {
					el.push_back(_else);
				}
				_else = true;
			}
			else if (tokens.size() != 0) {
				//The rest of the commands
				if (numIF == 0) {
					conditionIF = "no condition";
				}

				if (tokens[1].compare("=") != 0) {
					return -3; //Nothing found
				}
				else if (tokens.size() < 6) {//ITS ADD OR SUB OR MUL OR COMPO AND SHIFT
					if (tokens[3].compare("+") == 0 || tokens[3].compare("-") == 0 || tokens[3].compare("*") == 0 || tokens[3].compare("<") == 0 || tokens[3].compare(">") == 0 || tokens[3].compare("==") == 0 || tokens[3].compare("<<") == 0 || tokens[3].compare(">>") == 0) {
						int delay;
						if (tokens[3].compare("*") == 0) {
							delay = 2;
						}
						else {
							delay = 1;
						}
						Variable input1;
						Variable input2;
						Variable output;
						for (unsigned int i = 0; i < v.size(); i++) {
							if ((v[i]->getVar().compare(tokens[2])) == 0) { //Input1
								input1 = *v[i];
							}
							else if ((v[i]->getVar().compare(tokens[4])) == 0) { //Input2
								input2 = *v[i];
							}
							else if ((v[i]->getVar().compare(tokens[0])) == 0) { //output
								output = *v[i];
							}
						}
						Operation *otemp;
						if (tokens[3].compare("*") == 0) {
							otemp = new Operation(tokens[3], delay, vertex, input1, input2, output, 'm'); //Put in a vector for now, can pre schedule here
						}
						else {
							otemp = new Operation(tokens[3], delay, vertex, input1, input2, output, 'a');
						}

						//set the scheduling level
					
						
						vertex++;
						otemp->setNumIF(numIF);
						otemp->setConditionIF(conditionIF);
						otemp->setNumElse(numElse);
						otemp->_else = _else;
						otemp->uniqueNo = noWrite;
						otemp->setConditionIF(conditionIF);
						o.push_back(otemp);
				
					}
					else {
						return -2; //invalid operator
					}
				}
				else if (tokens.size() == 7) { //ITS MUX 
					if (tokens[3].compare("?") == 0 && tokens[5].compare(":") == 0) {
						Variable input1;
						Variable input2;
						Variable sel;
						Variable output;
						for (unsigned int i = 0; i < v.size(); i++) {
							if ((v[i]->getVar().compare(tokens[2])) == 0) { //sel
								sel = *v[i];
							}
							else if ((v[i]->getVar().compare(tokens[4])) == 0) { //Input1
								input1 = *v[i];
							}
							else if ((v[i]->getVar().compare(tokens[6])) == 0) { //Input2
								input2 = *v[i];
							}
							else if ((v[i]->getVar().compare(tokens[0])) == 0) { //output
								output = *v[i];
							}
						}
						Operation *mtemp = new Operation(tokens[3], 1, vertex, input1, input2, output, sel, 'a'); //Put in a vector for now, can pre schedule here
						vertex++;
						mtemp->setNumIF(numIF);
						mtemp->setNumElse(numElse);
						mtemp->setConditionIF(conditionIF);
						mtemp->_else = _else;
						mtemp->uniqueNo = noWrite;
						mtemp->setConditionIF(conditionIF);
						o.push_back(mtemp);
					}
					else {
						return -2;
					}
				}
				else {
					return -999; //critical error
				}
			}
		}


		a_file.close();
		return 0;
	}
	else {
		//Else file does not exist
		return -1;
	}
}


int checkVar(vector<Variable*> v, vector<Operation*> o) {
	vector<string> s;
	for (unsigned int i = 0; i < o.size(); i++) {
		if (!o[i]->getType().compare("?")) {
			s.push_back(o[i]->GetSel().getVar());
		}
		s.push_back(o[i]->getInput1().getVar());
		s.push_back(o[i]->getInput2().getVar());
		s.push_back(o[i]->getOutput().getVar());	
	}

	for (unsigned int i = 0; i < s.size(); i++) {
		if (!s[i].compare("error")) { //found an error variable does not exist
			return -9;
		}
	}
	
	return 0;
}
