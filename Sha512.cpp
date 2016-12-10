#include "TempBasedSteganography.h"
#include <boost/format.hpp>

using namespace stcr;
using namespace std;

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


const ullong Sha512::H_static[8] {
    0x6a09e667f3bcc908,
    0xbb67ae8584caa73b,
    0x3c6ef372fe94f82b,
    0xa54ff53a5f1d36f1, 
    0x510e527fade682d1,
    0x9b05688c2b3e6c1f,
    0x1f83d9abfb41bd6b,
    0x5be0cd19137e2179
};
        
const ullong Sha512::k[80] {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538, 
    0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe, 
    0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 
    0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, 0x983e5152ee66dfab, 
    0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725, 
    0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 
    0x53380d139d95b3df, 0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b, 
    0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218, 
    0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, 0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 
    0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 
    0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec, 
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 0xca273eceea26619c, 
    0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6, 
    0x113f9804bef90dae, 0x1b710b35131c471b, 0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 
    0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};