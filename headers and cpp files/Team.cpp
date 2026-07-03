#define _CRT_SECURE_NO_WARNINGS
#include "Team.h"
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <fstream>

Team::Team()
{
	this->nameTeam = new char[1];
	this->nameTeam[0] = '\0';
	this->nameCoach = new char[1];
	this->nameCoach[0] = '\0';
	this->nameStadium = new char[1];
	this->nameStadium[0] = '\0';

	this->budget = 0.0;
	this->matchesForSeason = 0;
	this->wins = 0;
	this->draws = 0;
	this->loses = 0;
	this->points = 0;
	this->allGoals = 0;
	this->allConcededGoals = 0;
	this->currentSize = 0;
}

Team::Team(const char* name, const char* coach, const char* stadium, double budget, int matchesForSeason, int wins, int draws, int loses, int points, int allGoals, int allConcededGoals)
{
	this->nameTeam = nullptr;
	this->nameCoach = nullptr;
	this->nameStadium = nullptr;
	this->currentSize = 0;

	try
	{
		setNameTeam(name);
		setNameCoach(coach);
		setNameStadium(stadium);

		setBudget(budget);
		setMatchesForSeason(matchesForSeason);
		setWins(wins);
		setDraws(draws);
		setLoses(loses);
		setPoints(points);
		setAllGoals(allGoals);
		setAllConcededGoals(allConcededGoals);
	}
	catch (...)
	{
		freeTeam();
		throw;
	}
}

Team::Team(const Team& other)
{
	nameTeam = nullptr;
	nameCoach = nullptr;
	nameStadium = nullptr;

	copyFromTeam(other);
}

Team& Team::operator=(const Team& other)
{
	if (this != &other)
	{
		copyFromTeam(other);
	}
	return *this;
}

Team::~Team()
{
	freeTeam();
}

const char* Team::getNameTeam() const
{
	return nameTeam;
}

const char* Team::getNameCoach() const
{
	return nameCoach;
}

const char* Team::getNameStadium() const
{
	return nameStadium;
}

double Team::getBudget() const
{
	return budget;
}

int Team::getMatchesForSeason() const
{
	return matchesForSeason;
}

int Team::getWins() const
{
	return wins;
}

int Team::getDraws() const
{
	return draws;
}

int Team::getLoses() const
{
	return loses;
}

int Team::getPoints() const
{
	return points;
}

int Team::getAllGoals() const
{
	return allGoals;
}

int Team::getAllConcededGoals() const
{
	return allConcededGoals;
}

void Team::setNameTeam(const char* name)
{
	if (!name) {
		throw std::invalid_argument("Invalid team name");
	}

	char* newName = new char[strlen(name) + 1];
	strcpy(newName, name);

	delete[] this->nameTeam;
	this->nameTeam = newName;
}

void Team::setNameCoach(const char* coach)
{
	if (!coach) {
		throw std::invalid_argument("Invalid coach name");
	}

	char* newCoach = new char[strlen(coach) + 1];
	strcpy(newCoach, coach);

	delete[] this->nameCoach;
	this->nameCoach = newCoach;
}

void Team::setNameStadium(const char* stadium)
{
	if (!stadium) {
		throw std::invalid_argument("Invalid stadium name");
	}

	char* newStadium = new char[strlen(stadium) + 1];
	strcpy(newStadium, stadium);

	delete[] this->nameStadium;
	this->nameStadium = newStadium;
}

void Team::setBudget(double budget)
{
	if (budget < 0) {
		throw std::invalid_argument("Budget cannot be negative");
	}
	this->budget = budget;
}

void Team::setMatchesForSeason(int matchesForSeason)
{
	if (matchesForSeason < 0)
		throw std::invalid_argument("Invalid number of matches");
	this->matchesForSeason = matchesForSeason;
}

void Team::setWins(int wins)
{
	if (wins < 0)
		throw std::invalid_argument("Invalid number of wins");
	this->wins = wins;
}

void Team::setDraws(int draws)
{
	if (draws < 0)
		throw std::invalid_argument("Invalid number of draws");
	this->draws = draws;
}

void Team::setLoses(int loses)
{
	if (loses < 0)
		throw std::invalid_argument("Invalid number of loses");
	this->loses = loses;
}

void Team::setPoints(int points)
{
	if (points < 0)
		throw std::invalid_argument("Invalid number of points");
	this->points = points;
}

void Team::setAllGoals(int allGoals)
{
	if (allGoals < 0)
		throw std::invalid_argument("Invalid number of goals");
	this->allGoals = allGoals;
}

void Team::setAllConcededGoals(int allConcededGoals)
{
	if (allConcededGoals < 0)
		throw std::invalid_argument("Invalid number of conceded goals");
	this->allConcededGoals = allConcededGoals;
}

void Team::addPlayer(const Player& player)
{
	if (!canBuyPlayer(player))
		throw std::invalid_argument("Cannot add player");

	players[currentSize] = player;
	currentSize++;

	budget -= player.getTransferSum();
}

