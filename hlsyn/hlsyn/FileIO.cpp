#include "FileIO.h"


int readfile(char* file, vector<Variable*> &v, vector<Operation*> &o) {
	ifstream a_file(file);
	string line;
	int vertex = 1;

	if (a_file) {
		//If File exist
		while (getline(a_file, line)) {
			int comment = line.find("//"); //Erases comment
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
			//else if (keyword.compare("if") == 0) { //an if statement
			//	//perform actions here if evaluates to true then skip else lines
			//	//ifTaken = true;
			//}
			//else if (keyword.compare("else") == 0) { //an if statement (else)
			//	//perform actions here if(ifTaken) == false
			//}
			else if (tokens.size() != 0) {
				//The rest of the commands
				

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
						for (int i = 0; i < v.size(); i++) {
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
						Operation *otemp = new Operation(tokens[3], delay, vertex, input1, input2, output); //Put in a vector for now, can pre schedule here
						vertex++;
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
						for (int i = 0; i < v.size(); i++) {
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
						Operation *mtemp = new Mux(tokens[3], 1, vertex, input1, input2, output, sel); //Put in a vector for now, can pre schedule here
						vertex++;
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

		return 0;
	}
	else {
		//Else file does not exist
		return -1;
	}

	a_file.close();
}



