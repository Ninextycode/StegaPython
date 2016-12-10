#include "TempBasedSteganography.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include  <boost/test/unit_test.hpp>

#include <cstdio> //removing files

using namespace std;
using namespace stcr;

BOOST_AUTO_TEST_SUITE(Cryptography)

BOOST_AUTO_TEST_SUITE(Sha512Test)

BOOST_AUTO_TEST_CASE(oneBlockMessage) {
    VectorSubroutines vs;
    vector<uchar> data = vs.vectorFromString("abc");
    Sha512 hasher;
    vector<uchar> hash = hasher.digest(data); 
    vector<uchar> expectedHash {
        0xdd, 0xaf, 0x35, 0xa1, 0x93, 0x61, 0x7a, 0xba, 
        0xcc, 0x41, 0x73, 0x49, 0xae, 0x20, 0x41, 0x31, 
        0x12, 0xe6, 0xfa, 0x4e, 0x89, 0xa9, 0x7e, 0xa2, 
        0x0a, 0x9e, 0xee, 0xe6, 0x4b, 0x55, 0xd3, 0x9a, 
        0x21, 0x92, 0x99, 0x2a, 0x27, 0x4f, 0xc1, 0xa8, 
        0x36, 0xba, 0x3c, 0x23, 0xa3, 0xfe, 0xeb, 0xbd, 
        0x45, 0x4d, 0x44, 0x23, 0x64, 0x3c, 0xe8, 0x0e, 
        0x2a, 0x9a, 0xc9, 0x4f, 0xa5, 0x4c, 0xa4, 0x9f
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(hash.begin(), hash.end(),
            expectedHash.begin(), expectedHash.end());
}

BOOST_AUTO_TEST_CASE(multiBlockMessage) {
    VectorSubroutines vs;
    vector<uchar> data = vs.vectorFromString(
            (string)"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn" +
           "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
    Sha512 hasher;
    vector<uchar> hash = hasher.digest(data); 
    vector<uchar> expectedHash {
        0x8e, 0x95, 0x9b, 0x75, 0xda, 0xe3, 0x13, 0xda, 
        0x8c, 0xf4, 0xf7, 0x28, 0x14, 0xfc, 0x14, 0x3f, 
        0x8f, 0x77, 0x79, 0xc6, 0xeb, 0x9f, 0x7f, 0xa1, 
        0x72, 0x99, 0xae, 0xad, 0xb6, 0x88, 0x90, 0x18, 
        0x50, 0x1d, 0x28, 0x9e, 0x49, 0x00, 0xf7, 0xe4, 
        0x33, 0x1b, 0x99, 0xde, 0xc4, 0xb5, 0x43, 0x3a, 
        0xc7, 0xd3, 0x29, 0xee, 0xb6, 0xdd, 0x26, 0x54, 
        0x5e, 0x96, 0xe5, 0x5b, 0x87, 0x4b, 0xe9, 0x09
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(hash.begin(), hash.end(),
            expectedHash.begin(), expectedHash.end());
}

BOOST_AUTO_TEST_CASE(longMessage) {
    VectorSubroutines vs;
    string s = "";
    for(int i = 0; i < 1000000; i++) {
        s += "a";
    }
    vector<uchar> data = vs.vectorFromString(s);
    Sha512 hasher;
    vector<uchar> hash = hasher.digest(data); 
    vector<uchar> expectedHash {
        0xe7, 0x18, 0x48, 0x3d, 0x0c, 0xe7, 0x69, 0x64, 
        0x4e, 0x2e, 0x42, 0xc7, 0xbc, 0x15, 0xb4, 0x63, 
        0x8e, 0x1f, 0x98, 0xb1, 0x3b, 0x20, 0x44, 0x28, 
        0x56, 0x32, 0xa8, 0x03, 0xaf, 0xa9, 0x73, 0xeb, 
        0xde, 0x0f, 0xf2, 0x44, 0x87, 0x7e, 0xa6, 0x0a, 
        0x4c, 0xb0, 0x43, 0x2c, 0xe5, 0x77, 0xc3, 0x1b, 
        0xeb, 0x00, 0x9c, 0x5c, 0x2c, 0x49, 0xaa, 0x2e, 
        0x4e, 0xad, 0xb2, 0x17, 0xad, 0x8c, 0xc0, 0x9b
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(hash.begin(), hash.end(),
            expectedHash.begin(), expectedHash.end());
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(Aes256)

BOOST_AUTO_TEST_CASE(encryptOneBlock) {
    vector<uchar> data {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
    };
    vector<uchar> key {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };
    LowLevelCrypto cypherer;
    vector<uchar> encrypted = cypherer.encryptAES(data, key); 
    vector<uchar> expectedEncrypted {
        0x8e, 0xa2, 0xb7, 0xca, 0x51, 0x67, 0x45, 0xbf, 
        0xea, 0xfc, 0x49, 0x90, 0x4b, 0x49, 0x60, 0x89
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(encrypted.begin(), encrypted.end(),
            expectedEncrypted.begin(), expectedEncrypted.end());
}

BOOST_AUTO_TEST_CASE(decryptOneBlock) {
    vector<uchar> data {
        0x8e, 0xa2, 0xb7, 0xca, 0x51, 0x67, 0x45, 0xbf, 
        0xea, 0xfc, 0x49, 0x90, 0x4b, 0x49, 0x60, 0x89
    };
    vector<uchar> key {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };
    LowLevelCrypto sypherer;
    vector<uchar> decrypted = sypherer.decryptAES(data, key); 
    vector<uchar> expectedDecrypted {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(decrypted.begin(), decrypted.end(),
            expectedDecrypted.begin(), expectedDecrypted.end());
}

BOOST_AUTO_TEST_CASE(encryptAndDecryptCBC) {
    VectorSubroutines vs;
    string message = (string)"This is message to be encrypted. This is long enough" + 
            " to be divided to several 256 bit blocks.";
    string password = "This is some stupid random password. It's hash will be used to make 16 byte IV and 32 byte key.";
    vector<uchar> data = vs.vectorFromString(message);
    vector<uchar> passwordHash = vs.vectorFromString(password);
    
    auto it = passwordHash.begin();
    advance(it, passwordHash.size() - 16);
    vector<uchar> IV(it, passwordHash.end());
    passwordHash.resize(32);
    
    
    HighLevelCrypto cypherer;
    vector<uchar> encrypted = cypherer.encryptVectorByCipherBlockChaining(data, passwordHash, IV); 
    vector<uchar> decrypted = cypherer.decryptVectorByCipherBlockChaining(encrypted, passwordHash);
    
    BOOST_CHECK_EQUAL_COLLECTIONS(data.begin(), data.end(),
            decrypted.begin(), decrypted.end());
}

BOOST_AUTO_TEST_CASE(encryptAndDecryptFile) {
    VectorSubroutines vs;
    Filer f;
    HighLevelCrypto cypherer;
    string message = (string)"This is message to be encrypted. This is long enough" + 
            " to be divided to several 256 bit blocks.";
    string password = "This is some stupid random password. It's hash will be used to make 16 byte IV and 32 byte key.";
    string secret = "secret";
    string encryptedSecret = secret + cypherer.getPostfixForEncryptedFiles();
    cout << 1 << "\n";
    vector<uchar> secretData = vs.vectorFromString(message);
    f.writeFile(secretData, secret);
    cout << 2 << "\n";   
    cypherer.encryptFile(secret, password, "/");
    cout << 3 << "\n";
    remove(secret.data());
    cout << 4 << "\n";
    cypherer.decryptFile(encryptedSecret, password, "/");
    cout << 5 << "\n";
    vector<uchar> decryptedData = f.readFile(secret);
    cout << 6 << "\n";
    remove(secret.data());
    remove(encryptedSecret.data());
    
    BOOST_CHECK_EQUAL_COLLECTIONS(secretData.begin(), secretData.end(),
            decryptedData.begin(), decryptedData.end());
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(Steganograpyh)

BOOST_AUTO_TEST_CASE(tempBasedSteganography) {
    LowLevelStega lstega;
    HighLevelStega hstega;
    VectorSubroutines vs;
    Filer f;
    string message = "Some not very long message to hide";
    string tempFileName = lstega.getTempFileName();
    string secret = "secret";
     
    int width = 100;
    int height = 100;
    vector<uchar> dataForTempFile;
    
    vs.appendUint(dataForTempFile, width);
    vs.appendUint(dataForTempFile, height);
    
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            for(int k = 0; k < 4; k++){ //four for alpha red green blue
                dataForTempFile.push_back(0x00);
            }
        }
    }
    
    f.writeFile(dataForTempFile, tempFileName);
    
    vector<uchar> dataToTemp = vs.vectorFromString(message);
    f.writeFile(dataToTemp, secret);
    hstega.hideFileInTemp(secret);
    
    remove(secret.data());

    hstega.takeFileFromTemp("");
    
    vector<uchar> dataFromTemp = f.readFile(secret);
    
    remove(secret.data());
    remove(tempFileName.data());
    
    BOOST_CHECK_EQUAL_COLLECTIONS(dataToTemp.begin(), dataToTemp.end(),
            dataFromTemp.begin(), dataFromTemp.end());
}

BOOST_AUTO_TEST_CASE(jpegSteganography) {
    HighLevelStega hstega;
    VectorSubroutines vs;
    string container = "container.jpeg";
    string secret = "secret";
    Filer f;
    string message = "Some not very long message to hide";
    
    vector<uchar> jpegFileData; //can be empty    
    f.writeFile(jpegFileData, container);
    
    vector<uchar> dataToJpeg = vs.vectorFromString(message);
    f.writeFile(dataToJpeg, container);
    hstega.hideFileToJpgStructure(secret, container);
    
    remove(secret.data());

    hstega.takeFileFromTemp("/");
    
    vector<uchar> dataFromJpeg = f.readFile(secret);
    
    remove(secret.data());
    remove(container.data());
    
    BOOST_CHECK_EQUAL_COLLECTIONS(dataToJpeg.begin(), dataToJpeg.end(),
            dataFromJpeg.begin(), dataFromJpeg.end());
}

BOOST_AUTO_TEST_SUITE_END()

        