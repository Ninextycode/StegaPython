#include "TempBasedStegonography.h"
using namespace stcr;
using namespace std;



/*
Hides file <i>filename</i> in temp.dat 
returns -1 if temp.dat is inacceptable
returns -2 if file with name *filename* is inacceptable
returns -3 if file with name *filename* is too large to hide in temp.dat

*/
int HighLevelStego::hideFileInTemp(std::string filename) {
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
int HighLevelStego::takeFileFromTemp(std::string resultDir) {
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

int HighLevelStego::takeFileFromJpgStructure(std::string imagenameStd, std::string resultDirStd) {
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

int HighLevelStego::hideFileToJpgStructure(std::string imagenameStd, std::string filenameStd) {
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