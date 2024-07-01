#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Coach.h"
#include "General.h"

void initCoach(Coach* coach) {
	coach->CoachName = NULL;
	coach->CoachSalary = 0;
	coach->CoachYearsOfExperience = 0;
}

int SetCoachName(Coach* coach) {
	char* str = getStrExactName("What is the name of the coach?\n");
	if (str) {
		coach->CoachName = (char*)malloc(strlen(str) * sizeof(char));
		if (!coach->CoachName)
			return 0;
		strcpy(coach->CoachName, str);
		free(str);
		return 1;
	}
	else {
		printf("Set name failed");
		return 0;
	}
}

int compareCoachName(const void* v1, const void* v2)
{
	Coach* c1 = (Coach*)v1;
	Coach* c2 = (Coach*)v2;
	return strcmp(c1->CoachName, c2->CoachName);
}

void SetCoachSalary(Coach* coach, int freeBudget)
{
	int count = 0, flag = 0;
	while (1)
	{
		if(count == 0)
			printf("What is the coach's salary?\n");
		scanf("%d", &coach->CoachSalary);
		if (coach->CoachSalary > 0)
		{
			if (coach->CoachSalary <= freeBudget)
				break;
			flag = 1;
		}
		if (flag != 0)
		{
			printf("Salary must be smaller than the free budget of the team, enter again\n");
			printf("The free budget is %d\n", freeBudget);
		}
		printf("Salary must be a positive number, enter again\n");
		count++;
	}
}

void SetCoachYearsOfExperience(Coach* coach) 
{
	int count = 0;
	while (1)
	{
		if(count == 0)
			printf("What is the coach's experience in years?\n");
		scanf("%d", &coach->CoachYearsOfExperience);
		if (coach->CoachYearsOfExperience >= 0)
			break;
		printf("Years of experience must be a positive number, enter again\n");
		count++;
	}
}

void freeCoach(Coach* coach)
{
	free(coach->CoachName);
}

void printCoach(const Coach* coach)
{
	printf("Coach:\n");
	printf("Coach's name: %-15s", coach->CoachName);
	printf("Coach's salary: %-10d", coach->CoachSalary);
	printf("Coach's years of experience: %-4d", coach->CoachYearsOfExperience);
}