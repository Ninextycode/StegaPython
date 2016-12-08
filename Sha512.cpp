#include "TempBasedStegonography.h"
#include <boost/format.hpp>

using namespace stcr;
using namespace std;

constexpr ullong Sha512::H_static[];
constexpr ullong Sha512::k[];

vector<uchar> Sha512::digest(vector<uchar>& data){
    copy(begin(H_static), end(H_static), begin(H));
    
    pad(data);

    preformComputations(data);   
    unpad(data);
    vector<uchar> hash = formHash();
    return hash;
}

void Sha512::preformComputations(std::vector<uchar>& data) {
    int nOfBlocks = data.size() / (16 * 8); // how many blocks of 16 64bit words
    for(int i = 0; i < nOfBlocks; i++) {
        digestBlock(data, i);
    }
}

void Sha512::pad(vector<uchar>& data){
    ullong l = data.size() * 8;
    int k; //solution to the  l + 1 + k ≡ 896mod1024

    k = 896 - ((l+1) % 1024);
    if(k < 0) {
        k += 1024;
    }
    
    // k + 1 is divisible by 8. We have to write p = (k + 1)/ 8 bytes
    // first should be 0x10000000, other 0b00000000
    int p = (k + 1)/ 8;
    
    append1followedBy0(data, p);
     
    sizeToVector(data, l);
}

void Sha512::append1followedBy0(vector<uchar>& data, int nBytes) {
    data.push_back(0b10000000);
    for(int i = 0; i < nBytes - 1; i++) {
        data.push_back(0x00);
    }
}

void Sha512::unpad(vector<uchar>& data){
    //it is possible to extract only 64 bit int, not 128, as sha512 specification suggests
    VectorSubroutines vs;
    
    ullong desiredLength = vs.getUllong(data, data.size() - 8);
    desiredLength = desiredLength / 8;
    auto padBegin = data.begin();
    advance(padBegin, desiredLength);
    data.erase(padBegin, data.end());
}
void Sha512::sizeToVector(vector<uchar>& data, ullong x){
    VectorSubroutines vs;
    
    //add 8 empty bits, because according to sha512 specification, size should occupy 128, not 64(ullong length) bits
    for(int i = 0; i < 8; i++) {
        data.push_back(0x00); 
    }
    vs.appendUllong(data, x);
}

void Sha512::digestBlock(std::vector<uchar>& data, int numberOfBlock){
    prepareMessageSchedule(data, numberOfBlock);
    initialiseWorkingVariables();
    mix();
    computeIntermediateHashValues();
}

void Sha512::prepareMessageSchedule(std::vector<uchar>& data, int numberOfBlock){
    VectorSubroutines vs;
    
    ullong blockStartIndex = numberOfBlock * 16 * 8;
    
    for(int t = 0; t < 16; t++) {
        w[t] = vs.getUllong(data, blockStartIndex + t*8);
    }
    for(int t = 16; t < 80; t++) {
        w[t] = sigma1(w[t-2]) + w[t-7] + sigma0(w[t-15]) + w[t-16];
    }
}

void Sha512::initialiseWorkingVariables() {
    a = H[0];
    b = H[1];
    c = H[2];
    d = H[3];
    e = H[4];
    f = H[5];
    g = H[6];
    h = H[7];
}

void Sha512::mix() {
    for(int t = 0; t < 80; t++){        
        ullong t1 = h + SIGMA1(e) + Ch(e, f ,g) + k[t] + w[t];
        ullong t2 = SIGMA0(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;        
    }
}

void Sha512::computeIntermediateHashValues() {
    H[0] = a + H[0];
    H[1] = b + H[1];
    H[2] = c + H[2];
    H[3] = d + H[3];
    H[4] = e + H[4];
    H[5] = f + H[5];
    H[6] = g + H[6];
    H[7] = h + H[7];
}

vector<uchar> Sha512::formHash() {
    vector<uchar> hash;
    hash.reserve(64);
    for(ullong l : H) {
        for(int i = 0; i < 8; i++){
            hash.push_back( (l >> 8 * (7 - i)) & 0xff );
        }
    }
    return(hash);
}

ullong Sha512::sigma0(ullong x) {
    return rotr(1, x) ^ rotr(8, x) ^ shr(7, x);
}

ullong Sha512::sigma1(ullong x) {
    return rotr(19, x) ^ rotr(61, x) ^ shr(6, x);
}

ullong Sha512::SIGMA0(ullong x) {
    return rotr(28, x) ^ rotr(34, x) ^ rotr(39, x);
}

ullong Sha512::SIGMA1(ullong x) {
    return rotr(14, x) ^ rotr(18, x) ^ rotr(41, x);
}

ullong Sha512::rotr(int n, ullong x){  
    return (x >> n) | (x << (8*8-n));
}

ullong Sha512::shr(int n, ullong x) {
    return x >> n;
}

ullong Sha512::Ch(ullong x, ullong y, ullong z) {
    return (x & y) ^ (~x & z);
}

ullong Sha512::Maj(ullong x, ullong y, ullong z) {
    return (x & y) ^ (x & z) ^ (y & z);
}
