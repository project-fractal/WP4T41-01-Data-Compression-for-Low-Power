//============================================================================
// Name        : data_compression.h
// Author      : Ro Technology
// Version     : v0.1
//============================================================================

/****
 *
 * This library has one entry point which takes as input:
 * **	string filePath
 * **	string fileName
 * **	string fileExtension
 *
 ****/

#pragma once
#include <iostream>
#include <vector>

#include "FileManag.h"

using std::string;
using namespace std;

class data_compression{

public:
	void startComponent(string filePath, string fileName, string fileExtension)
	{
	//	exception_ptr eptr;
		//cout << "[info]: program started" << endl;

		try
		{
			FileManag fileManag;
			fileManag.doAction(filePath, fileName, fileExtension);
		}
		catch (...)
		{
	//		eptr = current_exception();
			cout << "[error]: an exception occurred." << "\n" << endl;

	/*		try
			{
				if (eptr != NULL)
				{
	//				rethrow_exception(eptr);
				}
			}
			catch (const std::exception& e)
			{
				cout << "Caught exception \"" << e.what() << "\"\n";
			} */
			
		}

	}

};
