#include "Bits.h"

Bits::Bits() {
	bits = 0;
	sign = false;
}

Bits::Bits(bool s, int b) {
	bits = b;
	sign = s;
}

Bits::~Bits() {

}

int Bits::getBits() {
	return bits;
}

bool Bits::getSign() {
	return sign;
}

void Bits::setBits(int b) {
	bits = b;
}

void Bits::setSign(bool s) {
	sign = s;
}
