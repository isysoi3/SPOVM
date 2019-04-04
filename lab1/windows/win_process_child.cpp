#include <iostream>
#include <windows.h>
#include <ctime>
#include <fstream>
#include <string.h>

#define ARRAY_SIZE 10
#define FILE_MAPPING_OBJ_NAME "Global\FileMappingObject"

int* fillIntArray(int * arr)
{
	for (int i = 0; i < 10; i++)
	{
		arr[i] = rand() % 10;
	}
	return arr;
}

void printArray(int * arr) 
{
	std::cout << "Array is : \n";
	for (int i = 0; i < 10; i++)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << '\n';
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	std::cout << "Child process.\n";

	HANDLE hMapFile;
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		FILE_MAPPING_OBJ_NAME);
	if (hMapFile == NULL) 
	{
		std::cout << "error";
		system("pause");
		return GetLastError();
	}
	int* pBuf = static_cast<int*>(MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		ARRAY_SIZE * sizeof(int)));

	if (pBuf == NULL)
	{
		CloseHandle(hMapFile);
		return 	GetLastError();
	}
	fillIntArray(pBuf);
	printArray(pBuf);
	std::cout << "The process sent array to the parent.\n";
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);	
	system("pause");
	return 0;
}