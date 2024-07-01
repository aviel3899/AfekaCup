#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "TeamManagerBinary.h"
#include "General.h"

int	readTeamManagerFromBinaryFile(TeamManager* tManager, const char* fileName)
{
	FILE* fp = fopen(fileName, "rb");
	if (!fp)
		return 0;
	if (fread(&tManager->numOfTeams, sizeof(int), 1, fp) != 1) {
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
		if (readTeamFromBinaryFile(fp, team) == 0)
		{
			freeTeam(team);
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

int readTeamFromBinaryFile(FILE* fp, Team* team)
{
	if (!readNameAndCodeFromBinaryFile(fp, team))
		return 0;
	if (!ReadCoachFromBinaryFileCompressed(fp, &team->headCoach))
		return 0;
	/*if (!readCoachFromBinaryFile(fp, &team->headCoach))
		return 0;*/
	if (!readArenaFromBinaryFile(fp, &team->homeArena))
		return 0;
	if (fread(&team->budget, sizeof(int), 1, fp) != 1)
		return 0;
	if (!initSquad(&team->players))
		return 0;
	initStats(&team->teamStats);
	team->currentExpense = team->headCoach.CoachSalary;
	team->freeBudget = team->budget - team->currentExpense;
	team->place = 0;
	team->isChosenForLeague = 0;
	team->isChosenForMatch = 0;
	return  1;
}

int readNameAndCodeFromBinaryFile(FILE* fp, Team* team)
{
	char* teamName = readNameFromBFile(fp);
	team->TeamName = _strdup(teamName);
	if (!team->TeamName)
	{
		free(teamName);
		return 0;
	}
	char* teamCode = readNameFromBFile(fp);
	for (int i = 0; i < SIZE_OF_TEAM_CODE + 1; i++)
	{
		team->teamCode[i] = teamCode[i];
	}
	return 1;
}

int ReadCoachFromBinaryFileCompressed(FILE* fp, Coach* coach) 
{
	BYTE data[4];
	if (fread(data, sizeof(BYTE), 4, fp) != 4) 
		return 0;
	int len = data[0] >> 3;
	coach->CoachName = (char*)calloc((len + 1), sizeof(char));
	if (!coach->CoachName) {
		return 0;
	}
	BYTE tempFirst = data[0] & 0x7;
	BYTE tempLast = data[3] & 0xE0;
	coach->CoachSalary = (int)tempFirst << 19 | (int)data[1] << 11 | (int)data[2] << 3 | (int)tempLast >> 5;
	coach->CoachYearsOfExperience = data[3] & 0x1F;
	if (fread(coach->CoachName, sizeof(char), len, fp) != len) {
		free(coach->CoachName);
		return 0;
	}
	return 1;
}

int readCoachFromBinaryFile(FILE* fp, Coach* coach)
{
	char* coachName = readNameFromBFile(fp);
	coach->CoachName = _strdup(coachName);
	if (!coach->CoachName)
	{
		free(coachName);
		return 0;
	}
	if (fread(&coach->CoachSalary, sizeof(int), 1, fp) != 1)
	{
		free(coach->CoachName);
		free(coachName);
		return 0;
	}
	if (fread(&coach->CoachYearsOfExperience, sizeof(int), 1, fp) != 1)
	{
		free(coach->CoachName);
		free(coachName);
		return 0;
	}
	return 1;
}

int readArenaFromBinaryFile(FILE* fp, Arena* arena)
{
	char* arenaName = readNameFromBFile(fp);
	arena->ArenaName = _strdup(arenaName);
	if (!arena->ArenaName)
	{
		free(arenaName);
		return 0;
	}
	if (fread(&arena->capacity, sizeof(int), 1, fp) != 1)
	{
		free(arena->ArenaName);
		free(arenaName);
		return 0;
	}
	return 1;
}

int writeTeamManagerToBinaryFile(const TeamManager* tManager, const char* fileName)
{
	FILE* fp = fopen(fileName, "wb");
	if (!fp)
		return 0;
	if (fwrite(&tManager->numOfTeams, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return 0;
	}
	for (int i = 0; i < tManager->numOfTeams; i++)
	{
		if (!writeTeamToBinaryFile(tManager->teams[i], fp))
		{
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
}

int writeTeamToBinaryFile(const Team* team, FILE* fp)
{
	if (!WriteTeamNameToBinaryFile(fp, team))
		return 0;
	if (!WriteTeamCodeToBinaryFile(fp, team))
		return 0;
	if (!WriteCoachToBinaryFileCompressed(fp, &team->headCoach))
		return 0;
	/*if (!WriteCoachToBinaryFile(fp, &team->headCoach))
		return 0;*/
	if (!WriteArenaToBinaryFile(fp, &team->homeArena))
		return 0;
	if (fwrite(&team->budget, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

int WriteTeamNameToBinaryFile(FILE* fp, const Team* team)
{
	int StringCount = (int)strlen(team->TeamName) + 1;
	if (fwrite(&StringCount, sizeof(int), 1, fp) != 1)
		return 0;
	char* temp = _strdup(team->TeamName);
	if (!temp)
		return 0;
	if (fwrite(temp, sizeof(char), StringCount, fp) != StringCount)
	{
		free(temp);
		return 0;
	}
	free(temp);
	return 1;
}

int WriteTeamCodeToBinaryFile(FILE* fp, const Team* team)
{
	int StringCount = (int)strlen(team->teamCode) + 1;
	if (fwrite(&StringCount, sizeof(int), 1, fp) != 1)
		return 0;
	char* temp = _strdup(team->teamCode);
	if (!temp)
		return 0;
	if (fwrite(temp, sizeof(char), StringCount, fp) != StringCount)
	{
		free(temp);
		return 0;
	}
	free(temp);
	return 1;
}

int WriteCoachToBinaryFileCompressed(FILE* fp, const Coach* coach)
{
	BYTE data[4] = { 0 };
	int len = (int)strlen(coach->CoachName);
	data[0] = len << 3 | coach->CoachSalary >> 19;
	data[1] = coach->CoachSalary >> 11;
	data[2] = coach->CoachSalary >> 3;
	data[3] = coach->CoachSalary << 5 | coach->CoachYearsOfExperience;
	if (fwrite(data, sizeof(BYTE), 4, fp) != 4)
		return 0;
	if (fwrite(coach->CoachName, sizeof(char), len, fp) != len) 
		return 0;
	return 1;
}

int WriteCoachToBinaryFile(FILE* fp, const Coach* coach)
{
	int StringCount = (int)strlen(coach->CoachName) + 1;
	if (fwrite(&StringCount, sizeof(int), 1, fp) != 1)
		return 0;
	char* temp = _strdup(coach->CoachName);
	if (!temp)
		return 0;
	if (fwrite(temp, sizeof(char), StringCount, fp) != StringCount)
	{
		free(temp);
		return 0;
	}
	free(temp);
	if (fwrite(&coach->CoachSalary, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(&coach->CoachYearsOfExperience, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}

int WriteArenaToBinaryFile(FILE* fp, const Arena* arena)
{
	int StringCount = (int)strlen(arena->ArenaName) + 1;
	if (fwrite(&StringCount, sizeof(int), 1, fp) != 1)
		return 0;
	char* temp = _strdup(arena->ArenaName);
	if (!temp)
		return 0;
	if (fwrite(temp, sizeof(char), StringCount, fp) != StringCount)
	{
		free(temp);
		return 0;
	}
	free(temp);
	if (fwrite(&arena->capacity, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}