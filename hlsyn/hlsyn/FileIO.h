#ifndef FILEIO_H
#define FILEIO_H
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "Module.h"
#include "Bits.h"
#include "Variable.h"

using namespace std;

int writefile(char* file, vector<Variable> &v, vector<Module> &top);
Bits parseInt(string s);
int readfile(char* file, vector<Variable> &v, vector<Module> &top);
bool areSize(int s, vector<Variable> &v, string t);
int checkVar(vector<Variable> &v, vector<Module> &top);



#endif