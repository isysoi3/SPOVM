#include <windows.h>
#include <iostream>
#include <fstream>

#define CHILD_PROCESS_NAME "win_process_child.exe"
#define FILE_MAPPING_OBJ_NAME "Global\FileMappingObject"
#define ARRAY_SIZE 10

void printArray(int * arr)
{
	std::cout << "Array is : \n";
	for (int i = 0; i < 10; i++)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << '\n';
}

int main()
{
	HANDLE hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		ARRAY_SIZE*sizeof(int),                // maximum object size (low-order DWORD)
		FILE_MAPPING_OBJ_NAME);  // name of mapping object

	if (hMapFile == NULL) {
		std::cout << "Error were occurred while creating file mapping.";
		return GetLastError();
	}

	//starting child process
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(CHILD_PROCESS_NAME, NULL, NULL, NULL, TRUE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
	{
		std::cout << "Error were occurred while creating new process.\n";
		return GetLastError();
	}

	std::cout << "Waiting for completion of the created process.\n";

	//waiting child process
	WaitForSingleObject(piApp.hProcess, INFINITE);

	//close handles
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);

	int* childArray = static_cast<int*>(MapViewOfFile(hMapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,
		0,
		ARRAY_SIZE * sizeof(int)));

	if (childArray == NULL)
	{
		std::cout << "Error were occured while reading data";
		CloseHandle(hMapFile);
		return GetLastError();
	}

	printArray(childArray);
	UnmapViewOfFile(childArray);
	CloseHandle(hMapFile);
	std::cout << "Process finished work.\n";
	return 0;
}