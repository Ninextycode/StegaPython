#include "TempBasedSteganography.h"
#include <iostream>


using namespace std;
using namespace stcr;

#include <boost/python.hpp>

using namespace boost::python;

BOOST_PYTHON_MODULE( stega ) {
    class_<HighLevelStega>("Stega")
        .def("hide_file_in_temp", &HighLevelStega::hideFileInTemp, args("file"))
        .def("take_file_from_temp", &HighLevelStega::takeFileFromTemp, args("output_dir"))
        
        .def("hide_file_in_jpeg", &HighLevelStega::hideFileInJpgStructure, args("file", "container"))
        .def("take_file_from_jpeg", &HighLevelStega::takeFileFromJpgStructure, args("container", "output_dir"))
        .def("clean_jpeg", &HighLevelStega::cleanJpegFile, args("container"))
    
        .def("encode_and_hide_file_in_temp", &HighLevelStega::encodeAndHideFielInTemp, 
            args("password", "file"))
        .def("decode_and_take_file_from_temp", &HighLevelStega::decodeAndTakeFileFromTemp, 
            args("password", "output_dir"))
        
        .def("encode_and_hide_file_in_jpeg", &HighLevelStega::encodeAndHideFileInJpgStructure,
            args("password", "file", "container"))
        .def("decode_and_take_file_from_jpeg", &HighLevelStega::decodeAndTakeFileFromJpgStructure,
            args("password", "container", "output_dir"));
    
    class_<HighLevelCrypto>("Crypto")
        .def("encrypt_file", &HighLevelCrypto::encryptFile, args("file", "password", "output"))
        .def("decrypt_file", &HighLevelCrypto::decryptFile, args("file", "password", "output"));
    
}; 


/* int main() {
    HighLevelStega hstega;
    hstega.decodeAndTakeFileFromJpgStructure("vodkaseledka14448", "quickie-jpeg-example.jpg", "");
}; */
