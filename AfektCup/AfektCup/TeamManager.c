#include <stdio.h>
#include <stdlib.h>
#include "TeamManager.h"
#include "TeamManagerText.h"
#include "TeamManagerBinary.h"
#include "General.h"

typedef enum
{
	ePrintTeamsList, eSortTeams,eFindTeam, eAddTeam, eNofTeamOptions
} eTeamManagerOptions;

const char* TM[eNofTeamOptions] = { "print teams list", "sort teams", "find team", "add team"};

int initTeamManager(TeamManager* tManager, PlayerManager* pManager, const char* textFile, const char* binaryFile)
{
	int choice, flag = 0;
	printf("\nHow would you like to upload the team manager file?\n");
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
		if (readTeamManagerFromTextFile(tManager, textFile) == 0)
		{
			printf("Couldn't read the team's list from text file, set team's list manualy\n");
			tManager->numOfTeams = 0;
			if (!setTeamsList(tManager, pManager))
				return 0;
		}
	}
	else
		if (readTeamManagerFromBinaryFile(tManager, binaryFile) == 0)
		{
			printf("Couldn't read the team's list from binary file, set team's list manualy\n");
			if (!setPlayersList(pManager))
				return 0;
		}
	tManager->sort = 0;
	return 1;
}

int saveTeamManagerToTextFile(const char* fileName, const TeamManager* tManager)
{
	if (!writeTeamManagerToTextFile(fileName, tManager))
	{
		printf("Error saving the team manager to text file\n");
		return 0;
	}
	return 1;
}

int saveTeamManagerToBinaryFile(const char* fileName, const TeamManager* tManager)
{
	if (!writeTeamManagerToBinaryFile(tManager, fileName))
	{
		printf("Error saving the team manager to binary file\n");
		return 0;
	}
	return 1;
}

int setTeamsList(TeamManager* tManager, PlayerManager* pManager)
{
	int temasCounter;
	printf("How many teams would you like in your list?\n");
	while (1)
	{
		scanf("%d", &temasCounter);
		if (temasCounter >= MIN_TEMAS_IN_CUP)
			break;
		printf("List of teams must include at least %d teams, enter again\n", MIN_TEMAS_IN_CUP);
	}
	tManager->teams = (Team**)malloc(temasCounter * sizeof(Team*));
	if (!tManager->teams)
		return 0;
	for (int i = 0; i < temasCounter; i++)
	{
		if (!addTeam(tManager,pManager))
			return 0;
	}
	return 1;
}

int	addTeam(TeamManager* tManager, PlayerManager* pManager)
{
	if (pManager->numOfPlayers < MIN_PLAYERS_IN_CUP)
	{
		printf("You cannot add a team while you don't stand the minimum require of players\n");
		return 0;
	}
	if (tManager->numOfTeams == 0)
	{
		tManager->teams = (Team**)malloc(sizeof(Team*));
		if (!tManager->teams)
			return 0;
	}
	else
	{
		tManager->teams = (Team**)realloc(tManager->teams, (tManager->numOfTeams + 1) * sizeof(Team*));
		if (!tManager->teams)
		{
			freeTeamsList(tManager);
			return 0;
		}
	}
	Team* team = (Team*)malloc(sizeof(Team));
	if (!team)
		return 0;
	if (!setTeam(team, tManager))
	{
		free(team);
		return 0;
	}
	tManager->teams[tManager->numOfTeams] = team;
	tManager->numOfTeams++;
	return 1;
}

int setTeam(Team* team, TeamManager* tManager)
{
	char* msg = NULL;
	int count = 0;
	if (initTeam(team))
		return 0;
	while (1)
	{
		if (count == 0)
			msg = "What is the name of the team ? \n";
		setTeamName(team, msg);
		if (findTeam(tManager, team, compareTeamName) == -1)
			break;
		printf("This team already exists - enter a different team name\n");
		count++;
	}
	count = 0;
	while (1)
	{
		if(count == 0)
			printf("Enter team code  - %d UPPER CASE letters\t", SIZE_OF_TEAM_CODE);
		getTeamCode(team->teamCode);
		if (findTeam(tManager, team, compareTeamCode) == -1)
			break;
		printf("This team code already exists - enter a different team code\n");
		count++;
	}
	while (!setTeamBudget(team, count))
	{
		printf("Budget doesnt stand the requirement, enter again\n");
		count++;
	}
	team->freeBudget = team->budget;
	if (!setArena(&team->homeArena))
		return 0;
	if (!setTeamCoach(team))
		return 0;
	team->currentExpense = team->budget - team->freeBudget;
	return 1;
}

int findTeam(TeamManager* tManager, Team* team, int (*compare)(void* v1, void* v2))
{
	int flag = -1;
	for (int i = 0; i < tManager->numOfTeams; i++)
	{
		if (compare(&team, &tManager->teams[i]) == 0)
		{
			flag = i;
			break;
		}
	}
	return flag;
}


