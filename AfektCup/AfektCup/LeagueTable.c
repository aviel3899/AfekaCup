#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "LeagueTable.h"
#include "Match.h"
#include "LeagueTableText.h"
#include "LeagueTableBinary.h"

typedef enum
{
	ePrintLeague, ePrintTeamInLeague, eprintAllPlayersInLeague, eTrade, eLeagueRound, eNofLeagueOptions
} eLeagueOptions;

const char* League[eNofLeagueOptions] = { "print league table", "print a team from the league", "print all players in the league", "trade 2 players from different teams", "set a league round"};

int initLeagueTable(LeagueTable* lt, TeamManager* tManager, PlayerManager* pManager, const char* textFile, const char* binaryFile)
{
	int choice, flag = 0;
	printf("\nHow would you like to upload the league table file?\n");
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
		if (readLeagueTableFromTextFile(lt, pManager, textFile) == 0)
		{
			printf("Couldn't read the league table's list from text file, set league table manualy\n");
			if (!setLeagueTable(lt, tManager, pManager))
				return 0;
		}
	}
	else
		if (readLeagueTableFromBinaryFile(lt, pManager, binaryFile) == 0)
		{
			printf("Couldn't read the league table's list from binary file, set league table manualy\n");
			if (!setLeagueTable(lt, tManager, pManager))
				return 0;
		}
	updateStandings(lt);
	return 1;
}

int saveLeagueTableToTextFile(const char* fileName, const LeagueTable* lt)
{
	if (!writeLeagueTableToTextFile(fileName, lt))
	{
		printf("Error saving the league table to text file\n");
		return 0;
	}
	return 1;
}

int saveLeagueTableToBinaryFile(const char* fileName, const LeagueTable* lt)
{
	if (!writeLeagueTableToBinaryFile(fileName, lt))
	{
		printf("Error saving the league table to text file\n");
		return 0;
	}
	return 1;
}

int setLeagueTable(LeagueTable* lt, TeamManager* tManager, PlayerManager* pManager)
{
	int teamsCounter, choice, whichWayToEnterToAllTeams;
	teamsCounter = getNumOfTeamsInTheLeague(lt, tManager);
	lt->leagueTeams = (Team**)malloc(teamsCounter * sizeof(Team*));
	if (!lt->leagueTeams)
		return 0;
	if (!setLeagueTeams(lt, tManager, teamsCounter))
	{
		freeLegueTable(lt);
		return 0;
	}
	lt->numOfLeagueTeams = teamsCounter;
	whichWayToEnterToAllTeams = setWhichWayToEnterPlayersToTeam();
	if (whichWayToEnterToAllTeams == 1)
		choice = setWhichWayToEnterThePlayers(1, NULL);
	for (int i = 0; i < lt->numOfLeagueTeams; i++)
	{
		if (whichWayToEnterToAllTeams == 1)
		{
			if (!EnterPlayersToTeam(lt->leagueTeams[i], pManager, choice))
				return 0;
		}
		else
		{
			choice = setWhichWayToEnterThePlayers(2, lt->leagueTeams[i]);
			if (!EnterPlayersToTeam(lt->leagueTeams[i], pManager, choice))
				return 0;
		}
	}
	updateStandings(lt);
	return 1;
}

void updateStandings(LeagueTable* lt)
{
	qsort(lt->leagueTeams, lt->numOfLeagueTeams, sizeof(Team**), compareTeamByPointsFomBigToSmall);
	for (int i = 0; i < lt->numOfLeagueTeams; i++)
	{
		lt->leagueTeams[i]->place = i + 1;
	}
}

int getNumOfTeamsInTheLeague(LeagueTable* lt, TeamManager* tManager)
{
	int teamsCounter;
	printf("How many teams would you like in your league?\n");
	while (1)
	{
		int flag = 0;
		scanf("%d", &teamsCounter);
		if (teamsCounter >= MIN_TEMAS_IN_CUP)
		{
			if (teamsCounter <= MAX_TEAMS_IN_CUP)
			{
				if (teamsCounter % 2 == 0)
					break;
				flag = 1;
			}
			else
				flag = 2;
		}
		if (flag == 1)
			printf("num of teams nust be an even number, enter again\n");
		else
			if(flag == 2)
				printf("League can't include more than %d teams, enter again\n", MAX_TEAMS_IN_CUP);
			else
				printf("League must include at least %d teams, enter again\n", MIN_TEMAS_IN_CUP);
	}
	return teamsCounter;
}

