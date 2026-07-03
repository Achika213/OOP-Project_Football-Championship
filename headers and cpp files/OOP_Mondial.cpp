#include <iostream>
#include <stdexcept>

#include "Player.h"
#include "AttackTeam.h"
#include "DefenceTeam.h"
#include "BalanceTeam.h"
#include "Lineup.h"
#include "Match.h"
#include "Championship.h"
#include "ChampionshipCollection.h"

Position readPosition()
{
    int choice;

    std::cout << "Choose position:\n";
    std::cout << "1. Goalkeeper\n";
    std::cout << "2. Defender\n";
    std::cout << "3. Midfielder\n";
    std::cout << "4. Winger\n";
    std::cout << "5. Forward\n";
    std::cout << "Choice: ";
    std::cin >> choice;
    std::cin.ignore();

    switch (choice)
    {
    case 1:
        return Position::GOALKEEPER;
    case 2:
        return Position::DEFENDER;
    case 3:
        return Position::MIDFIELDER;
    case 4:
        return Position::WINGER;
    case 5:
        return Position::FORWARD;
    default:
        throw std::invalid_argument("Invalid position");
    }
}

void addPlayersFromConsole(Team& team)
{
    int playersCount;

    std::cout << "How many players do you want to add? ";
    std::cin >> playersCount;
    std::cin.ignore();

    for (int i = 0; i < playersCount; i++)
    {
        char name[128];
        int number;
        double salary;
        double transferSum;
        int matches;
        int goals;

        std::cout << "\nPlayer " << i + 1 << ":\n";

        std::cout << "Name: ";
        std::cin.getline(name, 128);

        std::cout << "Number: ";
        std::cin >> number;

        Position position = readPosition();

        std::cout << "Salary: ";
        std::cin >> salary;

        std::cout << "Transfer sum: ";
        std::cin >> transferSum;

        std::cout << "Matches: ";
        std::cin >> matches;

        std::cout << "Goals: ";
        std::cin >> goals;
        std::cin.ignore();

        Player player(name, number, position, salary, transferSum, matches, goals);

        try
        {
            team.addPlayer(player);
            std::cout << "Player added successfully.\n";
        }
        catch (const std::exception& ex)
        {
            std::cout << "Cannot add player: " << ex.what() << '\n';
        }
    }
}

void addTeamToSeason(Championship& championship)
{
    int type;

    char teamName[128];
    char coachName[128];
    char stadiumName[128];
    double budget;

    std::cout << "Choose team type:\n";
    std::cout << "1. Attack team\n";
    std::cout << "2. Defence team\n";
    std::cout << "3. Balance team\n";
    std::cout << "Choice: ";
    std::cin >> type;
    std::cin.ignore();

    std::cout << "Team name: ";
    std::cin.getline(teamName, 128);

    std::cout << "Coach name: ";
    std::cin.getline(coachName, 128);

    std::cout << "Stadium name: ";
    std::cin.getline(stadiumName, 128);

    std::cout << "Budget: ";
    std::cin >> budget;
    std::cin.ignore();

    if (type == 1)
    {
        AttackTeam team(teamName, coachName, stadiumName, budget,
            0, 0, 0, 0, 0, 0, 0);

        addPlayersFromConsole(team);
        championship.addTeam(team);
    }
    else if (type == 2)
    {
        DefenceTeam team(teamName, coachName, stadiumName, budget,
            0, 0, 0, 0, 0, 0, 0);

        addPlayersFromConsole(team);
        championship.addTeam(team);
    }
    else if (type == 3)
    {
        BalanceTeam team(teamName, coachName, stadiumName, budget,
            0, 0, 0, 0, 0, 0, 0);

        addPlayersFromConsole(team);
        championship.addTeam(team);
    }
    else
    {
        throw std::invalid_argument("Invalid team type");
    }

    std::cout << "Team added to championship.\n";
}

Championship* chooseSeason(ChampionshipCollection& collection)
{
    int year;

    std::cout << "Enter championship year: ";
    std::cin >> year;
    std::cin.ignore();

    Championship* championship = collection.getChampionshipByYear(year);

    if (!championship)
    {
        std::cout << "Championship not found.\n";
        return nullptr;
    }

    return championship;
}

