#pragma once

#include "Squad.h"
#include "Coach.h"
#include "Arena.h"

typedef struct
{
	int place;
	char* TeamName;
	char teamCode[SIZE_OF_TEAM_CODE + 1];
	Squad players;
	Coach headCoach;
	Stats teamStats;
	Arena homeArena;
	int currentExpense;
	int freeBudget;
	int budget;
	int isChosenForLeague;
	int isChosenForMatch;
}Team;

int initTeam(Team* team);
int setTeamName(Team* team, char* msg);
int compareTeamName(const void* v1, const void* v2);
int isTeamName(const void* v1, const void* v2);
int isTeamCode(const void* v1, const void* v2);
int isCoachName(const void* v1, const void* v2);
int isArenaName(const void* v1, const void* v2);
void getTeamCode(char* code);
int compareTeamCode(const void* v1, const void* v2);
int setTeamSquad(Team* team, PlayerManager* pManager, int kindOfSet);
int setTeamCoach(Team* team);
int compareTeamByCoachName(const void* v1, const void* v2);
int setTeamStats(Team* team);
int compareTeamByPoints(const void* v1, const void* v2);
int compareTeamByPointsFomBigToSmall(const void* v1, const void* v2);
int compareTeamByRebouns(const void* v1, const void* v2);
int compareTeamByAssists(const void* v1, const void* v2);
int compareTeamByArenaName(const void* v1, const void* v2);
int setTeamBudget(Team* team, int count);
int compareTeamByBudget(const void* v1, const void* v2);
int compareTeamFreeByBudget(const void* v1, const void* v2);
void updateTeamAllBudgets(Team* team, int squadBudget);
void freeTeam(Team* team);
void printTeamStatsTabletile();
void printTeamForLeagueTable(const void* v1);
void printTeamGeneralTable(const void* v1);
void printAllTeamParameters(const Team* team);
void printDelimeters(int size, const char delimeter);