int setLeagueTeams(LeagueTable* lt, TeamManager* tManager, int teamsCounter)
{
	int teamIndex, exists, flag;
	printf("Which teams would you like in your league?, enter team's code\n");
	printTeamsList(tManager, 0);
	for (int i = 0; i < teamsCounter; i++, lt->numOfLeagueTeams++)
	{
		flag = 0;
		Team* team = (Team*)malloc(sizeof(Team));
		if (!team)
			return 0;
		while (1)
		{
			getTeamCode(team->teamCode);
			teamIndex = findTeam(tManager, team, compareTeamCode);
			if (teamIndex != -1)
			{
				exists = findTeamInLeague(lt, team, compareTeamCode);
				if (exists == -1)
					break;
				flag = 1;
			}
			if (flag == 1)
				printf("This team already exists in the league - enter a different team\n");
			else
				printf("There is no team with this code, enter again\n");
		}
		tManager->teams[teamIndex]->isChosenForLeague = 1;
		team = tManager->teams[teamIndex];
		lt->leagueTeams[i] = team;
	}
	return 1;
}

int findTeamInLeague(LeagueTable* lt, Team* team, int (*compare)(void* v1, void* v2))
{
	int flag = -1;
	for (int i = 0; i < lt->numOfLeagueTeams; i++)
	{
		if (compare(&team, &lt->leagueTeams[i]) == 0)
		{
			flag = i;
			break;
		}
	}
	return flag;
}

int setWhichWayToEnterPlayersToTeam()
{
	int choice, flag = 0;
	printf("\nHow would you like to add the players?\n");
	printf("1) Add the players for all the teams on base of the same field (manually/randomly)\n");
	printf("2) Decide for each team separately\n");
	do {
		if (flag != 0)
			printf("Wrong Input, enter again\n");
		scanf("%d", &choice);
		flag++;
	} while (choice < 1 || choice > 2);
	return choice;
}
	
int setWhichWayToEnterThePlayers(int index, Team* team)
{
	int choice, flag = 0;
	if(index != 1)
		printf("\nBase on what field do you want to enter the players to %s?\n", team->TeamName);
	else
		printf("\nBase on what field do you want to enter the players?\n");
	do {
		if (flag != 0)
			printf("Wrong Input, enter again\n");
		for (int i = 0; i < eNoOfWays; i++)
			printf(" Enter %d for %s\n", i, WayOfEnter[i]);
		scanf("%d", &choice);
		flag++;
	} while (choice < 0 || choice >= eNoOfWays);
	return choice;
}


int EnterPlayersToTeam(Team* team, PlayerManager* pManager, int choice)
{
	switch (choice)
	{
	case (eManually):
		if (!setTeamSquad(team, pManager, eManually))
			return 0;
		if (!setTeamStats(team))
			return 0;
		break;
	case (eRandomly):
		if (!setTeamSquad(team, pManager, eRandomly))
			return 0;
		if (!setTeamStats(team))
			return 0;
		break;
	}
	return 1;
}

int tradePlayers(LeagueTable* lt, PlayerManager* pManager)
{
	int OriginTeamIndex = 0, destTeamIndex = 0;
	char* msg = "Which player you want to trade?, enter his id\n";
	OriginTeamIndex = chooseOriginTeamTrade(lt, OriginTeamIndex);
	printSquad(&lt->leagueTeams[OriginTeamIndex]->players, 0);
	Player* firstPlayerToTrade = deletePlayerFromSquad(&lt->leagueTeams[OriginTeamIndex]->players, msg, NULL);
	if (!firstPlayerToTrade)
		return 0;
	if (!updateStatsAndBudget(lt, OriginTeamIndex))
		return 0;
	destTeamIndex = chooseDestTeamTrade(lt, destTeamIndex, OriginTeamIndex);
	printSquad(&lt->leagueTeams[destTeamIndex]->players, 0);
	Player* secondPlayerToTrade = chooseWhichPlayerToTradeFromDestTeam(lt, firstPlayerToTrade, destTeamIndex, OriginTeamIndex, msg);
	if (!secondPlayerToTrade)
		return 0;
	if (!updateStatsAndBudget(lt, destTeamIndex))
		return 0;
	if (!addPlayerToTeamAfterTrade(lt, pManager, OriginTeamIndex, secondPlayerToTrade))
		return 0;
	if (!addPlayerToTeamAfterTrade(lt, pManager, destTeamIndex, firstPlayerToTrade))
		return 0;
	return 1;
}

