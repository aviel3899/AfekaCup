#include <stdio.h>
#include "TeamManager.h"
#include "PlayerManager.h"
#include "LeagueTable.h"

#define PLAYER_TEXT_FILE_NAME_SRC "Players.txt"
#define PLAYER_BINARY_FILE_NAME_SRC "BinaryPlayersOutput.bin"
#define PLAYER_TEXT_FILE_NAME_DEST "PlayersOutput.txt"
#define PLAYER_BINARY_FILE_NAME_DST "BinaryPlayersOutput.bin"
#define TEAM_TEXT_FILE_NAME_SRC "Teams.txt"
#define TEAM_BINARY_FILE_NAME_SRC "BinaryTeamsOutput.bin"
#define TEAM_TEXT_FILE_NAME_DST "TeamsOutput.txt"
#define TEAM_BINARY_FILE_NAME_DST "BinaryTeamsOutput.bin"
#define LEAGUE_TEXT_FILE_NAME_SRC "LeagueTable.txt"
#define LEAGUE_BINARY_FILE_NAME_SRC "BinaryLeagueTableOutput.bin"
#define LEAGUE_TEXT_FILE_NAME_DST "LeagueTableOutput.txt"
#define LEAGUE_BINARY_FILE_NAME_DST "BinaryLeagueTableOutput.bin"


typedef enum
{
	eLeagueTable, ePlyersList, eTeamsList, eNofOptions
} eMainMenuOptions;

const char* str[eNofOptions] = { "LEAGUE TABLE", "PLAYERS LIST", "TEAMS LIST"};

int menu();
int initPlayerManagerAndTeamManager(PlayerManager* pManager, TeamManager* tManager, LeagueTable* LManager);
int saveTheSystemToFiles(PlayerManager* pManager, TeamManager* tManager, LeagueTable* LManager);

int main()
{
	PlayerManager pManager;
	TeamManager tManager;
	LeagueTable LManager;

	if (initPlayerManagerAndTeamManager(&pManager, &tManager, &LManager) == 0)
	{
		printf("error init manager\n");
		return 0;
	}

	int option;
	int stop = 0;
	do
	{
		option = menu();
		switch (option)
		{

		case eLeagueTable:
			LeagueFunc(&pManager, &tManager, &LManager);
			break;

		case ePlyersList:
			PlayerManagerFunc(&pManager);
			break;

		case eTeamsList:
			TeamManagerFunc(&tManager, &pManager);
			break;

		case EXIT:
			printf("Bye bye\n");
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);

	if (!saveTheSystemToFiles(&pManager, &tManager, &LManager))
		return 0;

	freeAll(&pManager, &tManager, &LManager);
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofOptions; i++)
		printf("%d - %s\n", i, str[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

int initPlayerManagerAndTeamManager(PlayerManager* pManager, TeamManager* tManager, LeagueTable* LManager)
{
	if (initPlayerManager(pManager, PLAYER_TEXT_FILE_NAME_SRC, PLAYER_BINARY_FILE_NAME_SRC) == 1)
	{
		if (initTeamManager(tManager, pManager, TEAM_TEXT_FILE_NAME_SRC, TEAM_BINARY_FILE_NAME_SRC))
			if(!initLeagueTable(LManager, tManager, pManager, LEAGUE_TEXT_FILE_NAME_SRC, LEAGUE_BINARY_FILE_NAME_SRC))
				{
					printf("Error innitig the system\n");
					return 0;
				}
			return 1;
	}
	return 0;
}

int saveTheSystemToFiles(PlayerManager* pManager, TeamManager* tManager, LeagueTable* LManager)
{
	if (!savePlayerManagerToTextFile(PLAYER_TEXT_FILE_NAME_DEST, pManager))
		return 0;
	if (!savePlayerManagerToBinaryFile(PLAYER_BINARY_FILE_NAME_DST, pManager))
		return 0;
	if (!saveTeamManagerToTextFile(TEAM_TEXT_FILE_NAME_DST, tManager))
		return 0;
	if (!saveTeamManagerToBinaryFile(TEAM_BINARY_FILE_NAME_DST, tManager))
		return 0;
	if (!saveLeagueTableToTextFile(LEAGUE_TEXT_FILE_NAME_DST, LManager))
		return 0;
	if (!saveLeagueTableToBinaryFile(LEAGUE_BINARY_FILE_NAME_DST, LManager))
		return 0;
	return 1;
}
