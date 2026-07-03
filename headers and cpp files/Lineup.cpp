#define _CRT_SECURE_NO_WARNINGS
#include "Lineup.h"
#include <iostream>
#include <stdexcept>
#include <cstring>

Lineup::Lineup()
{
    team = nullptr;
    lineupSize = 0;

    for (int i = 0; i < MAX_LINEUP_SIZE; i++)
    {
        playersInLineup[i] = nullptr;
    }
}

Lineup::Lineup(Team* team)
{
    if (!team) {
        throw std::invalid_argument("Invalid team");
    }

    this->team = team;
    lineupSize = 0;

    for (int i = 0; i < MAX_LINEUP_SIZE; i++)
    {
        playersInLineup[i] = nullptr;
    }
}

void Lineup::choosePlayersFromConsole()
{
    choosePlayersFromConsole(false);
}

void Lineup::choosePlayersFromConsole(bool tryToIncludePlayersWithLessThan3Matches)
{
    if (tryToIncludePlayersWithLessThan3Matches)
    {
        std::cout << "Trying to include players with less than 3 matches...\n";
        this->tryIncludePlayersWithLessThan3Matches();
    }

    char name[128];

    while (lineupSize < MAX_LINEUP_SIZE)
    {
        std::cout << "Enter player name: ";
        std::cin.getline(name, 128);

        if (!addPlayerByName(name))
        {
            std::cout << "Player not found, already selected, or lineup is full. Try again.\n";
        }
    }
}

bool Lineup::addPlayerByName(const char* name)
{
    if (!team || !name)
        return false;

    if (lineupSize >= MAX_LINEUP_SIZE)
        return false;

    Player* player = team->getPlayerByName(name);

    if (!player)
        return false;

    for (int i = 0; i < lineupSize; i++)
    {
        if (playersInLineup[i] == player)
            return false;
    }

    playersInLineup[lineupSize] = player;
    lineupSize++;

    return true;
}

Player* Lineup::getPlayerByNameInLineup(const char* name) const
{
    if (!name)
        return nullptr;

    for (int i = 0; i < lineupSize; i++)
    {
        if (playersInLineup[i] && std::strcmp(playersInLineup[i]->getName(), name) == 0)
            return playersInLineup[i];
    }

    return nullptr;
}

bool Lineup::isValid() const
{
    if (lineupSize != MAX_LINEUP_SIZE)
        return false;

    int goalkeepers = 0;
    int defenders = 0;
    int midfielders = 0;
    int wingers = 0;
    int forwards = 0;

    for (int i = 0; i < lineupSize; i++)
    {
        switch (playersInLineup[i]->getPosition())
        {
        case Position::GOALKEEPER:
            goalkeepers++;
            break;
        case Position::DEFENDER:
            defenders++;
            break;
        case Position::MIDFIELDER:
            midfielders++;
            break;
        case Position::WINGER:
            wingers++;
            break;
        case Position::FORWARD:
            forwards++;
            break;
        default:
            break;
        }
    }

    return goalkeepers == 1 &&
        defenders >= 2 &&
        midfielders >= 2 &&
        wingers >= 2 &&
        forwards >= 2;
}

void Lineup::markMatchPlayed()
{
    for (int i = 0; i < lineupSize; i++)
    {
        playersInLineup[i]->setMatches(
            playersInLineup[i]->getMatches() + 1);
    }
}

void Lineup::print() const
{
    std::cout << "Lineup:\n";

    for (int i = 0; i < lineupSize; i++)
    {
        std::cout << "\nPlayer " << i + 1 << ":\n";
        playersInLineup[i]->print();
    }
}

bool Lineup::isPlayerAlreadySelected(Player* player) const
{
    if (!player)
        return false;

    for (int i = 0; i < lineupSize; i++)
    {
        if (playersInLineup[i] == player)
            return true;
    }

    return false;
}

