#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stdexcept> 
#include <iterator> 
#include "sha512.h"

typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long long  ullong;

namespace filer {
    class Filer {
    public:
        std::vector<uchar> readAndEncodeFile(std::string pathToFile);
        void writeEncodedFile(const std::vector<uchar>& data, std::string directory);
        void saveFile(const std::vector<uchar>& data, std::string filename);
    private:
        std::string extractFileNameFromVector(const std::vector<uchar>& data);
        int extractLengthFromVector(const std::vector<uchar>& data, int encodedFileNameLength);
        
        
        std::string getProperFileName(std::string pathToFile);
        ullong getFileLength(std::string pathToFile);
        
        void writeFileNameToVector(std::vector<uchar>& data, std::string filename);
        void writeLengthToVector(std::vector<uchar>& data, ullong length);
        void writeFileToVector(std::vector<uchar>& data, std::string pathToFile);
    };
}

namespace stego {
    class Low_Level {
    public:
        ullong readTemp(uchar** data);
        void writeTemp(uchar** data);
        int keylessLSB(uchar ** container, uchar ** secret);
        ullong de_keylessLSB(uchar ** container, uchar ** secret);
        ullong takeFileBufferFromJpgStructure(std::string imagename, uchar** data);
        int hideFileBufferInJpgStructure(std::string imagename, uchar** data, ullong size);
        Low_Level();
        Low_Level(int a) {};
    private:
        uchar read8LSB(uchar* data);
        std::string directory;
    };

    class High_Level {
    public:

        High_Level();
        
        /*
        Hides file <i>filename</i> in temp.dat
        returns -1 if temp.dat is inacceptable
        returns -2 if file with name *filename* is inacceptable
        returns -3 if file with name *filename* is too large to hide in temp.dat
        returns 0 if OK
        */
        int hideFileInTemp(std::string filename);

        /*
        Takes from temp.dat and saves it to *resultDir* directory
        returns -1 if temp.dat is inacceptable
        returns -2 if cannot read file from temp.dat
        returns 0 if OK
        */
        int takeFileFromTemp(std::string resultDir);

        /*
        returns -1 if no file found
        returns 0 if OK
        */
        int takeFileFromJpgStructure(std::string imagenameStd, std::string resultDirStd);

        /*
        returns -1 if problem reading file
        returns -2 if there is already file in image
        returns 0 if OK
        */
        int hideFileToJpgStructure(std::string imagenameStd, std::string filenameStd);


        ~High_Level();
    private:
            Low_Level low_level;
            filer::Filer filer;
    };
}

namespace crypto {
    class Low_Level {
    public:
        void encryptAES(uchar(&data)[16], uchar(&key)[32]);
        void decryptAES(uchar(&data)[16], uchar(&key)[32]);

        Low_Level() {
                //nothing to do here
        };
    private:
        void keyScheduleCore(uchar(&data)[4], int i);
        void leftRotation(uchar(&data)[4]);

        void addRoundKey(uchar(&key)[240], uchar(&data)[4][4], int round);

        void subWord(uchar(&data)[4]);

        void invShiftRows(uchar(&data)[4][4]);
        void invMixCols(uchar(&data)[4][4]);
        void invSubData(uchar(&data)[4][4]);

        void shiftRows(uchar(&data)[4][4]);
        void mixCols(uchar(&data)[4][4]);
        void subData(uchar(&data)[4][4]);

        void multiply4(uchar(&pred)[4][4], uchar(&data)[4][4]);


        void keyExpantion(uchar(&key)[32], uchar(&w_key)[240]);
        uchar multiplyInG256(uchar a, uchar b);



        static uchar s_box[16 * 16];
        static uchar inverce_s_box[16 * 16];
        static uchar rcon[256];
        static uchar rijndael[4][4];
        static uchar inverce_rijndael[4][4];

    };

    class High_Level {
    public:
        High_Level();

        /*
        returns -1 if cannot open file
        */
        int encryptFile(std::string filename, std::string password, std::string resultDir);
        int decryptFile(std::string filename, std::string password, std::string resultDir);

        ~High_Level();

    private:
        crypto::Low_Level low_level;
        filer::Filer filer;
        void xorFirstWithSecond(uchar* a, uchar* b, ullong length);
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
        void computeIntermideateHashValues();
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
        
        //it is possible to extract only 64 bit int, not 128, as sha512 specification suggests
        ullong ullongFromVector(std::vector<uchar>& data, ullong start); 
        
        ullong a, b, c, d, e, f, g, h;
        ullong H[8];
        ullong w[80];
        
        static constexpr ullong H_static[8] {
            0x6a09e667f3bcc908,
            0xbb67ae8584caa73b,
            0x3c6ef372fe94f82b,
            0xa54ff53a5f1d36f1, 
            0x510e527fade682d1,
            0x9b05688c2b3e6c1f,
            0x1f83d9abfb41bd6b,
            0x5be0cd19137e2179
        };
        
        static constexpr ullong k[80] {
              0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538, 
              0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe, 
              0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 
              0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 
              0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, 0x983e5152ee66dfab, 
              0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725, 
              0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 
              0x53380d139d95b3df, 0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b, 
              0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218, 
              0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, 0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 
              0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 
              0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec, 
              0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 0xca273eceea26619c, 
              0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6, 
              0x113f9804bef90dae, 0x1b710b35131c471b, 0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 
              0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
        };
        
    };
}
