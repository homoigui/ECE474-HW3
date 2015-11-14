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
	Schedule *list_R;

	if (argc == 4) {
		int read = readfile(argv[1], v, o);
		read = checkVar(v, o);
		int latency = *argv[2] - '0'; //converts the latency to int

		if (read == 0) { // No errors
			list_R = new Schedule(o, latency);
			int check = list_R->listR(latency);
			if (check == -1) {
				cout << "Invalid Latency constraint. Try a bigger latency" << endl;
				return -1;
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