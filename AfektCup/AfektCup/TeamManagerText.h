#pragma once

#include <stdio.h>
#include "TeamManager.h"

int	readTeamFromTextFile(FILE* fp, Team* team);
int	readTeamManagerFromTextFile(TeamManager* tManager, const char* fileName);
int readTeamNameAndCodeFromTextFile(FILE* fp, Team* team);
int readCoachFromTextFile(FILE* fp, Team* team);
int readArenaFromTextFile(FILE* fp, Team* team);
int initTeamFromTextFile(Team* team);
int writeTeamManagerToTextFile(const char* fileName, const TeamManager* teams);
void writeTeamToTextFile(FILE* fp, const Team* team);
void writeCoachToTextFile(FILE* fp, const Coach* coach);
void writeArenaToTextFile(FILE* fp, const Arena* arena);