#include "TempBasedStegonography.h"
ullong filer::Filer::readAndEncodeFile(std::string filename, uchar** data) {
	std::ifstream in(filename, std::ios::binary | std::ios::ate);
	if (!in.is_open()) {
		return 0;
	}


	in.seekg(0, in.end);
	ullong length = in.tellg();
	in.seekg(0, in.beg);



	std::string clearFilename;
	int i = filename.length() - 1;
	while (i >= 0 && filename.at(i) != '\\') {
		clearFilename = filename.at(i--) + clearFilename;
	}
	*data = new uchar[(uint)(1 + filename.length() + 8) + length];


	//Writing the size of type
	*((*data)++) = (uchar)(clearFilename.length());

	//Writing the type of file

	for (uint i = 0; i < clearFilename.size(); i++) {
		*((*data)++) = (uchar)clearFilename.at(i);
	}

	//Writing the size of file
	for (int i = 0; i < 8; i++) {
		*((*data)++) = (uchar)((length >> (8 * (7 - i))) & 0xff);
		////		std::cout <<" " << i <<" work with: "<< (uint)((length >> (8 * (7 - i))) & 0xff) << "\n";
	}



	in.read((char*)*data, length);  //i do not reallu understnd behaviour seems no chane to *data position

	*data -= (1 + clearFilename.length() + 8); //reset the pointer to the initial position;
											   //for (int i = 0; i < (1 + clearFilename.length() + 8); i++) {
											   //	std::cout << (uint)(*data)[i] << "\n";
											   //}
	in.close();
	return 1 + clearFilename.length() + 8 + length;
}

void filer::Filer::writeEncodedFile(std::string resultDir, uchar** data) {

	uchar filenameSize = *(*data)++;

	//filename.append("_de.");
	std::string filename = "";
	for (uchar i = 0; i < filenameSize; i++) {
		filename += ((char)*((*data)++));
	}


	ullong filesize = 0;
	for (int i = 0; i < 8; i++) {
		filesize |= (((ullong)*((*data)++)) << ((7 - i) * 8));
	}


	std::ofstream out(resultDir + filename
		//+ "." + format
		, std::ios::binary);

	out.write((char*)*data, filesize);

	out.close();

	*(data) -= (1 + filename.size() + 8);
}

void filer::Filer::writeFile(std::string filename, uchar* data, ullong length){
    std::ofstream out(filename, std::ios::binary);
    out.write((char*)data, length);
    out.close();
}