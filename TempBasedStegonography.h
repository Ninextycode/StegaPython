#pragma once

#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "sha512.h"
typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long long  ullong;

namespace filer {
    class Filer {
    public:
	Filer(){};
	ullong readAndEncodeFile(std::string filename, uchar** data);
	void writeEncodedFile(std::string resultDir, uchar** data);
        void writeFile(std::string filename,  uchar* data, ullong length);
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
        Sha512();
        int digest(uchar* data, &(uchar[512]))
    };
}
