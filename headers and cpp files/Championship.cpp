#include "Championship.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <cstring>

#include "AttackTeam.h"
#include "DefenceTeam.h"
#include "BalanceTeam.h"

Championship::Championship()
{
    year = 0;

    capacity = 4;
    teamCount = 0;
    teams = new Team * [capacity];

    for (int i = 0; i < capacity; i++)
    {
        teams[i] = nullptr;
    }

    matches = nullptr;
    matchesCount = 0;

    currentRound = 1;
    finished = false;
}
Championship::Championship(int year)
{
    if (year <= 0)
        throw std::invalid_argument("Invalid year");

    this->year = year;

    capacity = 4;
    teamCount = 0;
    teams = new Team * [capacity];

    for (int i = 0; i < capacity; i++)
        teams[i] = nullptr;

    matches = nullptr;
    matchesCount = 0;

    currentRound = 1;
    finished = false;
}

Championship::Championship(const Championship& other)
{
    teams = nullptr;
    matches = nullptr;

    copyFromChampionship(other);
}

Championship& Championship::operator=(const Championship& other)
{
    if (this != &other)
    {
        copyFromChampionship(other);
    }
    return *this;
}

Championship::~Championship()
{
    freeChampionship();
}

void Championship::addTeam(const Team& team)
{
    if (teamCount == capacity)
    {
        int newCapacity = capacity == 0 ? 4 : capacity * 2;

        Team** newTeams = new Team * [newCapacity];

        for (int i = 0; i < newCapacity; i++)
        {
            newTeams[i] = nullptr;
        }

        for (int i = 0; i < teamCount; i++)
        {
            newTeams[i] = teams[i];
        }

        delete[] teams;

        teams = newTeams;
        capacity = newCapacity;
    }

    teams[teamCount] = team.clone();
    teamCount++;
}

bool Championship::isValidTeamCount() const
{
    return teamCount >= 4 && teamCount % 2 == 0;
}

void Championship::generateSchedule()
{
    if (!isValidTeamCount())
        throw std::logic_error("Invalid team count. Teams must be at least 4 and even.");

    delete[] matches;

    int roundsPerHalf = teamCount - 1;
    int matchesPerRound = teamCount / 2;

    int totalRounds = roundsPerHalf * 2;

    matchesCount = teamCount * (teamCount - 1);
    matches = new Match[matchesCount];

    int* indexes = new int[teamCount];

    for (int i = 0; i < teamCount; i++)
    {
        indexes[i] = i;
    }

    int matchIndex = 0;

    for (int round = 1; round <= roundsPerHalf; round++)
    {
        for (int i = 0; i < matchesPerRound; i++)
        {
            int firstTeamIndex = indexes[i];
            int secondTeamIndex = indexes[teamCount - 1 - i];

            if ((round + i) % 2 == 0)
            {
                matches[matchIndex] = 
                    Match(teams[firstTeamIndex], teams[secondTeamIndex], round, totalRounds);
                matchIndex++;
            }
            else
            {
                matches[matchIndex] = Match(teams[secondTeamIndex], teams[firstTeamIndex], round + roundsPerHalf, totalRounds);
                matchIndex++;
            }
        }

        int last = indexes[teamCount - 1];

        for (int i = teamCount - 1; i > 1; i--)
        {
            indexes[i] = indexes[i - 1];
        }

        indexes[1] = last;
    }

    for (int i = 0; i < roundsPerHalf; i++)
    {
        int start = i * matchesPerRound;
        int secondLegRound = i + 1 + roundsPerHalf;

        for (int j = 0; j < matchesPerRound; j++)
        {
            Match firstLeg = matches[start + j];

            int matchPosition = start + j;

            Team* home = nullptr;
            Team* away = nullptr;
        }
    }

    delete[] matches;

    matches = new Match[matchesCount];
    matchIndex = 0;

    for (int i = 0; i < teamCount; i++)
    {
        indexes[i] = i;
    }

    for (int round = 1; round <= roundsPerHalf; round++)
    {
        for (int i = 0; i < matchesPerRound; i++)
        {
            int firstTeamIndex = indexes[i];
            int secondTeamIndex = indexes[teamCount - 1 - i];

            if ((round + i) % 2 == 0)
            {
                matches[matchIndex] = Match(teams[firstTeamIndex], teams[secondTeamIndex], round, totalRounds);
                matchIndex++;

                matches[matchIndex] = Match(teams[secondTeamIndex], teams[firstTeamIndex], round + roundsPerHalf, totalRounds);
                matchIndex++;
            }
            else
            {
                matches[matchIndex] = Match(teams[secondTeamIndex], teams[firstTeamIndex], round + roundsPerHalf, totalRounds);
                matchIndex++;

                matches[matchIndex] = Match(teams[firstTeamIndex], teams[secondTeamIndex], round, totalRounds);
                matchIndex++;
            }
        }

        int last = indexes[teamCount - 1];

        for (int i = teamCount - 1; i > 1; i--)
        {
            indexes[i] = indexes[i - 1];
        }

        indexes[1] = last;
    }

    delete[] indexes;

    currentRound = 1;
    finished = false;
}