void Team::removePlayer(const char* namePlayer)
{
	if (!namePlayer)
		throw std::invalid_argument("Invalid player name");

	int index = -1;

	for (int i = 0; i < currentSize; i++)
	{
		if (strcmp(players[i].getName(), namePlayer) == 0)
		{
			index = i;
			break;
		}
	}

	if (index == -1)
		throw std::logic_error("Player not found");

	for (int i = index; i < currentSize - 1; i++)
	{
		players[i] = players[i + 1];
	}

	currentSize--;
}

void Team::updateStatistics(int goalsScored, int goalsConceded)
{
	if (goalsScored < 0 || goalsConceded < 0)
		throw std::invalid_argument("Invalid number of goals");

	matchesForSeason++;

	allGoals += goalsScored;
	allConcededGoals += goalsConceded;

	if (goalsScored > goalsConceded)
	{
		wins++;
		points += 3;
	}
	else if (goalsScored == goalsConceded)
	{
		draws++;
		points += 1;

	}
	else
	{
		loses++;
	}
}

void Team::startNewSeason()
{
	matchesForSeason = 0;
	wins = 0;
	draws = 0;
	loses = 0;
	points = 0;
	allGoals = 0;
	allConcededGoals = 0;
}

void Team::printTeamInfo() const
{
	std::cout << "Team name: " << nameTeam << '\n';
	std::cout << "Coach: " << nameCoach << '\n';
	std::cout << "Stadium: " << nameStadium << '\n';
	std::cout << "Budget: " << budget << '\n';

	std::cout << "Matches: " << matchesForSeason << '\n';
	std::cout << "Wins: " << wins << '\n';
	std::cout << "Draws: " << draws << '\n';
	std::cout << "Loses: " << loses << '\n';
	std::cout << "Points: " << points << '\n';
	std::cout << "Goals scored: " << allGoals << '\n';
	std::cout << "Goals conceded: " << allConcededGoals << '\n';

	std::cout << "Players count: " << currentSize << '\n';

	for (int i = 0; i < currentSize; i++)
	{
		std::cout << "\nPlayer " << i + 1 << ":\n";
		players[i].print();
	}
}

void Team::copyFromTeam(const Team& other)
{
	char* newName = nullptr;
	char* newCoach = nullptr;
	char* newStadium = nullptr;

	try
	{
		newName = new char[strlen(other.nameTeam) + 1];
		strcpy(newName, other.nameTeam);

		newCoach = new char[strlen(other.nameCoach) + 1];
		strcpy(newCoach, other.nameCoach);

		newStadium = new char[strlen(other.nameStadium) + 1];
		strcpy(newStadium, other.nameStadium);
	}
	catch (...)
	{
		delete[] newName;
		delete[] newCoach;
		delete[] newStadium;
		throw;
	}
	delete[] nameTeam;
	delete[] nameCoach;
	delete[] nameStadium;

	nameTeam = newName;
	nameCoach = newCoach;
	nameStadium = newStadium;

	budget = other.budget;
	matchesForSeason = other.matchesForSeason;
	wins = other.wins;
	draws = other.draws;
	loses = other.loses;
	points = other.points;
	allGoals = other.allGoals;
	allConcededGoals = other.allConcededGoals;

	currentSize = other.currentSize;
	for (int i = 0; i < currentSize; i++)
	{
		players[i] = other.players[i];
	}
}

void Team::freeTeam()
{
	delete[] nameTeam;
	delete[] nameCoach;
	delete[] nameStadium;

	nameTeam = nullptr;
	nameCoach = nullptr;
	nameStadium = nullptr;
}

int Team::countByPosition(Position position) const
{
	int counter = 0;

	for (int i = 0; i < currentSize; i++)
	{
		if (players[i].getPosition() == position)
		{
			counter++;
		}
	}
	return counter;
}

bool Team::hasPlayerWithName(const char* namePlayer) const
{
	if (!namePlayer) {
		throw std::invalid_argument("Invalid player name");
	}
	for (int i = 0; i < currentSize; i++)
	{
		if (strcmp(players[i].getName(), namePlayer) == 0) {
			return true;
		}
	}
	return false;
}

bool Team::hasPlayerWithNumber(int number) const
{
	for (int i = 0; i < currentSize; i++)
	{
		if (players[i].getNumber() == number)
		{
			return true;
		}
	}
	return false;
}

int Team::findPlayerIndexByName(const char* namePlayer) const
{
	if (!namePlayer)
		throw std::invalid_argument("Invalid player name");

	for (int i = 0; i < currentSize; i++)
	{
		if (strcmp(players[i].getName(), namePlayer) == 0)
		{
			return i;
		}
	}

	return -1;
}

