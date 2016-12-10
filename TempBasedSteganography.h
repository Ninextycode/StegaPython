#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stdexcept> 
#include <iterator> 


typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long long  ullong;

namespace stcr {
    class VectorSubroutines {
    public:
        std::vector<uchar> vectorFromString(std::string s);
        std::string stringFromVector(const std::vector<uchar>& data, ullong begin, ullong end);
        void appendUllong(std::vector<uchar>& data, ullong x);
        ullong getUllong(const std::vector<uchar>& data, ullong position);
        
        void appendUint(std::vector<uchar>& data, uint x);
        uint getUint(const std::vector<uchar>& data, ullong position);
    };

    class Filer {
    public:
        std::vector<uchar> readAndEncodeFile(std::string pathToFile); //encodes file name and size to vector
        std::vector<uchar> readFile(std::string pathToFile); //gets only the binary representation of a file
        void writeEncodedFile(const std::vector<uchar>& data, std::string directory);
        void writeFile(const std::vector<uchar>& data, std::string filename);
    private:
        std::string extractFileNameFromVector(const std::vector<uchar>& data);
        
        std::string getProperFileName(std::string pathToFile);
        ullong getFileLength(std::string pathToFile);
        
        void writeFileNameToVector(std::vector<uchar>& data, std::string filename);
        void writeLengthToVector(std::vector<uchar>& data, ullong length);
        void writeFileToVector(std::vector<uchar>& data, std::string pathToFile);
    };

    class LowLevelStega {
    public:
        std::vector<uchar> readTemp();
        void writeTemp(uchar** data);
        int keylessLSB(uchar ** container, uchar ** secret);
        ullong de_keylessLSB(uchar ** container, uchar ** secret);
        ullong takeFileBufferFromJpgStructure(std::string imagename, uchar** data);
        int hideFileBufferInJpgStructure(std::string imagename, uchar** data, ullong size);
        std::string getTempFileName();
    private:
        std::string tempFileName = "temp.dat";
        uchar read8LSB(uchar* data);
        std::string directory;
    };

    class HighLevelStega {
    public:
        
        void hideFileInTemp(std::string filename);

        void takeFileFromTemp(std::string resultDir);

        void takeFileFromJpgStructure(std::string imagenameStd, std::string resultDirStd);

        void hideFileToJpgStructure(std::string imagenameStd, std::string filenameStd);

    };

    class LowLevelCrypto {
    public:
        std::vector<uchar> encryptAES(const std::vector<uchar>& data, const std::vector<uchar>& key);
        std::vector<uchar> decryptAES(const std::vector<uchar>& data, const std::vector<uchar>& key);
    private:
        void keyScheduleCore(uchar(&data)[4], int i);
        void leftRotation(uchar(&data)[4]);

        void addRoundKey(const std::vector<uchar>& key, uchar(&data)[4][4], int round);

        void subWord(uchar(&data)[4]);

        void invShiftRows(uchar(&data)[4][4]);
        void invMixCols(uchar(&data)[4][4]);
        void invSubData(uchar(&data)[4][4]);

        void shiftRows(uchar(&data)[4][4]);
        void mixCols(uchar(&data)[4][4]);
        void subData(uchar(&data)[4][4]);
        
        //result stored in second argument
        void multiply4(const uchar(&pred)[4][4], uchar(&data)[4][4]);


        std::vector<uchar> keyExpantion(const std::vector<uchar>& key);
        uchar multiplyInG256(uchar a, uchar b);



        static const uchar s_box[16 * 16];
        static const uchar inverce_s_box[16 * 16];
        static const uchar rcon[256];
        static const uchar rijndael[4][4];
        static const uchar inverce_rijndael[4][4];

    };

    class HighLevelCrypto {
    public:
        void encryptFile(std::string filename, std::string password, std::string resultDir);
        void decryptFile(std::string filename, std::string password, std::string resultDir);
        
        std::vector<uchar> encryptVectorByCipherBlockChaining(std::vector<uchar>& data, 
                const std::vector<uchar>& password, const std::vector<uchar>& IV);        
        std::vector<uchar> decryptVectorByCipherBlockChaining(const std::vector<uchar>& data, 
                const std::vector<uchar>& password);
        std::string getPostfixForEncryptedFiles();
    private:
        void appendHashToVector(std::vector<uchar>& data);
        bool checkHashAtTheEnd(const std::vector<uchar>& data);
        
        void padPKCS7(std::vector<uchar>&  data);
        void unpadPKCS7(std::vector<uchar>& data);
        
        std::vector<uchar> xorFirstWithSecond(const std::vector<uchar>& a, const std::vector<uchar>& b);   
        
        std::string postfixForEncryptedFiles = "_crypt.dat";
    };

    class Sha512 {
    public:
        std::vector<uchar> digest(std::vector<uchar>& data);
    
    private:
        void preformComputations(std::vector<uchar>& data);
        void digestBlock(std::vector<uchar>& data, int numberOfBlock);
        void prepareMessageSchedule(std::vector<uchar>& data, int numberOfBlock);
        void initialiseWorkingVariables();
        void mix();
        void computeIntermediateHashValues();
        std::vector<uchar> formHash();
        
        ullong rotr(int n, ullong x);
        ullong shr(int n, ullong x);
        
        ullong sigma0(ullong x);
        ullong sigma1(ullong x);
        
        ullong SIGMA0(ullong x);
        ullong SIGMA1(ullong x);
        
        ullong Ch(ullong x, ullong y, ullong z);
        ullong Maj(ullong x, ullong y, ullong z);
        
        void pad(std::vector<uchar>& data);
        void unpad(std::vector<uchar>& data);
        
        void append1followedBy0(std::vector<uchar>& data, int nBytes);
        
        void sizeToVector(std::vector<uchar>& data, ullong size);
        
        ullong a, b, c, d, e, f, g, h;
        ullong H[8];
        ullong w[80];
        
        static const ullong H_static[8];
        
        static const ullong k[80];
        
    };
}
