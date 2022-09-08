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

#include "FileManag.h"

class data_compression{

public:
	void startComponent(string filePath, string fileName, string fileExtension)
	{
		exception_ptr eptr;
		FileManag fileManag;
		
		try
		{
			fileManag.doAction(filePath, fileName, fileExtension);
		}
		catch (...)
		{
			eptr = current_exception();
			cout << "[error]: an exception occurred." << "\n" << endl;

			try
			{
				if (eptr != NULL)
				{
					rethrow_exception(eptr);
				}
			}
			catch (const std::exception& e)
			{
				cout << "Caught exception \"" << e.what() << "\"\n";
			}
			
		}

	}

};
