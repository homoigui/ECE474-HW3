#include "Module.h"




Module::Module()
{
	this->op = "NOTHING";
	this->size.setBits(0);
	this->size.setSign(false);
}

Module::Module(string o, vector<string> v) {
	this->op = o;
	for (int i = 0; i < v.size(); i++) {
		this->var.push_back(v[i]);
	}
	//this->var = v;
	this->size.setBits(0);
	this->size.setSign(false);
}

Module::~Module()
{
}

void Module::setOp(string o) {
	op = o;
}

string Module::getOp() {
	return op;
}

void Module::setSize() {
	if (this->getOp().compare(">") == 0 || this->getOp().compare("<") == 0 || this->getOp().compare("==") == 0) { //if a comparator use largest input to get size
		for (int i = 1; i < ports.size(); i++) {
			if (ports[i].size > size.getBits()) {
				size.setBits(ports[i].size);
				size.setSign(ports[i].sign);
			}
		}

	}
	else { //else use outputs to get sizes
		size.setBits(ports[0].size); //ports are ordered outputs -> inputs, so port[0] is always wire or output
		size.setSign(ports[0].sign);
	}
}

Bits Module::getSize() {
	return size;
}

void Module::addPort(vector<Variable> vs) {
	for (int i = 0; i < var.size(); i++) {
		for (int k = 0; k < vs.size(); k++) {
			if (var[i] == vs[k].var) {
				ports.push_back(vs[k]);
			}
		}
	}
}

void Module::generateModule(ofstream& w_file, int& aCount, int& sCount, int& mCount, int& cCount, int& shrCount, int& shlCount, int& rCount, int& muxCount, int& scCount, int& sshrCount, int& srCount) {
	string moduleText;

		if (this->getOp().compare("+") == 0) { //adder module
			w_file << "\tADD #(" << this->getSize().getBits() << ") ADD_" << this->getSize().getBits() << "_" << aCount << "(" << ports[1].var << ", " << ports[2].var << ", " << ports[0].var << ");" << endl;
			aCount++;
		}
		else if (this->getOp().compare("-") == 0) { //sub module
			w_file << "\tSUB #(" << this->getSize().getBits() << ") SUB_" << this->getSize().getBits() << "_" << sCount << "(" << ports[1].var << ", " << ports[2].var << ", " << ports[0].var << ");" << endl;
			sCount++;
		}
		else if (this->getOp().compare("*") == 0) { //mul module
			w_file << "\tMUL #(" << this->getSize().getBits() << ") MUL_" << this->getSize().getBits() << "_" << mCount << "(" << ports[1].var << ", " << ports[2].var << ", " << ports[0].var << ");" << endl;
			mCount++;
		}
		else if (this->getOp().compare(">") == 0 && this->getSize().getSign() == false) { //comparator gt, needs signed version
			w_file << "\tCOMP #(" << this->getSize().getBits() << ") COMP_" << this->getSize().getBits() << "_" << cCount << "(" << ports[1].var << ", " << ports[2].var << ", " << ports[0].var << ", lt, eq);" << endl;
			cCount++;
		}
		else if (this->getOp().compare("<") == 0 && this->getSize().getSign() == false) { //comparator lt, needs signed version
			w_file << "\tCOMP #(" << this->getSize().getBits() << ") COMP_" << this->getSize().getBits() << "_" << cCount << "(" << ports[1].var << ", " << ports[2].var << "gt, " << ports[0].var << ", eq);" << endl;
			cCount++;
		}
		else if (this->getOp().compare("==") == 0 && this->getSize().getSign() == false) { //comparator eq, needs signed version
			w_file << "\tCOMP #(" << this->getSize().getBits() << ") COMP_" << this->getSize().getBits() << "_" << cCount << "(" << ports[1].var << ", " << ports[2].var << "gt, lt, " << ports[0].var << ");" << endl;
			cCount++;
		}
		else if (this->getOp().compare(">>") == 0 && this->getSize().getSign() == false) { //SHR, needs signed version
			w_file << "\tSHR #(" << this->getSize().getBits() << ") SHR_" << this->getSize().getBits() << "_" << shrCount << "(" << ports[1].var << ", " << ports[2].var << ", " << ports[0].var << ");" << endl;
			shrCount++;
		}
		else if (this->getOp().compare("<<") == 0) { //SHL
			w_file << "\tSHL #(" << this->getSize().getBits() << ") SHL_" << this->getSize().getBits() << "_" << shlCount << "(" << ports[1].var << ", " << ports[2].var << ", " << ports[0].var << ");" << endl;
			shlCount++;
		}
		else if (this->getOp().compare("?") == 0) { //MUX
			w_file << "\tMUX2x1 #(" << this->getSize().getBits() << ") MUX_" << this->getSize().getBits() << "_" << muxCount << "(" << ports[2].var << ", " << ports[3].var << ", " << ports[1].var << ", " << ports[0].var << ");" << endl;
			muxCount++;
		}
		else if (this->getOp().compare("=") == 0 && this->getSize().getSign() == false) { //reg, needs signed version
			w_file << "\tREG #(" << this->getSize().getBits() << ") REG_" << this->getSize().getBits() << "_" << rCount << "(" << ports[1].var << ", Clk, Rst, " << ports[0].var << ");" << endl;
			rCount++;
		}

		//signed modules
		else if (this->getOp().compare(">") == 0 && this->getSize().getSign() == true) { //comparator gt signed
			w_file << "\tSCOMP #(" << this->getSize().getBits() << ") SCOMP_" << this->getSize().getBits() << "_" << scCount << "(" << ports[1].var << ", " << ports[2].var << ", " << ports[0].var << ", lt, eq);" << endl;
			scCount++;
		}
		else if (this->getOp().compare("<") == 0 && this->getSize().getSign() == true) { //comparator lt signed
			w_file << "\tSCOMP #(" << this->getSize().getBits() << ") SCOMP_" << this->getSize().getBits() << "_" << scCount << "(" << ports[1].var << ", " << ports[2].var << ", gt, " << ports[0].var << ", eq);" << endl;
			scCount++;
		}
		else if (this->getOp().compare("==") == 0 && this->getSize().getSign() == true) { //comparator eq signed
			w_file << "\tSCOMP #(" << this->getSize().getBits() << ") SCOMP_" << this->getSize().getBits() << "_" << scCount << "(" << ports[1].var << ", " << ports[2].var << ", gt, lt, " << ports[0].var << ");" << endl;
			scCount++;
		}
		else if (this->getOp().compare(">>") == 0 && this->getSize().getSign() == true) { //SHR, needs signed version
			w_file << "\tSSHR #(" << this->getSize().getBits() << ") SSHR_" << this->getSize().getBits() << "_" << sshrCount << "(" << ports[1].var << ", " << ports[2].var << ", " << ports[0].var << ");" << endl;
			sshrCount++;
		}
		else if (this->getOp().compare("=") == 0 && this->getSize().getSign() == true) { //reg, needs signed version
			w_file << "\tSREG #(" << this->getSize().getBits() << ") SREG_" << this->getSize().getBits() << "_" << srCount << "(" << ports[1].var << ", Clk, Rst, " << ports[0].var << ");" << endl;
			srCount++;
		}
}
