#include <stdio.h>
#include <stdlib.h>
#include "PlayerManager.h"
#include "PlayerManagerText.h"
#include "PlayerManagerBinary.h"

typedef enum
{
	ePrintPlayersList, eSortPlayers, eAddPlayer, eNofPlayerOptions
} ePlayerManagerOptions;

const char* PM[eNofPlayerOptions] = { "print players list", "sort players", "add player" };

int initPlayerManager(PlayerManager* pManager, const char* textFile, const char* binaryFile)
{
	int choice, flag = 0;
	printf("How would you like to upload the player manager file?\n");
	printf("1) From text file\n");
	printf("2) From binary file\n");
	do {
		if (flag != 0)
			printf("Worng input, enter again\n");
		scanf("%d", &choice);
		flag++;
	} while (choice < 1 || choice > 2);
	if (choice == 1)
	{
		if (readPlayerManagerFromTextFile(pManager, textFile) == 0)
		{
			printf("Couldn't read the player's list from text file, set Player's list manualy\n");
			if (!setPlayersList(pManager))
				return 0;
		}
	}
	else
		if (readPlayerManagerFromBinaryFile(pManager, binaryFile) == 0)
		{
			printf("Couldn't read the player's list from binary file, set Player's list manualy\n");
			if (!setPlayersList(pManager))
				return 0;
		}
	pManager->sort = 0;
	return 1;
}

int savePlayerManagerToTextFile(const char* fileName, const PlayerManager* pManager)
{
	if (!writePlayerManagerToTextFile(fileName, pManager))
	{
		printf("Error saving the player manager to text file\n");
		return 0;
	}
	return 1;
}

int savePlayerManagerToBinaryFile(const char* fileName, const PlayerManager* pManager)
{
	if (!writePlayerManagerToBinaryFile(pManager, fileName))
	{
		printf("Error saving the player manager to binary file\n");
		return 0;
	}
	return 1;
}

int setPlayersList(PlayerManager* pManager)
{
	int playersCounter;
	printf("How many players would you like in your list?\n");
	while (1)
	{
		scanf("%d", &playersCounter);
		if (playersCounter >= MIN_PLAYERS_IN_CUP)
			break;
		printf("List of players must include at least %d players, enter again\n", MIN_PLAYERS_IN_CUP);
	}
	pManager->allPlayers = (Player**)malloc(playersCounter * sizeof(Player*));
	if (!pManager->allPlayers)
		return 0;
	for (int i = 0; i < playersCounter; i++)
	{
		if (!addPlayer(pManager))
			return 0;
	}
	return 1;
}

int addPlayer(PlayerManager* pManager)
{
	if (pManager->numOfPlayers == 0)
	{
		pManager->allPlayers = (Player**)malloc(sizeof(Player*));
		if (!pManager->allPlayers)
			return 0;
	}
	else
	{
		pManager->allPlayers = (Player**)realloc(pManager->allPlayers, (pManager->numOfPlayers + 1) * sizeof(Player*));
		if (!pManager->allPlayers)
		{
			freePlayersList(pManager);
			return 0;
		}
	}
	Player* player = (Player*)malloc(sizeof(Player));
	if (!player)
	{
		free(pManager->allPlayers[pManager->numOfPlayers]);
		return 0;
	}
	if (!setPlayer(pManager, player))
	{
		free(player);
		free(pManager->allPlayers[pManager->numOfPlayers]);
		return 0;
	}
	pManager->allPlayers[pManager->numOfPlayers] = player;
	pManager->numOfPlayers++;
	return 1;
}

int setPlayer(PlayerManager* pManager, Player* player)
{
	int count = 0;
	initPlayer(player);
	if (!setPlayerName(player))
		return 0;
	if (!setPlayerNation(player))
		return 0;
	while (1)
	{
		setPlayerID(player, count);
		if (findPlayer(pManager, player) == -1)
			break;
		printf("There is already a player with this id, enter again\n");
		count++;
	}
	setPlayerNumber(player);
	setPlayerPosition(player);
	setPlayerSalary(player);
	if (!setPlayerTeam(player))
		return 0;
	return 1;
}

int findPlayer(PlayerManager* pManager, Player* player)
{
	int flag = -1;
	for (int i = 0; i < pManager->numOfPlayers; i++)
	{
		if (comparePlayerByID(&player, &pManager->allPlayers[i]) == 0)
		{
			flag = i;
			break;
		}
	}
	return flag;
}

