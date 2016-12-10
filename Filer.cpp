#include "TempBasedSteganography.h"
using namespace stcr;
using namespace std;

vector<uchar> Filer::readFile(string pathToFile) {
    vector<uchar> data; 
    writeFileToVector(data, pathToFile);
    return data;
}

vector<uchar> Filer::readAndEncodeFile(string pathToFile) {     
    vector<uchar> data;
    
    string filename = getProperFileName(pathToFile);
    writeFileNameToVector(data, filename);
    
    ullong fileLength = getFileLength(pathToFile);
    writeLengthToVector(data, fileLength);
    
    writeFileToVector(data, pathToFile);

    Sha512 hash;
    hash.appendHashToVector(data);
    return data;
}

string Filer::getProperFileName(string pathToFile){
    string clearFilename;
	int i = pathToFile.length() - 1;
	while (i >= 0 && pathToFile.at(i) != '\\') {
		clearFilename = pathToFile.at(i--) + clearFilename;
	}
    return clearFilename;
}

ullong Filer::getFileLength(string pathToFile) {
    ifstream in(pathToFile, ifstream::ate | ifstream::binary);
    if(!in.is_open()){
        throw invalid_argument("Cannot get file length. Probably wrong filename " + pathToFile );
    }
    ullong length = in.tellg(); 
    in.close();
    return length;
}

void Filer::writeFileNameToVector(vector<uchar>& data, string filename) {
    data.push_back((uchar)filename.length());
    for(auto c: filename){
        data.push_back((uchar)c);
    }
}

void Filer::writeLengthToVector(vector<uchar>& data, ullong length) {
    for(int i = 0; i < 8; i++) {
        data.push_back( (uchar)((0xff00000000000000 & length) >> (7*8)) );
        length <<= 8;
    }
}

void Filer::writeFileToVector(std::vector<uchar>& data, std::string pathToFile){
    ifstream in(pathToFile, ios::binary);
    if(!in.is_open()){
        throw invalid_argument("Cannot write file to vector. Probably wrong filename " + pathToFile);
    }
    data.reserve(data.size() + getFileLength(pathToFile));
    char b;
    while(in.read(&b, 1)) {        
        data.push_back(b);
    }
    in.close();
}

void Filer::writeFile(const vector<uchar>& data, string pathToFile) {
    ofstream out(pathToFile, ios::binary);
    for(auto byte : data) {
        out << byte;
    }
    out.close();
}

void Filer::writeEncodedFile(vector<uchar>& data, string directory) {
    VectorSubroutines vs;
    Sha512 hash;

    ullong length = encodedFileLength(data) + 64; //64 is length of hash signature
    data.resize(length);

    bool isValid = hash.checkHashAtTheEnd(data);
    if(!isValid) {
        throw runtime_error("Wrong file format");
    }
    
	uchar filenameSize = data[0];
    
    ullong filenameEnd = 1 + filenameSize;
	string filename = vs.stringFromVector(data, 1, filenameEnd);
    
	ullong fileSize = vs.getUllong(data, filenameEnd);
    
    int index = 1 + filenameSize + 8; //1 for filename length  length of filename + 8 for file data length
    int startOfFileData = index;
    
	ofstream out(directory + filename, ios::binary);
	while (index < fileSize + startOfFileData){
        out << data[index++];
    }
	out.close();
}

ullong Filer::encodedFileLength(const std::vector<uchar>& data) {
    VectorSubroutines vs;
    uchar filenameSize = data[0];    
    ullong filenameEnd = 1 + filenameSize;
	ullong fileSize = vs.getUllong(data, filenameEnd);
    
    return  1 + filenameSize + 8 + fileSize; 
}
