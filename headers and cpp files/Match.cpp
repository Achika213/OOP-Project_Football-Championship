#include "Match.h"
#include <stdexcept>
#include <iostream>

Match::Match()
    : homeLineup(), awayLineup()
{
    homeTeam = nullptr;
    awayTeam = nullptr;
    homeGoals = 0;
    awayGoals = 0;
    round = 0;
    played = false;

    goalScorersCount = 0;
    for (int i = 0; i < MAX_GOAL_SCORERS_PER_MATCH; i++)
    {
        goalScorers[i] = nullptr;
    }

    totalRounds = 0;
}

Match::Match(Team* homeTeam, Team* awayTeam, int round, int totalRounds)
    : homeLineup(homeTeam), awayLineup(awayTeam)
{
    if (!homeTeam || !awayTeam)
        throw std::invalid_argument("Invalid teams");

    if (round <= 0)
        throw std::invalid_argument("Invalid round");

    this->homeTeam = homeTeam;
    this->awayTeam = awayTeam;
    this->round = round;

    this->totalRounds = totalRounds;

    homeGoals = 0;
    awayGoals = 0;
    played = false;

    goalScorersCount = 0;
    for (int i = 0; i < MAX_GOAL_SCORERS_PER_MATCH; i++)
    {
        goalScorers[i] = nullptr;
    }
}

void Match::chooseLineups()
{
    int mode;

    std::cout << "Choose lineup mode:\n";
    std::cout << "1. Manual\n";
    std::cout << "2. Automatic\n";
    std::cout << "Choice: ";
    std::cin >> mode;
    std::cin.ignore();

    bool shouldTry = shouldTryIncludePlayersWithLessThan3Matches();

    if (mode == 1)
    {
        std::cout << "Choose lineup for home team:\n";
        homeLineup.choosePlayersFromConsole(shouldTry);

        std::cout << "Choose lineup for away team:\n";
        awayLineup.choosePlayersFromConsole(shouldTry);
    }
    else if (mode == 2)
    {
        std::cout << "Automatic lineup for home team:\n";
        homeLineup.chooseAutomatically();

        std::cout << "Automatic lineup for away team:\n";
        awayLineup.chooseAutomatically();
    }
    else
    {
        throw std::invalid_argument("Invalid lineup mode");
    }
}

bool Match::shouldTryIncludePlayersWithLessThan3Matches() const
{
    if (totalRounds <= 0)
        return false;

    return round > (2 * totalRounds) / 3;
}

void Match::enterResult()
{
    std::cout << "Home goals: ";
    std::cin >> homeGoals;

    std::cout << "Away goals: ";
    std::cin >> awayGoals;

    if (homeGoals < 0 || awayGoals < 0)
        throw std::invalid_argument("Invalid result");

    if (homeGoals + awayGoals > MAX_GOAL_SCORERS_PER_MATCH)
        throw std::logic_error("Too many goals for the scorers list");

    std::cin.ignore();
}

void Match::playMatch()
{
    chooseLineups();

    if (!homeLineup.isValid() || !awayLineup.isValid())
        throw std::logic_error("Invalid lineup");

    enterResult();
    enterGoalScorers();

    homeTeam->updateStatistics(homeGoals, awayGoals);
    awayTeam->updateStatistics(awayGoals, homeGoals);

    homeLineup.markMatchPlayed();
    awayLineup.markMatchPlayed();

    played = true;
}

void Match::playMatchAutomatically()
{
    homeLineup.chooseAutomatically();
    awayLineup.chooseAutomatically();

    if (!homeLineup.isValid() || !awayLineup.isValid())
        throw std::logic_error("Invalid automatic lineup");

    goalScorersCount = 0;

    int base = round + homeTeam->getPoints() + awayTeam->getPoints();

    homeGoals = base % 4;
    awayGoals = (base + 1) % 3;

    if (homeGoals + awayGoals > MAX_GOAL_SCORERS_PER_MATCH)
        throw std::logic_error("Too many goals");

    addAutomaticGoalScorers(homeLineup, homeGoals);
    addAutomaticGoalScorers(awayLineup, awayGoals);

    homeTeam->updateStatistics(homeGoals, awayGoals);
    awayTeam->updateStatistics(awayGoals, homeGoals);

    homeLineup.markMatchPlayed();
    awayLineup.markMatchPlayed();

    played = true;

    std::cout << "Automatic match played:\n";
    print();
}

void Match::addAutomaticGoalScorers(Lineup& lineup, int goals)
{
    for (int i = 0; i < goals; i++)
    {
        Player* scorer = nullptr;

        for (int j = 0; j < MAX_LINEUP_SIZE; j++)
        {
            Player* current = lineup.getPlayerAtIndex(j);

            if (!current)
                continue;

            if (current->getPosition() == Position::FORWARD)
            {
                scorer = current;
                break;
            }
        }

        if (!scorer)
        {
            for (int j = 0; j < MAX_LINEUP_SIZE; j++)
            {
                Player* current = lineup.getPlayerAtIndex(j);

                if (current)
                {
                    scorer = current;
                    break;
                }
            }
        }

        if (scorer)
        {
            scorer->setGoals(scorer->getGoals() + 1);

            if (goalScorersCount < MAX_GOAL_SCORERS_PER_MATCH)
            {
                goalScorers[goalScorersCount] = scorer;
                goalScorersCount++;
            }
        }
    }
}

void Match::enterGoalScorers()
{
    goalScorersCount = 0;

    char teamChoice;
    char scorerName[128];

    const int totalGoals = homeGoals + awayGoals;

    for (int i = 0; i < totalGoals; i++)
    {
        Player* scorer = nullptr;

        while (!scorer)
        {
            std::cout << "Goal " << i + 1 << " is for which team? (h - home, a - away): ";
            std::cin >> teamChoice;

            std::cin.ignore();

            std::cout << "Enter scorer name: ";
            std::cin.getline(scorerName, 128);

            if (teamChoice == 'h' || teamChoice == 'H')
            {
                scorer = homeLineup.getPlayerByNameInLineup(scorerName);
            }
            else if (teamChoice == 'a' || teamChoice == 'A')
            {
                scorer = awayLineup.getPlayerByNameInLineup(scorerName);
            }
            else
            {
                std::cout << "Invalid team choice. Try again.\n";
                continue;
            }

            if (!scorer)
            {
                std::cout << "This player is not in the selected lineup. Try again.\n";
            }
        }

        scorer->setGoals(scorer->getGoals() + 1);

        goalScorers[goalScorersCount] = scorer;
        goalScorersCount++;
    }
}

int Match::getHomeGoals() const
{
    return homeGoals;
}

int Match::getAwayGoals() const
{
    return awayGoals;
}

int Match::getRound() const
{
    return round;
}

void Match::print() const
{
    std::cout << "Round: " << round << '\n';

    if (homeTeam)
        std::cout << "Home team: " << homeTeam->getNameTeam() << '\n';

    if (awayTeam)
        std::cout << "Away team: " << awayTeam->getNameTeam() << '\n';

    std::cout << "Result: " << homeGoals << " : " << awayGoals << '\n';
    std::cout << "Played: " << (played ? "Yes" : "No") << '\n';

    if (goalScorersCount > 0)
    {
        std::cout << "Goal scorers:\n";
        for (int i = 0; i < goalScorersCount; i++)
        {
            if (goalScorers[i])
                std::cout << "- " << goalScorers[i]->getName() << '\n';
        }
    }
}