#pragma once
#include "Team.h"

static const int MAX_LINEUP_SIZE = 11;

class Lineup
{
public:
	Lineup();
	Lineup(Team* team);

	void choosePlayersFromConsole();
	void choosePlayersFromConsole(bool tryToIncludePlayersWithLessThan3Matches);
	bool addPlayerByName(const char* name);
	Player* getPlayerByNameInLineup(const char* name) const;
	bool isValid() const;
	void markMatchPlayed();

	void tryIncludePlayersWithLessThan3Matches();

	void chooseAutomatically();
	Player* getPlayerAtIndex(int index) const;

	void print() const;
protected:
	Team* team;
	Player* playersInLineup[MAX_LINEUP_SIZE];
	int lineupSize;

	bool isPlayerAlreadySelected(Player* player) const;
	bool canAddPlayerWithoutBreakingLineup(Player* player) const;
};

