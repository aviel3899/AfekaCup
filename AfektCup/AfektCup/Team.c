#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Team.h"
#include "General.h"

#define MIN_TEAM_BUDGET 5000000

int initTeam(Team* team)
{
	team->TeamName = NULL;
	if (!initSquad(&team->players))
		return 0;
	initStats(&team->teamStats);
	initArena(&team->homeArena);
	initCoach(&team->headCoach);
	team->budget = 0;
	team->freeBudget = 0;
	team->currentExpense = 0;
	team->place = 0;
	team->isChosenForLeague = 0;
	team->isChosenForMatch = 0;
	return 1;
}

int setTeamName(Team* team, char* msg)
{
	team->TeamName = getStrExactName(msg);
	if (!team->TeamName)
		return 0;
	return 1;
}

int compareTeamName(const void* v1, const void* v2)
{
	const Team* t1 = *(const Team**)v1;
	const Team* t2 = *(const Team**)v2;
	return (strcmp(t1->TeamName, t2->TeamName));
}

int isTeamName(const void* v1, const void* v2) {
	const char* name = (char*)v1;
	const Team* pT = *(const Team**)v2;
	return strcmp(name, pT->TeamName);
}

int isTeamCode(const void* v1, const void* v2) {
	const char* code = (char*)v1;
	const Team* pT = *(const Team**)v2;
	return strcmp(code, pT->teamCode);
}

int isCoachName(const void* v1, const void* v2) {
	const char* name = (char*)v1;
	const Team* pT = *(const Team**)v2;
	return strcmp(name, pT->headCoach.CoachName);
}

int isArenaName(const void* v1, const void* v2) {
	const char* name = (char*)v1;
	const Team* pT = *(const Team**)v2;
	return strcmp(name, pT->homeArena.ArenaName);
}

void getTeamCode(char* code) 
{
	int ok = 0;
	do {
		ok = 0;
		do {
			fgets(code, sizeof(code), stdin);
		} while (isspace(*code));
		for (int i = 0; i < SIZE_OF_TEAM_CODE; i++)
		{
			if (!isupper(code[i]))
			{
				printf("Need to be upper case letter, enter again\n");
				ok = 1;
				break;
			}
		}
		if (strlen(code) - 1 != SIZE_OF_TEAM_CODE)
		{
			printf("code should be %d letters, enter again\n", SIZE_OF_TEAM_CODE);
			ok = 1;
		}
	} while (ok == 1);
	code[SIZE_OF_TEAM_CODE] = '\0';
}

int compareTeamCode(const void* v1, const void* v2)
{
	const Team* t1 = *(const Team**)v1;
	const Team* t2 = *(const Team**)v2;
	return strcmp(t1->teamCode, t2->teamCode);
}

int setTeamSquad(Team* team, PlayerManager* pManager, int kindOfSet)
{
	int playerSalary;
	for (int i = 0; i < SQUAD_SIZE; i++)
	{
		playerSalary = addPlayerToSquad(&team->players, pManager, NULL, team->freeBudget, kindOfSet, team->teamCode);
		if (!playerSalary)
			return 0;
		team->currentExpense += playerSalary;
		team->freeBudget -= playerSalary;
	}
	return 1;
}

int setTeamCoach(Team* team)
{
	if (!SetCoachName(&team->headCoach))
		return 0;
	SetCoachSalary(&team->headCoach, team->freeBudget);
	team->freeBudget -= team->headCoach.CoachSalary;
	SetCoachYearsOfExperience(&team->headCoach);
	return 1;
}

int compareTeamByCoachName(const void* v1, const void* v2)
{
	Team* t1 = *(Team**)v1;
	Team* t2 = *(Team**)v2;
	return compareCoachName(&t1->headCoach, &t2->headCoach);
}

int setTeamStats(Team* team)
{
	NODE* ptr = team->players.playersList.head.next;
	if (!ptr)
		return 0;
	while (ptr)
	{
		Player* p = (Player*)ptr->key;
		team->teamStats.points += p->playerStats.points;
		team->teamStats.rebounds += p->playerStats.rebounds;
		team->teamStats.assists += p->playerStats.assists;
		ptr = ptr->next;
	}
	return 1;
}

int compareTeamByPoints(const void* v1, const void* v2)
{
	Team* t1 = *(Team**)v1;
	Team* t2 = *(Team**)v2;
	return comparePoints(&t1->teamStats.points, &t2->teamStats.points);
}

