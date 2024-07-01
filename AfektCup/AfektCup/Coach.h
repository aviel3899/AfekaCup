#pragma once

typedef struct {
	char* CoachName;
	int CoachSalary;
	int CoachYearsOfExperience;
}Coach;

void initCoach(Coach* coach);
int SetCoachName(Coach* coach);
int compareCoachName(const void* v1, const void* v2);
void SetCoachSalary(Coach* coach, int freeBudget);
void SetCoachYearsOfExperience(Coach* coach);
void freeCoach(Coach* coach);
void printCoach(const Coach* coach);