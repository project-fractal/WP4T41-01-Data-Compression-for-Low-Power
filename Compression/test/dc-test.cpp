#include <iostream>

#include "data_compression.h"

int main()
{
    cout << "\n Start of Data Compression Component test!" << endl;

    //insert custom path and file name here
    string filePath = "";
    string fileName = "";
    string fileExtension = ".wav";

    data_compression dc;

    dc.startComponent(filePath, fileName, fileExtension);

    cout << "\n\n End of Data Compression Component test!\n" << endl;
}