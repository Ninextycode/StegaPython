#include "TempBasedSteganography.h"

using namespace stcr;
using namespace std;

void HighLevelCrypto::encryptFile(std::string filename, std::string password, std::string resultDir) {
    Filer filer;    
    vector<uchar> file = filer.readAndEncodeFile(filename);
    vector<uchar> encryptedFile = encryptVectorByCipherBlockChaining(file, password);
    
    Subroutines s;
    string name = s.getNameFromPath(filename);

    filer.writeFile(encryptedFile, resultDir+name+postfixForEncryptedFiles); 
}

void HighLevelCrypto::decryptFile(string filename, string password, string resultDir) {
	Filer filer;    
    Sha512 hash;

    vector<uchar> file = filer.readFile(filename);
    vector<uchar> decryptedFile = decryptVectorByCipherBlockChaining(file, password);
    filer.writeEncodedFile(decryptedFile, resultDir);
}


vector<uchar> HighLevelCrypto::encryptVectorByCipherBlockChaining(std::vector<uchar>& data, 
        const std::string password) {
    Sha512 hash;
    Subroutines vs;
    vector<uchar> passwordVector = vs.vectorFromString(password);
    vector<uchar> passwordHash = hash.digest(passwordVector);
    
    auto it = passwordHash.begin();
    advance(it, passwordHash.size() - 16);
    vector<uchar> IV(it, passwordHash.end());
    
    passwordHash.resize(32);
    
    return encryptVectorByCipherBlockChaining(data, passwordHash, IV);
}

vector<uchar> HighLevelCrypto::decryptVectorByCipherBlockChaining(const std::vector<uchar>& data, 
        const std::string password) {
    Sha512 hash;
    Subroutines vs;
    vector<uchar> passwordVector = vs.vectorFromString(password);
    vector<uchar> passwordHash = hash.digest(passwordVector);
    
    passwordHash.resize(32);
    
    return decryptVectorByCipherBlockChaining(data, passwordHash);
}


vector<uchar> HighLevelCrypto::encryptVectorByCipherBlockChaining(std::vector<uchar>& data, 
        const std::vector<uchar>& password, const std::vector<uchar>& IV) {
    LowLevelCrypto lowLevel;
    
    //16 bytes of IV + 16 * number of blocks rounded up bits of data
    ullong eryptedDataSize = 16 + (data.size() / 16 + 1) * 16; 

    vector<uchar> encryptedData;
    encryptedData.reserve(eryptedDataSize);
    
    for (int i = 0; i < 16; i++) {
        encryptedData.push_back(IV[i]);
    }

    padPKCS7(data);
    
    vector<uchar> tempBlockToEncrypt, tempBlockToXorWith;
    tempBlockToEncrypt.resize(16);  
    tempBlockToXorWith.resize(16);
    
    for (int i = 16; i < eryptedDataSize; i+=16) {        
        for (int j = 0; j < 16; j++) {
            tempBlockToXorWith[j] = encryptedData[i + j - 16]; //get values of the previously encryped block
            tempBlockToEncrypt[j] = data[i + j - 16];
        }
        
        vector<uchar> tempBlockToEncryptXored = xorFirstWithSecond(tempBlockToXorWith, tempBlockToEncrypt);		
        vector<uchar> encryptedBlock = lowLevel.encryptAES(tempBlockToEncryptXored, password);

        for (int j = 0; j < 16; j++) {
            encryptedData.push_back(encryptedBlock[j]);
        }
    }
    
    unpadPKCS7(data);
    return encryptedData;
}

void HighLevelCrypto::padPKCS7(vector<uchar>& data) {
    //value to be used in padding
    //16 - aes block size in bytes
    uchar paddingValue = 16 - data.size()%16;

    for(int i = 0; i < paddingValue; i++) {
        data.push_back(paddingValue);
    }    
}

vector<uchar> HighLevelCrypto::decryptVectorByCipherBlockChaining(const std::vector<uchar>& data, 
        const std::vector<uchar>& password) {
    LowLevelCrypto lowLevel;
    
    ullong deyptedDataSize = data.size() - 16;
    vector<uchar> decryptedData;
    decryptedData.reserve(deyptedDataSize);
    
    vector<uchar> tempBlockToDecrypt, tempBlockToXorWith;
    tempBlockToDecrypt.resize(16);  
    tempBlockToXorWith.resize(16);
    
    for (int i = 16; i < data.size(); i+=16) {        
        for (int j = 0; j < 16; j++) {
            tempBlockToXorWith[j] = data[i + j - 16]; //get values of the previously encryped block
            tempBlockToDecrypt[j] = data[i + j];
        }        
        		
        vector<uchar> decryptedBlock = lowLevel.decryptAES(tempBlockToDecrypt, password);
        decryptedBlock = xorFirstWithSecond(tempBlockToXorWith, decryptedBlock);
        for (int j = 0; j < 16; j++) {
            decryptedData.push_back(decryptedBlock[j]);
        }
    }

    unpadPKCS7(decryptedData);

    return decryptedData;
}


void HighLevelCrypto::unpadPKCS7(vector<uchar>& data) {
    uchar padLength = data.back();
    data.resize(data.size() - padLength);
}

vector<uchar> HighLevelCrypto::xorFirstWithSecond(const vector<uchar>& a, const vector<uchar>&  b) {
    ullong length = min(a.size(), b.size());
    vector<uchar> xored;
    xored.reserve(length);
    
    for (ullong i = 0; i < length; i++) {
        xored.push_back(a[i] ^ b[i]);
    }
    return xored;
}

string HighLevelCrypto::getPostfixForEncryptedFiles() {
    return postfixForEncryptedFiles;
}