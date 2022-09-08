#include "Utility.h"
using  namespace std;

#define DICTIONARY_LENGTH 4096
#define CODE_WORD_LENGTH 128
#define MIN_CHAR_INSERT 2

class LZWDeCompression : public Utility
{

	/*--------------------------Variables Declarations-----------------------------------*/
private:
	unsigned char uncompressedData[CODE_WORD_LENGTH];
	int decodedIndex;
	unsigned char dByte;
	int byteUsedCount;
	bool processingPrevious, processDataBit, dictHitFlag[CODE_WORD_LENGTH];
	int tableIndex;
	int ucIndex;
	int count8Bits, count12Bits, intBit;
	int decodeDataBufferIndex, currentProcessedIndex;
	int codeWordLength[DICTIONARY_LENGTH];
	unsigned char dictionary[DICTIONARY_LENGTH][CODE_WORD_LENGTH];
	unsigned char charToInsert[CODE_WORD_LENGTH + 1];

	Data* dataBuffer[CODE_WORD_LENGTH], * nodeBuffer[CODE_WORD_LENGTH], * node, * decodedDataBuffer[CODE_WORD_LENGTH];

/*--------------------------Initialize Variables -----------------------------------------*/
public: 
	LZWDeCompression()
	{
		DicInit();

		bit = (unsigned char)128;
		intBit = DICTIONARY_LENGTH;
		byteUsedCount = 0;
		decodedIndex = 0;
		processingPrevious = false;
		processDataBit = true;
		tableIndex = 0;
		dataBufferIndex = 0;
		currentProcessedIndex = 0;
		processedIndex = 0;
		ucIndex = 0;
		dictHit = false;
		count12Bits = 0;
		count8Bits = 0;
		writeIndex = 0;
		decodeDataBufferIndex = 0;
		charGIndex = 0;
		maxMatchCount = 0;
		constructDictIndex = 0;
	}// end of function


 /*--------------------------Dictionary initialization --------------------------------------*/
	void DicInit()
	{
		// clear the dictionary
		for (int i = 0; i < DICTIONARY_LENGTH; i++)
		{
			for (int j = 0; j < CODE_WORD_LENGTH; j++)
				dictionary[i][j] = '\0';
			codeWordLength[i] = 0;
		}
		// add single characters in dictionary
		unsigned char defaultChar = 0;
		for (int j = 0; j < 256; j++)
		{
			dictionary[j][0] = defaultChar++;
			codeWordLength[j] = 1;
		}

	}// end of function


