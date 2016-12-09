#include "TempBasedSteganography.h"

using namespace stcr;
using namespace std;

vector<uchar> LowLevelStega::readTemp() {
	Filer f;
    vector<uchar> data = f.readFile(tempFileName);
    return data;
}

void LowLevelStega::writeTemp(uchar ** data) {
	ofstream out("temp.dat", ios::binary);
	int width = 0;
	for (int i = 0; i < 4; i++) {
		width |= (((int)*((*data)++)) << ((3 - i) * 8));
	}
	int height = 0;
	for (int i = 0; i < 4; i++) {
		height |= (((int)*((*data)++)) << ((3 - i) * 8));
	}
	ullong containerSize = (uint)width*height * 3;

	(*data) -= 8;
	out.write((char*)*data, (containerSize+8));
	out.close();
}

ullong LowLevelStega::takeFileBufferFromJpgStructure(std::string imagename, uchar ** data) {
	ifstream in(imagename, ios::in | ios::binary);
	in.seekg(0, ios::end);
	ullong size = in.tellg();
	in.seekg(0, ios::beg);

	uchar last, now;
	in >> last;
	in >> now;
	ullong i = 0;
	ullong dataSize = 0;
	bool readFlag = false;
	for (; !in.eof(); last = now, in >> now) {
		if (last == 0xff && now == 0xc7) {
			if (!readFlag) {
				readFlag = true;
				dataSize = 0;
				uchar buffer;
				for (int i = 0; i < 8; i++) {
					in >> buffer;
					dataSize *= 256;
					dataSize += ( buffer) ;
				}
				*data = new uchar[dataSize];
				in.read((char*)*data, dataSize);
				break;

			} 
		}
		i++;
	}
	
	in.close();
	return dataSize;
}

int LowLevelStega::hideFileBufferInJpgStructure(string imagename, uchar ** data, ullong size) {
	ifstream in(imagename, ios::binary);

	uchar last, now;
	in >> last;
	in >> now;
	for (; !in.eof(); last = now, in >> now) {
		if (last == 0xff && now == 0xc7) {
			return 0;
		}
	}

	in.close();

	ofstream out(imagename, ios::binary | ios_base::app);
	if (!out.is_open()) {
		return 0;
	}

	uchar buffer = 0xff;
	out.write((char*)&buffer, 1);
	buffer = 0xc7;
	out.write((char*)&buffer, 1);


	for (int i = 0; i < 8; i++) {
		buffer = (size >> ((7 - i) * 8)) & 0xff;
		out.write((char*)&buffer, 1);
	}
	out.write((char *)(*data), size);

	out.close();
	return size;
}

uchar LowLevelStega::read8LSB(uchar* data) {
	uchar secret = 0;
	for (int i = 0; i < 8; i++) {
		secret |= (data[i] & 0x01) << (7 - i);
	}
	return secret;
}

/*
secret and container are the char arrays that were gotten from readFile and readImage
ONLY in this case the function works  properly
*/
int LowLevelStega::keylessLSB(uchar** container, uchar** secret) {

	uchar formatSize = *(*secret)++;
	*secret += formatSize;
	uint filesize = 0;
	for (int i = 0; i < 8; i++) {
		filesize |= (((uint)*((*secret)++)) << ((7 - i) * 8));
	}
	uint secretSize = 1 + 8 + filesize + formatSize;

	//We determine the size of the container to thech the possibility of hiding a secret in it
	int width = 0;
	for (int i = 0; i < 4; i++) {
		width |= (((int)*((*container)++)) << ((3 - i) * 8));
	}
	int height = 0;
	for (int i = 0; i < 4; i++) {
		height |= (((int)*((*container)++)) << ((3 - i) * 8));
	}
	ullong containerSize = (uint)width*height * 3;

	// secret's pointer is reseted to the position before header, because header should also be hided
	// container's is not, because we dont hide in the bits that store the size information
	*secret -= 1 + 8 + formatSize;



	//We determine the size of the container to thech the possibility of hiding a secret in it
	if ((secretSize * 8) > containerSize) 
		return -1;

	for (uint i = 0; i < secretSize; i++) {
		for (int j = 0; j < 8; j++) {
			*(*container)++ = (**container & 0xfe) | (0x01 & (**secret >> (7 - j)));
		}
		(*secret)++;
	}
	*secret -= secretSize;
	*container -= (secretSize * 8 + 8);

	int a = 0;
	return 0;
}

/*
container is a char array gotten fron imageReader, where image contained a secret in a format returned from readFile
secret is a pointer to  an empty char array
ONLY in this case the function works  properly
*/
ullong LowLevelStega::de_keylessLSB(uchar** container, uchar** secret) {

	int carate = 0;
	carate += 8; //avoid reading size information
	uchar filenameSize = read8LSB(&(*container)[carate]);
	carate += 8;

	string filename;
    
	for (int i = 0; i < filenameSize; i++) {
		uchar digit = read8LSB(&(*container)[carate]);

		filename += read8LSB(&(*container)[carate]);
		carate += 8;
	}

	ullong fileSize = 0;
	
	for (int i = 0; i < 8; i++) {
		fileSize |= (uint)(read8LSB(&(*container)[carate]) << ((7 - i) * 8));
		carate += 8;
	}

	if (fileSize > 1E9) return 0;
		*secret = new uchar[1 + filenameSize + 8 + fileSize];
	
	*(*secret)++ = filenameSize;
	
	for (int i = 0; i < filenameSize; i++) {
		*(*secret)++ = filename.at(i);
	}
	
	for (int i = 0; i < 8; i++) {
		*(*secret)++ = (uchar)((fileSize >> (8 * (7 - i))));
	}
	
	for (uint i = 0; i < fileSize; i++) {
		*(*secret)++ = read8LSB(&(*container)[carate]);
		carate += 8;
	}

	*secret -= (1 + filenameSize + 8 + fileSize);
	return (1 + filenameSize + 8 + fileSize);
}

string LowLevelStega::getTempFileName() {
    return tempFileName;
}