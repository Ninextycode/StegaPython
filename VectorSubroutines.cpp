#include "TempBasedSteganography.h"
using namespace stcr;
using namespace std;

vector<uchar> VectorSubroutines::vectorFromString(string s) {
    vector<uchar> v(s.begin(), s.end());
    return v;
}

void VectorSubroutines::appendUllong(vector<uchar>& data, ullong x) {
    for(int i = 0; i < 8; i++) {
        data.push_back( (uchar)((0xff00000000000000 & x) >> (7*8)) );
        x <<= 8;
    }
}

void VectorSubroutines::appendUint(vector<uchar>& data, uint x) {
    for(int i = 0; i < 4; i++) {
        data.push_back( (uchar)((0xff000000 & x) >> (3*8)) );
        x <<= 8;
    }
}

ullong VectorSubroutines::getUllong(const vector<uchar>& data, ullong position) {
    ullong x = 0;
	for (int i = 0; i < 8; i++) {
		x |= (((ullong)data[position++]) << ((7 - i) * 8));
	}
    return x;
}

uint VectorSubroutines::getUint(const vector<uchar>& data, ullong position) {
    ullong x = 0;
	for (int i = 0; i < 4; i++) {
		x |= (((ullong)data[position++]) << ((3 - i) * 8));
	}
    return x;
}

string VectorSubroutines::stringFromVector(const std::vector<uchar>& data, ullong begin, ullong end) {
    auto it1 = data.begin();
    auto it2 = data.begin();
    advance(it1, begin);
    advance(it2, end);
    string s(it1, it2);
    return s;
}