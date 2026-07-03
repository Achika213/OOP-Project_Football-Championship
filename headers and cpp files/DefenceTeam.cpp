#include "DefenceTeam.h"
#include <iostream>
#include <stdexcept>

DefenceTeam::DefenceTeam()
	: Team()
{
}

DefenceTeam::DefenceTeam(const char* name, const char* coach,
	const char* stadium,
	double budget,
	int matchesForSeason,
	int wins,
	int draws,
	int loses,
	int points,
	int allGoals,
	int allConcededGoals)
	: Team(name, coach, stadium, budget,
		matchesForSeason, wins, draws, loses,
		points, allGoals, allConcededGoals)
{
}

bool DefenceTeam::canBuyPlayer(const Player& player) const
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

bool DefenceTeam::isValidTeam() const
{

	return countByPosition(Position::GOALKEEPER) == 2 &&
		countByPosition(Position::DEFENDER) >= 6 &&
		countByPosition(Position::MIDFIELDER) >= 6 &&
		countByPosition(Position::WINGER) >= 2 &&
		countByPosition(Position::FORWARD) >= 2;
}

Team* DefenceTeam::clone() const
{
	return new DefenceTeam(*this);
}

void DefenceTeam::printTeamInfo() const
{
	std::cout << "Defence team info:\n";
	Team::printTeamInfo();
}

const char* DefenceTeam::getType() const
{
	return "DefenceTeam";
}