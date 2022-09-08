#include "Utility.h"


Utility::Utility() {}
Utility::~Utility() {}


/*--------------------------Get File Size-----------------------------------------*/
void Utility::GetFileSize()
{
	fpos_t filepos;
	fseek(getPtr, 0, SEEK_SET);
	fgetpos(getPtr, &filepos);
	fseek(getPtr, 0, SEEK_END);		// seeks to the end of file
	totalFileSize = ftell(getPtr);   // tells the size of the file
	std::cout << "\nTotal File Size = " << totalFileSize;
}


/*--------------------------Open File for Reading-----------------------------------------*/
void Utility::OpenFiles(string filePath, string fileName, string fileExtension)
{
	string filetoopen = filePath + fileName + ".lzw";
	string filetowrite = filePath + fileName + fileExtension;
	string filedict = filePath + "Decoded_File" + ".txt";

	if ((getPtr = fopen(filetoopen.c_str(), "rb")) == NULL)
	{
		cout << "\n Unable To Open File=" << filetoopen;
		exit(1);
	}

	if ((writePtr = fopen(filetowrite.c_str(), "wb")) == NULL)
	{
		cout << "\n Unable To Write File=" << filetowrite;
		exit(1);
	}

}


/*--------------------------Close File Pointers-----------------------------------------*/
void Utility::closepointers()
{
	fclose(getPtr);
	fclose(writePtr);
}


/*--------------------------Decimal to Binary Converion-----------------------------------------*/
void Utility::DecToBin(int data)
{
	//int data = (int) b;
	int rem = 0;
	data = abs(data);

	binaryIndex = 0;
	for (int g = 0; g < 12; g++)
		binary[g] = 0;

	while (data > 0)// || data!=1)
	{
		rem = data % 2;
		if (rem == 0)
			binary[binaryIndex++] = 0;
		else
			binary[binaryIndex++] = 1;

		data = data / 2;
	}

}
