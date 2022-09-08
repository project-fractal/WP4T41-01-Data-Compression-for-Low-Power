#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


class LZW
{
public:
	LZW();
	~LZW();
	
	FILE* getPtr, * writePtr; // stores the file pointer after the file has been opened
	
	//DEBUG ----------
	// FILE* writePtr3;
	// ---------------
	
	struct Data
	{
		unsigned char dataByte;
		int dictHitIndex;
		bool partofDict;
		bool processed;
		bool dictHit;
	};
		
protected:
	char  readFile[2048], writeFile[2048];	// stores the filenames used for opening and writing new files
	int dictionaryIndex, dataBufferIndex, processedIndex, writeIndex, constructDictIndex;
	double estimatedBits;
	long double totalFileSize;
	int dictionaryHits;
	long totalBytes; // count the total number of bytes in file
	int binary[12], binaryIndex;
	bool insertFlag, dictionaryEntry, dictHit, fetchNext;
	unsigned char byte, bit, dataBit;
	int charGIndex, maxMatchCount;
	int remMaxMatchIndex, recordMatchCount;
};

