#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "General.h"

void generalArrayFunction(void* element, int numOfElements, size_t sizeOfEachElement, void (*func)(void* v1, int v2))
{
	if (!element)
		return;
	func(element, numOfElements);
}

char* getStrExactName(const char* msg)
{
	char* str;
	char temp[MAX_STR_LEN];
	if(msg)
		printf("%s\t", msg);
	myGets(temp, MAX_STR_LEN, stdin);
	str = getDynStr(temp);
	return str;
}

char* getDynStr(char* str)
{
	char* theStr;
	theStr = (char*)malloc((strlen(str) + 1) * sizeof(char));
	if (!theStr)
		return NULL;
	strcpy(theStr, str);
	return theStr;
}

char* myGets(char* buffer, int size, void(*enterKey))
{
	char* ok;
	if (buffer != NULL && size > 0)
	{
		do { //skip only '\n' strings
			ok = fgets(buffer, size, enterKey);
		} while (ok && ((strlen(buffer) <= 1) && (isspace(buffer[0]))));
		if (ok)
		{
			char* back = buffer + strlen(buffer);
			//trim end spaces
			while ((buffer < back) && (isspace(*--back)));
			*(back + 1) = '\0';
			return buffer;
		}
		buffer[0] = '\0';
	}
	return NULL;
}

char** splitCharsToWords(char* str, int* pCount, int* pTotalLength)
{
	char temp[255];
	char* delimiters = " ";
	char* word;
	int count = 0;

	strcpy(temp, str);
	char** wordsArray = NULL;
	*pTotalLength = 0;

	word = strtok(temp, delimiters);
	while (word != NULL)
	{
		wordsArray = (char**)realloc(wordsArray, (count + 1) * sizeof(char*));
		if (!wordsArray)
			return 0;
		wordsArray[count] = getDynStr(word);
		count++;
		*pTotalLength += (int)strlen(word);
		word = strtok(NULL, delimiters);
	}
	*pCount = count;
	return wordsArray;
}

char* readNameFromBFile(FILE* fp)
{
	int StringCount = 0;
	if (fread(&StringCount, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return NULL;
	}
	char* temp = (char*)malloc(StringCount * sizeof(char));
	if (!temp)
	{
		fclose(fp);
		return NULL;
	}
	if (fread(temp, sizeof(char), StringCount, fp) != StringCount)
	{
		free(temp);
		fclose(fp);
		return NULL;
	}
	return temp;
}