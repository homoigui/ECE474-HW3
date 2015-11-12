#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Bits.h"
#include "Variable.h"
#include <fstream>

using namespace std;

class Module
{
private:
	string op;
	Bits size;


public:
	Module();
	Module(string o, vector<string> v);
	~Module();
	void setOp(string o);
	string getOp();
	vector<string> var; //Holds variable in order 
	vector<Variable> ports;
	void setSize();
	Bits getSize();
	void addPort(vector<Variable> vs);
	void generateModule(ofstream& w_file, int& aCount, int& sCount, int& mCount, int& cCount, int& shrCount, int& shlCount, int& rCount, int& muxCount, int& scCount, int& sshrCount, int& srCount);
};

