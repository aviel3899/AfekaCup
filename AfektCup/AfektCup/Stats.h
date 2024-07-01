#pragma once

typedef enum {
	ePoints, eRebounds, eAssists, eNotUpdate, eNofStatsTypes
} eStats;

static const char* stats[eNofStatsTypes]
= { "points", "rebounds", "assits", "Don't update"};

typedef struct
{
	int points;
	int rebounds;
	int assists;
}Stats;

void initStats(Stats* s);
int SetPoints(Stats* s, int x);
int SetRebounds(Stats* s, int x);
int SetAssists(Stats* s, int x);
void setStatsRandomly(Stats* stats);
int compareAllStats(const void* v1, const void* v2);
int comparePoints(const void* v1, const void* v2);
int compareRebounds(const void* v1, const void* v2);
int compareAssists(const void* v1, const void* v2);
void printStats(const Stats* s);
void printStatsTitle();