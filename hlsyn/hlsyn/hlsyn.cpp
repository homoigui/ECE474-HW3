#include <iostream>
#include <vector>
#include <string>
#include "FileIO.h"
#include "Variable.h"
#include "Operation.h"
#include "Schedule.h"


using namespace std;
//TODO: Check for latency error
//Friend class?
int main(int argc, char* argv[])
{
	vector<Variable*> v;
	vector<Operation*> o;
	Schedule *HLS;

	if (argc == 4) {
		int read = readfile(argv[1], v, o);
		int latency = *argv[2] - '0'; //converts the latency to int
		cout << "place holder" << endl;
		if (read == 0) { // No errors
			HLS = new Schedule(o, latency);
			HLS->listR(latency);

		}
		else if (read == -1) {
			cout << "Could not open file" << endl;
			return -1;
		}
		else if (read == -2) {
			cout << "invalid operator" << endl;
		}
		else if (read == -3) {
			cout << "Unknown operation must be a typo" << endl;
		}

	}
	else {
		cout << "Usage: hlsyn cFile latency verilogFile" << endl;
	}

	return 0;
}