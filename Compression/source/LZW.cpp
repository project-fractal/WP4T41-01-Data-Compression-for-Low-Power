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
	byte = 0;
	
	*readFile = '\0';
	*writeFile = '\0';
	// clear the pointers
	getPtr = NULL;
	writePtr = NULL;

	//DEBUG ---------
	//writePtr3 = NULL;
	//---------------
	
};
LZW::~LZW() {};
