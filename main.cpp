// tempBasedStegonography.cpp : Defines the entry point for the console application.
//
#include "TempBasedStegonography.h"
#include <iostream>
#include <iomanip>
#include <boost/python.hpp>

#include "sha512.h"
using namespace std;

char const* greet(){
    return "hello world";
}

BOOST_PYTHON_MODULE(stega){
    using namespace boost::python;
    def("greet", greet);
}

int main() {
    crypto::High_Level h;
    string path = "secret";
    string dir = "";
    
    int result = h.encryptFile(dir + "secret", "123456", dir+"output/");
    cout << result << "\n";
    result = h.decryptFile(dir + "output/secret_crypt.dat", "1223456", dir+"output/");
    cout << result << "\n";
    
    ofstream out(dir+"output/mama");
    out << "123";
    out.close();
    
    return result;
}