Player* checkIfPlayerInThePlayersList(PlayerManager* pManager, Player* player, char* msg)
{
	int chosenID, temp;
	printf("%s", msg);
	chosenID = checkIDIsValid();
	player->id = chosenID;
	temp = findPlayer(pManager, player);
	while (temp == -1)
	{
		printf("A player with this id doesn't exsist in the players list, enter again\n");
		chosenID = checkIDIsValid();
		player->id = chosenID;
		temp = findPlayer(pManager, player);
	}
	Player* chosenPlayer = pManager->allPlayers[temp];
	return chosenPlayer;
}


void sortPlayer(PlayerManager* pManager)
{
	int choice;
	printf("\nBase on what field do you want to sort?\n");
	do {
		for (int i = 1; i < eNofPlayerSorts; i++)
			printf(" Enter %d for %s\n", i, PlayerSort[i]);
		scanf("%d", &choice);
		switch (choice)
		{
		case (eSortByPlayerName):
			qsort(pManager->allPlayers, pManager->numOfPlayers, sizeof(Player**), comparePlayerName);
			pManager->sort = 1;
			break;
		case (eSortByPlayerID):
			qsort(pManager->allPlayers, pManager->numOfPlayers, sizeof(Player**), comparePlayerByID);
			pManager->sort = 2;
			break;
		case (eSortByPlayerNation):
			qsort(pManager->allPlayers, pManager->numOfPlayers, sizeof(Player**), comparePlayerByNation);
			pManager->sort = 3;
			break;
		case (eSortByPlayerPosition):
			qsort(pManager->allPlayers, pManager->numOfPlayers, sizeof(Player**), checkUniqePosition);
			pManager->sort = 4;
			break;
		case (eSortByPlayerNumber):
			qsort(pManager->allPlayers, pManager->numOfPlayers, sizeof(Player**), checkUniqeJerseyNumber);
			pManager->sort = 5;
			break;
		case (eSortByPlayerSalary):
			qsort(pManager->allPlayers, pManager->numOfPlayers, sizeof(Player**), comparePlayerSalary);
			pManager->sort = 6;
			break;
		case (eSortByPlayerPoints):
			qsort(pManager->allPlayers, pManager->numOfPlayers, sizeof(Player**), comparePlayerPoints);
			pManager->sort = 7;
			break;
		case (eSortByPlayerRebounds):
			qsort(pManager->allPlayers, pManager->numOfPlayers, sizeof(Player**), comparePlayerRebounds);
			pManager->sort = 8;
			break;
		case (eSortByPlayerAssits):
			qsort(pManager->allPlayers, pManager->numOfPlayers, sizeof(Player**), comparePlayerAssists);
			pManager->sort = 9;
			break;
		case (eSortByPlayerTeam):
			qsort(pManager->allPlayers, pManager->numOfPlayers, sizeof(Player**), comparePlayerTeam);
			pManager->sort = 9;
			break;
		default:
			printf("Wrong option\n");
			break;
		}
	} while (choice < 0 || choice >= eNofPlayerSorts);
}


void freePlayersList(PlayerManager* pManager)
{
	for (int i = 0; i < pManager->numOfPlayers; i++)
	{
		freePlayer(pManager->allPlayers[i]);
		free(pManager->allPlayers[i]);
	}
	free(pManager->allPlayers);
}

void printPlayersList(const PlayerManager* pManager, void (*print)(void* v1))
{
	printf("\nPlayers list:\n");
	printPlayerStatsTabletile();
	for (int i = 0; i < pManager->numOfPlayers; i++)
	{
		print(pManager->allPlayers[i]);
	}
}

int PlayerManagerMenu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofPlayerOptions; i++)
		printf("%d - %s\n", i, PM[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

void PlayerManagerFunc(PlayerManager* pManager)
{
	int option;
	int stop = 0;
	do
	{
		option = PlayerManagerMenu();
		switch (option)
		{

		case ePrintPlayersList:
			printPlayersList(pManager, printPlayer);
			break;

		case eSortPlayers:
			sortPlayer(pManager);
			break;

		case eAddPlayer:
			if (!addPlayer(pManager))
				printf("Error adding player\n");
			break;

		case EXIT:
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);
}