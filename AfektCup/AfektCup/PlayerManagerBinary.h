#pragma once

#include "PlayerManager.h"

int	readPlayerManagerFromBinaryFile(PlayerManager* pManager, const char* fileName);
int readPlayerFromBinaryFile(FILE* fp, Player* player);
void initIsChosenAndPlayerTeam(Player* player);
int readPlayerStatsFromBinaryFile(FILE* fp, Stats* stats);
int writePlayerManagerToBinaryFile(const PlayerManager* pManager, const char* fileName);
int writePlayerToBinaryFile(const Player* player, FILE* fp);
int WritePlayerNameToBinaryFile(FILE* fp, const Player* player);
int WritePlayerNationToBinaryFile(FILE* fp, const Player* player);
int WritePlayerStatsToBinaryFile(FILE* fp, const Stats* stats);
void freePlayerNameAndPlayerNation(FILE* fp, char* playerName, char* playerNation);