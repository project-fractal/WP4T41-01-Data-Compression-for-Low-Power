# **WP4T41-01 Data Compression Library for Low Power Services**

Description of the component.

How does the component satisfy the WP/FRACTAL objectives?

## Getting Started

These instructions will get you a copy of the component up and running on your local machine for testing and use purposes. See [Deployment](#deployment) for notes on how to deploy the component in a use case.

### Prerequisites

* CMake version 3.23.1 or higher is necessary to build the libraries; download it at this link: [https://cmake.org/download/]
* Microsoft Visual Studio (**Not** Microsoft Visual Studio Code) is necessary to build and test the libraries on Windows. Download it at this link: [https://visualstudio.microsoft.com/it/] .

    * During the installation procedure, make sure you install modules for C++ applications for Windows.

### Compile Procedure

This compile procedure can be followed to compile both the Compression and Decompression components.

1. On line 9 of the file _CMakeLists.txt_ in the _source_ folder, modify the type of library you need as output: SHARED or STATIC. 

2. Open the terminal in the _source_ folder and run the command:
   ```
    cmake .
   ```
3. This will create a *.sln* file. Double click on it and it will open the project in Microsoft Visual Studio.
4. Right-click on the name of the solution and run "Build Solution".
5. Back to the _source_, the output of the build will be in the folder _Debug_.

## Usage

This is a test procedure for Compression.lib and Decompression.lib with Microsoft Visual Studio. 

1. Create a new “Console App” project on Microsoft Visual Studio.

2. Copy and paste the code from _dc-test.cpp_, that you can find inside the _test_ folder, and change the path and name of the file you want compress or decompress.

3. You may have to change the target architecture of the project:

    * On the top bar of the IDE, select x64;
    * Right click on the project, then _Properties_, and make sure Platform on the top of the window is set to x64;
    * Also make sure _Linker > Advanced > Target Machine_ is set to **MachineX64(/MACHINE:X64)**.

4. Include the library files **Compression.lib** or **Decompression.lib** (according to the test you are performing) following these points:

    * Right click on the project, then choose _Properties_.
    * In the _VC++ Directories_ property page, add the path to the directory that contains the LIB file to _Library Directories_; make sure the option "inherit from parent or project default" is ticked off.
    * Add the path to the library header files (that you can find in _/test/include_) to _Include Directories_; make sure the option "inherit from parent or project default" is ticked off.
    * In the _Linker > Input_ property page, add the name of the LIB files to _Additional Dependencies_, along with the file extension.

5. Build and run the project.

## Deployment

Additional notes on how to deploy this in a system or use case. Explaining the most important interfaces, data structure, output (if anything special).

### Interface

* Input
* Output
* etc...

### Data Structure

* Class
* etc...

## Additional Documentation and Acknowledgments

* Secondary database
* Additional reading documents
* Helpful links
* etc...