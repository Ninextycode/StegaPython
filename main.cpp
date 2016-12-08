// tempBasedStegonography.cpp : Defines the entry point for the console application.
//
#include "TempBasedStegonography.h"
#include <iostream>

#include <boost/format.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE main
#include  <boost/test/unit_test.hpp>

using namespace std;

 
int add(int i, int j){
    return i + j;
}
 
BOOST_AUTO_TEST_CASE(universeInOrder){
    BOOST_CHECK(add(2, 2) == 4);
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