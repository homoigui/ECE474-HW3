#pragma once
#include <string>
#include <iostream>
#include "Bits.h"

using namespace std;

class Bits
{
private:
	bool sign;
	int bits;

public:
	Bits();
	Bits(bool s, int b);
	~Bits();

	void setSign(bool s);
	void setBits(int b);
	int getBits();
	bool getSign();
};