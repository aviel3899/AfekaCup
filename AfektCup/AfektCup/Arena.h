#pragma once

typedef struct {
	int capacity;
	char* ArenaName;
}Arena;

void initArena(Arena* arena);
int setArena(Arena* arena);
int IsCapacityOk(int x);
int SetArenaName(Arena* arena, char* msg);
int compareArenaName(const void* v1, const void* v2);
void freeArena(Arena* arena);
void printArena(const Arena* arena);