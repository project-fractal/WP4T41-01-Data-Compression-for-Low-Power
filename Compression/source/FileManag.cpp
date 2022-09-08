#include "fstream"

#include "FileManag.h"
#include "LZWCompression.cpp"

FileManag::FileManag() {}
FileManag::~FileManag() {}

void FileManag::doAction(string filePath, string fileName, string fileExtension)
{	
	//DEBUG ---
	//	cout << "\n\nDictionary length is " << DICTIONARY_LENGTH << endl;
	// ----

		LZWCompression compressFile;

		compressFile.OpenFiles(filePath, fileName, fileExtension);
		compressFile.ReadBytesFromFile();
		compressFile.closepointers();			

		std::cout << "\n ----Compression Done----\n" << endl;


}
