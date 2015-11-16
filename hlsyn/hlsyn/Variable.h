#pragma once
#include <string>
#include <iostream>

using namespace std;
class Variable
{
private:
	string var;
	string type; //Input output
	bool sign; //Int / Uint
	int size; // Amount of bits
	void parseDataType(string s);
public:
	Variable();
	~Variable();
	Variable(string v, string t, string d); //d is dataType
	string getVar();
	string getType();

	int getSize();
	bool getSign();
};

