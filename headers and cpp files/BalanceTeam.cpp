#include "BalanceTeam.h"
#include <stdexcept>
#include <iostream>

BalanceTeam::BalanceTeam()
	: Team()
{
}

BalanceTeam::BalanceTeam(const char* name,
	const char* coach,
	const char* stadium,
	double budget,
	int matchesForSeason,
	int wins, int draws,
	int loses,
	int points,
	int allGoals,
	int allConcededGoals)
	: Team(name, coach, stadium, budget,
		matchesForSeason, wins, draws, loses,
		points, allGoals, allConcededGoals)
{
}

bool BalanceTeam::canBuyPlayer(const Player& player) const
{
	if (currentSize >= maxSizeTeam)
		return false;

	if (hasPlayerWithName(player.getName()))
		return false;

	if (hasPlayerWithNumber(player.getNumber()))
		return false;

	if (budget < player.getTransferSum())
		return false;

	if (player.getPosition() == Position::GOALKEEPER &&
		countByPosition(Position::GOALKEEPER) >= 2)
		return false;

	return true;
}

bool BalanceTeam::isValidTeam() const
{
	return countByPosition(Position::GOALKEEPER) == 2 &&
		countByPosition(Position::DEFENDER) >= 4 &&
		countByPosition(Position::MIDFIELDER) >= 4 &&
		countByPosition(Position::WINGER) >= 4 &&
		countByPosition(Position::FORWARD) >= 4;
}

Team* BalanceTeam::clone() const
{
	return new BalanceTeam(*this);
}

void BalanceTeam::printTeamInfo() const
{
	std::cout << "Balance team info:\n";
	Team::printTeamInfo();
}

const char* BalanceTeam::getType() const
{
	return "BalanceTeam";
}