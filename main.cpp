// tempBasedSteganography.cpp : Defines the entry point for the console application.
//
#include "TempBasedSteganography.h"
#include <iostream>

#include <boost/format.hpp>


using namespace std;
using namespace stcr;

int test1();
int test2();

int main() {
    HighLevelStega hstega;
    hstega.encodeAndHideFileToJpgStructure("AllahAkbar", "");
}

int test1(){
    using namespace stcr;
    Filer f;
    vector<uchar> data = f.readAndEncodeFile("secret.txt");
    for(int i = 0; i < data.size(); i++) {
        cout << boost::format("%02x") % (int)data[i];
    }
    cout << "\n";
    return 0;
}

int test2(){
    using namespace stcr;
    Sha512 hash;
    vector<uchar> data;
    string s = "";
    for(int i = 0; i < 1000000; i++) {
        s += "a";
    }
    for(uchar c: s){
        data.push_back(c);
    }
    auto h = hash.digest(data);
    int counter = 0;
    for(int c: h) {
        if(counter++ % 4 == 0){
            cout << "\n";
        }
        cout << boost::format("%02x") % c;
    }
}