 /*--------------------------Read Data from File -----------------------------------------*/
	void ReadBytesFromFile()
	{
		unsigned char fileBuffer[CODE_WORD_LENGTH], ch;
		int bufferIndex = 0, previousDataBufferIndex = 0;

		processDataBit = true;
		while (!feof(getPtr))
		{
			ch = fgetc(getPtr);	// read one byte from file
			fileBuffer[bufferIndex++] = ch;

			byteUsedCount = 0;
			previousDataBufferIndex = dataBufferIndex;
			DecodeByte(ch);

			if (previousDataBufferIndex != dataBufferIndex)
			{
				// decode the characters and create a new list of coded and uncoded bytes	
				ProcessData();

				SearchDictionary();
				if (dictionaryIndex < DICTIONARY_LENGTH)
				{
					InsertInDictionary();
					fetchNext = false;
				}
				else // when the dictionary is full then only dictionary hits may occur
				{
					CheckDictHit();
				}// end of if

				WriteProcessedCharToDisk();

				if (decodeDataBufferIndex == CODE_WORD_LENGTH)
				{
					int temp = FlushBuffer(0);
					constructDictIndex = 0;
					if (temp >= CODE_WORD_LENGTH)
						cout << "\n Error";
					decodeDataBufferIndex = temp;
					writeIndex = constructDictIndex;

				}// end of if
			}// end of if on previousDataBufferIndex

			if (bufferIndex == CODE_WORD_LENGTH)
				bufferIndex = 0;

			if (dataBufferIndex == CODE_WORD_LENGTH)
			{
				int localIndex = 0;
				dataBufferIndex = 0;
				for (int j = currentProcessedIndex; j < CODE_WORD_LENGTH; j++)
					dataBuffer[localIndex++] = dataBuffer[j];

				currentProcessedIndex = 0;
				dataBufferIndex = localIndex;

			}// end of if

			totalBytes++;		// count total bytes in file
		} // end of while		

		if (constructDictIndex != decodeDataBufferIndex)
		{
			for (int m = constructDictIndex; m < decodeDataBufferIndex; m++)
				decodedDataBuffer[m]->processed = true;

			constructDictIndex = decodeDataBufferIndex;
			WriteProcessedCharToDisk();
		}
	}// end of function
	

/*---------------------------------Process Data-----------------------------------------*/
	void ProcessData()
	{
		while (currentProcessedIndex < dataBufferIndex)
		{
			node = dataBuffer[currentProcessedIndex];

			if (node->flag == false)
			{
				if (decodeDataBufferIndex < CODE_WORD_LENGTH)
				{
					node->decoded = true;
					decodedDataBuffer[decodeDataBufferIndex++] = node;
					dataBuffer[currentProcessedIndex++] = node;
				}
				else
					cout << "\n Error in Open Node.";
			}// end of if
			else
			{
				int i = -1, tabIndex = node->tableIndex;	
				for (i = 0; i < codeWordLength[tabIndex]; i++)
				{
					node = new Data;
					node->dataByte = dictionary[tabIndex][i];
					node->tableIndex = -1;
					node->flag = false;
					node->processed = false;
					node->partofDict = false;
					node->dictHit = false;
					node->dictHitIndex = -1;
					node->decoded = true;
					decodedDataBuffer[decodeDataBufferIndex++] = node;

					SearchDictionary();
					if (dictionaryIndex < DICTIONARY_LENGTH)
					{
						InsertInDictionary();
						fetchNext = false;
					}
					else // when the dictionary is full then only dictionary hits may occur
					{
						CheckDictHit();
					}// end of if

					WriteProcessedCharToDisk();

					if (decodeDataBufferIndex == CODE_WORD_LENGTH)
					{
						int temp = FlushBuffer(0);
						constructDictIndex = 0;
						if (temp >= CODE_WORD_LENGTH)
						{
							cout << "\n Error in Process Data Function";
							exit(0);
						}
						decodeDataBufferIndex = temp;
						writeIndex = constructDictIndex;

					}// end of if

				}// end of for

				if (i > 0)
				{
					node = dataBuffer[currentProcessedIndex];
					node->decoded = true;
					dataBuffer[currentProcessedIndex++] = node;
				}
				else
				{
					cout << "\n Error: Invalid Table Index = " << tabIndex;
					closepointers();
					break;
				}

			} // end of else

		} // end of while

	}// end of function


/*--------------------------Search Dictionary-----------------------------------------*/
	void SearchDictionary() //FIXME matchCount uses too much stack?
	{
		int matchCount[DICTIONARY_LENGTH];

		// find / group the characters that need to be inserted
		charGIndex = 0;
		for (int k = constructDictIndex; k < decodeDataBufferIndex; k++)
		{
			nodeBuffer[charGIndex] = decodedDataBuffer[k];
			charToInsert[charGIndex++] = nodeBuffer[charGIndex]->dataByte;
		}
		charToInsert[charGIndex] = '\0';

		// hashing can later be used here instead of linear searching
		maxMatchCount = 0;
		dictHit = false;

		for (int j = 0; j < (dictionaryIndex); j++)
		{
			if (charToInsert[0] == dictionary[j][0]) // check other characters for a match if the first character matches
			{
				matchCount[j] = 0;
				for (int k = 0; k < codeWordLength[j]; k++)
				{
					if (charToInsert[k] == dictionary[j][k])
					{
						matchCount[j]++;
					}
					else
					{
						matchCount[j] = 0;
						break;
					}
				}// end of for

				if (matchCount[j] > maxMatchCount)
				{
					maxMatchCount = matchCount[j];
					remMaxMatchIndex = j;
				}

				// mean the dictionary entry already exists for this character sequence
				if (maxMatchCount == charGIndex)
				{
					if (charGIndex > MIN_CHAR_INSERT && j > 255)
					{
						bool justOnce = false;
						for (int m = constructDictIndex; m < decodeDataBufferIndex; m++)
						{
							if (decodedDataBuffer[m]->processed == false && decodedDataBuffer[m]->partofDict == false)
							{
								decodedDataBuffer[m]->dictHit = true;
								if (justOnce == false)
								{
									decodedDataBuffer[m]->dictHitIndex = j;
									justOnce = true;
								}
							}
							else if (decodedDataBuffer[m]->partofDict == true)
							{
								maxMatchCount = charGIndex = 0;
								decodedDataBuffer[m]->processed = true;
								constructDictIndex++;
								break;
							}
						}// end of for

						if (justOnce == true)
						{
							dictHit = true;					// set the global hit flag
							dictionaryHits++;				// increment the counter
							recordMatchCount = charGIndex;	// record the number of characters found
							insertFlag = true;
						}
					}// end of if

					break;
				} // end of if

			} // end of if

		} // end of for

	}// end of function


/*--------------------------Insert Into Dictionary-----------------------------------------*/
	void InsertInDictionary()
	{
		char firstMatch;
		int localCount = 0;

		if (dictionaryIndex < DICTIONARY_LENGTH)
		{
			if (maxMatchCount != charGIndex && charGIndex > MIN_CHAR_INSERT && charGIndex < CODE_WORD_LENGTH) // we want to add only those character sequences to the dictionary that are atleast 3 characters long
			{
				// insert into dictionary as the character sequence is new and save the node state
				for (int k = 0, m = constructDictIndex; k < charGIndex; k++, m++)
				{
					if (k == 0)
						firstMatch = charToInsert[k];
					else
					{
						if (firstMatch == charToInsert[k])
						{
							localCount++;
						}
					}// end of else

					dictionary[dictionaryIndex][k] = charToInsert[k];
					codeWordLength[dictionaryIndex] = k + 1;

					if (nodeBuffer[k]->dictHit == false)
						nodeBuffer[k]->partofDict = true;

					decodedDataBuffer[m] = nodeBuffer[k];

				}// end of for

				dictionaryIndex++;

				nodeBuffer[0]->processed = true;
				decodedDataBuffer[constructDictIndex] = nodeBuffer[0];
				constructDictIndex++;

				if (insertFlag == true || localCount == (charGIndex - 1))
				{
					if (recordMatchCount == 0)
						recordMatchCount = (localCount + 1);

					for (int h = constructDictIndex; h < (constructDictIndex + (recordMatchCount - 1)); h++)
					{
						decodedDataBuffer[h]->processed = true;
						if (localCount == (charGIndex - 1) && decodedDataBuffer[h]->dictHit == false)
						{
							decodedDataBuffer[h]->partofDict = true;
						}
					}

					constructDictIndex += (recordMatchCount - 1);
					insertFlag = false;
					recordMatchCount = 0;
				}// end of if

			}// end of if

		} // end of if   

	}// end of function	


/*--------------------------Decimal to Binary Converion-----------------------------------------*/
	void DecodeByte(unsigned char byte)
	{
		int tBit = 0, intByte = (int)byte;
		while (byteUsedCount < 8)
		{
			if (processDataBit == true && byteUsedCount < 8) // extract the data bit
			{
				dataBit = 0;
				dByte = 0;
				tableIndex = 0;
				dataBit = (byte & bit);
				for (int f = 0; f < 7; f++)
					dataBit >>= 1;

				byte <<= 1;
				processDataBit = false;
				processingPrevious = true;
				byteUsedCount++;
				count8Bits = 0;
				count12Bits = 0;
			} // end of if		

			if (processingPrevious == true && byteUsedCount < 8)
			{
				if (dataBit == 0) // uncoded char
				{
					while (count8Bits < 8 && byteUsedCount < 8)
					{
						tBit = (byte & bit);
						dByte = (dByte | tBit);
						if (count8Bits < 7)
							dByte >>= 1;
						byte <<= 1;
						count8Bits++;
						byteUsedCount++;
					} // end of while

					if (count8Bits == 8)
					{
						node = new Data;
						node->dataByte = dByte;
						node->tableIndex = -1;
						node->flag = false;
						node->processed = false;
						node->partofDict = false;
						node->dictHit = false;
						node->dictHitIndex = -1;
						node->decoded = false;
						dataBuffer[dataBufferIndex++] = node;

						dByte = 0;
						processDataBit = true;
						processingPrevious = false;
					}// end of if					

				}// end of if
				else // coded char
				{
					while (byteUsedCount < 8 && count12Bits < 12)
					{
						tBit = (byte & bit);
						for (int shift = 0; shift < 4; shift++)
							tBit <<= 1;
						tableIndex = (tableIndex | tBit);
						if (count12Bits < 11)
							tableIndex >>= 1;
						byte <<= 1;
						count12Bits++;
						byteUsedCount++;
					} // end of while

					if (count12Bits == 12)
					{
						if (tableIndex < 256)
							cout << "\n Error in reading = " << tableIndex;
						node = new Data;
						node->dataByte = 'n';
						node->tableIndex = tableIndex;
						node->flag = true;
						node->processed = false;
						node->partofDict = false;
						node->dictHit = false;
						node->dictHitIndex = -1;
						node->decoded = false;
						dataBuffer[dataBufferIndex++] = node;

						tableIndex = 0;
						processDataBit = true;
						processingPrevious = false;
					}// end of if					

				}// end of else

			}// end of if 

		}// end of while

	} // end of function
	

/*--------------------------Write Processed Data to Disk-----------------------------------------*/
	void WriteProcessedCharToDisk()
	{
		bool writingSuccess = false;
		// write the decoded charaters to file that have been processed
		for (int w = writeIndex; w < constructDictIndex; w++)
		{
			if (decodedDataBuffer[w]->processed == true)
			{
				fputc(decodedDataBuffer[w]->dataByte, writePtr);
				writingSuccess = true;
			} // end of if
		} // end of for

		if (!writingSuccess && writeIndex != constructDictIndex)
			cout << "\n Error";

		writeIndex = constructDictIndex;

	} // end of function


