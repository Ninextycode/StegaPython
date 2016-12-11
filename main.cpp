#include "TempBasedSteganography.h"
#include <iostream>


using namespace std;
using namespace stcr;

// #include <boost/python.hpp>

//using namespace boost::python;


/* BOOST_PYTHON_MODULE( stega ) {
    class_<HighLevelStega>("Stega")
        .def("hideInTemp", &HighLevelStega::hideFileInTemp, args("filename"));
}; */


int main() {
    HighLevelStega hstega;
    hstega.decodeAndTakeFileFromJpgStructure("vodkaseledka14448", "quickie-jpeg-example.jpg", "");
};