void swapPlayersMenu(Championship& championship)
{
    char firstTeamName[128];
    char secondTeamName[128];
    char firstPlayerName[128];
    char secondPlayerName[128];

    championship.printTeams();

    std::cout << "First team name: ";
    std::cin.getline(firstTeamName, 128);

    std::cout << "Second team name: ";
    std::cin.getline(secondTeamName, 128);

    Team* firstTeam = championship.getTeamByName(firstTeamName);
    Team* secondTeam = championship.getTeamByName(secondTeamName);

    if (!firstTeam || !secondTeam)
    {
        std::cout << "One of the teams was not found.\n";
        return;
    }

    std::cout << "Player from first team: ";
    std::cin.getline(firstPlayerName, 128);

    std::cout << "Player from second team: ";
    std::cin.getline(secondPlayerName, 128);

    if (firstTeam->canSwapPlayersWith(*secondTeam, firstPlayerName, secondPlayerName))
    {
        firstTeam->swapPlayersWith(*secondTeam, firstPlayerName, secondPlayerName);
        std::cout << "Players swapped successfully.\n";
    }
    else
    {
        std::cout << "This swap is not valid.\n";
    }
}

void printMenu()
{
    std::cout << "\n========== FOOTBALL CHAMPIONSHIP ==========\n";
    std::cout << "1. Start new season\n";
    std::cout << "2. Add championship with custom year\n";
    std::cout << "3. Add team to season\n";
    std::cout << "4. Show teams in season\n";
    std::cout << "5. Generate schedule\n";
    std::cout << "6. Play one round\n";
    std::cout << "7. Play all rounds\n";
    std::cout << "8. Show standings\n";
    std::cout << "9. Swap players between teams\n";
    std::cout << "10. Show all championships\n";
    std::cout << "11. Save championships to file\n";
    std::cout << "12. Load championships from file\n";
    std::cout << "13. Show final results\n";
    std::cout << "14. Regulate salaries for team\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

int main()
{
    ChampionshipCollection collection;

    int choice;

    do
    {
        printMenu();
        std::cin >> choice;
        std::cin.ignore();

        try
        {
            if (choice == 1)
            {
                Championship& newSeason = collection.startNewSeason();
                std::cout << "Started new season: " << newSeason.getYear() << '\n';
            }
            else if (choice == 2)
            {
                int year;

                std::cout << "Enter year: ";
                std::cin >> year;
                std::cin.ignore();

                Championship championship(year);
                collection.addChampionship(championship);

                std::cout << "Championship added.\n";
            }
            else if (choice == 3)
            {
                Championship* championship = chooseSeason(collection);

                if (championship)
                    addTeamToSeason(*championship);
            }
            else if (choice == 4)
            {
                Championship* championship = chooseSeason(collection);

                if (championship)
                    championship->printTeams();
            }
            else if (choice == 5)
            {
                Championship* championship = chooseSeason(collection);

                if (championship)
                {
                    championship->generateSchedule();
                    std::cout << "Schedule generated.\n";
                }
            }
            else if (choice == 6)
            {
                Championship* championship = chooseSeason(collection);

                if (championship)
                {
                    championship->playRound();
                    std::cout << "Round played.\n";
                }
            }
            else if (choice == 7)
            {
                Championship* championship = chooseSeason(collection);

                if (championship)
                {
                    championship->playAllRounds();
                    std::cout << "All rounds played.\n";
                }
            }
            else if (choice == 8)
            {
                Championship* championship = chooseSeason(collection);

                if (championship)
                    championship->printStandings();
            }
            else if (choice == 9)
            {
                Championship* championship = chooseSeason(collection);

                if (championship)
                    swapPlayersMenu(*championship);
            }
            else if (choice == 10)
            {
                collection.printAllChampionships();
            }
            else if (choice == 11)
            {
                char fileName[128];

                std::cout << "File name: ";
                std::cin.getline(fileName, 128);

                collection.saveToFile(fileName);

                std::cout << "Saved successfully.\n";
            }
            else if (choice == 12)
            {
                char fileName[128];

                std::cout << "File name: ";
                std::cin.getline(fileName, 128);

                collection.loadFromFile(fileName);

                std::cout << "Loaded successfully.\n";
            }
            else if (choice == 13)
            {
                Championship* championship = chooseSeason(collection);

                if (championship)
                    championship->printFinalResults();
            }
            else if (choice == 14)
            {
                Championship* championship = chooseSeason(collection);

                if (championship)
                {
                    char teamName[128];

                    championship->printTeams();

                    std::cout << "Team name: ";
                    std::cin.getline(teamName, 128);

                    Team* team = championship->getTeamByName(teamName);

                    if (!team)
                    {
                        std::cout << "Team not found.\n";
                    }
                    else
                    {
                        team->regulatePlayersSalaries();
                        std::cout << "Salaries regulated successfully.\n";
                        team->printTeamInfo();
                    }
                }
                }
            else if (choice == 0)
            {
                std::cout << "Exiting...\n";
            }
            else
            {
                std::cout << "Invalid choice.\n";
            }
        }
        catch (const std::exception& ex)
        {
            std::cout << "Error: " << ex.what() << '\n';
        }

    } while (choice != 0);

    return 0;
}