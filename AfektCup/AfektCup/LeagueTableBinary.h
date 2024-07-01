#pragma once

#include "LeagueTable.h"

int	readLeagueTableFromBinaryFile(LeagueTable* lt, PlayerManager* pManager, const char* fileName);
int readTeamForLeagueTableFromBinaryFile(FILE* fp, Team* team, PlayerManager* pManager);
int readTeamSquadFromBinaryFile(FILE* fp, Team* team, PlayerManager* pManager);
int writeLeagueTableToBinaryFile(const char* fileName, const LeagueTable* lt);
int writeTeamFromLeagueToBinaryFile(Team* team, FILE* fp);