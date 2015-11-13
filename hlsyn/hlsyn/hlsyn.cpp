#include <iostream>
#include <vector>
#include <string>
#include "FileIO.h"
#include "Variable.h"
#include "Operation.h"


using namespace std;

int main(int argc, char* argv[])
{
	vector<Variable> v;
	vector<Operation> o;

	if (argc == 4) {
		int read = readfile(argv[1], v, o);
		cout << "place holder" << endl;
		if (read == 0) { // No errors
			
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