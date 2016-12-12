#include "TempBasedSteganography.h"
using namespace stcr;
using namespace std;

vector<uchar> Subroutines::vectorFromString(string s) {
    vector<uchar> v(s.begin(), s.end());
    return v;
}

void Subroutines::appendUllong(vector<uchar>& data, ullong x) {
    for(int i = 0; i < 8; i++) {
        data.push_back( (uchar)((0xff00000000000000 & x) >> (7*8)) );
        x <<= 8;
    }
}

void Subroutines::appendUint(vector<uchar>& data, uint x) {
    for(int i = 0; i < 4; i++) {
        data.push_back( (uchar)((0xff000000 & x) >> (3*8)) );
        x <<= 8;
    }
}

ullong Subroutines::getUllong(const vector<uchar>& data, ullong position) {
    ullong x = 0;
	for (int i = 0; i < 8; i++) {
		x |= (((ullong)data[position++]) << ((7 - i) * 8));
	}
    return x;
}

uint Subroutines::getUint(const vector<uchar>& data, ullong position) {
    ullong x = 0;
	for (int i = 0; i < 4; i++) {
		x |= (((ullong)data[position++]) << ((3 - i) * 8));
	}
    return x;
}

string Subroutines::stringFromVector(const std::vector<uchar>& data, ullong begin, ullong end) {
    auto it1 = data.begin();
    auto it2 = data.begin();
    advance(it1, begin);
    advance(it2, end);
    string s(it1, it2);
    return s;
}

string Subroutines::getNameFromPath(string path){
    int nameStart  = path.length();
    bool found = false;
    for(; nameStart >= 1; nameStart--) {
        if(path[nameStart-1] == '\\' || path[nameStart-1] == '/'){
            found = true;
            break;
        }
    }
    if(found) {
        string filename(path.begin() + nameStart, path.end());
        return filename;
    } else {
        return path;
    }
}