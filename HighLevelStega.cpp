#include "TempBasedSteganography.h"
using namespace stcr;
using namespace std;


void HighLevelStega::hideFileInTemp(std::string filename) {
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
}


void HighLevelStega::takeFileFromTemp(std::string resultDir) {
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
}

void HighLevelStega::takeFileFromJpgStructure(std::string imagenameStd, std::string resultDirStd) {
    /* uchar** secretBuffer = new uchar*;
    ullong secretSize = low_level.takeFileBufferFromJpgStructure(imagenameStd, secretBuffer);
    if (secretSize == 0) {
        delete secretBuffer;
        return -1;
    }
    filer.writeEncodedFile(resultDirStd, secretBuffer);

    delete[] *secretBuffer;
    delete secretBuffer; */
}

void HighLevelStega::hideFileToJpgStructure(std::string imagenameStd, std::string filenameStd) {
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
}