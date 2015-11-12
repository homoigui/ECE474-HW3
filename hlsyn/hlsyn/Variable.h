#pragma once
#include <string>
#include <iostream>

using namespace std;
class Variable
{
public:
	Variable();
	~Variable();

	string var;
	string type;
	bool sign;
	int size;
};

