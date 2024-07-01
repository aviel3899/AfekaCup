#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Player.h"
#include "General.h"

#define ID_NUM_OF_DIGITS 5

int initPlayer(Player* p)
{
	p->playerName = NULL;
	p->id = 0;
	p->isChosen = 0;
	p->nation = NULL;
	p->playerNumber = 0;
	initStats(&p->playerStats);
	p->position = 0;
	p->PlayerSalary = 0;
	initPlayerTeam(p);
	return 1;
}

void initPlayerTeam(Player* p)
{
	strcpy(p->playerTeam, "----");
}

int setPlayerTeam(Player* player)
{
	char* msg = {"---"};
	player->playerTeam = _strdup(msg);
	if (!player->playerTeam)
		return 0;
	return 1;
}

int comparePlayerTeam(const void* v1, const void* v2)
{
	const Player* p1 = *(const Player**)v1;
	const Player* p2 = *(const Player**)v2;
	return strcmp(p2->playerTeam, p1->playerTeam);
}

int setPlayerName(Player* p)
{
	p->playerName = getStrExactName("What is the name of the player?\n");
	if (!p->playerName)
		return 0;
	return 1;
}

int comparePlayerName(const void* v1, const void* v2)
{
	const Player* p1 = *(const Player**)v1;
	const Player* p2 = *(const Player**)v2;
	return strcmp(p1->playerName, p2->playerName);
}

void setPlayerID(Player* player, int count)
{
	if(count == 0)
		printf("Enter player's id, must be a %d digit number\n", ID_NUM_OF_DIGITS);
	player->id = checkIDIsValid();
}

int checkIDIsValid()
{
	int ans, count, temp, flag = 0;
	do {
		if (flag != 0)
			printf("id must be a %d digit number, enter again\n", ID_NUM_OF_DIGITS);
		count = 0;
		scanf("%d", &ans);
		temp = ans;
		while (ans != 0)
		{
			count++;
			ans = ans / 10;
		}
		flag = 1;
	} while (count != ID_NUM_OF_DIGITS);
	return temp;
}

int comparePlayerByID(const void* v1, const void* v2)
{
	const Player* p1 = *(const Player**)v1;
	const Player* p2 = *(const Player**)v2;
	return (p1->id - p2->id);
}

int setPlayerNation(Player* p)
{
	p->nation = getStrExactName("What is the nation of the player?\n");
	if (!p->nation)
		return 0;
	return 1;
}

int comparePlayerByNation(const void* v1, const void* v2)
{
	const Player* p1 = *(const Player**)v1;
	const Player* p2 = *(const Player**)v2;
	return strcmp(p1->nation, p2->nation);
}

void setPlayerNumber(Player* p)
{
	int jerseyNumber, count = 0;;
	do {
		if (count != 0)
			printf("Jersey number must be a positive number and maximum 99\n");
		printf("What is the player's jersey number?\n");
		scanf("%d", &jerseyNumber);
		count++;
	} while (jerseyNumber < 0 || jerseyNumber > 99);
	p->playerNumber = jerseyNumber;
}

int checkUniqeJerseyNumber(const void* v1, const void* v2)
{
	const Player* p1 = *(const Player**)v1;
	const Player* p2 = *(const Player**)v2;
	return p1->playerNumber - p2->playerNumber;
}

void setPlayerStats(Player* p)
{
	int choose, flag = 0;
	char* msg1 = "How many ";
	char* msg2 = "would you like to update?\n";
	printMenu(&choose, "Which stat would you like to update?\n", eNofStatsTypes, stats);
	while(flag == 0)
	{
		switch (choose - 1)
		{
		case ePoints:
			generalStats(&p->playerStats, msg1, stats[ePoints], msg2, SetPoints);
			flag++;
			break;
		case eRebounds:
			generalStats(&p->playerStats, msg1, stats[eRebounds], msg2, SetRebounds);
			flag++;
			break;
		case eAssists:
			generalStats(&p->playerStats, msg1, stats[eAssists], msg2, SetAssists);
			flag++;
			break;
		case eNotUpdate:
			flag++;
			break;
		default:
			printf("incorrent input, enter again\n");
			scanf("%d", &choose);
			break;
		}
	} 
}

void printMenu(int* choose, const char* msg, int size, const char* (*arr))
{
	printf("%s", msg);
	for (int i = 0; i < size; i++)
		printf("%d) %s\n", i + 1, arr[i]);
	scanf("%d", choose);
}

void generalStats(Stats* s, const char* msg1, const char* typeOfStats, const char* msg2, int (*setStats)(void* v1, int v2))
{
	int x;
	printf("%s", msg1);
	printf("%s ", typeOfStats);
	printf("%s", msg2);
	scanf("%d", &x);
	while (!setStats(s, x))
		printf("%s must be a positie number, enter again\n", typeOfStats);
}

void setPlayerPosition(Player* p)
{
	int choose;
	printMenu(&choose, "What is the player's position on the court?\n", eNofPositions, PlayerPosition);
	while (choose <= 0 || choose > 5)
	{
		printf("incorrent input, enter again\n");
		scanf("%d", &choose);
	}
	p->position = choose-1;
}

int checkUniqePosition(const void* v1, const void* v2)
{
	const Player* p1 = *(const Player**)v1;
	const Player* p2 = *(const Player**)v2;
	return (p1->position - p2->position);
}

void setPlayerSalary(Player* p)
{
	int salary;
	do {
		printf("What is the player's salary?\n");
		scanf("%d", &salary);
	} while (salary <= 0);
	p->PlayerSalary = salary;
}

int comparePlayerSalary(const void* v1, const void* v2)
{
	const Player* p1 = *(const Player**)v1;
	const Player* p2 = *(const Player**)v2;
	return (p1->PlayerSalary - p2->PlayerSalary);
}

int comparePlayerPoints(const void* v1, const void* v2) 
{
	const Player* p1 = *(const Player**)v1;
	const Player* p2 = *(const Player**)v2;
	return comparePoints(&p1->playerStats, &p2->playerStats);
}

int comparePlayerRebounds(const void* v1, const void* v2)
{
	const Player* p1 = *(const Player**)v1;
	const Player* p2 = *(const Player**)v2;
	return compareRebounds(&p1->playerStats, &p2->playerStats);
}

int comparePlayerAssists(const void* v1, const void* v2)
{
	const Player* p1 = *(const Player**)v1;
	const Player* p2 = *(const Player**)v2;
	return compareAssists(&p1->playerStats, &p2->playerStats);
}

void freePlayer(Player* p)
{
	free(p->playerName);
	free(p->nation);
}

void printOnlyNotChosenPlayers(const void* v1)
{
	const Player* p = (Player*)v1;
	if (p->isChosen == 1)
		return;
	printPlayer(p);
}

void printPlayer(const void* v1)
{
	const Player* p = (Player*)v1;
	printf("\n");
	printf("%-22s", p->playerName);
	printf("%-12d", p->id);
	printf("%-18s", p->nation);
	printf("     %-10d", p->playerNumber);
	printf("%-17s", PlayerPosition[p->position]);
	printf("%-15d", p->PlayerSalary);
	printStats(&p->playerStats);
	printf("%-4s", p->playerTeam);
	printf("\n");
}

void printPlayerStatsTabletile()
{
	printf("Name%-18s", "");
	printf("ID%-10s", "");
	printf("Nation%-12s", "");
	printf("Jersey Number%-2s", "");
	printf("Position%-9s", "");
	printf("Salary%-9s", "");
	printStatsTitle();
	printf("Team\n");
}



