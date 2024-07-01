#pragma once
#include "Stats.h"

#define SIZE_OF_TEAM_CODE 3

typedef enum {
	ePointGuard, eShootingGuard, eSmallForward, ePowerForward, eCenter, eNofPositions
} ePlayerPosition;

static const char* PlayerPosition[eNofPositions]
= { "Point Guard", "Shooting Guard", "Small Forward", "Power Forward", "Center" };

typedef struct
{
	char* playerName;
	int id;
	int isChosen;
	char* nation;
	ePlayerPosition position;
	int playerNumber;
	int PlayerSalary;
	Stats playerStats;
	char* playerTeam;
}Player;

int initPlayer(Player* p);
void initPlayerTeam(Player* p);
int setPlayerName(Player* p);
int setPlayerTeam(Player* player);
int comparePlayerTeam(const void* v1, const void* v2);
int comparePlayerName(const void* v1, const void* v2);
int checkUniqeJerseyNumber(const void* v1, const void* v2);
int checkUniqePosition(const void* v1, const void* v2);
int comparePlayerByID(const void* v1, const void* v2);
int comparePlayerSalary(const void* v1, const void* v2);
int comparePlayerByNation(const void* v1, const void* v2);
int comparePlayerPoints(const void* v1, const void* v2);
int comparePlayerRebounds(const void* v1, const void* v2);
int comparePlayerAssists(const void* v1, const void* v2);
void setPlayerID(Player* player, int count);
int setPlayerNation(Player* p);
int checkIDIsValid();
void setPlayerNumber(Player* p);
void setPlayerStats(Player* p);
void printMenu(int* choose, const char* msg, int size, const char* (*arr));
void generalStats(Stats* s, const char* msg1, const char* typeOfStats, const char* msg2, int (*setStats)(void* v1, int v2));
void setPlayerPosition(Player* p);
void setPlayerSalary(Player* p);
void freePlayer(Player* p);
void printOnlyNotChosenPlayers(const void* v1);
void printPlayer(const void* v1);
void printPlayerStatsTabletile();