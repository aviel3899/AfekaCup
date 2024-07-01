#pragma once

#include "Team.h"

#define MIN_TEMAS_IN_CUP 2

typedef enum {
	eTeamNotSorted, eSortByTeamName, eSortByTeamCode, eSortByCoachName, eSortByBudget, eSortByFreeBudget, eSortByArenaName, eNofTeamSorts
} eTeamSort;

static const char* TeamSort[eNofTeamSorts]
= { "The search cannot be performed, array not sorted", "Team's name", "Team's code", "Coach's name", "Team's budget", "Team's free budget", "Team's arena name" };

typedef struct
{
	Team** teams;
	int numOfTeams;
	eTeamSort sort;
}TeamManager;

int initTeamManager(TeamManager* tManager, PlayerManager* pManager, const char* textFile, const char* binaryFile);
int saveTeamManagerToTextFile(const char* fileName, const TeamManager* tManager);
int saveTeamManagerToBinaryFile(const char* fileName, const TeamManager* tManager);
int setTeamsList(TeamManager* tManager, PlayerManager* pManager);
int	addTeam(TeamManager* tManager, PlayerManager* pManager);
int setTeam(Team* team, TeamManager* tManager);
int findTeam(TeamManager* tManager, Team* team, int (*compare)(void* v1, void* v2));
void sortTeam(TeamManager* tManager);
void FindTeamFromMenu(TeamManager* tManager);
void freeTeamsList(TeamManager* tManager);
void printTeamsList(const TeamManager* tManager, int printOnlyNotChosenTeams);
int TeamManagerMenu();
void TeamManagerFunc(TeamManager* tManager, PlayerManager* pManager);