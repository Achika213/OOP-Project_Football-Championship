#pragma once
#include "Team.h"

class AttackTeam : public Team
{
public:
	AttackTeam();
	AttackTeam(const char* name,
		const char* coach,
		const char* stadium,
		double budget,
		int matchesForSeason,
		int wins,
		int draws,
		int loses,
		int points,
		int allGoals,
		int allConcededGoals);

	bool canBuyPlayer(const Player& player) const override;
	bool isValidTeam() const override;
	Team* clone() const override;
	void printTeamInfo() const override;

	const char* getType() const override;
};
