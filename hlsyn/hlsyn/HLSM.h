#pragma once
#include "Operation.h"
#include "Schedule.h"
#include "Variable.h"
#include <fstream>
#include <string>
#include <iostream>
//Output
class HLSM
{
private:
	vector<Schedule*> schedules;
	vector<Variable*> allVars; //operations in c code order

public:
	HLSM();
	HLSM(vector<Schedule*> s, vector<Variable*> o);
	~HLSM();

	void setSchedules(vector<Schedule*> s);
	vector<Schedule*> getSchedules();
	void setAllVars(vector<Variable*> o);
	vector<Variable*> getAllVars();

	int createHSM(char* file);
	void createHeader(ofstream &outputFile);
	int createStateReg(ofstream &outputFile);
	bool areSize(int i, string s, bool sign);
	int getPrevCycles(int schedule);
	int getSubIfElse(int s, int v, int c, int startIf, int startElse);
};