void sortTeam(TeamManager* tManager)
{
	int choice;
	printf("\nBase on what field do you want to sort?\n");
	do {
		for (int i = 1; i < eNofTeamSorts; i++)
			printf(" Enter %d for %s\n", i, TeamSort[i]);
		scanf("%d", &choice);
		switch (choice)
		{
		case (eSortByTeamName):
			qsort(tManager->teams, tManager->numOfTeams, sizeof(Team**), compareTeamName);
			tManager->sort = 1;
			break;
		case (eSortByTeamCode):
			qsort(tManager->teams, tManager->numOfTeams, sizeof(Team**), compareTeamCode);
			tManager->sort = 2;
			break;
		case (eSortByCoachName):
			qsort(tManager->teams, tManager->numOfTeams, sizeof(Team**), compareTeamByCoachName);
			tManager->sort = 3;
			break;
		case (eSortByBudget):
			qsort(tManager->teams, tManager->numOfTeams, sizeof(Team**), compareTeamByBudget);
			tManager->sort = 4;
			break;
		case (eSortByFreeBudget):
			qsort(tManager->teams, tManager->numOfTeams, sizeof(Team**), compareTeamFreeByBudget);
			tManager->sort = 5;
			break;
		case (eSortByArenaName):
			qsort(tManager->teams, tManager->numOfTeams, sizeof(Team**), compareTeamByArenaName);
			tManager->sort = 6;
			break;
		default:
			printf("Wrong option\n");
			break;
		}
	} while (choice < 0 || choice >= eNofTeamSorts);
}

void FindTeamFromMenu(TeamManager* tManager)
{
	char temp[MAX_STR_LEN];
	char* str = NULL;
	Team** pT = NULL;
	int pick = 0;
	int count = 0;
		do {
			if (count == 0)
				printf("\nplease enter on which basis you wish to find the team\n"); // bsearch on basis of budget/free budget(op 4 and 5) is not allowed because possibly two teams have the same budget
			else
				printf("wrong input, try again\n");
			for (int i = 1; i < 4; i++) {
				printf("%d) %s\n", i, TeamSort[i]);
			}
			printf("4) %s\n", TeamSort[6]);
			scanf("%d", &pick);
			getchar();
			count++;

		} while (pick != 1 && pick != 2 && pick != 3 && pick != 4);

		switch (pick) {

		case 1:
			qsort(tManager->teams, tManager->numOfTeams, sizeof(Team**), compareTeamName);
			tManager->sort = 1;
			str = getStrExactName("please enter team's name to search\n");
			pT = (Team**)bsearch(str, tManager->teams, tManager->numOfTeams, sizeof(Team*), isTeamName);
			break;

		case 2:
			qsort(tManager->teams, tManager->numOfTeams, sizeof(Team**), compareTeamCode);
			tManager->sort = 2;
			printf("please enter team code to search\n");
			getTeamCode(temp);
			pT = (Team**)bsearch(temp, tManager->teams, tManager->numOfTeams, sizeof(Team*), isTeamCode);
			break;

		case 3:
			qsort(tManager->teams, tManager->numOfTeams, sizeof(Team**), compareTeamByCoachName);
			tManager->sort = 3;
			str = getStrExactName("please enter coach name to search\n");
			pT = (Team**)bsearch(str, tManager->teams, tManager->numOfTeams, sizeof(Team*), isCoachName);
			break;

		case 4:
			qsort(tManager->teams, tManager->numOfTeams, sizeof(Team**), compareTeamByArenaName);
			tManager->sort = 6;
			str = getStrExactName("please enter arena name to search\n");
			pT = (Team**)bsearch(str, tManager->teams, tManager->numOfTeams, sizeof(Team*), isArenaName);
			break;
		}

		if (!pT) {
			printf("no corresponding team found\n");
		}
		else
			printAllTeamParameters(*pT);
		return;
}

void freeTeamsList(TeamManager* tManager)
{
	for (int i = 0; i < tManager->numOfTeams; i++)
	{
		freeTeam(tManager->teams[i]);
		free(tManager->teams[i]);
	}
	free(tManager->teams);
}

void printTeamsList(const TeamManager* tManager, int printOnlyNotChosenTeams)
{
	printf("\nTeams list:\n");
	printTeamStatsTabletile();
	for (int i = 0; i < tManager->numOfTeams; i++)
	{
		if (printOnlyNotChosenTeams == 1)
			if (tManager->teams[i]->isChosenForLeague == 0)
				printTeamGeneralTable(tManager->teams[i]);
			else
				continue;
		else
				printTeamGeneralTable(tManager->teams[i]);
	}
}

int TeamManagerMenu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofTeamOptions; i++)
		printf("%d - %s\n", i, TM[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

void TeamManagerFunc(TeamManager* tManager, PlayerManager* pManager)
{
	int option;
	int stop = 0;
	do
	{
		option = TeamManagerMenu();
		switch (option)
		{

		case ePrintTeamsList:
			printTeamsList(tManager, 0);
			break;

		case eSortTeams:
			sortTeam(tManager);
			break;

		case eFindTeam:
			FindTeamFromMenu(tManager);
			break;

		case eAddTeam:
			if (!addTeam(tManager, pManager))
				printf("Error adding a team\n");
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