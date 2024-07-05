#pragma once

#include "LeagueTable.h"

typedef struct {
	Team* HomeTeam;
	Team* AwayTeam;
	Arena* MatchArena;
	int HomeTeamPoints;
	int AwayTeamPoints;
} Match;

int initMatch(Match* Event);
void setHomeAndAwayTeams(Team* T1, Team* T2, Match* Event, PlayerManager* pManager);
void SetStatsRandomTeam(Team* HomeTeam, Match* Event, int homeOrAway, PlayerManager* pManager);
void SetMatchArena(Match* Event);
void printMatch(Match* Event);
void freeMatch(Match* event);