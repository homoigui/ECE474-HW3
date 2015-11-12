#include "FileIO.h"
#include <sstream>
#include <algorithm>
#include <iterator>


int readfile(char* file) {
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



