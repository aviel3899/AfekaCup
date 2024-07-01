#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Arena.h"
#include "General.h"

#define MIN_ARENA_SEATS 1000
#define CODE_SIZE 3

void initArena(Arena* arena)
{
	arena->ArenaName = NULL;
	arena->capacity = 0;
}

int setArena(Arena* arena) {
	int count = 0;
	int x = 0;
	do {
		if (count == 0)
			printf("how many seats in the arena? has to be at least %d\n", MIN_ARENA_SEATS);
		else
			printf("wrong input, try again\n");
		scanf("%d", &x);
		count++;
	} while (!IsCapacityOk(x));
	arena->capacity = x;

	if (!SetArenaName(arena, "please enter Arena name")) {
		printf("initialization failed\n");
		return 0;
	}
	return 1;
}

int SetArenaName(Arena* arena, char* msg) 
{
	char* str = getStrExactName(msg);
	if (str) {
		arena->ArenaName = (char*)malloc(strlen(str) * sizeof(char));
		if (!arena->ArenaName)
			return 0;
		strcpy(arena->ArenaName, str);
		free(str);
		return 1;
	}
	return 0;
}

int IsCapacityOk(int x) {
	if (x >= MIN_ARENA_SEATS)
		return 1;
	return 0;
}

int compareArenaName(const void* v1, const void* v2)
{
	Arena* a1 = (Arena*)v1;
	Arena* a2 = (Arena*)v2;
	return strcmp(a1->ArenaName, a2->ArenaName);
}

void freeArena(Arena* arena)
{
	free(arena->ArenaName);
}

void printArena(const Arena* arena)
{
	printf("Arena:\n");
	printf("Arena's name: %s\t", arena->ArenaName);
	printf("Arena's capacity: %d\n", arena->capacity);
}