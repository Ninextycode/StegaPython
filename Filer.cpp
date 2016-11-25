#include "TempBasedStegonography.h"
using namespace filer;
using namespace std;

vector<uchar> Filer::readAndEncodeFile(string pathToFile) {     
    vector<uchar> data;
    
    string filename = getProperFileName(pathToFile);
    writeFileName(data, filename);
    
    ullong fileLength = getFileLength(pathToFile);
    writeFileLength(data, fileLength);
    
    writeFile(data, pathToFile);

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

void Filer::writeFileName(vector<uchar>& data, string filename) {
    data.push_back((uchar)filename.length());
    for(auto c: filename){
        data.push_back((uchar)c);
    }
}

void Filer::writeFileLength(vector<uchar>& data, ullong length) {
    for(int i = 0; i < 8; i++) {
        data.push_back( (uchar)((0xff00000000000000 & length) >> (7*8)) );
        length <<= 8;
    }
}

void Filer::writeFile(vector<uchar>& data, string pathToFile) {
    ifstream in(pathToFile, ios::binary);
    if(!in.is_open()){
        throw invalid_argument("Wrong filenamme");
    }
    istream_iterator<uchar> in_iter(in), end;    
    data.insert(data.end(), in_iter, end);    
    in.close();
}

void filer::Filer::writeEncodedFile(string resultDir, uchar** data) {

	uchar filenameSize = *(*data)++;

	//filename.append("_de.");
	string filename = "";
	for (uchar i = 0; i < filenameSize; i++) {
		filename += ((char)*((*data)++));
	}


	ullong filesize = 0;
	for (int i = 0; i < 8; i++) {
		filesize |= (((ullong)*((*data)++)) << ((7 - i) * 8));
	}


	ofstream out(resultDir + filename
		//+ "." + format
		, ios::binary);

	out.write((char*)*data, filesize);

	out.close();

	*(data) -= (1 + filename.size() + 8);
}

void filer::Filer::writeFile(string filename, uchar* data, ullong length){
    ofstream out(filename, ios::binary);
    out.write((char*)data, length);
    out.close();
}