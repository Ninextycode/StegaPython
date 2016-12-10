#include "TempBasedSteganography.h"
using namespace stcr;
using namespace std;


void HighLevelStega::hideVectorInTemp(const vector<uchar>& data) {
    Filer f;
    VectorSubroutines vs;
    vector<uchar> tempData = f.readFile(tempFileName);
    
    uint width = vs.getUint(tempData, 0);    
    uint height = vs.getUint(tempData, 4);
    
    ullong rgbRegionSize = 3 * width * height;
    ullong rgbRegionStart = 8;
    ullong rgbRegionEnd = rgbRegionStart + rgbRegionSize;
    
    LowLevelStega lstega;
    lstega.keylessLSB(tempData, data, rgbRegionStart, rgbRegionEnd);
    
    f.writeFile(tempData, tempFileName);
}


vector<uchar> HighLevelStega::takeVectorFromTemp() {
    Filer f;
    VectorSubroutines vs;
    vector<uchar> tempData = f.readFile(tempFileName);
    
    uint width = vs.getUint(tempData, 0);    
    uint height = vs.getUint(tempData, 4);
    
    ullong rgbRegionSize = 3 * width * height;
    ullong rgbRegionStart = 8;
    ullong rgbRegionEnd = rgbRegionStart + rgbRegionSize;
    
    LowLevelStega lstega;
    vector<uchar> data = lstega.de_keylessLSB(tempData, rgbRegionStart, rgbRegionEnd);
    return data;
}

void HighLevelStega::hideVectorInJpgStructure(const vector<uchar>& data, std::string imagename){
    Filer f;
    LowLevelStega lstega;
    vector<uchar> imageData = f.readFile(imagename);
    lstega.hideVectorInJpgVector(imageData, data);
    f.writeFile(imageData, imagename);
};

vector<uchar> HighLevelStega::takeVectorFromJpgStructure(std::string imagename){
    Filer f;
    LowLevelStega lstega;
    vector<uchar> imageData = f.readFile(imagename);
    vector<uchar> secret = lstega.takeVectorFromJpgVector(imageData);
    return secret;
};

void HighLevelStega::cleanJpegFile(string filename) {
    Filer f;
    LowLevelStega lstega;
    vector<uchar> imageData = f.readFile(filename);
    lstega.eraseJpegTagData(imageData);
    f.writeFile(imageData, filename);
}


void HighLevelStega::hideFielInTemp(std::string filename) {
    Filer f;
    vector<uchar> data = f.readAndEncodeFile(filename);
    hideVectorInTemp(data);
};

void HighLevelStega::takeFileFromTemp(std::string resultDir){
    vector<uchar> data = takeVectorFromTemp();
    Filer f;
    f.writeEncodedFile(data, resultDir);
};

void HighLevelStega::hideFileInJpgStructure(std::string filename, std::string imagename) {
    Filer f;
    vector<uchar> data = f.readAndEncodeFile(filename);
    hideVectorInJpgStructure(data, imagename);
};

void HighLevelStega::takeFileFromJpgStructure( std::string imagename, std::string resultDir) {
    vector<uchar> data = takeVectorFromJpgStructure(imagename);
    Filer f;
    f.writeEncodedFile(data, resultDir); 
};


void HighLevelStega::encodeAndHideFielInTemp(std::string password, std::string filename) {
    Filer f;
    vector<uchar> data = f.readAndEncodeFile(filename);
    HighLevelCrypto hcrypt;
    VectorSubroutines vs;
    vector<uchar> encrypted = hcrypt.encryptVectorByCipherBlockChaining(data, password);
    hideVectorInTemp(encrypted);
};

void HighLevelStega::decodeAndTakeFileFromTemp(std::string password, std::string resultDir) {
    Filer f;
    vector<uchar> encrypted = takeVectorFromTemp();
    HighLevelCrypto hcrypt;
    vector<uchar> data = hcrypt.decryptVectorByCipherBlockChaining(encrypted, password);
   
    f.writeEncodedFile(data, resultDir);
};

void HighLevelStega::encodeAndHideFileToJpgStructure(std::string password, std::string filename, std::string imagename) {
    Filer f;
    vector<uchar> data = f.readAndEncodeFile(filename);
    HighLevelCrypto hcrypt;
    VectorSubroutines vs;
    vector<uchar> encrypted = hcrypt.encryptVectorByCipherBlockChaining(data, password);
    hideVectorInJpgStructure(encrypted, imagename);
};

void HighLevelStega::decodeAndTakeFileFromJpgStructure(std::string password, std::string imagename, std::string resultDir) {
    Filer f;
    vector<uchar> encrypted = takeVectorFromJpgStructure(imagename);
    HighLevelCrypto hcrypt;
    vector<uchar> data = hcrypt.decryptVectorByCipherBlockChaining(encrypted, password);
    
    f.writeEncodedFile(data, resultDir);
};

string HighLevelStega::getTempFileName() {
    return tempFileName;
}