#pragma once

#include "Player.h"

#define MIN_PLAYERS_IN_CUP 100
#define EXIT -1

typedef enum {
	ePlayerNotSorted, eSortByPlayerName, eSortByPlayerID, eSortByPlayerNation, eSortByPlayerPosition, eSortByPlayerNumber, eSortByPlayerSalary, eSortByPlayerPoints, eSortByPlayerRebounds, eSortByPlayerAssits, eSortByPlayerTeam, eNofPlayerSorts
} ePlayerSort;

static const char* PlayerSort[eNofPlayerSorts]
= { "The search cannot be performed, array not sorted", "Player name", "Player ID", "Player nation", "Player position", "Player number", "Player salary", "Player points", "Player rebounds", "Player assists", "Player team"};

typedef struct {
	Player** allPlayers;
	int numOfPlayers;
	ePlayerSort sort;
}PlayerManager;

int initPlayerManager(PlayerManager* pManager, const char* textFile, const char* binaryFile);
int savePlayerManagerToTextFile(const char* fileName, const PlayerManager* pManager);
int savePlayerManagerToBinaryFile(const char* fileName, const PlayerManager* pManager);
int setPlayersList(PlayerManager* pManager);
int addPlayer(PlayerManager* pManager);
int setPlayer(PlayerManager* pManager, Player* player);
int findPlayer(PlayerManager* pManager, Player* player);
Player* checkIfPlayerInThePlayersList(PlayerManager* pManager, Player* player, char* msg);
void sortPlayer(PlayerManager* pManager);
void freePlayersList(PlayerManager* pManager);
void printPlayersList(const PlayerManager* pManager, void (*print)(void* v1));
int PlayerManagerMenu();
void PlayerManagerFunc(PlayerManager* pManager);