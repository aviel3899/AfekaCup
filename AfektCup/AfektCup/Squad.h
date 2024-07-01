#pragma once

#include "PlayerManager.h"
#include "List.h"

#define SQUAD_SIZE 5

typedef struct
{
	LIST playersList;
	int numOfPlayers;
	int squadExpense;
}Squad;

int initSquad(Squad* s);
int addPlayerToSquad(Squad* squad, PlayerManager* pManager, Player* player, int freeBudget, int kindOfSet, char* teamCode);
Player* choosePlayerManually(Squad* squad, Player* player, PlayerManager* pManager, NODE* temp, int freeBudget, char* teamCode);
Player* choosePlayerRandomly(Squad* squad, Player* player, PlayerManager* pManager, NODE* temp, int freeBudget, char* teamCode);
int setPlayerInSquad(Player* addedPlayer, Player* chosenPlayer);
Player* whichPlayerToDelete(Squad* squad, char* msg);
Player* checkIdAndIfPlayerInTheTeam(Squad* squad, Player* player, NODE* temp, const char* msg);
NODE* checkIfPlayerInList(Squad* squad, Player* player, NODE* temp);
Player* deletePlayerFromSquad(Squad* s, char* msg, Player* player);
int freeSquad(Squad* s);
int printSquad(const Squad* s, int ok);