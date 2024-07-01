#pragma once

#include "TeamManager.h"
#include "PlayerManager.h"

#define MAX_TEAMS_IN_CUP 16

typedef enum {
	eManually, eRandomly, eNoOfWays
} eWhichWayToEnter;

static const char* WayOfEnter[eNoOfWays]
= { "manually", "randomly" };

typedef struct {
	Team** leagueTeams;
	int numOfLeagueTeams;
}LeagueTable;

int initLeagueTable(LeagueTable* lt, TeamManager* tManager, PlayerManager* pManager, const char* textFile, const char* binaryFile);
void updateStandings(LeagueTable* lt);
int saveLeagueTableToTextFile(const char* fileName, const LeagueTable* lt);
int saveLeagueTableToBinaryFile(const char* fileName, const LeagueTable* lt);
int setLeagueTable(LeagueTable* lt, TeamManager* tManager, PlayerManager* pManager);
int getNumOfTeamsInTheLeague(LeagueTable* lt, TeamManager* tManager);
int setLeagueTeams(LeagueTable* lt, TeamManager* tManager, int teamsCounter);
int findTeamInLeague(LeagueTable* lt, Team* team, int (*compare)(void* v1, void* v2));
int setWhichWayToEnterPlayersToTeam();
int EnterPlayersToTeam(Team* team, PlayerManager* pManager, int choice);
int setWhichWayToEnterThePlayers(int index, Team* team);
int tradePlayers(LeagueTable* lt, PlayerManager* pManager);
int chooseWhichTeamToTrade(LeagueTable* lt, Team* team);
int chooseOriginTeamTrade(LeagueTable* lt, int OriginTeamIndex);
int chooseDestTeamTrade(LeagueTable* lt, int destTeamIndex, int OriginTeamIndex);
int addPlayerToTeamAfterTrade(LeagueTable* lt, PlayerManager* pManager, int teamIndex, Player* player);
int updateStatsAndBudget(LeagueTable* lt, int teamIndex);
Player* chooseWhichPlayerToTradeFromDestTeam(LeagueTable* lt, Player* firstPlayerToTrade, int destTeamIndex, int OriginTeamIndex, char* msg);
int chooseWhichTeamToTrade(LeagueTable* lt, Team* team);
int LeagueRound(LeagueTable* lt, PlayerManager* pManager);
int checkIfTeamWasChosenForMatch(LeagueTable* lt);
void freeLegueTable(LeagueTable* lt);
void printTableStatsTabletile();
void printPlayersListInLeague(const LeagueTable* lt);
void printLeagueTable(const LeagueTable* lt);
void printSpecificTeamInTheLeague(LeagueTable* lt);
void freeAll(PlayerManager* pManager, TeamManager* tManager, LeagueTable* LManager);
int LeagueMenu();
void LeagueFunc(PlayerManager* pManager, TeamManager* tManager, LeagueTable* LManager);