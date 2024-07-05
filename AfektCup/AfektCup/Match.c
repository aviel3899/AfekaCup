#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "Stats.h"
#include "Match.h"

int initMatch(Match* Event)
{
	Event->HomeTeam = (Team*)malloc(sizeof(Team));
	if (!Event->HomeTeam)
		return 0;
	if (!initTeam(Event->HomeTeam))
		return 0;
	Event->AwayTeam = (Team*)malloc(sizeof(Team));
	if (!Event->AwayTeam)
		return 0;
	if (!initTeam(Event->AwayTeam))
		return 0;
	Event->MatchArena = (Arena*)malloc(sizeof(Arena));
	if (!Event->MatchArena)
		return 0;
	initArena(Event->MatchArena);
	Event->HomeTeamPoints = 0;
	Event->AwayTeamPoints = 0;
	return 1;
}

void setHomeAndAwayTeams(Team* T1, Team* T2, Match* Event, PlayerManager* pManager) 
{
	Event->HomeTeam = T1;
	Event->AwayTeam = T2;
	SetStatsRandomTeam(Event->HomeTeam, Event, 0, pManager);
	SetStatsRandomTeam(Event->AwayTeam, Event, 1, pManager);
	Event->MatchArena = &(Event->HomeTeam->homeArena);
}

void SetStatsRandomTeam(Team* TheTeam, Match* Event, int homeOrAway, PlayerManager* pManager) 
{
	initStats(&TheTeam->teamStats);
	int Advantage = 0;
	if (strcmp(Event->HomeTeam->TeamName, TheTeam->TeamName) == 0) {
		Advantage = 2;
	}
	NODE* ptr = TheTeam->players.playersList.head.next;
	while (ptr)
	{
		Player* p1 = (Player*)malloc(sizeof(Player));
		if (!p1)
			return;
		p1 = (Player*)ptr->key;
		setStatsRandomly(&p1->playerStats);
		p1->playerStats.points += Advantage;
		int playerIndex = findPlayer(pManager, p1);
		pManager->allPlayers[playerIndex] = p1;
		ptr = ptr->next;
	}
	setTeamStats(TheTeam);
	if(homeOrAway == 0)
		Event->HomeTeamPoints = TheTeam->teamStats.points;
	else
		Event->AwayTeamPoints = TheTeam->teamStats.points;
}

void SetMatchArena(Match* Event) {
	Event->MatchArena = &(Event->HomeTeam->homeArena);
}

void printMatch(Match* Event)
{
	printTeamForLeagueTable(Event->HomeTeam);
	printTeamForLeagueTable(Event->AwayTeam);
}

void freeMatch(Match* event)
{
	freeTeam(event->HomeTeam);
	freeTeam(event->AwayTeam);
	// freeArena not needed because the match arena is the homeTeam arena and when we free the homeTeam we free the match arena
}