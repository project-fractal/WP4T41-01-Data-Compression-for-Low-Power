#pragma once

using std::string;
using namespace std;

//this class must
//open and close file at the path taken as input
//read file and call compress or decompress according to input int
class FileManag
{
public:
	FileManag();
	~FileManag();

	virtual void doAction(string filePath, string fileName, string fileExtension);
};

