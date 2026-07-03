#include "AttackTeam.h"
#include <iostream>
#include <stdexcept>

AttackTeam::AttackTeam()
    : Team()
{
}

AttackTeam::AttackTeam(const char* name,
    const char* coach,
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

bool AttackTeam::canBuyPlayer(const Player& player) const
{
    if (currentSize >= maxSizeTeam)
        return false;

    if (hasPlayerWithName(player.getName()))
        return false;

    if (hasPlayerWithNumber(player.getNumber()))
        return false;

    if (player.getPosition() == Position::FORWARD &&
        player.getGoals() < 6)
        return false;

    if (budget < player.getTransferSum())
        return false;

    if (player.getPosition() == Position::GOALKEEPER &&
        countByPosition(Position::GOALKEEPER) >= 2)
        return false;

    return true;
}

bool AttackTeam::isValidTeam() const
{
    return countByPosition(Position::GOALKEEPER) == 2 &&
        countByPosition(Position::FORWARD) >= 8 &&
        countByPosition(Position::MIDFIELDER) >= 2 &&
        countByPosition(Position::DEFENDER) >= 2 &&
        countByPosition(Position::WINGER) >= 2;
}

Team* AttackTeam::clone() const
{
    return new AttackTeam(*this);
}

void AttackTeam::printTeamInfo() const
{
    std::cout << "Attack team info:\n";
    Team::printTeamInfo();
}

const char* AttackTeam::getType() const
{
    return "AttackTeam";
}