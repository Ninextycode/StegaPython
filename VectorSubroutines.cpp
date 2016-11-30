#include "TempBasedStegonography.h"
using namespace subroutines;
using namespace std;

VectorSubroutines::vectorFromString(string s) {
    vector<uchar> v(s.begin(), s.end());
    return v;
}

VectorSubroutines::appendUllong(vector<uchar>& data, ullong x) {
    for(int i = 0; i < 8; i++) {
        data.push_back( (uchar)((0xff00000000000000 & x) >> (7*8)) );
        x <<= 8;
    }
}

VectorSubroutines::getUllong(vector<uchar>& data, ullong position) {
    ullong x = 0;
	for (int i = 0; i < 8; i++) {
		x |= (((ullong)data[position++]) << ((7 - i) * 8));
	}
    return x;
}


VectorSubroutines::stringFromVector(std::vector<uchar>& data, ullong begin, ullong end) {
    auto it1 = data.begin();
    auto it2 = data.begin();
    advance(it1, begin);
    advance(it2, end);
    string s(it1, it2);
    return s;
}

VectorSubroutines::truncateToSile(std::vector<uchar>& data, ullong desiredSize) {
    auto it = data.begin();
    advance(it, desiredSize);
    data.erase(it, data.end());
}