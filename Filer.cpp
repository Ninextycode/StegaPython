#include "TempBasedStegonography.h"
using namespace filer;
using namespace std;

vector<uchar> Filer::readAndEncodeFile(string pathToFile) {     
    vector<uchar> data;
    
    string filename = getProperFileName(pathToFile);
    writeFileNameToVector(data, filename);
    
    ullong fileLength = getFileLength(pathToFile);
    writeLengthToVector(data, fileLength);
    
    writeFileToVector(data, pathToFile);

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
        throw invalid_argument("Wrong filenamme");
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
        throw invalid_argument("Wrong filenamme");
    }
    istream_iterator<uchar> in_iter(in), end;    
    data.insert(data.end(), in_iter, end);    
    in.close();
}

void Filer::saveFile(const vector<uchar>& data, string pathToFile) {
    ofstream out(pathToFile, ios::binary);
    for(auto byte : data) {
        out << byte;
    }
    out.close();
}

void filer::Filer::writeEncodedFile(const vector<uchar>& data, string directory) {
    int index = 0;
	uchar filenameSize = data[index++];
    
	string filename = "";
	for (uchar i = 0; i < filenameSize;) {
		filename += data[index++];
	}


	ullong filesize = 0;
	for (int i = 0; i < 8; i++) {
		filesize |= (((ullong)data[index++]) << ((7 - i) * 8));
	}

    
	ofstream out(directory + filename, ios::binary);
	while (index < data.size()){
        out << data[index++];
    }
	out.close();
}