	int FlushBuffer(int temp)
	{
		if (constructDictIndex == 0)
		{
			if (decodedDataBuffer[constructDictIndex]->dictHit == true && charGIndex == CODE_WORD_LENGTH && dictionaryIndex < DICTIONARY_LENGTH)
			{
				// flush the buffer and start again
				int count = 0;
				for (int m = 0; m < (decodeDataBufferIndex - 1); m++)
				{
					if (decodedDataBuffer[m]->dictHit == true)
					{
						decodedDataBuffer[m]->processed = true;
						count++;
					}
				}// end of for
				constructDictIndex += count;
				recordMatchCount = 0;
				insertFlag = false;
			}
			else
			{
				decodedDataBuffer[constructDictIndex]->processed = true;
				decodedDataBuffer[constructDictIndex]->partofDict = true;
				constructDictIndex++;
			}

			WriteProcessedCharToDisk();
		}// end of if

		for (int y = constructDictIndex; y < decodeDataBufferIndex; y++)
		{
			decodedDataBuffer[temp] = NULL;
			decodedDataBuffer[temp++] = decodedDataBuffer[y];
		}// end of for

		return temp;
	}

	void CheckDictHit()
	{
		if (charGIndex == maxMatchCount && charGIndex < CODE_WORD_LENGTH)
		{
			fetchNext = true;
		}
		else if (fetchNext == true)
		{
			int count = 0;
			for (int m = constructDictIndex; m < (decodeDataBufferIndex - 1); m++)
			{
				if (decodedDataBuffer[m]->dictHit == true)
				{
					decodedDataBuffer[m]->processed = true;
					count++;
				}
			}
			constructDictIndex += count;
			fetchNext = false;

		}// end of if
		else if (charGIndex > MIN_CHAR_INSERT)
		{
			decodedDataBuffer[constructDictIndex]->processed = true;
			decodedDataBuffer[constructDictIndex]->partofDict = true;
			constructDictIndex++;
		}
	}

}; 