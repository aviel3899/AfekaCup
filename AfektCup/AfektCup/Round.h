#pragma once

#include "LeagueTable.h"

typedef struct {
	Match** RoundMatches;
	int numOfMatches;
}Round;

int initRound(Round* round, int numOfMatches);
void setRound(Round* round, int index, Match* match);
void printRound(Round* round);
