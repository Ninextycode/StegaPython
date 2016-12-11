#include "TempBasedSteganography.h"

using namespace stcr;
using namespace std;


vector<uchar> LowLevelStega::takeVectorFromJpgVector(const vector<uchar>& container) {
    vector<uchar> secret;
    bool validContainer = vectorContainsJpegTag(container);
    if(!validContainer) {
        throw runtime_error("Bad container. No data to be extracted.");
    } else {
        ullong start = startOfJpegTag(container);
        start += 2;
        VectorSubroutines vs;
        ullong length = vs.getUllong(container, start);
        start += 8;
        secret.insert(secret.end(), container.begin() + start, container.begin() + start + length);
    } 
    return secret;
}

void LowLevelStega::hideVectorInJpgVector(vector<uchar>& container, const vector<uchar>& secret) {
    if(vectorContainsJpegTag(container)) {
        eraseJpegTagData(container);
    }
    container.reserve(container.size() + 2 + 8 + secret.size()); //2 for tag 8 for length
    container.push_back(firstByteTag);
    container.push_back(secondByteTag);
    VectorSubroutines vs;
    vs.appendUllong(container, secret.size());
    container.insert(container.end(), secret.begin(), secret.end());
    
}

void LowLevelStega::eraseJpegTagData(vector<uchar>& container) {
    if(vectorContainsJpegTag(container)) {
        ullong startOfTag = startOfJpegTag(container);
        VectorSubroutines vs;
        ullong length = vs.getUllong(container, startOfTag + 2);
        ullong endOfTag = startOfTag + 2 + 8 + length;
        container.erase(container.begin() + startOfTag, container.begin() + endOfTag);
    }
}

bool LowLevelStega::vectorContainsJpegTag(const std::vector<uchar>& container) {
    for(ullong i = 1; i < container.size(); i++) {
        if(container[i-1] == firstByteTag && container[i] == secondByteTag) {
            return true;
        }
    }
    return false;
}

ullong LowLevelStega::startOfJpegTag(const vector<uchar>& container) {
    for(ullong i = 1; i < container.size(); i++) {
        if(container[i-1] == firstByteTag && container[i] == secondByteTag) {
            return i-1;
        }
    }    
    throw runtime_error("Cannot find required tag");
}

void LowLevelStega::keylessLSB(vector<uchar>& container, const vector<uchar>& secret, ullong start, ullong end) {
    ullong availableContainerSize = end - start;
    if(secret.size() * 8 > availableContainerSize) {
        throw runtime_error("Too small container");
    } 
    
    int constainerIndex = start;
    for(int i = 0; i < secret.size(); i++) {
        write8LSB(container[constainerIndex], container[constainerIndex+1],
                container[constainerIndex+2], container[constainerIndex+3],
                container[constainerIndex+4], container[constainerIndex+5],
                container[constainerIndex+6], container[constainerIndex+7],
                secret[i]);
        constainerIndex += 8;
    }
}

void LowLevelStega::write8LSB( uchar& a, uchar& b, uchar& c, uchar& d,
                        uchar& e, uchar& f,  uchar& g,  uchar& h,
                        uchar secret) {
    a = (a & 0xfe) | ((secret >> 7) & 0x01);
    b = (b & 0xfe) | ((secret >> 6) & 0x01);
    c = (c & 0xfe) | ((secret >> 5) & 0x01);
    d = (d & 0xfe) | ((secret >> 4) & 0x01);
    e = (e & 0xfe) | ((secret >> 3) & 0x01);
    f = (f & 0xfe) | ((secret >> 2) & 0x01);
    g = (g & 0xfe) | ((secret >> 1) & 0x01);
    h = (h & 0xfe) | ((secret) & 0x01);
}

vector<uchar> LowLevelStega::de_keylessLSB(const vector<uchar>& container, ullong start, ullong end) {   
    vector<uchar> data;
    data.reserve((end-start) / 8);
    for(int i = start; i < end; i+=8) {
        uchar c = read8LSB(container[i], container[i+1], container[i+2], container[i+3],
                container[i+4], container[i+5], container[i+6], container[i+7]);
        data.push_back(c);
    }
    return data;
}

uchar LowLevelStega::read8LSB(uchar a, uchar b, uchar c, uchar d,
                        uchar e, uchar f, uchar g,  uchar h) {
	uchar secret = 0;
	secret = secret | (0b10000000 & (a << 7));
    secret = secret | (0b01000000 & (b << 6));
    secret = secret | (0b00100000 & (c << 5));
    secret = secret | (0b00010000 & (d << 4));
    secret = secret | (0b00001000 & (e << 3));
    secret = secret | (0b00000100 & (f << 2));
    secret = secret | (0b00000010 & (g << 1));
    secret = secret | (0b00000001 & (h));
    

	return secret;
}