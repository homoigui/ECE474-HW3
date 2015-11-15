#ifndef FILEIO_H
#define FILEIO_H
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "Variable.h"
#include "Operation.h"

using namespace std;

int readfile(char* file, vector<Variable*> &v, vector<Operation*> &o);
int checkVar(vector<Variable*> v, vector<Operation*> o);

#endif