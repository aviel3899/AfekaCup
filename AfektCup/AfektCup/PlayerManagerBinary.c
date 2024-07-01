#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "PlayerManagerBinary.h"
#include "General.h"

int	readPlayerManagerFromBinaryFile(PlayerManager* pManager, const char* fileName)
{
	FILE* fp = fopen(fileName, "rb");
	if (!fp)
		return 0;
	if (fread(&pManager->numOfPlayers, sizeof(int), 1, fp) != 1) {
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
		if (readPlayerFromBinaryFile(fp, player) == 0)
		{
			freePlayer(player);
			free(player);
			free(pManager->allPlayers);
			fclose(fp);
			return 0;
		}
		initIsChosenAndPlayerTeam(player);
		pManager->allPlayers[i] = player;
	}
	fclose(fp);
	return 1;
}

int readPlayerFromBinaryFile(FILE* fp, Player* player)
{
	char* playerName = readNameFromBFile(fp);
	player->playerName = _strdup(playerName);
	if (!player->playerName)
	{
		freePlayerNameAndPlayerNation(fp, playerName, NULL);
		return 0;
	}
	if (fread(&player->id, sizeof(int), 1, fp) != 1)
	{
		freePlayerNameAndPlayerNation(fp, playerName, NULL);
		return 0;
	}
	char* playerNation = readNameFromBFile(fp);
	player->nation = _strdup(playerNation);
	if (!player->nation)
	{
		freePlayerNameAndPlayerNation(fp, playerName, playerNation);
		return 0;
	}
	if (fread(&player->position, sizeof(int), 1, fp) != 1)
	{
		freePlayerNameAndPlayerNation(fp, playerName, playerNation);
		return 0;
	}
	/*player->position -= 1;*/
	if (fread(&player->playerNumber, sizeof(int), 1, fp) != 1)
	{
		freePlayerNameAndPlayerNation(fp, playerName, playerNation);
		return 0;
	}
	if (fread(&player->PlayerSalary, sizeof(int), 1, fp) != 1)
	{
		freePlayerNameAndPlayerNation(fp, playerName, playerNation);
		return 0;
	}
	if (!readPlayerStatsFromBinaryFile(fp, &player->playerStats))
	{
		freePlayerNameAndPlayerNation(fp, playerName, playerNation);
		return 0;
	}
	player->isChosen = 1;
	return  1;
}

void initIsChosenAndPlayerTeam(Player* player)
{
	player->isChosen = 0;
	player->playerTeam = _strdup("----");
}

int readPlayerStatsFromBinaryFile(FILE* fp, Stats* stats)
{
	if (fread(&stats->points, sizeof(int), 1, fp) != 1)
		return 0;
	if (fread(&stats->rebounds, sizeof(int), 1, fp) != 1)
		return 0;
	if (fread(&stats->assists, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

int writePlayerManagerToBinaryFile(const PlayerManager* pManager, const char* fileName)
{
	FILE* fp = fopen(fileName, "wb");
	if (!fp)
		return 0;
	if (fwrite(&pManager->numOfPlayers, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return 0;
	}
	for (int i = 0; i < pManager->numOfPlayers; i++)
	{
		if (!writePlayerToBinaryFile(pManager->allPlayers[i], fp))
		{
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
}

int writePlayerToBinaryFile(const Player* player, FILE* fp)
{
	if(!WritePlayerNameToBinaryFile(fp, player))
		return 0;
	if (fwrite(&player->id, sizeof(int), 1, fp) != 1)
		return 0;
	if (!WritePlayerNationToBinaryFile(fp, player))
		return 0;
	if (fwrite(&player->position, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(&player->playerNumber, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(&player->PlayerSalary, sizeof(int), 1, fp) != 1)
		return 0;
	if (!WritePlayerStatsToBinaryFile(fp, &player->playerStats))
		return 0;
	return 1;
}

int WritePlayerNameToBinaryFile(FILE* fp, const Player* player)
{
	int StringCount = (int)strlen(player->playerName) + 1;
	if (fwrite(&StringCount, sizeof(int), 1, fp) != 1)
		return 0;
	char* temp = _strdup(player->playerName);
	if (!temp)
		return 0;
	if (fwrite(temp, sizeof(char), StringCount, fp) != StringCount)
	{
		free(temp);
		return 0;
	}
	free(temp);
	return 1;
}

int WritePlayerNationToBinaryFile(FILE* fp, const Player* player)
{
	int StringCount = (int)strlen(player->nation) + 1;
	if (fwrite(&StringCount, sizeof(int), 1, fp) != 1)
		return 0;
	char* temp = _strdup(player->nation);
	if (!temp)
		return 0;
	if (fwrite(temp, sizeof(char), StringCount, fp) != StringCount)
	{
		free(temp);
		return 0;
	}
	free(temp);
	return 1;
}

int WritePlayerStatsToBinaryFile(FILE* fp, const Stats* stats)
{
	if (fwrite(&stats->points, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(&stats->rebounds, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(&stats->assists, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

void freePlayerNameAndPlayerNation(FILE* fp, char* playerName, char* playerNation)
{
	free(playerName);
	if(playerNation)
		free(playerNation);
}