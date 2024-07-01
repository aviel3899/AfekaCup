#include <stdlib.h>
#include <string.h>
#include "TeamManagerText.h"
#include "PlayerManagerText.h"
#include "LeagueTableText.h"
#include "General.h"

int readLeagueTableFromTextFile(LeagueTable* lt, PlayerManager* pManager, const char* fileName)
{
	FILE* fp = fopen(fileName, "r");
	if (!fp)
		return 0;
	if (fscanf(fp, "%d\n", &lt->numOfLeagueTeams) != 1) {
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
		if (readTeamForLeagueTableFromTextFile(fp, team, pManager) == 0)
		{
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

int readTeamForLeagueTableFromTextFile(FILE* fp, Team* team, PlayerManager* pManager)
{
	if (!readTeamNameAndCodeFromTextFile(fp, team))
		return 0;
	if (!readPlayersListForTeamInLeague(fp, team, pManager))
		return 0;
	if (!readCoachFromTextFile(fp, team))
		return 0;
	if (!readArenaFromTextFile(fp, team))
		return 0;
	if (fscanf(fp, "%d\n", &team->budget) != 1)
		return 0;
	team->place = 0;
	team->isChosenForMatch = 0;
	team->isChosenForLeague = 1;
	initStats(&team->teamStats);
	if (!setTeamStats(team))
		return 0;
	updateTeamAllBudgets(team, team->players.squadExpense);
	return 1;
}

int readPlayersListForTeamInLeague(FILE* fp, Team* team, PlayerManager* pManager)
{
	int pManagerIndex;
	if (!initSquad(&team->players))
		return 0;
	if (fscanf(fp, "%d\n", &team->players.numOfPlayers) != 1)
		return 0;
	NODE* ptr = &team->players.playersList.head;
	for (int i = 0; i < team->players.numOfPlayers; i++)
	{
		Player* player = (Player*)malloc(sizeof(Player));
		if (!player)
			return 0;
		if (!player->playerTeam)
			return 0;
		if (!readPlayerFromTextFile(fp, player))
			return 0;
		strcpy(player->playerTeam, team->teamCode);
		if (!L_insert(ptr, player))
			return 0;
		pManagerIndex = findPlayer(pManager, player);
		strcpy(pManager->allPlayers[pManagerIndex]->playerTeam, team->teamCode);
		team->players.squadExpense += player->PlayerSalary;
		ptr = ptr->next;
	}
	return 1;
}

int writeLeagueTableToTextFile(const char* fileName, const LeagueTable* lt)
{
	FILE* fp = fopen(fileName, "w");
	if (!fp)
		return 0;
	fprintf(fp, "%d\n", lt->numOfLeagueTeams);
	for (int i = 0; i < lt->numOfLeagueTeams; i++)
	{
		if (!writeTeamFromLeagueToTextFile(fp, lt->leagueTeams[i]))
		{
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
}

int writeTeamFromLeagueToTextFile(FILE* fp, Team* team)
{
	fprintf(fp, "%s\n", team->TeamName);
	fprintf(fp, "%s\n", team->teamCode);
	fprintf(fp, "%d\n", team->players.numOfPlayers);
	NODE* ptr = team->players.playersList.head.next;
	for (int i = 0; i < team->players.numOfPlayers; i++)
	{
		Player* player = (Player*)malloc(sizeof(Player));
		if (!player)
			return 0;
		player = (Player*)ptr->key;
		writePlayerToTextFile(fp, player);
		ptr = ptr->next;
	}
	writeCoachToTextFile(fp, &team->headCoach);
	writeArenaToTextFile(fp, &team->homeArena);
	fprintf(fp, "%d\n", team->budget);
	return 1;
}