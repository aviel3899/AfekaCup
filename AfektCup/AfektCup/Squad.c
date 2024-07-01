#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Squad.h"

int initSquad(Squad* s)
{
	if (!L_init(&s->playersList))
		return 0;
	s->numOfPlayers = 0;
	s->squadExpense = 0;
	return 1;
}

int addPlayerToSquad(Squad* squad, PlayerManager* pManager, Player* player, int freeBudget, int kindOfSet, char* teamCode)
{
	NODE* ptr = &squad->playersList.head;
	NODE* temp = NULL;
	if (!player)
	{
		player = (Player*)malloc(sizeof(Player));
		if (!player)
			return 0;
		if (kindOfSet == 0)
			player = choosePlayerManually(squad, player, pManager, temp, freeBudget, teamCode);
		else
			player = choosePlayerRandomly(squad, player, pManager, temp, freeBudget, teamCode);
		if (!player)
		{
			free(player);
			return 0;
		}
	}
	ptr = L_insert(ptr, player);
	if (!ptr)
	{
		free(player);
		return 0;
	}
	squad->numOfPlayers++;
	squad->squadExpense += player->PlayerSalary;
	return player->PlayerSalary;
}

Player* choosePlayerManually(Squad* squad, Player* player, PlayerManager* pManager, NODE* temp, int freeBudget, char* teamCode)
{
	int playerIndex;
	printPlayersList(pManager, printOnlyNotChosenPlayers);
	Player* chosenPlayer = checkIfPlayerInThePlayersList(pManager, player, "\nWhich player would you like to add to your team?, enter his id\n");
	while (chosenPlayer->PlayerSalary > freeBudget)
		chosenPlayer = checkIfPlayerInThePlayersList(pManager, player, "This player's salary exceeds the team's budget, enter again\n");
	while(chosenPlayer->isChosen == 1)
		chosenPlayer = checkIfPlayerInThePlayersList(pManager, player, "This player was already chosen for other team, enter again\n");
	/*while (1)
	{
		if (!L_find(squad->playersList.head.next, chosenPlayer, checkUniqePosition))
			break;
		chosenPlayer = checkIdAndIfPlayerInTheTeam(squad, player, temp, "A player with this position already exsists in the team, enter again\n");
	}*/
	playerIndex = findPlayer(pManager, chosenPlayer);
	strcpy(pManager->allPlayers[playerIndex]->playerTeam, teamCode);
	strcpy(chosenPlayer->playerTeam, teamCode);
	chosenPlayer->isChosen = 1;
	if (!setPlayerInSquad(player, chosenPlayer))
		return NULL;
	return player;
}

Player* choosePlayerRandomly(Squad* squad, Player* player, PlayerManager* pManager, NODE* temp, int freeBudget, char* teamCode)
{
	srand((unsigned int)time(NULL));
	int randomIndex;
	while (1)
	{
		randomIndex = rand() % pManager->numOfPlayers;
		if (pManager->allPlayers[randomIndex]->PlayerSalary <= freeBudget)
			if (pManager->allPlayers[randomIndex]->isChosen != 1)
				/*if (!L_find(squad->playersList.head.next, &pManager->allPlayers[randomIndex], checkUniqePosition))*/
				{
					strcpy(pManager->allPlayers[randomIndex]->playerTeam, teamCode);
					break;
				}
	}
	pManager->allPlayers[randomIndex]->isChosen = 1;
	if (!setPlayerInSquad(player, pManager->allPlayers[randomIndex]))
		return NULL;
	return player;
}

int setPlayerInSquad(Player* addedPlayer, Player* chosenPlayer)
{
	if (!addedPlayer || !chosenPlayer)
		return 0;
	addedPlayer->playerName = chosenPlayer->playerName;
	addedPlayer->id = chosenPlayer->id;
	addedPlayer->isChosen = chosenPlayer->isChosen;
	addedPlayer->nation = chosenPlayer->nation;
	addedPlayer->playerNumber = chosenPlayer->playerNumber;
	addedPlayer->PlayerSalary = chosenPlayer->PlayerSalary;
	addedPlayer->playerStats = chosenPlayer->playerStats;
	addedPlayer->position = chosenPlayer->position;
	addedPlayer->playerTeam = chosenPlayer->playerTeam;
	return 1;
}

Player* whichPlayerToDelete(Squad* squad, char* msg)
{
	NODE* temp = NULL;
	Player* player = (Player*)malloc(sizeof(Player));
	if (!player)
		return 0;
	return checkIdAndIfPlayerInTheTeam(squad, player, temp, msg);
}

Player* checkIdAndIfPlayerInTheTeam(Squad* squad, Player* player, NODE* temp, const char* msg)
{
	if(msg)
		printf("%s", msg);
	temp = checkIfPlayerInList(squad, player, temp);
	return (Player*)temp->key;
}

NODE* checkIfPlayerInList(Squad* squad, Player* player, NODE* temp)
{
	int chosenID;
	while (1)
	{
		chosenID = checkIDIsValid();
		player->id = chosenID;
		temp = (NODE*)L_find(squad->playersList.head.next, &player, comparePlayerByID);
		if (temp)
			break;
		printf("A player with this id doesn't exsist in the team, enter again\n");
	}
	return temp;
}

Player* deletePlayerFromSquad(Squad* squad, char* msg, Player* player)
{
	if(!player)
		player = whichPlayerToDelete(squad, msg);
	NODE* before = &squad->playersList.head;
	NODE* after = squad->playersList.head.next;
	while (after)
	{
		if (comparePlayerByID(&after->key, &player) == 0)
			break;
		before = after;
		after = after->next;
	}
	if (!L_delete(before, NULL))
		return NULL;
	initPlayerTeam(player);
	squad->squadExpense -= player->PlayerSalary;
	squad->numOfPlayers--;
	return player;
}

//int copySquadToArray(Squad* squad, int advantage)
//{
//	Player* updatedSquad = (Player*)malloc(squad->numOfPlayers * sizeof(Player));
//	if (!updatedSquad)
//		return 0;
//	NODE* ptr = squad->playersList.head.next;
//	for (int i = 0; i < squad->numOfPlayers && ptr; i++, ptr = ptr->next)
//	{
//		Player player = *(Player*)ptr->key;
//		updatedSquad[i] = player;
//	}
//	for (int i = 0; i < squad->numOfPlayers; i++)
//	{
//
//	}
//	freeSquad(squad);
//	return 1;
//}

int freeSquad(Squad* s)
{
	if (!L_free(&s->playersList, freePlayer))
		return 0;
	return 1;
}

int printSquad(const Squad* s, int ok)
{
	if (ok == 0)
	{
		printPlayerStatsTabletile();
	}
	if (!L_print(&s->playersList, printPlayer))
		return 0;
	return 1;
}