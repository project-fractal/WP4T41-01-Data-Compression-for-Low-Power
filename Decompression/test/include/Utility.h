//============================================================================
// Name        : utility.h
// Author      : Ro Technology
// Version     : v0.1
//============================================================================

#pragma once

#include <string>
#include "LZW.h"


using std::string;
using namespace std;


class Utility : public LZW 
{
public:
	Utility();
	~Utility();

	void OpenFiles(string filePath, string fileName, string fileExtension);
	void closepointers();
	void GetFileSize();
	void DecToBin(int data);
};