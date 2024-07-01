#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "LeagueTableBinary.h"
#include "PlayerManagerBinary.h"
#include "TeamManagerBinary.h"
#include "General.h"

int	readLeagueTableFromBinaryFile(LeagueTable* lt, PlayerManager* pManager, const char* fileName)
{
	FILE* fp = fopen(fileName, "rb");
	if (!fp)
		return 0;
	if (fread(&lt->numOfLeagueTeams, sizeof(int), 1, fp) != 1) {
		fclose(fp);
		return 0;
	}
	lt->leagueTeams = (Team**)malloc(lt->numOfLeagueTeams * sizeof(Team*));
	if (!lt->leagueTeams)
	{
		fclose(fp);
		return 0;
	}
	for (int i = 0; i < lt->numOfLeagueTeams; i++)
	{
		Team* team = (Team*)malloc(sizeof(Team));
		if (!team)
		{
			free(lt->leagueTeams);
			fclose(fp);
			return 0;
		}
		if (readTeamForLeagueTableFromBinaryFile(fp, team, pManager) == 0)
		{
			freeTeam(team);
			free(team);
			free(lt->leagueTeams);
			fclose(fp);
			return 0;
		}
		lt->leagueTeams[i] = team;
	}
	fclose(fp);
	return 1;
}

int readTeamForLeagueTableFromBinaryFile(FILE* fp, Team* team, PlayerManager* pManager)
{
	if (!readNameAndCodeFromBinaryFile(fp, team))
		return 0;
	if (!readTeamSquadFromBinaryFile(fp, team, pManager))
		return 0;
	if (!ReadCoachFromBinaryFileCompressed(fp, &team->headCoach))
		return 0;
	/*if (!readCoachFromBinaryFile(fp, &team->headCoach))
		return 0;*/
	if (!readArenaFromBinaryFile(fp, &team->homeArena))
		return 0;
	if (fread(&team->budget, sizeof(int), 1, fp) != 1)
		return 0;
	team->place = 0;
	team->isChosenForMatch = 0;
	team->isChosenForLeague = 1;
	initStats(&team->teamStats);
	if (!setTeamStats(team))
		return 0;
	updateTeamAllBudgets(team, team->players.squadExpense);
	return  1;
}

int readTeamSquadFromBinaryFile(FILE* fp, Team* team, PlayerManager* pManager)
{
	int pManagerIndex;
	if (!initSquad(&team->players))
		return 0;
	if (fread(&team->players.numOfPlayers, sizeof(int), 1, fp) != 1)
		return 0;
	NODE* ptr = &team->players.playersList.head;
	for (int i = 0; i < team->players.numOfPlayers; i++)
	{
		Player* player = (Player*)malloc(sizeof(Player));
		if (!player)
			return 0;
		if (!player->position)
			return 0;
		if (!readPlayerFromBinaryFile(fp, player))
			return 0;
		player->playerTeam = _strdup(team->teamCode);
		if (!player->playerTeam)
			return 0;
		if (!L_insert(ptr, player))
			return 0;
		pManagerIndex = findPlayer(pManager, player);
		strcpy(pManager->allPlayers[pManagerIndex]->playerTeam, team->teamCode);
		team->players.squadExpense += player->PlayerSalary;
		ptr = ptr->next;
	}
	return 1;
}

int writeLeagueTableToBinaryFile(const char* fileName, const LeagueTable* lt)
{
	FILE* fp = fopen(fileName, "wb");
	if (!fp)
		return 0;
	if (fwrite(&lt->numOfLeagueTeams, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return 0;
	}
	for (int i = 0; i < lt->numOfLeagueTeams; i++)
	{
		if (!writeTeamFromLeagueToBinaryFile(lt->leagueTeams[i], fp))
		{
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
}

int writeTeamFromLeagueToBinaryFile(Team* team, FILE* fp)
{
	if (!WriteTeamNameToBinaryFile(fp, team))
		return 0;
	if (!WriteTeamCodeToBinaryFile(fp, team))
		return 0;
	if (fwrite(&team->players.numOfPlayers, sizeof(int), 1, fp) != 1)
		return 0;
	NODE* ptr = team->players.playersList.head.next;
	for (int i = 0; i < team->players.numOfPlayers; i++)
	{
		Player* player = (Player*)malloc(sizeof(Player));
		if (!player)
			return 0;
		player = (Player*)ptr->key;
		if (!writePlayerToBinaryFile(player, fp))
			return 0;
		ptr = ptr->next;
		/*free(player);*/
	}
	if (!WriteCoachToBinaryFileCompressed(fp, &team->headCoach))
		return 0;
	if (!WriteArenaToBinaryFile(fp, &team->homeArena))
		return 0;
	if (fwrite(&team->budget, sizeof(int), 1, fp) != 1)
		return 0;
	return 1;
}