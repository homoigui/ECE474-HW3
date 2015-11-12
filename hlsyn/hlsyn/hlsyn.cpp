#include <iostream>
#include "Bits.h"
#include "FileIO.h"
#include "Module.h"
#include "Variable.h"

using namespace std;

int main(int argc, char* argv[])
{
	vector<Variable> var;
	readfile(argv[1], var);
	return 0;
}