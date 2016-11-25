#include "TempBasedStegonography.h"

using namespace crypto;

int Sha512::digest(uchar* data, uint length, uchar (&output)[64]){
    return 0;
}

void Sha512::rrot(uint& x, int n){
    x = (x >> n) || (x << (sizeof(x)*8-n));
}