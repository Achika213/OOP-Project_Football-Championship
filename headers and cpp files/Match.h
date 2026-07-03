#pragma once
#include "Team.h"
#include "Lineup.h"

const int MAX_GOAL_SCORERS_PER_MATCH = 30;

class Match
{
public:
	Match();
	Match(Team* homeTeam,
		Team* awayTeam,
		int round,
		int totalRounds);

	void chooseLineups();
	void enterResult();
	void playMatch();
	void playMatchAutomatically();
	void addAutomaticGoalScorers(Lineup& lineup, int goals);

	void enterGoalScorers();

	int getHomeGoals() const;
	int getAwayGoals() const;
	int getRound() const;

	bool shouldTryIncludePlayersWithLessThan3Matches() const;


	void print() const;
private:
	Team* homeTeam;
	Team* awayTeam;
	Lineup homeLineup;
	Lineup awayLineup;
	int homeGoals;
	int awayGoals;
	int round;
	bool played;

	Player* goalScorers[MAX_GOAL_SCORERS_PER_MATCH];
	int goalScorersCount;

	int totalRounds;
};


