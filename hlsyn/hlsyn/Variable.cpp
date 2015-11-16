#include "Variable.h"



Variable::Variable(){
	this->var = "error";
	this->type = "error";
	this->sign = false;
	this->size = 0;
}

Variable::Variable(string v, string t, string d) {
	this->var = v;
	this->type = t;
	this->parseDataType(d);
}

Variable::~Variable(){

}

string Variable::getVar() {
	return var;
}

void Variable::parseDataType(string s) {
	if (s.compare("Int1") == 0) {
		this->size = 1;
		this->sign = true;
	}
	else if (s.compare("UInt1") == 0) {
		this->size = 1;
		this->sign = false;
	}
	else if (s.compare("Int2") == 0) {
		this->size = 2;
		this->sign = true;
	}
	else if (s.compare("UInt2") == 0) {
		this->size = 2;
		this->sign = false;
	}
	else if (s.compare("Int8") == 0) {
		this->size = 8;
		this->sign = true;
	}
	else if (s.compare("UInt8") == 0) {
		this->size = 8;
		this->sign = false;
	}
	else if (s.compare("Int16") == 0) {
		this->size = 16;
		this->sign = true;
	}
	else if (s.compare("UInt16") == 0) {
		this->size = 16;
		this->sign = false;
	}
	else if (s.compare("Int32") == 0) {
		this->size = 32;
		this->sign = true;
	}
	else if (s.compare("UInt32") == 0) {
		this->size = 32;
		this->sign = false;
	}
	else if (s.compare("Int64") == 0) {
		this->size = 64;
		this->sign = true;
	}
	else if (s.compare("UInt64") == 0) {
		this->size = 64;
		this->sign = false;
	}
}

string Variable::getType() {
	return type;
}

int Variable::getSize() {
	return size;
}

bool Variable::getSign() {
	return sign;
}