int compareTeamByPointsFomBigToSmall(const void* v1, const void* v2)
{
	Team* t1 = *(Team**)v1;
	Team* t2 = *(Team**)v2;
	return comparePoints(&t2->teamStats.points, &t1->teamStats.points);
}

int compareTeamByRebouns(const void* v1, const void* v2)
{
	Team* t1 = *(Team**)v1;
	Team* t2 = *(Team**)v2;
	return compareRebounds(&t1->teamStats.rebounds, &t2->teamStats.rebounds);
}

int compareTeamByAssists(const void* v1, const void* v2)
{
	Team* t1 = *(Team**)v1;
	Team* t2 = *(Team**)v2;
	return compareAssists(&t1->teamStats.assists, &t2->teamStats.assists);
}

int compareTeamByArenaName(const void* v1, const void* v2)
{
	Team* t1 = *(Team**)v1;
	Team* t2 = *(Team**)v2;
	return compareArenaName(&t1->homeArena, &t2->homeArena);
}

int setTeamBudget(Team* team, int count)
{
	if (count == 0)
		printf("What is the budget of the team? must be above %d\n", MIN_TEAM_BUDGET);
	scanf("%d", &team->budget);
	if (team->budget < MIN_TEAM_BUDGET)
		return 0;
	return 1;
}

int compareTeamByBudget(const void* v1, const void* v2)
{
	Team* t1 = *(Team**)v1;
	Team* t2 = *(Team**)v2;
	return (t1->budget - t2->budget);
}

int compareTeamFreeByBudget(const void* v1, const void* v2)
{
	Team* t1 = *(Team**)v1;
	Team* t2 = *(Team**)v2;
	return (t1->freeBudget - t2->freeBudget);
}

void updateTeamAllBudgets(Team* team, int squadExpence)
{
	team->currentExpense = squadExpence + team->headCoach.CoachSalary;
	team->freeBudget = team->budget - team->currentExpense;
}

void freeTeam(Team* team)
{
	free(team->TeamName);
	freeSquad(&team->players);
	freeCoach(&team->headCoach);
	freeArena(&team->homeArena);
}

void printTeamStatsTabletile()
{
	printf("Name%-21s", "");
	printf("code%-2s", "");
	printf("Coach%-15s", "");
	printf("budget%-9s", "");
	printf("free budget%-9s", "");
	printf("Arena%-20s", "");
	printf("\n");
}

void printTeamGeneralTable(const void* v1)
{
	const Team* t = (Team*)v1;
	printf("\n");
	printf("%-25s", t->TeamName);
	printf("%-6s", t->teamCode);
	printf("%-20s", t->headCoach.CoachName);
	printf("%-15d", t->budget);
	printf("%-20d", t->freeBudget);
	printf("%-25s", t->homeArena.ArenaName);
	printf("\n");
}

void printTeamForLeagueTable(const void* v1)
{
	const Team* t = (Team*)v1;
	printf("\n");
	printf("%  -6d", t->place);
	printf("%-25s", t->TeamName);
	printf("%-6s", t->teamCode);
	printStats(&t->teamStats);
	printf("\n");
}

void printAllTeamParameters(const Team* team)
{
	char del = '-';
	printDelimeters(100, del);
	printf("\nTeam's name: %s\n", team->TeamName);
	printf("Team's name code: %s\n", team->teamCode);
	printDelimeters(25, del);
	printf("\n");
	printf("\nTeam's budget: %d$\n", team->budget);
	printf("Team's current expense: %d$\n", team->currentExpense);
	printf("Team's free budget: %d$\n", team->freeBudget);
	printDelimeters(25, del);
	printf("\n");
	printf("Squad: \n\n");
	printSquad(&team->players, 0);
	printDelimeters(25, del);
	printf("\n");
	printCoach(&team->headCoach);
	printf("\n");
	printDelimeters(25, del);
	printf("\n");
	printf("Team stats:\n");
	printStatsTitle();
	printf("\n");
	printStats(&team->teamStats);
	printf("\n");
	printDelimeters(25, del);
	printf("\n");
	printArena(&team->homeArena);
}

void printDelimeters(int size, const char delimeter)
{
	for (int i = 0; i < size; i++)
	{
		printf("%c", delimeter);
	}
}