int addPlayerToTeamAfterTrade(LeagueTable* lt, PlayerManager* pManager, int teamIndex, Player* player)
{
	int pManagerIndex;
	strcpy(player->playerTeam, lt->leagueTeams[teamIndex]->teamCode);
	pManagerIndex = findPlayer(pManager, player);
	strcpy(pManager->allPlayers[pManagerIndex]->playerTeam, lt->leagueTeams[teamIndex]->teamCode);
	if (!addPlayerToSquad(&lt->leagueTeams[teamIndex]->players, NULL, player, 0, 0, NULL))
		return 0;
	if (!updateStatsAndBudget(lt, teamIndex))
		return 0;
	return 1;
}

int updateStatsAndBudget(LeagueTable* lt, int teamIndex)
{
	initStats(&lt->leagueTeams[teamIndex]->teamStats);
	if (!setTeamStats(lt->leagueTeams[teamIndex]))
		return 0;
	updateTeamAllBudgets(lt->leagueTeams[teamIndex], lt->leagueTeams[teamIndex]->players.squadExpense);
	return 1;
}

Player* chooseWhichPlayerToTradeFromDestTeam(LeagueTable* lt, Player* firstPlayerToTrade, int destTeamIndex, int OriginTeamIndex, char* msg)
{
	Player* secondPlayerToTrade;
	int flag = 0;
	while (1)
	{
		if (flag != 0)
			msg = NULL;
		secondPlayerToTrade = whichPlayerToDelete(&lt->leagueTeams[destTeamIndex]->players, msg);
		flag = 0;
		if (firstPlayerToTrade->PlayerSalary <= lt->leagueTeams[destTeamIndex]->freeBudget - secondPlayerToTrade->PlayerSalary)
		{
			if (secondPlayerToTrade->PlayerSalary <= lt->leagueTeams[OriginTeamIndex]->freeBudget - firstPlayerToTrade->PlayerSalary)
				break;
			flag = 1;
		}
		if (flag == 1)
			printf("%s salary is too high for %s, enter again\n", secondPlayerToTrade->playerName, lt->leagueTeams[OriginTeamIndex]->TeamName);
		else
			printf("%s salary is too high for %s, enter again\n", firstPlayerToTrade->playerName, lt->leagueTeams[destTeamIndex]->TeamName);
	}
	secondPlayerToTrade = deletePlayerFromSquad(&lt->leagueTeams[destTeamIndex]->players, msg, secondPlayerToTrade);
	if (!secondPlayerToTrade)
		return NULL;
	return secondPlayerToTrade;
}

int chooseOriginTeamTrade(LeagueTable* lt, int OriginTeamIndex)
{
	Team* OriginTeamTrade = (Team*)malloc(sizeof(Team));
	if (!OriginTeamTrade)
		return 0;
	printf("From which team would you like to trade a player?, enter team code\n");
	OriginTeamIndex = chooseWhichTeamToTrade(lt, OriginTeamTrade);
	OriginTeamTrade = lt->leagueTeams[OriginTeamIndex];
	return OriginTeamIndex;
}

int chooseDestTeamTrade(LeagueTable* lt, int destTeamIndex,  int OriginTeamIndex)
{
	Team* DestTeamTrade = (Team*)malloc(sizeof(Team));
	if (!DestTeamTrade)
		return 0;
	printf("To which team would you like to trade the player?, enter team code\n");
	while (1)
	{
		destTeamIndex = chooseWhichTeamToTrade(lt, DestTeamTrade);
		if (destTeamIndex != OriginTeamIndex)
			break;
		printf("You can't trade from and to the same team, enter again\n");
	}
	DestTeamTrade = lt->leagueTeams[destTeamIndex];
	return destTeamIndex;
}

