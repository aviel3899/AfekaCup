#include <stdlib.h>
#include <string.h>
#include "PlayerManagerText.h"
#include "General.h"

int readPlayerManagerFromTextFile(PlayerManager* pManager, const char* fileName)
{
	FILE* fp = fopen(fileName, "r");
	if (!fp)
		return 0;
	if (fscanf(fp, "%d\n", &pManager->numOfPlayers) != 1) {
		fclose(fp);
		return 0;
	}
	pManager->allPlayers = (Player**)malloc(pManager->numOfPlayers * sizeof(Player*));
	if (!pManager->allPlayers)
	{
		fclose(fp);
		return 0;
	}
	for (int i = 0; i < pManager->numOfPlayers; i++)
	{
		Player* player = (Player*)malloc(sizeof(Player));
		if (!player)
		{
			free(pManager->allPlayers);
			fclose(fp);
			return 0;
		}
		if (readPlayerFromTextFile(fp, player) == 0)
		{
			free(player);
			free(pManager->allPlayers);
			fclose(fp);
			return 0;
		}
		pManager->allPlayers[i] = player;
	}
	fclose(fp);
	return 1;
}

int readPlayerFromTextFile(FILE* fp, Player* player)
{
	char name[MAX_STR_LEN] = { 0 };
	char country[MAX_STR_LEN] = { 0 };
	if (!myGets(name, MAX_STR_LEN, fp))
		return 0;
	if (fscanf(fp, "%d\n", &player->id) != 1) 
		return 0;
	if (!myGets(country, MAX_STR_LEN, fp))
		return 0;
	if (fscanf(fp, "%d\n", &player->position) != 1) 
		return 0;
	player->position -= 1;
	if (fscanf(fp, "%d\n", &player->playerNumber) != 1)
		return 0;
	if (fscanf(fp, "%d\n", &player->PlayerSalary) != 1)
		return 0;
	if (fscanf(fp, "%d\n", &player->playerStats.points) != 1)
		return 0;
	if (fscanf(fp, "%d\n", &player->playerStats.rebounds) != 1)
		return 0;
	if (fscanf(fp, "%d\n", &player->playerStats.assists) != 1)
		return 0;
	player->playerName = _strdup(name);
	if (!player->playerName)
		return 0;
	player->nation = _strdup(country);
	if (!player->nation)
		return 0;
	player->isChosen = 0;
	player->playerTeam = _strdup("----");
	if (!player->playerTeam)
		return 0;
	return  1;
}

int writePlayerManagerToTextFile(const char* fileName, const PlayerManager* players)
{
	FILE* fp = fopen(fileName, "w");
	if (!fp)
		return 0;
	fprintf(fp, "%d\n", players->numOfPlayers);
	for (int i = 0; i < players->numOfPlayers; i++)
	{
		writePlayerToTextFile(fp, players->allPlayers[i]);
	}
	fclose(fp);
	return 1;
}

void writePlayerToTextFile(FILE* fp, const Player* player)
{
	fprintf(fp, "%s\n", player->playerName);
	fprintf(fp, "%d\n", player->id);
	fprintf(fp, "%s\n", player->nation);
	fprintf(fp, "%d\n", player ->position + 1);
	fprintf(fp, "%d\n", player->playerNumber);
	fprintf(fp, "%d\n", player->PlayerSalary);
	writePlayerStatsToTextFile(fp, &player->playerStats);
}

void writePlayerStatsToTextFile(FILE* fp, const Stats* playerStats)
{
	fprintf(fp, "%d\n", playerStats->points);
	fprintf(fp, "%d\n", playerStats->rebounds);
	fprintf(fp, "%d\n", playerStats->assists);
}