bool Team::hasPlayerWithNameExceptIndex(const char* namePlayer, int skippedIndex) const
{
	if (!namePlayer)
		throw std::invalid_argument("Invalid player name");

	for (int i = 0; i < currentSize; i++)
	{
		if (i != skippedIndex && strcmp(players[i].getName(), namePlayer) == 0)
		{
			return true;
		}
	}

	return false;
}

bool Team::hasPlayerWithNumberExceptIndex(int number, int skippedIndex) const
{
	for (int i = 0; i < currentSize; i++)
	{
		if (i != skippedIndex && players[i].getNumber() == number)
		{
			return true;
		}
	}

	return false;
}

Player* Team::getPlayerByName(const char* namePlayer)
{
	if (!namePlayer)
		throw std::invalid_argument("Invalid player name");

	for (int i = 0; i < currentSize; i++)
	{
		if (strcmp(players[i].getName(), namePlayer) == 0)
		{
			return &players[i];
		}
	}

	return nullptr;
}

int Team::getCurrentSize() const
{
	return currentSize;
}

const Player& Team::getPlayer(int index) const
{
	if (index < 0 || index >= currentSize)
		throw std::out_of_range("Invalid index");

	return players[index];
}

bool Team::canSwapPlayersWith(const Team& other, const char* playerFromThisTeam, const char* playerFromOtherTeam) const
{
	if (&other == this)
		return false;

	int firstIndex = findPlayerIndexByName(playerFromThisTeam);
	int secondIndex = other.findPlayerIndexByName(playerFromOtherTeam);

	if (firstIndex == -1 || secondIndex == -1)
		return false;

	const Player& firstPlayer = players[firstIndex];
	const Player& secondPlayer = other.players[secondIndex];

	if (hasPlayerWithNameExceptIndex(secondPlayer.getName(), firstIndex))
		return false;

	if (hasPlayerWithNumberExceptIndex(secondPlayer.getNumber(), firstIndex))
		return false;

	if (other.hasPlayerWithNameExceptIndex(firstPlayer.getName(), secondIndex))
		return false;

	if (other.hasPlayerWithNumberExceptIndex(firstPlayer.getNumber(), secondIndex))
		return false;

	Team* firstTeamCopy = nullptr;
	Team* secondTeamCopy = nullptr;

	try
	{
		firstTeamCopy = clone();
		secondTeamCopy = other.clone();

		Player temp = firstTeamCopy->players[firstIndex];
		firstTeamCopy->players[firstIndex] = secondTeamCopy->players[secondIndex];
		secondTeamCopy->players[secondIndex] = temp;

		bool validAfterSwap = firstTeamCopy->isValidTeam() && secondTeamCopy->isValidTeam();

		delete firstTeamCopy;
		delete secondTeamCopy;

		return validAfterSwap;
	}
	catch (...)
	{
		delete firstTeamCopy;
		delete secondTeamCopy;
		throw;
	}
}

void Team::swapPlayersWith(Team& other, const char* playerFromThisTeam, const char* playerFromOtherTeam)
{
	if (!canSwapPlayersWith(other, playerFromThisTeam, playerFromOtherTeam))
		throw std::logic_error("Cannot swap these players");

	int firstIndex = findPlayerIndexByName(playerFromThisTeam);
	int secondIndex = other.findPlayerIndexByName(playerFromOtherTeam);

	Player temp = players[firstIndex];
	players[firstIndex] = other.players[secondIndex];
	other.players[secondIndex] = temp;
}

void Team::regulatePlayersSalaries()
{
	for (int i = 0; i < currentSize; i++)
	{
		players[i].salaryRegulation(matchesForSeason);
	}
}

void Team::saveToFile(std::ofstream& out) const
{
	if (!out)
		throw std::logic_error("Invalid output file");

	out << getType() << '\n';

	out << nameTeam << '\n';
	out << nameCoach << '\n';
	out << nameStadium << '\n';

	out << budget << '\n';

	out << matchesForSeason << '\n';
	out << wins << '\n';
	out << draws << '\n';
	out << loses << '\n';
	out << points << '\n';
	out << allGoals << '\n';
	out << allConcededGoals << '\n';

	out << currentSize << '\n';

	for (int i = 0; i < currentSize; i++)
	{
		players[i].saveToFile(out);
	}
}

void Team::loadFromFile(std::ifstream& in)
{
	if (!in)
		throw std::logic_error("Invalid input file");

	char buffer[128];

	in.getline(buffer, 128);
	setNameTeam(buffer);

	in.getline(buffer, 128);
	setNameCoach(buffer);

	in.getline(buffer, 128);
	setNameStadium(buffer);

	in >> budget;

	in >> matchesForSeason;
	in >> wins;
	in >> draws;
	in >> loses;
	in >> points;
	in >> allGoals;
	in >> allConcededGoals;

	in >> currentSize;
	in.ignore();

	for (int i = 0; i < currentSize; i++)
	{
		players[i].loadFromFile(in);
	}
}