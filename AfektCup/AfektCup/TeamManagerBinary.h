#pragma once

#include "TeamManager.h"

typedef unsigned char BYTE;

int	readTeamManagerFromBinaryFile(TeamManager* tManager, const char* fileName);
int readTeamFromBinaryFile(FILE* fp, Team* team);
int readNameAndCodeFromBinaryFile(FILE* fp, Team* team);
int ReadCoachFromBinaryFileCompressed(FILE* fp, Coach* coach);
int readCoachFromBinaryFile(FILE* fp, Coach* coach);
int readArenaFromBinaryFile(FILE* fp, Arena* arena);
int writeTeamManagerToBinaryFile(const TeamManager* tManager, const char* fileName);
int writeTeamToBinaryFile(const Team* team, FILE* fp);
int WriteTeamNameToBinaryFile(FILE* fp, const Team* team);
int WriteTeamCodeToBinaryFile(FILE* fp, const Team* team);
int WriteCoachToBinaryFileCompressed(FILE* fp, const Coach* coach);
int WriteCoachToBinaryFile(FILE* fp, const Coach* coach);
int WriteArenaToBinaryFile(FILE* fp, const Arena* arena);