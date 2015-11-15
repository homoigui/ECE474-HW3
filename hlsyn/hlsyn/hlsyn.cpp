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
	vector<Schedule*> schedules;
	vector<int> levels;
	string s = argv[2];
	if (argc == 4) {
		int read = readfile(argv[1], v, o, levels);
		read = checkVar(v, o);
		int latency = stoi(s); //converts the latency to int

		if (read == 0) { // No errors

			//initialize each schedule
			for (int i = 0; i < levels.size(); i++) {
				schedules.push_back(new Schedule(latency));

				//add each operation of that scheduling level to that scheduling level
				for (int j = 0; j < o.size(); j++) {
					if (o[j]->getLevel() == levels[i]) {
						schedules[i]->getVertices().push_back(o[j]);
					}
				}
			}
			
			//do this for each individual schedule
			for (int i = 0; i < schedules.size(); i++) {
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