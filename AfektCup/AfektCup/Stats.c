#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "Stats.h"

void initStats(Stats* s) {
	s->assists = 0;
	s->points = 0;
	s->rebounds = 0;
}

int SetPoints(Stats* s, int x) {
	if (x >= 0) {
		s->points = x;
		return 1;
	}
	return 0;
}

int SetRebounds(Stats* s, int x) {
	if (x >= 0) {
		s->rebounds = x;
		return 1;
	}
	return 0;
}

int SetAssists(Stats* s, int x) {
	if (x >= 0) {
		s->assists = x;
		return 1;
	}
	return 0;
}

void setStatsRandomly(Stats* stats)
{
	stats->points = rand() % 11 + 10; // Home team has home advantage of 2 pts per player;
	stats->rebounds = rand() % 11;
	stats->assists = rand() % 11;
}

int compareAllStats(const void* v1, const void* v2)
{
	if (comparePoints(v1, v2) != 0 || compareRebounds(v1, v2) != 0 || compareAssists(v1, v2) != 0)
		return 0;
	return 1;

}

int comparePoints(const void* v1, const void* v2)
{
	Stats* s1 = (Stats*)v1;
	Stats* s2 = (Stats*)v2;
	return s1->points - s2->points;
}

int compareRebounds(const void* v1, const void* v2)
{
	Stats* s1 = (Stats*)v1;
	Stats* s2 = (Stats*)v2;
	return s1->rebounds - s2->rebounds;
}

int compareAssists(const void* v1, const void* v2)
{
	Stats* s1 = (Stats*)v1;
	Stats* s2 = (Stats*)v2;
	return s1->assists - s2->assists;
}

void printStats(const Stats* s)
{
	printf("%-9d", s->points);
	printf("%-9d", s->rebounds);
	printf("%-9d", s->assists);
}

void printStatsTitle()
{
	printf("Points%-3s", "");
	printf("Rebounds%-1s", "");
	printf("Assists%-2s", "");
}