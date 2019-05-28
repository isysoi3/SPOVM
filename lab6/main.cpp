#include <windows.h>
#include <windowsx.h>
#include <iostream>

//выделение памяти
void* memoryAllocation(int size) {
    //Выделяет указанное количество байтов из кучи.
	HLOCAL hmemLocal = LocalAlloc(GHND, size + sizeof(HLOCAL));

	if (hmemLocal != NULL) {
        //Блокирует объект локальной памяти и возвращает указатель на первый байт блока памяти объекта.
		void* pLocal = LocalLock(hmemLocal);
		if (pLocal != NULL) {
			// Запись дескриптора в начало выделенной памяти
			pLocal = hmemLocal;
			// Установка начала памяти на следующий после дескриптора байт
			pLocal = (char*)pLocal + sizeof(HLOCAL); //(мне не нрав что чар!!!ЛИЗА)
			return pLocal;
		} else {
			std::cout << "Error in locking block" << std::endl;
            std::cout << GetLastError() << std::endl;
            return nullptr;
		}
	} else {
	    std::cout << "Not enough memory" << std::endl;
        std::cout << GetLastError() << std::endl;
        return nullptr;
	}
}

// находит место и возвращает указатель
void* myMalloc(int size) {
	void* pLocal = memoryAllocation(size);

    if (pLocal == nullptr) {
		// попытка дефрагментации памяти и ее повторного выделения
		UINT uMaxFreeMem = LocalCompact(size);
		std::cout << "Trying to defragmentate memory" << std::endl;
		pLocal = memoryAllocation(size);
		if (pLocal == nullptr) {
			std::cout << "Not enough memory" << std::endl;
			return nullptr;
		} else {
            return pLocal;
        }
	} else {
	    return pLocal;
    }
}

void myFree(void* ptr) {
    // получение дескриптора памяти
	HLOCAL hmemLocal = (char*)ptr - sizeof(HLOCAL); //(мне не нрав что чар!!!ЛИЗА)

	// разлок hmemLocal
	LocalUnlock(hmemLocal);

	// возвращает NULL при успешном освобождении памяти
	if (LocalFree(hmemLocal) != NULL) {
		std::cout << "Error in making memory free" << std::endl ; 
        std::cout << GetLastError() << std::endl;
	} else {
		std::cout << std::endl << "Memory is free";
        ptr = nullptr;
	}
}
 
void* myRealloc(void* ptr, int newSize) {
    // получение дескриптора памяти
	HLOCAL hmemLoDiscard = (char*)ptr - sizeof(HLOCAL);

    //Измене размера указанного объекта локальной памяти. Размер может увеличиваться или уменьшаться.
	hmemLoDiscard = LocalReAlloc(hmemLoDiscard, newSize, NULL);

	if(hmemLoDiscard == NULL) {
		std::cout << "Error in realloc block" << std::endl;
        std::cout << GetLastError() <<std::endl;
		return nullptr;
	}

	void* startMem = (char*)ptr - sizeof(HLOCAL);
	startMem = LocalLock(hmemLoDiscard);
	if(startMem == NULL) {
		std::cout << "Error in locking block" << std::endl;
		std::cout << GetLastError() <<std::endl;
	}
	startMem = hmemLoDiscard;
	return (char*)startMem + sizeof(HLOCAL);
}


int main() {
	char* tmp;
	tmp = (char*)myMalloc(20);
	std::cin >> tmp;
	std::cout << tmp << '\n';

	tmp = (char*)myRealloc(tmp, 30);
	std::cin >> tmp;
	std::cout << tmp;
	myFree(tmp);

	return 0;
}
