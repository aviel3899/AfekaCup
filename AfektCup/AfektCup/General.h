#pragma once

#define MAX_STR_LEN 255

void generalArrayFunction(void* element, int numOfElements, size_t sizeOfEachElement, void (*func)(void* v1, int v2));
char* getStrExactName(const char* msg);
char* myGets(char* buffer, int size, void(*enterKey));
char* getDynStr(char* str);
char** splitCharsToWords(char* str, int* pCount, int* pTotalLength);
char* readNameFromBFile(FILE* fp);