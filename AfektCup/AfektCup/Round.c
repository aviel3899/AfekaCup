#include <stdlib.h>
#include "Round.h"

int initRound(Round* round, int numOfMatches)
{
	round->RoundMatches = (Match**)malloc(numOfMatches * sizeof(Match*));
	if (!round->RoundMatches)
		return 0;
	round->numOfMatches = numOfMatches;
	return 1;
}

void setRound(Round* round, int index, Match* match)
{
	round->RoundMatches[index] = match;
}

int LeagueRound(LeagueTable* lt, PlayerManager* pManager)
{
	int homeIndex, awayIndex;
	if (lt->numOfLeagueRounds == 0)
	{
		lt->leagueRounds = (Round**)malloc(sizeof(Round));
		if (!lt->leagueRounds)
			return 0;
	}
	else
	{
		lt->leagueRounds = (Round**)realloc(lt->leagueRounds, (lt->numOfLeagueRounds + 1) * sizeof(Round));
		if (!lt->leagueRounds)
			return 0;
	}
	if (!initRound(lt->leagueRounds[lt->numOfLeagueRounds], lt->numOfLeagueTeams / 2))
		return 0;
	Match* roundMatches = (Match*)malloc(lt->numOfLeagueTeams / 2 * sizeof(Match));
	if (!roundMatches)
		return 0;
	for (int i = 0; i < lt->numOfLeagueTeams; i++)
	{
		lt->leagueTeams[i]->isChosenForMatch = 0;
	}
	for (int i = 0; i < lt->numOfLeagueTeams / 2; i++)
	{
		if (!initMatch(&roundMatches[i]))
			return 0;
		homeIndex = checkIfTeamWasChosenForMatch(lt);
		awayIndex = checkIfTeamWasChosenForMatch(lt);
		setHomeAndAwayTeams(lt->leagueTeams[homeIndex], lt->leagueTeams[awayIndex], &roundMatches[i], pManager);
		setRound(lt->leagueRounds[lt->numOfLeagueRounds], i, &roundMatches[i]);
	}
	updateStandings(lt);
	return 1;
}

int checkIfTeamWasChosenForMatch(LeagueTable* lt)
{
	int randomIndex;
	srand((unsigned int)time(NULL));
	do {
		randomIndex = rand() % lt->numOfLeagueTeams;
	} while (lt->leagueTeams[randomIndex]->isChosenForMatch == 1);
	lt->leagueTeams[randomIndex]->isChosenForMatch = 1;
	return randomIndex;
}

void printRound(Round* round)
{
	for (int i = 0; i < round->numOfMatches; i++)
	{
		printMatch(round->RoundMatches[i]);
	}
}