void Championship::playRound()
{
    if (finished)
    {
        throw std::logic_error("Championship already finished");
    }

    if (!matches || matchesCount == 0)
    {
        throw std::logic_error("Schedule is not generated");
    }

    int mode;

    std::cout << "Choose round mode:\n";
    std::cout << "1. Manual round\n";
    std::cout << "2. Automatic round\n";
    std::cout << "Choice: ";
    std::cin >> mode;
    std::cin.ignore();

    bool playedMatch = false;

    for (int i = 0; i < matchesCount; i++)
    {
        if (matches[i].getRound() == currentRound)
        {
            if (mode == 1)
            {
                matches[i].playMatch();
            }
            else if (mode == 2)
            {
                matches[i].playMatchAutomatically();
            }
            else
            {
                throw std::invalid_argument("Invalid round mode");
            }

            playedMatch = true;
        }
    }

    if (playedMatch)
    {
        currentRound++;
    }

    int totalRounds = (teamCount - 1) * 2;

    if (currentRound > totalRounds)
    {
        finished = true;
    }
}

void Championship::playAllRounds()
{
    while (!finished)
    {
        playRound();
    }
}

void Championship::printStandings() const
{
    Team** sorted = new Team * [teamCount];

    for (int i = 0; i < teamCount; i++)
        sorted[i] = teams[i];

    for (int i = 0; i < teamCount - 1; i++)
    {
        for (int j = 0; j < teamCount - i - 1; j++)
        {
            int gd1 = sorted[j]->getAllGoals() - sorted[j]->getAllConcededGoals();
            int gd2 = sorted[j + 1]->getAllGoals() - sorted[j + 1]->getAllConcededGoals();

            if (sorted[j]->getPoints() < sorted[j + 1]->getPoints() ||
                (sorted[j]->getPoints() == sorted[j + 1]->getPoints() && gd1 < gd2))
            {
                Team* temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    std::cout << "Standings:\n";

    for (int i = 0; i < teamCount; i++)
    {
        int gd = sorted[i]->getAllGoals() - sorted[i]->getAllConcededGoals();

        std::cout << i + 1 << ". "
            << sorted[i]->getNameTeam()
            << " | Points: " << sorted[i]->getPoints()
            << " | GD: " << gd
            << " | Goals: " << sorted[i]->getAllGoals()
            << ":" << sorted[i]->getAllConcededGoals()
            << '\n';
    }

    delete[] sorted;
}

const Team* Championship::getChampion() const
{
    if (teamCount == 0)
        return nullptr;

    int best = 0;

    for (int i = 1; i < teamCount; i++)
    {
        int gdCurrent =
            teams[i]->getAllGoals() - teams[i]->getAllConcededGoals();

        int gdBest =
            teams[best]->getAllGoals() - teams[best]->getAllConcededGoals();

        if (teams[i]->getPoints() > teams[best]->getPoints() ||
            (teams[i]->getPoints() == teams[best]->getPoints() &&
                gdCurrent > gdBest))
        {
            best = i;
        }
    }

    return teams[best];
}

const Team* Championship::getRunnerUp() const
{
    if (teamCount < 2)
        return nullptr;

    const Team* champion = getChampion();

    int best = -1;

    for (int i = 0; i < teamCount; i++)
    {
        if (teams[i] == champion)
            continue;

        if (best == -1)
        {
            best = i;
            continue;
        }

        int gdCurrent = teams[i]->getAllGoals() - teams[i]->getAllConcededGoals();
        int gdBest = teams[best]->getAllGoals() - teams[best]->getAllConcededGoals();

        if (teams[i]->getPoints() > teams[best]->getPoints() ||
            (teams[i]->getPoints() == teams[best]->getPoints() && gdCurrent > gdBest))
        {
            best = i;
        }
    }

    return teams[best];
}

const Team* Championship::getThirdPlace() const
{
    if (teamCount < 3)
        return nullptr;

    const Team* champion = getChampion();
    const Team* runnerUp = getRunnerUp();

    int best = -1;

    for (int i = 0; i < teamCount; i++)
    {
        if (teams[i] == champion || teams[i] == runnerUp)
            continue;

        if (best == -1)
        {
            best = i;
            continue;
        }

        int gdCurrent = teams[i]->getAllGoals() - teams[i]->getAllConcededGoals();
        int gdBest = teams[best]->getAllGoals() - teams[best]->getAllConcededGoals();

        if (teams[i]->getPoints() > teams[best]->getPoints() ||
            (teams[i]->getPoints() == teams[best]->getPoints() && gdCurrent > gdBest))
        {
            best = i;
        }
    }

    return teams[best];

}

void Championship::printTopScorers() const
{
    int bestGoals = -1;
    int bestMatches = 1000000;

    for (int i = 0; i < teamCount; i++)
    {
        for (int j = 0; j < teams[i]->getCurrentSize(); j++)
        {
            const Player& p = teams[i]->getPlayer(j);

            if (p.getMatches() < 7)
                continue;

            if (p.getGoals() > bestGoals)
            {
                bestGoals = p.getGoals();
                bestMatches = p.getMatches();
            }
            else if (p.getGoals() == bestGoals &&
                p.getMatches() < bestMatches)
            {
                bestMatches = p.getMatches();
            }
        }
    }

    if (bestGoals == -1)
    {
        std::cout << "No eligible top scorers.\n";
        return;
    }

    std::cout << "Top scorer(s):\n";

    for (int i = 0; i < teamCount; i++)
    {
        for (int j = 0; j < teams[i]->getCurrentSize(); j++)
        {
            const Player& p = teams[i]->getPlayer(j);

            if (p.getMatches() >= 7 &&
                p.getGoals() == bestGoals &&
                p.getMatches() == bestMatches)
            {
                p.print();
                std::cout << '\n';
            }
        }
    }
}

int Championship::getYear() const
{
    return year;
}

bool Championship::isFinished() const
{
    return finished;
}

void Championship::copyFromChampionship(const Championship& other)
{
    year = other.year;
    teamCount = other.teamCount;
    capacity = other.capacity;
    currentRound = other.currentRound;
    finished = other.finished;

    matchesCount = 0;
    matches = nullptr;

    teams = new Team*[capacity];

    for (int i = 0; i < capacity; i++)
    {
        teams[i] = nullptr;
    }

    try
    {
        for (int i = 0; i < teamCount; i++)
        {
            if (other.teams[i])
                teams[i] = other.teams[i]->clone();
        }
    }
    catch (...)
    {
        for (int i = 0; i < capacity; i++)
        {
            delete teams[i];
        }

        delete[] teams;

        teams = nullptr;
        teamCount = 0;
        capacity = 0;
        currentRound = 1;
        finished = false;

        throw;
    }
}

void Championship::freeChampionship()
{
    if (teams)
    {
        for (int i = 0; i < teamCount; i++)
        {
            delete teams[i];
        }

        delete[] teams;
    }

    delete[] matches;

    teams = nullptr;
    matches = nullptr;
    teamCount = 0;
    capacity = 0;
    matchesCount = 0;
    currentRound = 1;
    finished = false;
}

Team* Championship::getTeamByName(const char* teamName)
{
    if (!teamName)
        return nullptr;

    for (int i = 0; i < teamCount; i++)
    {
        if (teams[i] && strcmp(teams[i]->getNameTeam(), teamName) == 0)
            return teams[i];
    }

    return nullptr;
}

const Team* Championship::getTeamByName(const char* teamName) const
{
    if (!teamName)
        return nullptr;

    for (int i = 0; i < teamCount; i++)
    {
        if (teams[i] && strcmp(teams[i]->getNameTeam(), teamName) == 0)
            return teams[i];
    }

    return nullptr;
}

void Championship::printTeams() const
{
    if (teamCount == 0)
    {
        std::cout << "No teams in this championship.\n";
        return;
    }

    std::cout << "Teams:\n";

    for (int i = 0; i < teamCount; i++)
    {
        if (teams[i])
            std::cout << i + 1 << ". " << teams[i]->getNameTeam() << '\n';
    }
}

void Championship::printFinalResults() const
{
    if (teamCount < 3)
    {
        std::cout << "Not enough teams for final results.\n";
        return;
    }

    Team** sortedTeams = new Team * [teamCount];

    for (int i = 0; i < teamCount; i++)
    {
        sortedTeams[i] = teams[i];
    }

    for (int i = 0; i < teamCount - 1; i++)
    {
        for (int j = 0; j < teamCount - i - 1; j++)
        {
            int currentGoalDifference = sortedTeams[j]->getAllGoals() - sortedTeams[j]->getAllConcededGoals();
            int nextGoalDifference = sortedTeams[j + 1]->getAllGoals() - sortedTeams[j + 1]->getAllConcededGoals();

            bool shouldSwap = false;

            if (sortedTeams[j]->getPoints() < sortedTeams[j + 1]->getPoints())
            {
                shouldSwap = true;
            }
            else if (sortedTeams[j]->getPoints() == sortedTeams[j + 1]->getPoints() &&
                currentGoalDifference < nextGoalDifference)
            {
                shouldSwap = true;
            }

            if (shouldSwap)
            {
                Team* temp = sortedTeams[j];
                sortedTeams[j] = sortedTeams[j + 1];
                sortedTeams[j + 1] = temp;
            }
        }
    }

    std::cout << "\n===== FINAL RESULTS =====\n";
    std::cout << "Champion: " << sortedTeams[0]->getNameTeam() << '\n';
    std::cout << "Runner-up: " << sortedTeams[1]->getNameTeam() << '\n';
    std::cout << "Third place: " << sortedTeams[2]->getNameTeam() << '\n';

    const Player* topScorers[100];
    int topScorersCount = 0;
    int maxGoals = -1;
    int minMatchesForTopScorer = 1000000;

    for (int i = 0; i < teamCount; i++)
    {
        for (int j = 0; j < teams[i]->getCurrentSize(); j++)
        {
            const Player& player = teams[i]->getPlayer(j);

            if (player.getMatches() < 7)
                continue;

            if (player.getGoals() > maxGoals)
            {
                maxGoals = player.getGoals();
                minMatchesForTopScorer = player.getMatches();

                topScorersCount = 0;
                topScorers[topScorersCount] = &player;
                topScorersCount++;
            }
            else if (player.getGoals() == maxGoals)
            {
                if (player.getMatches() < minMatchesForTopScorer)
                {
                    minMatchesForTopScorer = player.getMatches();

                    topScorersCount = 0;
                    topScorers[topScorersCount] = &player;
                    topScorersCount++;
                }
                else if (player.getMatches() == minMatchesForTopScorer)
                {
                    topScorers[topScorersCount] = &player;
                    topScorersCount++;
                }
            }
        }
    }

    if (topScorersCount == 0)
    {
        std::cout << "Top scorer: No player with at least 7 matches.\n";
    }
    else
    {
        std::cout << "Top scorer";

        if (topScorersCount > 1)
            std::cout << "s";

        std::cout << ":\n";

        for (int i = 0; i < topScorersCount; i++)
        {
            std::cout << "- " << topScorers[i]->getName()
                << " | Goals: " << topScorers[i]->getGoals()
                << " | Matches: " << topScorers[i]->getMatches()
                << '\n';
        }
    }

    delete[] sortedTeams;
}

Team* createTeamByType(const char* type)
{
    if (strcmp(type, "AttackTeam") == 0)
        return new AttackTeam();

    if (strcmp(type, "DefenceTeam") == 0)
        return new DefenceTeam();

    if (strcmp(type, "BalanceTeam") == 0)
        return new BalanceTeam();

    throw std::invalid_argument("Invalid team type in file");
}

void Championship::saveToFile(std::ofstream& out) const
{
    if (!out)
        throw std::logic_error("Invalid output file");

    out << year << '\n';
    out << currentRound << '\n';
    out << finished << '\n';

    out << teamCount << '\n';

    for (int i = 0; i < teamCount; i++)
    {
        if (teams[i])
            teams[i]->saveToFile(out);
    }

}

void Championship::loadFromFile(std::ifstream& in)
{
    if (!in)
        throw std::logic_error("Invalid input file");

    freeChampionship();

    in >> year;
    in >> currentRound;
    in >> finished;

    in >> teamCount;
    in.ignore();

    capacity = teamCount;
    teams = new Team * [capacity];

    for (int i = 0; i < teamCount; i++)
    {
        char type[128];

        in.getline(type, 128);

        teams[i] = createTeamByType(type);
        teams[i]->loadFromFile(in);
    }

    matches = nullptr;
    matchesCount = 0;
}