#pragma once

#include <string.h>
using  namespace std;


class FileManag
{
public:
	FileManag();
	~FileManag();

	virtual void doAction(string filePath, string fileName, string fileExtension);
};

