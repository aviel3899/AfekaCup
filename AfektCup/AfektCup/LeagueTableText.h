#pragma once

#include "LeagueTable.h"

int readLeagueTableFromTextFile(LeagueTable* lt, PlayerManager* pManager, const char* fileName);
int readTeamForLeagueTableFromTextFile(FILE* fp, Team* team, PlayerManager* pManager);
int readPlayersListForTeamInLeague(FILE* fp, Team* team, PlayerManager* pManager);
int writeLeagueTableToTextFile(const char* fileName, const LeagueTable* lt);
int writeTeamFromLeagueToTextFile(FILE* fp, Team* team);
