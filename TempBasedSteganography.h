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
        void writeEncodedFile(std::vector<uchar>& data, std::string directory);
        void writeFile(const std::vector<uchar>& data, std::string filename);
        ullong getFileLength(std::string pathToFile);
    private:
        ullong encodedFileLength(const std::vector<uchar>& data);
        std::string extractFileNameFromVector(const std::vector<uchar>& data);
        
        std::string getProperFileName(std::string pathToFile);       
        
        void writeFileNameToVector(std::vector<uchar>& data, std::string filename);
        void writeLengthToVector(std::vector<uchar>& data, ullong length);
        void writeFileToVector(std::vector<uchar>& data, std::string pathToFile);
    };

    class LowLevelStega {
    public:
        void keylessLSB(std::vector<uchar>& container, const std::vector<uchar>& secret, ullong start, ullong end);
        std::vector<uchar> de_keylessLSB(const std::vector<uchar>& container, ullong start, ullong end);
       
        std::vector<uchar> takeVectorFromJpgVector(const std::vector<uchar>& container);
        void hideVectorInJpgVector(std::vector<uchar>& container, const std::vector<uchar>& data);
        void eraseJpegTagData(std::vector<uchar>& container);
    private:
        bool vectorContainsJpegTag(const std::vector<uchar>& container);
        ullong startOfJpegTag(const std::vector<uchar>& container);
        
        void write8LSB(uchar& a, uchar& b, uchar& c, uchar& d,
                        uchar& e, uchar& f, uchar& g,  uchar& h,
                        uchar secret);
        
        uchar read8LSB(uchar a, uchar b, uchar c, uchar d,
                        uchar e, uchar f, uchar g,  uchar h);
        
        uchar firstByteTag = 0xff;
        uchar secondByteTag = 0xc7;
    };

    class HighLevelStega {
    public:
        
        void hideVectorInTemp(const std::vector<uchar>& data);
        std::vector<uchar> takeVectorFromTemp();
        
        void hideVectorInJpgStructure(const std::vector<uchar>& data, std::string imagename);
        std::vector<uchar> takeVectorFromJpgStructure(std::string imagename);
        
        void cleanJpegFile(std::string filename);
        
        void hideFielInTemp(std::string filename);
        void takeFileFromTemp(std::string resultDir);
        void hideFileInJpgStructure(std::string filename, std::string imagename);
        void takeFileFromJpgStructure( std::string imagename, std::string resultDir);

        void encodeAndHideFielInTemp(std::string password, std::string filename);
        void decodeAndTakeFileFromTemp(std::string password, std::string resultDir);
        void encodeAndHideFileInJpgStructure(std::string password, std::string filename, std::string imagename);
        void decodeAndTakeFileFromJpgStructure(std::string password, std::string imagename, std::string resultDir);
        
        
        std::string getTempFileName();
    private:
        std::string tempFileName = "temp.dat";
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
        std::vector<uchar> encryptVectorByCipherBlockChaining(std::vector<uchar>& data, 
                const std::string password);
        std::vector<uchar> decryptVectorByCipherBlockChaining(const std::vector<uchar>& data, 
                const std::vector<uchar>& password);
        std::vector<uchar> decryptVectorByCipherBlockChaining(const std::vector<uchar>& data, 
                const std::string password);
        std::string getPostfixForEncryptedFiles();
    private:        
        void padPKCS7(std::vector<uchar>&  data);
        void unpadPKCS7(std::vector<uchar>& data);
        
        std::vector<uchar> xorFirstWithSecond(const std::vector<uchar>& a, const std::vector<uchar>& b);   
        
        std::string postfixForEncryptedFiles = "_crypt.dat";
    };

    class Sha512 {
    public:
        std::vector<uchar> digest(std::vector<uchar>& data);
        void appendHashToVector(std::vector<uchar>& data);
        bool checkHashAtTheEnd(const std::vector<uchar>& data);
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