int chooseWhichTeamToTrade(LeagueTable* lt, Team* team)
{
	int teamIndex;
	while (1)
	{
		getTeamCode(team->teamCode);
		teamIndex = findTeamInLeague(lt, team, compareTeamCode);
		if (teamIndex != -1)
			break;
		printf("There is no team with this code in the league, enter again\n");
	}
	team = lt->leagueTeams[teamIndex];
	return teamIndex;
}

int LeagueRound(LeagueTable* lt, PlayerManager* pManager)
{
	int homeIndex, awayIndex;
	Match* roundMatches = (Match*)malloc((lt->numOfLeagueTeams / 2) * sizeof(Match));
	if (!roundMatches)
		return 0;
	for (int i = 0; i < lt->numOfLeagueTeams; i++)
	{
		lt->leagueTeams[i]->isChosenForMatch = 0;
	}
	for (int i = 0; i < lt->numOfLeagueTeams / 2; i++)
	{
		if (!initMatch(&roundMatches[i]))
			return 0;
		homeIndex = checkIfTeamWasChosenForMatch(lt);
		awayIndex = checkIfTeamWasChosenForMatch(lt);
		setHomeAndAwayTeams(lt->leagueTeams[homeIndex], lt->leagueTeams[awayIndex], &roundMatches[i], pManager);
	}

	updateStandings(lt);
	return 1;
}

int checkIfTeamWasChosenForMatch(LeagueTable* lt)
{
	int randomIndex;
	srand((unsigned int)time(NULL));
	do {
		randomIndex = rand() % lt->numOfLeagueTeams;
	} while (lt->leagueTeams[randomIndex]->isChosenForMatch == 1);
	lt->leagueTeams[randomIndex]->isChosenForMatch = 1;
	return randomIndex;
}

void printPlayersListInLeague(const LeagueTable* lt)
{
	for (int i = 0; i < lt->numOfLeagueTeams; i++)
	{
		printSquad(&lt->leagueTeams[i]->players, i);
	}
}

void freeLegueTable(LeagueTable* lt)
{
	for (int i = 0; i < lt->numOfLeagueTeams; i++)
	{
		freeTeam(lt->leagueTeams[i]);
		free(lt->leagueTeams[i]);
	}
	free(lt->leagueTeams);
}

void freeAll(PlayerManager* pManager, TeamManager* tManager, LeagueTable* LManager)
{
	freeLegueTable(LManager);
	freeTeamsList(tManager);
	freePlayersList(pManager);
}

void printTableStatsTabletile()
{
	printf("Place%-1s", "");
	printf("Name%-21s", "");
	printf("code%-2s", "");
	printStatsTitle();
	printf("\n");
}

void printLeagueTable(const LeagueTable* lt)
{
	printf("\n");
	printTableStatsTabletile();
	for (int i = 0; i < lt->numOfLeagueTeams; i++)
	{
		printTeamForLeagueTable(lt->leagueTeams[i]);
	}
}

void printSpecificTeamInTheLeague(LeagueTable* lt)
{
	int index, count = 0;
	Team* team = (Team*)malloc(sizeof(Team));
	if (!team)
	{
		printf("ERROR\n");
		return;
	}
	while (1)
	{
		if (count == 0)
			printf("Which team would you like to see? enter team's code\n");
		getTeamCode(team->teamCode);
		index = findTeamInLeague(lt, team, compareTeamCode);
		if (index != -1)
			break;
		printf("There is no team with this code, enter again\n");
		count++;
	}
	free(team);
	printAllTeamParameters(lt->leagueTeams[index]);

}

int LeagueMenu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofLeagueOptions; i++)
		printf("%d - %s\n", i, League[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

void LeagueFunc(PlayerManager* pManager, TeamManager* tManager, LeagueTable* LManager)
{
	int option;
	int stop = 0;
	do
	{
		option = LeagueMenu();
		switch (option)
		{

		case ePrintLeague:
			printLeagueTable(LManager);
			break;

		case ePrintTeamInLeague:
			printSpecificTeamInTheLeague(LManager);
			break;

		case eprintAllPlayersInLeague:
			printPlayersListInLeague(LManager);
			break;

		case eTrade:
			tradePlayers(LManager, pManager);
			break;

		case eLeagueRound:
			if(!LeagueRound(LManager, pManager))
				printf("Error setting league round\n");
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