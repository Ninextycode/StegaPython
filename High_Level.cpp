#include "TempBasedStegonography.h"
using namespace stego;




/*
Hides file <i>filename</i> in temp.dat 
returns -1 if temp.dat is inacceptable
returns -2 if file with name *filename* is inacceptable
returns -3 if file with name *filename* is too large to hide in temp.dat

*/
int High_Level::hideFileInTemp(std::string filename) {
 /*   uchar** secretBuffer = new uchar*;
    uchar** containerBuffer = new uchar*;

    ullong imageSize = low_level.readTemp(containerBuffer);
    if (imageSize == 0) {
        delete[] *containerBuffer;
        delete containerBuffer;
        delete secretBuffer;
        return -1;
    }

    ullong fileSize = filer.readAndEncodeFile(filename, secretBuffer);
    if (fileSize == 0) {
        delete[] *secretBuffer;
        delete[] *containerBuffer;
        delete secretBuffer;
        delete containerBuffer;
        return -2;
    }

    int result = low_level.keylessLSB(containerBuffer, secretBuffer);
    if (result != 0) return -3;
    low_level.writeTemp(containerBuffer);

    delete[] *secretBuffer;
    delete[] *containerBuffer;
    delete secretBuffer;
    delete containerBuffer; */
    return 0;
}


/*
Takes from temp.dat and saves it to *resultDir* directory
returns -1 if temp.dat is inacceptable
returns -2 if cannot read file from temp.dat
returns 0 if OK
*/
int High_Level::takeFileFromTemp(std::string resultDir) {
    /* uchar** secretBuffer = new uchar*;
    uchar** containerBuffer = new uchar*;

    ullong containerSize = low_level.readTemp(containerBuffer);
    if (containerSize == 0) {
        delete[] *containerBuffer;
        delete containerBuffer;
        delete secretBuffer;
        return -1;
    }
    ullong secretSize = low_level.de_keylessLSB(containerBuffer, secretBuffer);
    if (secretSize == 0) {
        delete[] *containerBuffer;
        delete containerBuffer;
        delete secretBuffer;
        return -2;
    }
    filer.writeEncodedFile(resultDir, secretBuffer);


    delete[] *secretBuffer;
    delete[] *containerBuffer;
    delete secretBuffer;
    delete containerBuffer; */
    return 0;
}

stego::High_Level::High_Level()
	: low_level(1), filer()
{
}

int stego::High_Level::takeFileFromJpgStructure(std::string imagenameStd, std::string resultDirStd) {
    /* uchar** secretBuffer = new uchar*;
    ullong secretSize = low_level.takeFileBufferFromJpgStructure(imagenameStd, secretBuffer);
    if (secretSize == 0) {
        delete secretBuffer;
        return -1;
    }
    filer.writeEncodedFile(resultDirStd, secretBuffer);

    delete[] *secretBuffer;
    delete secretBuffer; */
    return 0;
}

int stego::High_Level::hideFileToJpgStructure(std::string imagenameStd, std::string filenameStd) {
    /*
    uchar** secretBuffer = new uchar*;
    ullong fileSize = filer.readAndEncodeFile(filenameStd, secretBuffer);
    if (fileSize == 0) {
        delete secretBuffer;
        return -1;
    }
    int result = low_level.hideFileBufferInJpgStructure(imagenameStd, secretBuffer, fileSize);
    if (result == 0) {
        delete secretBuffer;
        return -2;
    }
    delete[] *secretBuffer;
    delete secretBuffer;
    */
    return 0;
}

stego::High_Level::~High_Level() {
}





//CRYPTO GOES HERE




crypto::High_Level::High_Level() 
	:filer(), low_level()
{
}

crypto::High_Level::~High_Level() {
}


int crypto::High_Level::encryptFile(std::string filename, std::string password, std::string resultDir) {
    /*
    using namespace std;
    vector<uchar> file = filer.readAndEncodeFile(filename);


    ullong fileSize = filer.readAndEncodeFile, file);
    if (fileSize == 0) {
        return -1;
    }	
    ullong allSize = 16 + (fileSize / 16 + 1) * 16;

    uchar * encryptedFile = new uchar[allSize];
    uchar hash[64];
    sha512((uchar *)password.c_str(), password.length(), hash);

    //IV
    for (int i = 0; i < 16; i++) {
        encryptedFile[i] = hash[i];
    }


    //do not include IV
    uchar difference = allSize - 16 - fileSize;

    //sypher
    uchar key[32];
    for (int i = 0; i < 32; i++) {
        key[i] = hash[i + 16];
    }

    for (int i = 16; i < allSize; i+=16) {
        uchar block[16];
        for (int j = 0; j < 16; j++) {
            if (i + j - 16 < fileSize) {
                block[j] = (*file)[i + j - 16];
            } else {
                block[j] = difference;
            }
        }	
        xorFirstWithSecond(block, &encryptedFile[i-16], 16);		
        low_level.encryptAES(block, key);

        for (int j = 0; j < 16; j++) {
                encryptedFile[i + j] = block[j];
        }
    }


    filer.writeFile(resultDir+filename+"_crypt.dat", encryptedFile, allSize);
    
    delete[] encryptedFile;
    delete[] *file;
    delete file; */
    return 0;
}

int crypto::High_Level::decryptFile(std::string filename, std::string password, std::string resultDir) {
	/* std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (!in.is_open())
            return -1;

    in.seekg(0, std::ios::end);
    ullong size = in.tellg();
    in.seekg(0, std::ios::beg);
    uchar * encryptedFile = new uchar[size];
    uchar ** file = new uchar*;
    *file = new uchar[size - 16];

    in.read((char *)encryptedFile, size);

    uchar hash[64];
    sha512((uchar *)password.c_str(), password.length(), hash);

    uchar key[32];
    for (int i = 0; i < 32; i++) {
        key[i] = hash[i + 16];
    }

    for (int i = 16; i < size; i+=16) {
        uchar block[16];
        for (int j = 0; j < 16; j++) {
            block[j] = encryptedFile[i + j];	
        }
        low_level.decryptAES(block, key);
        xorFirstWithSecond(block, &encryptedFile[i - 16], 16);
        for (int j = 0; j < 16; j++) {
            (*file)[i - 16 + j] = block[j];
        }
    }

    filer.writeEncodedFile(resultDir, file);
    in.close();

    delete[] encryptedFile;
    delete[] * file;
    delete file; */
    return 0;	
}


void crypto::High_Level::xorFirstWithSecond(uchar * a, uchar * b, ullong length) {
    for (ullong i = 0; i < length; i++) {
        a[i] = a[i] ^ b[i];
    }
}
