#pragma once

#include "Match.h"

typedef struct {
	Match* RoundMatches;
	int numOfMatches;
}LeagueRound;

void initRound(LeagueRound* lr);
