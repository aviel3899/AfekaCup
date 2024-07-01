#include <stdlib.h>
#include <string.h>
#include "TeamManagerText.h"
#include "General.h"

int readTeamManagerFromTextFile(TeamManager* tManager, const char* fileName)
{
	FILE* fp = fopen(fileName, "r");
	if (!fp)
		return 0;
	if (fscanf(fp, "%d\n", &tManager->numOfTeams) != 1) {
		fclose(fp);
		return 0;
	}
	tManager->teams = (Team**)malloc(tManager->numOfTeams * sizeof(Team*));
	if (!tManager->teams)
	{
		fclose(fp);
		return 0;
	}
	for (int i = 0; i < tManager->numOfTeams; i++)
	{
		Team* team = (Team*)malloc(sizeof(Team));
		if (!team)
		{
			free(tManager->teams);
			fclose(fp);
			return 0;
		}
		if (readTeamFromTextFile(fp, team) == 0)
		{
			free(team);
			free(tManager->teams);
			fclose(fp);
			return 0;
		}
		tManager->teams[i] = team;
	}
	fclose(fp);
	return 1;
}

int readTeamFromTextFile(FILE* fp, Team* team)
{
	if (!readTeamNameAndCodeFromTextFile(fp, team))
		return 0;
	if (!readCoachFromTextFile(fp, team))
		return 0;
	if (!readArenaFromTextFile(fp, team))
		return 0;
	if (fscanf(fp, "%d\n", &team->budget) != 1)
		return 0;
	if (!initTeamFromTextFile(team))
		return 0;
	return  1;
}

int readTeamNameAndCodeFromTextFile(FILE* fp, Team* team)
{
	char TeamName[MAX_STR_LEN] = { 0 };
	char TeamCode[SIZE_OF_TEAM_CODE + 2] = { 0 };
	if (!myGets(TeamName, MAX_STR_LEN, fp))
		return 0;
	if (!myGets(TeamCode, SIZE_OF_TEAM_CODE + 2, fp))
		return 0;
	team->TeamName = _strdup(TeamName);
	if (!team->TeamName)
		return 0;
	for (int i = 0; i <= SIZE_OF_TEAM_CODE; i++)
	{
		team->teamCode[i] = TeamCode[i];
	}
	return 1;
}

int readCoachFromTextFile(FILE* fp, Team* team)
{
	char coachName[MAX_STR_LEN] = { 0 };
	if (!myGets(coachName, MAX_STR_LEN, fp))
		return 0;
	team->headCoach.CoachName = _strdup(coachName);
	if (!team->headCoach.CoachName)
		return 0;
	if (fscanf(fp, "%d\n", &team->headCoach.CoachSalary) != 1)
		return 0;
	if (fscanf(fp, "%d\n", &team->headCoach.CoachYearsOfExperience) != 1)
		return 0;
	return 1;
}

int readArenaFromTextFile(FILE* fp, Team* team)
{
	char arenaName[MAX_STR_LEN] = { 0 };
	if (!myGets(arenaName, MAX_STR_LEN, fp))
		return 0;
	if (fscanf(fp, "%d\n", &team->homeArena.capacity) != 1)
		return 0;
	team->homeArena.ArenaName = _strdup(arenaName);
	if (!team->homeArena.ArenaName)
		return 0;
	return 1;
}

int initTeamFromTextFile(Team* team)
{
	if (!initSquad(&team->players))
		return 0;
	initStats(&team->teamStats);
	team->currentExpense = team->headCoach.CoachSalary;
	team->freeBudget = team->budget - team->currentExpense;
	team->place = 0;
	team->isChosenForLeague = 0;
	team->isChosenForMatch = 0;
	return 1;
}

int writeTeamManagerToTextFile(const char* fileName, const TeamManager* teams)
{
	FILE* fp = fopen(fileName, "w");
	if (!fp)
		return 0;
	fprintf(fp, "%d\n", teams->numOfTeams);
	for (int i = 0; i < teams->numOfTeams; i++)
	{
		writeTeamToTextFile(fp, teams->teams[i]);
	}
	fclose(fp);
	return 1;
}

void writeTeamToTextFile(FILE* fp, const Team* team)
{
	fprintf(fp, "%s\n", team->TeamName);
	fprintf(fp, "%s\n", team->teamCode);
	writeCoachToTextFile(fp, &team->headCoach);
	writeArenaToTextFile(fp, &team->homeArena);
	fprintf(fp, "%d\n", team->budget);
}

void writeCoachToTextFile(FILE* fp, const Coach* coach)
{
	fprintf(fp, "%s\n", coach->CoachName);
	fprintf(fp, "%d\n", coach->CoachSalary);
	fprintf(fp, "%d\n", coach->CoachYearsOfExperience);
}

void writeArenaToTextFile(FILE* fp, const Arena* arena)
{
	fprintf(fp, "%s\n", arena->ArenaName);
	fprintf(fp, "%d\n", arena->capacity);
}