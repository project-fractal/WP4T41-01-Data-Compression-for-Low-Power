#include <iostream>
#include "fstream"

using namespace std; 

#include "FileManag.h"
#include "LZWDeCompression.cpp"
//#include "Utility.h"

FileManag::FileManag() {}
FileManag::~FileManag() {}

void FileManag::doAction(string filePath, string fileName, string fileExtension)
{	
	//Decompress
	std::cout << "\n ----DECOMPRESSION----\n" << endl;

	LZWDeCompression decompressFile;
	//Utility utility;

	decompressFile.OpenFiles(filePath, fileName, fileExtension);
	decompressFile.ReadBytesFromFile();
	decompressFile.closepointers();

	std::cout << "\n ----Decompression Done----" << endl;
}