bool Lineup::canAddPlayerWithoutBreakingLineup(Player* player) const
{
    if (!player)
        return false;

    if (lineupSize >= MAX_LINEUP_SIZE)
        return false;

    if (isPlayerAlreadySelected(player))
        return false;

    int goalkeepers = 0;
    int defenders = 0;
    int midfielders = 0;
    int wingers = 0;
    int forwards = 0;

    for (int i = 0; i < lineupSize; i++)
    {
        if (!playersInLineup[i])
            continue;

        switch (playersInLineup[i]->getPosition())
        {
        case Position::GOALKEEPER:
            goalkeepers++;
            break;
        case Position::DEFENDER:
            defenders++;
            break;
        case Position::MIDFIELDER:
            midfielders++;
            break;
        case Position::WINGER:
            wingers++;
            break;
        case Position::FORWARD:
            forwards++;
            break;
        default:
            break;
        }
    }

    switch (player->getPosition())
    {
    case Position::GOALKEEPER:
        goalkeepers++;
        break;
    case Position::DEFENDER:
        defenders++;
        break;
    case Position::MIDFIELDER:
        midfielders++;
        break;
    case Position::WINGER:
        wingers++;
        break;
    case Position::FORWARD:
        forwards++;
        break;
    default:
        return false;
    }

    if (goalkeepers > 1)
        return false;

    int remainingPlaces = MAX_LINEUP_SIZE - (lineupSize + 1);

    int missingPlayers = 0;

    if (goalkeepers < 1)
        missingPlayers += 1 - goalkeepers;

    if (defenders < 2)
        missingPlayers += 2 - defenders;

    if (midfielders < 2)
        missingPlayers += 2 - midfielders;

    if (wingers < 2)
        missingPlayers += 2 - wingers;

    if (forwards < 2)
        missingPlayers += 2 - forwards;

    return missingPlayers <= remainingPlaces;
}

void Lineup::tryIncludePlayersWithLessThan3Matches()
{
    if (!team)
        return;

    for (int i = 0; i < team->getCurrentSize(); i++)
    {
        const Player& currentPlayer = team->getPlayer(i);

        if (currentPlayer.getMatches() < 3)
        {
            Player* player = team->getPlayerByName(currentPlayer.getName());

            if (canAddPlayerWithoutBreakingLineup(player))
            {
                playersInLineup[lineupSize] = player;
                lineupSize++;

                std::cout << "Automatically added player with less than 3 matches: "
                    << player->getName() << '\n';
            }
        }

        if (lineupSize >= MAX_LINEUP_SIZE)
            return;
    }
}

void Lineup::chooseAutomatically()
{
    if (!team)
        throw std::logic_error("No team for lineup");

    lineupSize = 0;

    for (int i = 0; i < MAX_LINEUP_SIZE; i++)
    {
        playersInLineup[i] = nullptr;
    }

    int neededGoalkeepers = 1;
    int neededDefenders = 2;
    int neededMidfielders = 2;
    int neededWingers = 2;
    int neededForwards = 4;

    for (int i = 0; i < team->getCurrentSize(); i++)
    {
        Player* player = team->getPlayerByName(team->getPlayer(i).getName());

        if (!player)
            continue;

        if (player->getPosition() == Position::GOALKEEPER && neededGoalkeepers > 0)
        {
            playersInLineup[lineupSize++] = player;
            neededGoalkeepers--;
        }
        else if (player->getPosition() == Position::DEFENDER && neededDefenders > 0)
        {
            playersInLineup[lineupSize++] = player;
            neededDefenders--;
        }
        else if (player->getPosition() == Position::MIDFIELDER && neededMidfielders > 0)
        {
            playersInLineup[lineupSize++] = player;
            neededMidfielders--;
        }
        else if (player->getPosition() == Position::WINGER && neededWingers > 0)
        {
            playersInLineup[lineupSize++] = player;
            neededWingers--;
        }
        else if (player->getPosition() == Position::FORWARD && neededForwards > 0)
        {
            playersInLineup[lineupSize++] = player;
            neededForwards--;
        }

        if (lineupSize == MAX_LINEUP_SIZE)
            break;
    }

    if (!isValid())
        throw std::logic_error("Could not create automatic valid lineup");

    std::cout << "Automatic lineup selected:\n";
    print();
}

Player* Lineup::getPlayerAtIndex(int index) const
{
    if (index < 0 || index >= lineupSize)
        return nullptr;

    return playersInLineup[index];
}
