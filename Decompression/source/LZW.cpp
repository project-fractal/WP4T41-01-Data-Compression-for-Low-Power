#include "LZW.h"

LZW::LZW() 
{
	totalBytes = 0;
	remMaxMatchIndex = 0;
	dictionaryIndex = 256;
	estimatedBits = 0;
	dictionaryHits = 0;
	recordMatchCount = 0;

	fetchNext = false;
	insertFlag = false;
	
	*readFile = '\0';
	*writeFile = '\0';
	// clear the pointers
	getPtr = NULL;
	writePtr = NULL;
	writePtr3 = NULL;
	byte = 0;
};
LZW::~LZW() {};
