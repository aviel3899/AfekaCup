#pragma once

#include <stdio.h>
#include "PlayerManager.h"

int	readPlayerFromTextFile(FILE* fp, Player* player);
int	readPlayerManagerFromTextFile(PlayerManager* pManager, const char* fileName);
int writePlayerManagerToTextFile(const char* fileName, const PlayerManager* players);
void writePlayerToTextFile(FILE* fp, const Player* player);
void writePlayerStatsToTextFile(FILE* fp, const Stats* playerStats);