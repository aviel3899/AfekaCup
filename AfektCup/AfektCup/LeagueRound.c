#include <stdlib.h>
#include <stdio.h>
#include "LeagueRound.h"

void initRound(LeagueRound* lr)
{
	lr->RoundMatches = NULL;
	lr->numOfMatches = 0;
}

int setLeagueRound(LeagueRound* lr, int numOfmatches)
{
	int homeIndex, awayIndex;
	for (int i = 0; i < numOfmatches / 2; i++)
	{
		if (!initMatch(lt->leagueRounds[lt->numOfRounds]))
			return 0;
		homeIndex = checkIfTeamWasChosenForMatch(lt);
		awayIndex = checkIfTeamWasChosenForMatch(lt);
		setHomeAndAwayTeams(lt->leagueTeams[homeIndex], lt->leagueTeams[awayIndex], lt->leagueRounds[i], pManager);
	}
}