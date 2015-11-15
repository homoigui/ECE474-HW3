#include <iostream>
#include <vector>
#include <string>
#include "FileIO.h"
#include "Variable.h"
#include "Operation.h"
#include "Schedule.h"


using namespace std;

int main(int argc, char* argv[])
{
	vector<Variable*> v;
	vector<Operation*> o;
	vector<vector<Operation*> > o_list;
	vector<Schedule*> schedules;
	string s = argv[2];
	if (argc == 4) {
		int latency = stoi(s); //converts the latency to int
		int read = readfile(argv[1], v, o);
		read = checkVar(v, o);
		Operation::seperateOperator(o_list, o); //Gets different things for scheduling


		Schedule::renewOperations(o_list); //Renew all pointers in this list so it doesn't write to the same one

		if (read == 0) { // No errors
			for (unsigned int i = 0; i < o_list.size(); i++) {
				Schedule* temp = new Schedule(o_list[i], latency);
				schedules.push_back(temp);
			}
			for (unsigned int i = 0; i < schedules.size(); i++) {
				int check = schedules[i]->listR(latency);

				if (check == -1) {
					cout << "Invalid Latency constraint. Try a bigger latency" << endl;
					return -1;
				}
			}
		}
		else if (read == -1) {
			cout << "Could not open file" << endl;
			return -1;
		}
		else if (read == -2) {
			cout << "invalid operator" << endl;
			return -1;
		}
		else if (read == -3) {
			cout << "Unknown operation must be a typo" << endl;
			return -1;
		}
		else if (read == -9) {
			cout << "Missing declariation of variable" << endl;
			return -1;
		}

	}
	else {
		cout << "Usage: ./hlsyn cFile latency verilogFile" << endl;
	}

	return 0;
}