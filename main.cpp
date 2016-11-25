// tempBasedStegonography.cpp : Defines the entry point for the console application.
//
#include "TempBasedStegonography.h"
#include <iostream>

#include <boost/format.hpp>

#include "sha512.h"
using namespace std;

char const* greet(){
    return "hello world";
}


int main() {
    using namespace filer;
    Filer f;
    vector<uchar> data = f.readAndEncodeFile("secret.txt");
    for(int i = 0; i < data.size(); i++) {
        cout << boost::format("%02x") % (int)data[i];
    }
    cout << "\n";
    return 0;
}
